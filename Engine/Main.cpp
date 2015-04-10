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

int xAxis = 0;
int yAxis = 0;

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

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		
	}

	virtual void Draw() override
	{
		Shader.Use();
		Shader.GetUniform("modelMatrix").Set(CreateMatrix());
		Shader.GetUniform("viewMatrix").Set(glm::mat4());
		Shader.GetUniform("projectionMatrix").Set(glm::mat4());

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual void Tick() override
	{
		SetPosition(GetPosition() + glm::vec3(xAxis, yAxis, 0) * 0.0001f);
	}

private:
	ShaderProgram Shader;

	GLuint vbo;
	GLuint vao;
};

TestActor* Test = nullptr;

void OnInput_Test(const Input& Input)
{
	if (Test != nullptr)
	{
		Test->SetScale(Test->GetScale() + glm::vec3(0.1f, 0.1f, 0.1f));
	}
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
	W_INPUT.CreateBinding("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
	
	W_INPUT.CreateBinding("Test", INPUT_BIND(Keyboard, Pressed, Space));
	W_INPUT.Event("Test").Bind(&OnInput_Test);

	W_INPUT.CreateBinding("LeftDown", INPUT_BIND(Keyboard, Pressed, Left));
	W_INPUT.CreateBinding("LeftUp", INPUT_BIND(Keyboard, Released, Left));
	W_INPUT.CreateBinding("RightDown", INPUT_BIND(Keyboard, Pressed, Right));
	W_INPUT.CreateBinding("RightUp", INPUT_BIND(Keyboard, Released, Right));
	W_INPUT.CreateBinding("UpDown", INPUT_BIND(Keyboard, Pressed, Up));
	W_INPUT.CreateBinding("UpUp", INPUT_BIND(Keyboard, Released, Up));
	W_INPUT.CreateBinding("DownDown", INPUT_BIND(Keyboard, Pressed, Down));
	W_INPUT.CreateBinding("DownUp", INPUT_BIND(Keyboard, Released, Down));

	W_INPUT.Event("LeftDown").Bind(&Left);
	W_INPUT.Event("LeftUp").Bind(&Left);
	W_INPUT.Event("RightDown").Bind(&Right);
	W_INPUT.Event("RightUp").Bind(&Right);
	W_INPUT.Event("UpDown").Bind(&Up);
	W_INPUT.Event("UpUp").Bind(&Up);
	W_INPUT.Event("DownDown").Bind(&Down);
	W_INPUT.Event("DownUp").Bind(&Down);

	Test = W_WORLD.SpawnActor<TestActor>(true, ShaderProgram::LoadProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag"));
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