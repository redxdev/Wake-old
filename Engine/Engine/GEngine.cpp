#include "GEngine.h"

#include <SFML/OpenGL.hpp>

#include "../WakeDefines.h"
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

bool GEngine::Startup(const WindowOptions& Options)
{
	CLOG_DEBUG("Engine startup");

	GameWindow.Initialize(Options);

	glClearColor(WAKE_CLEAR_COLOR, 1.0f);

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
	Running = true;
	while (Running)
	{
		GameWindow.PollEvents();

		W_WORLD.Tick();

		Render();

		GameWindow.Display();

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

void GEngine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	W_WORLD.Draw();
}