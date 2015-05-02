#pragma once

#include "../Logging/LogMacros.h"
#include "../Engine/Window.h"

struct BootstrapOptions
{
	bool DefaultLogSinks = true;
	bool DontRunEngine = false;
};

/**
 * Sets up and starts the engine. You don't need to use this class if you want to do everything yourself, but
 * it is recommended to use this. If you just want to do extra setup, call Startup, do your setup, then
 * call Run.
 */
class Bootstrap
{
	CLOG_LOGGER_DECL;

public:
	Bootstrap(int ArgC, char** ArgV, const BootstrapOptions& Options);
	~Bootstrap();

	bool Startup();

	void Run();

private:
	bool Shutdown();

	BootstrapOptions Options;

	bool DidStartup;
};