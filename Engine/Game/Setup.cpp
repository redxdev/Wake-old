#include "Setup.h"

#include <gl/glew.h>
#include <SFML/OpenGL.hpp>

#include <random>

#include "../Engine/GEngine.h"
#include "../Engine/InputManager.h"
#include "../World/World.h"
#include "../Engine/Shader.h"
#include "../Engine/Mesh.h"
#include "../Logging/LogMacros.h"
#include "../World/CameraComponent.h"
#include "../World/StaticMeshComponent.h"

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

		Cam = CreateComponent<CameraComponent>();
	}

private:
	CameraComponent* Cam;
};

class TestActor : public Actor
{
public:
	TestActor(ActorID Id, bool StartActive, StaticMesh* Mesh, ShaderProgram* Shader)
		: Actor(Id, StartActive), Mesh(Mesh), Shader(Shader)
	{
		
	}

	virtual void Spawn() override
	{
		Actor::Spawn();

		MeshComponent = CreateComponent<StaticMeshComponent>(true, Mesh, Shader);
		MeshComponent->SetR(1.f);
	}

private:
	StaticMesh* Mesh;
	ShaderProgram* Shader;

	StaticMeshComponent* MeshComponent;
};

CameraActor* Cam;
ShaderProgram* Shader;
StaticMesh* Mesh;

void Tick()
{
	sf::Vector2i CurPos = sf::Mouse::getPosition(*W_ENGINE.GetGameWindow().GetRenderWindow());
	sf::Vector2u WinSize = W_ENGINE.GetGameWindow().GetRenderWindow()->getSize();
	sf::Vector2i MousePos = sf::Vector2i(WinSize.x / 2, WinSize.y / 2);

	float xMove = CurPos.x - MousePos.x;
	xMove *= W_ENGINE.GetDeltaTime();
	float yMove = CurPos.y - MousePos.y;
	yMove *= W_ENGINE.GetDeltaTime();

	Cam->SetRotation(Cam->GetRotation() * glm::quat(glm::vec3(yMove, xMove, 0)));

	sf::Mouse::setPosition(MousePos, *W_ENGINE.GetGameWindow().GetRenderWindow());
}

void Input_Stop(const Input& Input)
{
	W_ENGINE.Stop();
}

void GameSetup()
{
	Shader = new ShaderProgram(ShaderProgram::LoadProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag"));

	GLuint vao;
	GLuint vbo;
	GLint PositionAttr = glGetAttribLocation(Shader->GetProgram(), "position");

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float Vertices[] = {
		-1, 0.0, 0.0,
		0.0, 2, 0.0,
		1, 0.0, 0.0,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(PositionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(PositionAttr);

	Mesh = new StaticMesh(vbo, vao, 3, GL_TRIANGLES);

	Cam = W_WORLD.SpawnActor<CameraActor>();
	Cam->SetPosition(glm::vec3(0, 0, 10));

	W_WORLD.SpawnActor<TestActor>(true, Mesh, Shader);

	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Released, Escape));
	W_INPUT.Event("Exit").Bind(&Input_Stop);

	sf::Vector2u WinSize = W_ENGINE.GetGameWindow().GetRenderWindow()->getSize();
	sf::Vector2i MousePos = sf::Vector2i(WinSize.x / 2, WinSize.y / 2);
	sf::Mouse::setPosition(MousePos, *W_ENGINE.GetGameWindow().GetRenderWindow());

	W_ENGINE.Tick.Bind(&Tick);
}

void GameTeardown()
{
	delete Mesh;
	delete Shader;

	// Actors do not need to be deleted; they are automatically deleted by the world when the engine stops.
	// See Engine/Bootstrap.cpp and World/World.cpp
}