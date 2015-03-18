#include "GEngine.h"

namespace Engine
{
	CLOG_LOGGER_DEF(Engine::GEngine);

	GEngine& GEngine::Get()
	{
		static GEngine Instance;
		return Instance;
	}

	GEngine::GEngine()
	{
		running = false;
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
		running = true;
		while (running)
		{
			GameWindow.PollEvents();

			if (!GameWindow.IsOpen())
				Stop();
		}
	}

	void GEngine::Stop()
	{
		running = false;
	}
}