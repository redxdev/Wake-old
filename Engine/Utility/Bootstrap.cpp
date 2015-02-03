#include "Bootstrap.h"
#include "../Logging/LogManager.h"
#include "../Logging/ConsoleSink.h"
#include "../Logging/FileSink.h"

#ifdef _MSC_VER
#include "../Logging/VSDebugSink.h"
#endif

#include "../Engine/EntityManager.h"
#include "../Engine/GEngine.h"

namespace Utility
{
	CLOG_LOGGER_DEF(Utility::Bootstrap);

	Bootstrap::Bootstrap(int ArgC, char** ArgV, const BootstrapOptions& Options)
		: Options(Options)
	{
		DidStartup = false;
	}

	Bootstrap::~Bootstrap()
	{
		if (!DidStartup)
			return;

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

		W_ENGINE.Run();
	}

	bool Bootstrap::Startup()
	{
		if(!W_LOG.Startup())
		{
			std::cerr << "WLOG.Startup() failed" << std::endl;
			return false;
		}

		if (Options.DefaultLogSinks)
		{
			W_LOG.AddSink(new Logging::ConsoleSink());
			W_LOG.AddSink(new Logging::FileSink("logs/engine.log"));

#ifdef _MSC_VER
			W_LOG.AddSink(new Logging::VSDebugSink());
#endif
		}

		CLOG_INFO("init");

		if(!W_ENTITY.Startup())
		{
			CLOG_FATAL("WENTITY.Startup() failed");
			return false;
		}

		if(!W_ENGINE.Startup(Options.WindowOptions))
		{
			CLOG_FATAL("WENGINE.Startup() failed");
			return false;
		}

		DidStartup = true;

		return true;
	}

	bool Bootstrap::Shutdown()
	{
		CLOG_INFO("shutdown");

		if(!W_ENGINE.Shutdown())
		{
			CLOG_FATAL("WENGINE.Shutdown() failed");
			return false;
		}

		if(!W_ENTITY.Shutdown())
		{
			CLOG_FATAL("WENTITY.Shutdown() failed");
			return false;
		}

		if(!W_LOG.Shutdown())
		{
			std::cerr << "WLOG.Shutdown() failed" << std::endl;
			return false;
		}

		return true;
	}
}