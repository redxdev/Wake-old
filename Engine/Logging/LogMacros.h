#pragma once

#include <sstream>

#include "Logger.h"
#include "LogManager.h"
#include "LogLevel.h"

/**
* Declare a logger for use with CLOG_ macros. Use within a class.
*/
#define CLOG_LOGGER_DECL private: static Logger __wlog_logger;

/**
* Define a logger for use with CLOG_ macros. Use within a CPP file.
*
* \param	classname	The name of the class this logger is for.
*/
#define CLOG_LOGGER_DEF(classname) Logger classname::__wlog_logger(#classname);

/**
* Log a message.
*
* \param	logger		The logger object.
* \param	level		The LogLevel to use.
* \param	message		The message to log. This supports the use of stream operators (>> and <<).
*/
#define WAKE_LOG(logger, level, message) { \
	std::stringstream __wlog_sstream; \
	__wlog_sstream << message; \
	Wake::Logging::LogManager::getInstance().log(logger, level, __wlog_sstream.str()); \
}

/**
* Log a message to a class logger.
*
* \param	level		The LogLevel to use.
* \param	message		The message to log. This supports the use of stream operators (>> and <<).
*/
#define WAKE_CLOG(level, message) WAKE_LOG(__wlog_logger, level, message)

#ifndef WAKE_LOG_NO_TRACE
#define CLOG_TRACE(message) WAKE_CLOG(Wake::Logging::LogLevel::Trace, message)
#define LOG_TRACE(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Trace, message)
#else
#define CLOG_TRACE(message)
#define LOG_TRACE(logger, message)
#endif

#ifndef WAKE_LOG_NO_DEBUG
#define CLOG_DEBUG(message) WAKE_CLOG(Wake::Logging::LogLevel::Debug, message)
#define LOG_DEBUG(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Debug, message)
#else
#define CLOG_DEBUG(message)
#define LOG_DEBUG(logger, message)
#endif

#ifndef WAKE_LOG_NO_INFO
#define CLOG_INFO(message) WAKE_CLOG(Wake::Logging::LogLevel::Info, message)
#define LOG_INFO(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Info, message)
#else
#define CLOG_INFO(message)
#define LOG_INFO(logger, message)
#endif

#ifndef WAKE_LOG_NO_WARN
#define CLOG_WARN(message) WAKE_CLOG(Wake::Logging::LogLevel::Warn, message)
#define LOG_WARN(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Warn, message)
#else
#define CLOG_WARN(message)
#define LOG_WARN(logger, message)
#endif

#ifndef WAKE_LOG_NO_ERROR
#define CLOG_ERROR(message) WAKE_CLOG(Wake::Logging::LogLevel::Error, message)
#define LOG_ERROR(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Error, message)
#else
#define CLOG_ERROR(message)
#define LOG_ERROR(logger, message)
#endif

#ifndef WAKE_LOG_NO_FATAL
#define CLOG_FATAL(message) WAKE_CLOG(Wake::Logging::LogLevel::Fatal, message)
#define LOG_FATAL(logger, message) WAKE_LOG(logger, Wake::Logging::LogLevel::Fatal, message)
#else
#define CLOG_FATAL(message)
#define LOG_FATAL(logger, message)
#endif