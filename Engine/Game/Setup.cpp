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

#include "Player.h"
#include "FallingObject.h"

StaticMesh* Mesh = nullptr;
StaticMesh* Mesh2 = nullptr;
ShaderProgram* Program = nullptr;

void Input_Stop(const Input& Input)
{
	W_ENGINE.Stop();
}

void GameSetup()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> d(0, 1);

	// See Player.cpp for more explainations on the input system
	W_INPUT.Bind("Exit", INPUT_BIND(Keyboard, Released, Escape));
	W_INPUT.Event("Exit").Bind(&Input_Stop);

	// Make a single mesh for all to share
	Program = new ShaderProgram(ShaderProgram::LoadProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag"));
	if (Program->GetProgram() == 0)
	{
		LOG_ERROR(GlobalLogger, "Something went wrong loading the shader program!");
	}

	GLint PositionAttr = glGetAttribLocation(Program->GetProgram(), "position");

	float Vertices[] = {
		-1, 0.0, 0.0,
		0.0, 2, 0.0,
		1, 0.0, 0.0,
	};

	GLuint vbo;
	GLuint vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(PositionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(PositionAttr);

	Mesh = new StaticMesh(vbo, vao, 3, GL_TRIANGLES);

	float Vertices2[] = {
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		0.5, 0.5, 0.0,
		-0.5, 0.5, 0.0
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(PositionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(PositionAttr);

	Mesh2 = new StaticMesh(vbo, vao, 4, GL_QUADS);

	auto p = W_WORLD.SpawnActor<Player>(true, Program, Mesh);
	p->SetScale(glm::vec3(0.2, 0.2, 1));

	for (int i = 0; i < 400; ++i)
	{
		auto o = W_WORLD.SpawnActor<FallingObject>(true, Program, Mesh2, p->GetActorID(), d(gen) * 2 - 1);
		o->SetScale(glm::vec3(0.02f, 0.08f, 1.0f));
		o->SetSpeed(d(gen) + 0.1f);
		o->SetPosition(o->GetPosition() + glm::vec3(0.f, d(gen) * 2 + 1, 0.f));
	}

	W_ENGINE.GetGameWindow().SetTitle("Rain Thing, use WASD+Space to move");
}

void GameTeardown()
{
	delete Mesh;
	delete Mesh2;
	delete Program;

	// Actors do not need to be deleted; they are automatically deleted by the world when the engine stops.
	// See Engine/Bootstrap.cpp and World/World.cpp
}