#pragma once

#include <string>

#include "LogMessage.h"

namespace Logging
{
	/**
	* Interface for logging outputs.
	*/
	class LogSink
	{
	public:
		virtual ~LogSink();

		/**
		* Open this sink. Any file open or related operations should be done here.
		*/
		virtual void OpenSink();

		/**
		* Close this sink. Any file close or related operations should be done here.
		*/
		virtual void CloseSink();


		/**
		* Append a message to this sink.
		*
		* \param Message The LogMessage to append.
		*/
		virtual void Append(const LogMessage& Message) = 0;
	};
}