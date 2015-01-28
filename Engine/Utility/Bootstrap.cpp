#include "Bootstrap.h"
#include "../Logging/LogManager.h"
#include "../Logging/ConsoleSink.h"
#include "../Logging/FileSink.h"

#ifdef _MSC_VER
#include "../Logging/VSDebugSink.h"
#endif

#include "../Entity/EntityManager.h"

namespace Wake
{
	namespace Utility
	{
		CLOG_LOGGER_DEF(Utility::Bootstrap);

		Bootstrap::Bootstrap(int ArgC, char** ArgV, const BootstrapOptions& Options)
			: Options(Options)
		{
			Startup();
		}

		Bootstrap::~Bootstrap()
		{
			Shutdown();
		}

		void Bootstrap::Run()
		{
			if (Options.DontRunEngine)
			{
				CLOG_INFO("Not running engine (DontRunEngine set to true)");
				return;
			}

			CLOG_INFO("Running engine");
		}

		void Bootstrap::Startup()
		{
			Logging::LogManager::Get().Startup();

			if (Options.AutoRegisterLogSinks)
			{
				Logging::LogManager::Get().AddSink(new Logging::ConsoleSink());
				Logging::LogManager::Get().AddSink(new Logging::FileSink("logs/engine.log"));

#ifdef _MSC_VER
				Logging::LogManager::Get().AddSink(new Logging::VSDebugSink());
#endif
			}

			CLOG_INFO("Engine bootstrap startup");

			Entity::EntityManager::Get().Startup();
		}

		void Bootstrap::Shutdown()
		{
			CLOG_INFO("Engine bootstrap shutdown");

			Entity::EntityManager::Get().Shutdown();

			Logging::LogManager::Get().Shutdown();
		}
	}
}