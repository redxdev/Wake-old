#include <iostream>

#include "WakeDefines.h"

#include "Logging/LogMacros.h"

#include "Engine/InputManager.h"
#include "Engine/GEngine.h"
#include "Engine/Bootstrap.h"
#include "Engine/Shader.h"

#include "World/World.h"
#include "World/StaticMeshComponent.h"
#include "World/CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

int xAxis = 0;
int yAxis = 0;
int rot = 0;

class CameraActor : public Actor
{
public:
	CameraActor(ActorID Id, bool StartActive)
		: Actor(Id, StartActive)
	{
	}

	virtual void Spawn() override
	{
		Actor::Spawn();

		Camera = CreateComponent<CameraComponent>(true);
	}

private:
	CameraComponent* Camera;
};

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
	}

	virtual void Tick() override
	{
	}

private:
	StaticMeshComponent* MeshComponent;
};

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

void RotLeft(const Input& Input)
{
	rot += Input.Mode == EInputMode::Pressed ? 1 : -1;
}

void RotRight(const Input& Input)
{
	rot += Input.Mode == EInputMode::Pressed ? -1 : 1;
}

void OnInput_Exit(const Input& Input)
{
	W_ENGINE.Stop();
}

CameraActor* Cam = nullptr;

void Tick()
{
	auto fwd = Cam->GetForward();
	auto right = Cam->GetRight();
	Cam->SetPosition(Cam->GetPosition() + (Cam->GetForward() * (float)yAxis + Cam->GetRight() * (float)xAxis) * W_ENGINE.GetDeltaTime());
	Cam->SetRotation(Cam->GetRotation() * glm::quat(glm::vec3(0, rot, 0) * W_ENGINE.GetDeltaTime()));
}

void Setup()
{
	Actor* Test = W_WORLD.SpawnActor<TestActor>();
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(1, 0, 1));
	Test->SetRotation(glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));

	Cam = W_WORLD.SpawnActor<CameraActor>(true);
	Cam->SetPosition(glm::vec3(0, 0, 3));
	Cam->SetRotation(glm::quat(glm::vec3(0, 0, 0)));

	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);

	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Pressed, Left));
	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Released, Left));

	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Pressed, Right));
	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Released, Right));

	W_INPUT.Bind("Up", INPUT_BIND(Keyboard, Pressed, Up));
	W_INPUT.Bind("Up", INPUT_BIND(Keyboard, Released, Up));

	W_INPUT.Bind("Down", INPUT_BIND(Keyboard, Pressed, Down));
	W_INPUT.Bind("Down", INPUT_BIND(Keyboard, Released, Down));

	W_INPUT.Bind("RotLeft", INPUT_BIND(Keyboard, Pressed, A));
	W_INPUT.Bind("RotLeft", INPUT_BIND(Keyboard, Released, A));
	
	W_INPUT.Bind("RotRight", INPUT_BIND(Keyboard, Pressed, D));
	W_INPUT.Bind("RotRight", INPUT_BIND(Keyboard, Released, D));
	
	W_INPUT.Event("Left").Bind(&Left);
	W_INPUT.Event("Right").Bind(&Right);
	W_INPUT.Event("Up").Bind(&Up);
	W_INPUT.Event("Down").Bind(&Down);

	W_INPUT.Event("RotLeft").Bind(&RotLeft);
	W_INPUT.Event("RotRight").Bind(&RotRight);

	W_ENGINE.Tick.Bind(&Tick);
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