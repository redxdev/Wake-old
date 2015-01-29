#include "ConsoleSink.h"

#include <iostream>

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