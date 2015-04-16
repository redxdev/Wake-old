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
#include <glm/gtx/quaternion.hpp>

// Simple actor that just has a camera component attached to it.
// Only here because you can't spawn a component without an actor.
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

		// Make our camera component
		Camera = CreateComponent<CameraComponent>(true);
	}

private:
	CameraComponent* Camera;
};

// Spinning triangle actor. Does its own setup/teardown, so all we have
// to do is spawn it in whatever position we like.
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
		delete MeshComponent->GetShader();
		delete MeshComponent->GetMesh();

		Actor::Destroy();
	}

	virtual void Tick() override
	{
		Actor::Tick();

		SetRotation(GetRotation() * glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0) * W_ENGINE.GetDeltaTime()));
	}

private:
	StaticMeshComponent* MeshComponent;
};

void OnInput_Exit(const Input& Input)
{
	W_ENGINE.Stop();
}

CameraActor* Cam = nullptr;

void Setup()
{
	// Spawn a bunch of triangles
	Actor* Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(-1, 0, -1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(-1, 0, 1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(1, 0, -1));
	Test = W_WORLD.SpawnActor<TestActor>();
	Test->SetPosition(glm::vec3(1, 0, 1));
	Test->SetRotation(glm::quat(glm::vec3(0, glm::pi<float>() / 2.f, 0)));

	// Create the camera
	Cam = W_WORLD.SpawnActor<CameraActor>(true);
	Cam->SetPosition(glm::vec3(0, 0, 3));

	// Create a named binding "Exit" and bind escape to it
	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Pressed, Escape));

	// Bind the function OnInput_Exit to the Exit event.
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
}

// Create the bootstrap in a separate scope since the bootstrap's shutdown
// happens when it gets destroyed (falls out of scope).
void RunEngine(int ArgC, char** ArgV)
{
	// Set some options
	BootstrapOptions Options;
	Options.DefaultLogSinks = true;
	Options.DontRunEngine = false;

	// Create the bootstrap
	Bootstrap Engine(ArgC, ArgV, Options);
	if (!Engine.Startup())
	{
		// Something went wrong :(
		return;
	}

	// Run our custom setup
	Setup();

	// Run the engine
	Engine.Run();

	// Everything is cleaned up when the bootstrap falls out of scope.
}

int main(int ArgC, char** ArgV)
{
	// Run the engine
	RunEngine(ArgC, ArgV);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}