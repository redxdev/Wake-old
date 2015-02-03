#pragma once

#include "../Logging/LogMacros.h"
#include "EntityManager.h"
#include "Window.h"

#define W_ENGINE (Engine::GEngine::Get())

namespace Engine
{
	class GEngine
	{
		CLOG_LOGGER_DECL;
	public:
		static GEngine& Get();

		bool Startup(const WindowOptions& Options = WindowOptions());
		bool Shutdown();

		void Run();
		void Stop();

		inline Window& GetGameWindow()
		{
			return GameWindow;
		}

		inline bool IsRunning()
		{
			return running;
		}

	private:
		GEngine();
		~GEngine();

		Window GameWindow;

		EID BaseEID;

		bool running;
	};
}