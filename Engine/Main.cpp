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

	virtual void Tick() override
	{
		Actor::Tick();

		SetRotation(GetRotation() * glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0) * W_ENGINE.GetDeltaTime()));
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

	auto MousePos = sf::Mouse::getPosition(*W_ENGINE.GetGameWindow().GetRenderWindow());
	auto Center = sf::Vector2i(W_ENGINE.GetGameWindow().GetWidth() / 2, W_ENGINE.GetGameWindow().GetHeight() / 2);
	float xChange = Center.x - MousePos.x;
	float yChange = Center.y - MousePos.y;
	auto rot = glm::vec3(yChange, xChange, 0) * W_ENGINE.GetDeltaTime();
	Cam->SetRotation(Cam->GetRotation() * glm::quat(rot));
	sf::Mouse::setPosition(Center, *W_ENGINE.GetGameWindow().GetRenderWindow());
}

void Setup()
{
	Actor* Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(-1, 0, -1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(-1, 0, 1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(1, 0, -1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(1, 0, 1));
	Test->SetRotation(glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));

	Cam = W_WORLD.SpawnActor<CameraActor>(true);
	Cam->SetPosition(glm::vec3(0, 0, 3));

	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);

	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Pressed, A));
	W_INPUT.Bind("Left", INPUT_BIND(Keyboard, Released, A));

	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Pressed, D));
	W_INPUT.Bind("Right", INPUT_BIND(Keyboard, Released, D));

	W_INPUT.Bind("Up", INPUT_BIND(Keyboard, Pressed, W));
	W_INPUT.Bind("Up", INPUT_BIND(Keyboard, Released, W));

	W_INPUT.Bind("Down", INPUT_BIND(Keyboard, Pressed, S));
	W_INPUT.Bind("Down", INPUT_BIND(Keyboard, Released, S));
	
	W_INPUT.Event("Left").Bind(&Left);
	W_INPUT.Event("Right").Bind(&Right);
	W_INPUT.Event("Up").Bind(&Up);
	W_INPUT.Event("Down").Bind(&Down);

	W_ENGINE.Tick.Bind(&Tick);

	sf::Mouse::setPosition(sf::Vector2i(W_ENGINE.GetGameWindow().GetWidth() / 2, W_ENGINE.GetGameWindow().GetHeight() / 2), *W_ENGINE.GetGameWindow().GetRenderWindow());
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