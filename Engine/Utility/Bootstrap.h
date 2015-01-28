#pragma once

#include "../Logging/LogMacros.h"

#define WAKE_BOOTSTRAP_FUNC_NAME(Name) __wake_bootstrap_##Name
#define WAKE_BOOTSTRAP_FUNC_DECL(Name) void WAKE_BOOTSTRAP_FUNC_NAME(Name) (int ArgC, char** ArgV)
#define WAKE_BOOTSTRAP_OBJ __wake_bootstrap
#define WAKE_BOOTSTRAOP_OPT_OBJ __wake_bootstrap_opt
#define WAKE_BOOTSTRAP_INTERNAL(ArgC, ArgV, Options) Wake::Utility::BootstrapOptions WAKE_BOOTSTRAP_OPT_OBJ; \
{Options} \
Wake::Utility::Bootstrap WAKE_BOOTSTRAP_OBJ ((ArgC), (ArgV), WAKE_BOOTSTRAP_OPT_OBJ);

#define WAKE_CUSTOM_BOOTSTRAP(Name, Options, Code) WAKE_BOOTSTRAP_FUNC_DECL(Name) { WAKE_BOOTSTRAP_INTERNAL(ArgC, ArgV, Options); {Code} WAKE_BOOTSTRAP_OBJ.Run(); }
#define WBS_OPT(Id, Value) WAKE_BOOTSTRAP_OPT_OBJ.Id = (Value);
#define WAKE_BOOTSTRAP(Name) WAKE_CUSTOM_BOOTSTRAP(Name,,)
#define WAKE_CALL_BOOTSTRAP(Name, ArgC, ArgV) WAKE_BOOTSTRAP_FUNC_NAME(Name) ((ArgC), (ArgV))

namespace Wake
{
	namespace Utility
	{
		struct BootstrapOptions
		{
			bool AutoRegisterLogSinks = true;
			bool DontRunEngine = false;
		};

		class Bootstrap
		{
			CLOG_LOGGER_DECL;

		public:
			Bootstrap(int ArgC, char** ArgV, const BootstrapOptions& Options);
			~Bootstrap();

			void Run();

		private:
			void Startup();
			void Shutdown();

			BootstrapOptions Options;
		};
	}
}