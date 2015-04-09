#include "GEngine.h"

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
	CLOG_INFO("Engine startup");

	GameWindow.Initialize(Options);

	GameWindow.Closed.Bind(this, &GEngine::Stop);

	return true;
}

bool GEngine::Shutdown()
{
	CLOG_INFO("Engine shutdown");
	GameWindow.Deinitialize();
	return true;
}

void GEngine::Run()
{
	Running = true;
	while (Running)
	{
		GameWindow.PollEvents();

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