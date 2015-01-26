#include "Logging/LogMacros.h"
#include "Logging/LogManager.h"
#include "Logging/ConsoleSink.h"
#include "Logging/FileSink.h"

#ifdef _MSC_VER
#include "Logging/VSDebugSink.h"
#endif

using namespace Wake::Logging;

int main(int argc, char** argv)
{
	LogManager::GetInstance().Startup();

	LogManager::GetInstance().AddSink(new ConsoleSink());
	LogManager::GetInstance().AddSink(new FileSink("logs/engine.log"));

#ifdef _MSC_VER
	LogManager::GetInstance().AddSink(new VSDebugSink());
#endif

	LOG_INFO(GlobalLogger, "Session started");

	LOG_INFO(GlobalLogger, "Session ended");

	LogManager::GetInstance().Shutdown();
}