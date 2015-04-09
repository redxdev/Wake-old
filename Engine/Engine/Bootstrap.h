#pragma once

#include "../Logging/LogMacros.h"
#include "../Engine/Window.h"

struct BootstrapOptions
{
	bool DefaultLogSinks = true;
	bool DontRunEngine = false;
	WindowOptions WindowOptions;
};

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