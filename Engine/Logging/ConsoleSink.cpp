#include "ConsoleSink.h"

#include <iostream>

namespace Wake
{
	namespace Logging
	{
		ConsoleSink::~ConsoleSink()
		{
		}

		void ConsoleSink::Append(const LogMessage& Message)
		{
			switch (Message.Level)
			{
			case LogLevel::Trace:
			case LogLevel::Debug:
			case LogLevel::Info:
				std::cout << Message.FormattedMessage << std::endl;
				break;

			case LogLevel::Warn:
			case LogLevel::Error:
			case LogLevel::Fatal:
				std::cerr << Message.FormattedMessage << std::endl;
				break;
			}
		}
	}
}