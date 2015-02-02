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

		inline Window& GetGameWindow()
		{
			return GameWindow;
		}

	private:
		GEngine();
		~GEngine();

		Window GameWindow;

		EID BaseEID;
	};
}