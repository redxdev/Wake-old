#include "GEngine.h"

#include <SFML/OpenGL.hpp>

#include "../World/World.h"

CLOG_LOGGER_DEF(GEngine);

GEngine& GEngine::Get()
{
	static GEngine Instance;
	return Instance;
}

GEngine::GEngine()
{
	Running = false;
}

GEngine::~GEngine()
{
}

bool GEngine::Startup()
{
	CLOG_DEBUG("Engine startup");

	GameWindow.Closed.Bind(this, &GEngine::Stop);

	return true;
}

bool GEngine::Shutdown()
{
	CLOG_DEBUG("Engine shutdown");
	GameWindow.Deinitialize();
	return true;
}

void GEngine::Run()
{
	DeltaClock.restart();

	Running = true;
	while (Running)
	{
		GameWindow.PollEvents();

		Tick.Call();
		W_WORLD.Tick();

		Draw.Call();
		Render();

		GameWindow.Display();

		DeltaTime = DeltaClock.restart().asSeconds();

		if (!GameWindow.IsOpen())
			Stop();
	}
}

void GEngine::Stop()
{
	Running = false;
}

Window& GEngine::GetGameWindow()
{
	return GameWindow;
}

bool GEngine::IsRunning()
{
	return Running;
}

void GEngine::SetViewMatrix(const glm::mat4& Matrix)
{
	ViewMatrix = Matrix;
}

void GEngine::SetProjectionMatrix(const glm::mat4& Matrix)
{
	ProjectionMatrix = Matrix;
}

const glm::mat4& GEngine::GetViewMatrix() const
{
	return ViewMatrix;
}

const glm::mat4& GEngine::GetProjectionMatrix() const
{
	return ProjectionMatrix;
}

float GEngine::GetDeltaTime() const
{
	return DeltaTime;
}

void GEngine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	W_WORLD.Draw();
}