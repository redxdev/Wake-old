#include "LogLevel.h"

namespace Logging
{
	const char* LogLevelToString(LogLevel Level)
	{
		switch (Level)
		{
		case LogLevel::Trace:
			return "Trace";

		case LogLevel::Debug:
			return "Debug";

		case LogLevel::Info:
			return "Info";

		case LogLevel::Warn:
			return "Warn";

		case LogLevel::Error:
			return "Error";

		case LogLevel::Fatal:
			return "Fatal";
		}

		return "Unknown";
	}
}