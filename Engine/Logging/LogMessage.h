#pragma once

#include <string>
#include <time.h>

#include "LogLevel.h"
#include "Logger.h"

namespace Wake
{
	namespace Logging
	{
		/**
		* Used for passing around the parameters of a logging message.
		*/
		struct LogMessage
		{
			LogMessage(const Logger& Log)
				: Log(Log)
			{
			}

			const Logger& Log;

			std::string RawMessage;

			std::string FormattedMessage;

			LogLevel Level;

			time_t Time;
		};
	}
}