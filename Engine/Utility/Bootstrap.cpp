#include "Bootstrap.h"
#include "../Logging/LogManager.h"
#include "../Logging/ConsoleSink.h"
#include "../Logging/FileSink.h"

#ifdef _MSC_VER
#include "../Logging/VSDebugSink.h"
#endif

namespace Wake
{
	namespace Utility
	{
		CLOG_LOGGER_DEF(Bootstrap);

		Bootstrap::Bootstrap()
		{
			Logging::LogManager::Get().Startup();

			Logging::LogManager::Get().AddSink(new Logging::ConsoleSink());
			Logging::LogManager::Get().AddSink(new Logging::FileSink("logs/engine.log"));
		}

		Bootstrap::~Bootstrap()
		{
			
		}
	}
}