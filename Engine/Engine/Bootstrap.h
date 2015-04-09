#pragma once

#include "../Logging/LogMacros.h"
#include "../Engine/Window.h"

// The Wake engine bootstrap looks fairly complex due to the use of macros, but it actually simplifies things a bit.
// Generally, all you need to do is write WAKE_BOOTSTRAP(Main); at the top of your main.cpp, and then put
// WAKE_CALL_BOOTSTRAP(Main, argc, argv); in your main function.
//
// The reason for the bootstrap having all these macros is that the bootstrap should be called in a very specific
// way. This also allows the bootstrap to call itself again if the engine needs to be reinitialized.

#define WAKE_BOOTSTRAP_FUNC_NAME(Name) __wake_bootstrap_##Name
#define WAKE_BOOTSTRAP_FUNC_DECL(Name) void WAKE_BOOTSTRAP_FUNC_NAME(Name) (int ArgC, char** ArgV)
#define WAKE_BOOTSTRAP_OBJ __wake_bootstrap
#define WAKE_BOOTSTRAOP_OPT_OBJ __wake_bootstrap_opt
#define WAKE_BOOTSTRAP_INTERNAL(ArgC, ArgV, Options) \
BootstrapOptions WAKE_BOOTSTRAP_OPT_OBJ; \
{Options} \
Bootstrap WAKE_BOOTSTRAP_OBJ ((ArgC), (ArgV), WAKE_BOOTSTRAP_OPT_OBJ); \
if(!WAKE_BOOTSTRAP_OBJ.Startup()) return;

#define WAKE_CUSTOM_BOOTSTRAP(Name, Options, Code) WAKE_BOOTSTRAP_FUNC_DECL(Name) { WAKE_BOOTSTRAP_INTERNAL(ArgC, ArgV, Options); {Code} WAKE_BOOTSTRAP_OBJ.Run(); }

#define WBS_OPT(Id, Value) WAKE_BOOTSTRAP_OPT_OBJ.Id = (Value);

#define WAKE_BOOTSTRAP(Name) WAKE_CUSTOM_BOOTSTRAP(Name,,)
#define WAKE_BOOTSTRAP_OPT(Name, Options) WAKE_CUSTOM_BOOTSTRAP(Name, Options,)
#define WAKE_CALL_BOOTSTRAP(Name, ArgC, ArgV) WAKE_BOOTSTRAP_FUNC_NAME(Name) ((ArgC), (ArgV))

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