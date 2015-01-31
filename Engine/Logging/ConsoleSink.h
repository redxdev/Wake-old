#pragma once

#include <iostream>

#include "LogSink.h"

namespace Logging
{
	/**
	* Logging sink for output to the console.
	*/
	class ConsoleSink : public LogSink
	{
	public:
		virtual ~ConsoleSink();

		virtual void Append(const LogMessage& Message) override;
	};
}