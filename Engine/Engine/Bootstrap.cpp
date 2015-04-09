#include "Bootstrap.h"
#include "../Logging/LogManager.h"
#include "../Logging/ConsoleSink.h"
#include "../Logging/FileSink.h"

#ifdef _MSC_VER
#include "../Logging/VSDebugSink.h"
#endif

#include "../Engine/GEngine.h"
#include "../Engine/InputManager.h"

#include <fstream>
#include <signal.h>

CLOG_LOGGER_DEF(Bootstrap);

void BootstrapSignalHandler(int signal)
{
	W_ENGINE.Stop();
}

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

	CLOG_DEBUG("Setting signal handler");
	signal(SIGINT, &BootstrapSignalHandler);

	CLOG_INFO("Running engine");

	W_ENGINE.Run();
}

bool Bootstrap::Startup()
{
	std::ifstream ConfigTest("wake.cfg");
	if (!ConfigTest.good())
	{
		std::cerr << "Bootstrap: Unable to open wake.cfg. Are we in the correct directory?" << std::endl;
		return false;
	}

	if(!W_LOG.Startup())
	{
		std::cerr << "Bootstrap: WLOG.Startup() failed" << std::endl;
		return false;
	}

	if (Options.DefaultLogSinks)
	{
		W_LOG.AddSink(new ConsoleSink());
		W_LOG.AddSink(new FileSink("logs/engine.log"));

#ifdef _MSC_VER
		W_LOG.AddSink(new VSDebugSink());
#endif
	}

	CLOG_INFO("init");

	if(!W_ENGINE.Startup(Options.WindowOptions))
	{
		CLOG_FATAL("W_ENGINE.Startup() failed");
		return false;
	}

	if (!W_INPUT.Startup())
	{
		CLOG_FATAL("W_INPUT.Startup() failed");
		return false;
	}

	DidStartup = true;

	return true;
}

bool Bootstrap::Shutdown()
{
	CLOG_INFO("shutdown");

	if (!W_INPUT.Shutdown())
	{
		CLOG_FATAL("W_INPUT.Shutdown() failed");
		return false;
	}

	if(!W_ENGINE.Shutdown())
	{
		CLOG_FATAL("W_ENGINE.Shutdown() failed");
		return false;
	}

	if(!W_LOG.Shutdown())
	{
		std::cerr << "W_LOG.Shutdown() failed" << std::endl;
		return false;
	}

	return true;
}