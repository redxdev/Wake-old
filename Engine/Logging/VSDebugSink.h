#pragma once

#ifdef _MSC_VER

#include "LogSink.h"

namespace Logging
{
	/**
	* LogSink for output to visual studio's debugger.
	*/
	class VSDebugSink : public LogSink
	{
	public:
		virtual ~VSDebugSink();

		virtual void Append(const LogMessage& Message) override;
	};
}

#endif