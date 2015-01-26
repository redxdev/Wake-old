#include "LogManager.h"

#include <assert.h>
#include <time.h>
#include <sstream>
#include <tuple>
#include <ctime>

#include "LogSink.h"
#include "Logger.h"

namespace Wake
{
	namespace Logging
	{
		LogManager& LogManager::GetInstance()
		{
			static LogManager Instance;
			return Instance;
		}

		LogManager::LogManager()
		{
		}

		LogManager::~LogManager()
		{
		}

		void LogManager::Startup()
		{
			assert(Sinks.size() == 0 && "Trying to start up LogManager when sinks have already been registered");
		}

		void LogManager::Shutdown()
		{
			for (auto it = Sinks.begin(); it != Sinks.end();)
			{
				(*it)->CloseSink();
				delete *it;
				it = Sinks.erase(it);
			}

			assert(Sinks.size() == 0 && "mSinks is not zero after being cleared");
		}

		void LogManager::AddSink(LogSink* Sink)
		{
			Sinks.push_back(Sink);
			Sink->OpenSink();
		}

		void LogManager::Log(const Logger& Log, LogLevel Level, const std::string& Message)
		{
			assert(Sinks.size() > 0 && "No sinks to log to, possible leak after LogManager shutdown?");

			LogMessage LoggedMessage(Log);
			LoggedMessage.Time = time(NULL);
			LoggedMessage.Level = Level;
			LoggedMessage.RawMessage = Message;

			struct std::tm* tm = localtime(&LoggedMessage.Time);
			char dateTime[20] = "";
			std::strftime(dateTime, 20, "%Y/%m/%d %H:%M:%S", tm);

			std::stringstream format_stream;
			format_stream
				<< dateTime
				<< " [" << LogLevelToString(LoggedMessage.Level) << "] "
				<< LoggedMessage.Log.GetName()
				<< " - " << LoggedMessage.RawMessage;
			LoggedMessage.FormattedMessage = format_stream.str();

			for (auto it = Sinks.begin(); it != Sinks.end(); ++it)
			{
				(*it)->Append(LoggedMessage);
			}
		}
	}
}