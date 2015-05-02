#include "Bootstrap.h"
#include "../Logging/LogManager.h"
#include "../Logging/ConsoleSink.h"
#include "../Logging/FileSink.h"

#ifdef _MSC_VER
#include "../Logging/VSDebugSink.h"
#endif

#include "../Engine/GEngine.h"
#include "../Engine/InputManager.h"

#include "../World/World.h"

#include "../Scripting/ScriptManager.h"

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

	CLOG_DEBUG("Initializing scripting engine");
	if(!W_SCRIPT.Init())
	{
		CLOG_ERROR("W_SCRIPT.Init() failed");
		return;
	}

	CLOG_INFO("Running engine");

	W_ENGINE.Run();
}

bool Bootstrap::Startup()
{
	std::ifstream ConfigTest("config/cfg.lua");
	if (!ConfigTest.good())
	{
		std::cerr << "Bootstrap: Unable to find config/cfg.lua, make sure the working directory is set to Distribution." << std::endl;
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

	if(!W_ENGINE.Startup())
	{
		CLOG_FATAL("W_ENGINE.Startup() failed");
		return false;
	}

	if (!W_INPUT.Startup())
	{
		CLOG_FATAL("W_INPUT.Startup() failed");
		return false;
	}

	if (!W_WORLD.Startup())
	{
		CLOG_FATAL("W_WORLD.Startup() failed");
		return false;
	}

	if (!W_SCRIPT.Startup())
	{
		CLOG_FATAL("W_SCRIPT.Startup() failed");
		return false;
	}

	DidStartup = true;

	return true;
}

bool Bootstrap::Shutdown()
{
	CLOG_INFO("shutdown");

	if (!W_SCRIPT.Shutdown())
	{
		CLOG_FATAL("W_SCRIPT.Shutdown() failed");
		return false;
	}

	if (!W_WORLD.Shutdown())
	{
		CLOG_FATAL("W_WORLD.Shutdown() failed");
		return false;
	}

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