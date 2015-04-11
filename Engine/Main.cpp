#include <iostream>

#include "WakeDefines.h"

#include "Logging/LogMacros.h"

#include "Engine/InputManager.h"
#include "Engine/GEngine.h"
#include "Engine/Bootstrap.h"
#include "Engine/Shader.h"

#include "World/World.h"
#include "World/StaticMeshComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

int xAxis = 0;
int yAxis = 0;

class TestActor : public Actor
{
public:
	TestActor(ActorID Id, bool StartActive)
		: Actor(Id, StartActive)
	{
	}

	virtual void Spawn() override
	{
		Actor::Spawn();

		LOG_INFO(GlobalLogger, "TestActor spawned with an id of " << GetActorID() << "!");

		ShaderProgram* Shader = new ShaderProgram(ShaderProgram::LoadProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag"));
		GLint posAttrib = glGetAttribLocation(Shader->GetProgram(), "position");

		float vertices[] = {
			0.f, 0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f
		};

		GLuint vbo;
		GLuint vao;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		MeshComponent = CreateComponent<StaticMeshComponent>(true, new StaticMesh(vbo, vao, 3), Shader);
	}

	virtual void Destroy() override
	{
		Actor::Destroy();

		LOG_INFO(GlobalLogger, "TestActor is being destroyed with an id of " << GetActorID() << "!");
	}

	virtual void Tick() override
	{
		Actor::Tick();
		SetPosition(GetPosition() + glm::vec3(xAxis, yAxis, 0) * W_ENGINE.GetDeltaTime());
	}

private:
	StaticMeshComponent* MeshComponent;


};

void OnInput_New(const Input& Input)
{
	W_WORLD.SpawnActor<TestActor>();
}

void Left(const Input& Input)
{
	xAxis += Input.Mode == EInputMode::Pressed ? -1 : 1;
}

void Right(const Input& Input)
{
	xAxis += Input.Mode == EInputMode::Pressed ? 1 : -1;
}

void Up(const Input& Input)
{
	yAxis += Input.Mode == EInputMode::Pressed ? 1 : -1;
}

void Down(const Input& Input)
{
	yAxis += Input.Mode == EInputMode::Pressed ? -1 : 1;
}

void OnInput_Exit(const Input& Input)
{
	W_ENGINE.Stop();
}

void Setup()
{
	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
	
	/*W_INPUT.CreateBinding("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);

	W_INPUT.CreateBinding("New", INPUT_BIND(Keyboard, Pressed, Return));
	W_INPUT.Event("New").Bind(&OnInput_New);

	W_INPUT.CreateBinding("Left", INPUT_BIND(Keyboard, Pressed, Left));
	W_INPUT.CreateBinding("Left", INPUT_BIND(Keyboard, Released, Left));
	W_INPUT.CreateBinding("Right", INPUT_BIND(Keyboard, Pressed, Right));
	W_INPUT.CreateBinding("Right", INPUT_BIND(Keyboard, Released, Right));
	W_INPUT.CreateBinding("Up", INPUT_BIND(Keyboard, Pressed, Up));
	W_INPUT.CreateBinding("Up", INPUT_BIND(Keyboard, Released, Up));
	W_INPUT.CreateBinding("Down", INPUT_BIND(Keyboard, Pressed, Down));
	W_INPUT.CreateBinding("Down", INPUT_BIND(Keyboard, Released, Down));

	W_INPUT.Event("Left").Bind(&Left);
	W_INPUT.Event("Right").Bind(&Right);
	W_INPUT.Event("Up").Bind(&Up);
	W_INPUT.Event("Down").Bind(&Down);

	W_WORLD.SpawnActor<TestActor>();*/
}

void RunEngine(int ArgC, char** ArgV)
{
	BootstrapOptions Options;
	Options.DefaultLogSinks = true;
	Options.DontRunEngine = false;

	Bootstrap Engine(ArgC, ArgV, Options);
	if (!Engine.Startup())
	{
		return;
	}

	Setup();

	Engine.Run();
}

int main(int argc, char** argv)
{
	RunEngine(argc, argv);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}