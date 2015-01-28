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
			std::cout << Message.FormattedMessage << std::endl;
		}
	}
}