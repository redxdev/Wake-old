#pragma once

#include <list>
#include <string>
#include <tuple>

#include "LogLevel.h"

namespace Wake
{
	namespace Logging
	{
		class Logger;

		class LogSink;

		/**
		* Manages the logging hierarchy and LogSinks.
		*/
		class LogManager
		{
		public:
			/**
			* Get the singleton instance of the LogManager.
			*/
			static LogManager& Get();

			/**
			* Startup the log manager.
			*/
			void Startup();

			/**
			* Shutdown the log manager.
			*/
			void Shutdown();

			/**
			* Add a log sink.
			*
			* \param Sink The log sink to add.
			*/
			void AddSink(LogSink* Sink);

			/**
			* Log a message. It is preferred to use the LOG_, CLOG_, or WAKE_LOG series of macros.
			*
			* \param Logger The Logger that is logging the message.
			* \param Level The LogLevel to use.
			* \param Message The message to log.
			*/
			void Log(const Logger& Log, LogLevel Level, const std::string& Message);

		private:
			LogManager();
			~LogManager();

			LogManager(const LogManager& Copy) {}

			std::list<LogSink*> Sinks;
		};
	}
}