#include <iostream>

#include "WakeDefines.h"

#include "Logging/LogMacros.h"

#include "Engine/InputManager.h"
#include "Engine/GEngine.h"
#include "Engine/Bootstrap.h"
#include "Engine/Shader.h"

#include "World/World.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestActor : public Actor
{
public:
	TestActor(ActorID Id, bool StartActive, GLuint Shader)
		: Actor(Id, StartActive), Shader(Shader)
	{
	}

	virtual void Spawn() override
	{
		LOG_INFO(GlobalLogger, "TestActor spawned with an id of " << GetActorID() << "!");

		GLint posAttrib = glGetAttribLocation(Shader.GetProgram(), "position");

		float vertices[] = {
			0.f, 0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f
		};

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttrib);
	}

	virtual void Destroy() override
	{
		LOG_INFO(GlobalLogger, "TestActor is being destroyed with an id of " << GetActorID() << "!");
	}

	virtual void Draw() override
	{
		Shader.Use();
		Shader.GetUniform("matrix").Set(CreateMatrix());

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	ShaderProgram Shader;

	GLuint vbo;
	GLuint vao;
};

void OnInput_Test(const Input& Input)
{
	LOG_INFO(GlobalLogger, "Hello!");
}

void OnInput_Exit(const Input& Input)
{
	W_ENGINE.Stop();
}

void Setup()
{
	W_INPUT.CreateBinding("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
	
	W_INPUT.CreateBinding("Test", INPUT_BIND(Keyboard, Pressed, Space));
	W_INPUT.Event("Test").Bind(&OnInput_Test);

	TestActor* A1 = W_WORLD.SpawnActor<TestActor>(true, ShaderProgram::LoadProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag"));

	A1->SetPosition(glm::vec3(0, 0, 0));
}

WAKE_CUSTOM_BOOTSTRAP(
	Main,
	WBS_OPT(WindowOptions.AntiAliasing, 4),
	Setup();
)

int main(int argc, char** argv)
{
	WAKE_CALL_BOOTSTRAP(Main, argc, argv);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}