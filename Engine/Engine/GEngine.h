#pragma once

#include "../Logging/LogMacros.h"
#include "EntityManager.h"

#define WENGINE (Engine::GEngine::Get())

namespace Engine
{
	class GEngine
	{
		CLOG_LOGGER_DECL;
	public:
		static GEngine& Get();

		bool Startup();
		bool Shutdown();

	private:
		GEngine();
		~GEngine();

		EID BaseEID;
	};
}