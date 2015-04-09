#pragma once

#include <sstream>

#include "Logger.h"
#include "LogManager.h"
#include "LogLevel.h"

#define CLOG_INSTANCE __wlog_class_logger

/**
* Declare a logger for use with CLOG_ macros. Use within a class.
*/
#define CLOG_LOGGER_DECL private: static Logger CLOG_INSTANCE;

/**
* Define a logger for use with CLOG_ macros. Use within a CPP file.
*
* \param classname The name of the class this logger is for.
*/
#define CLOG_LOGGER_DEF(classname) Logger classname::CLOG_INSTANCE(#classname);

/**
* Log a message.
*
* \param logger The logger object.
* \param level The LogLevel to use.
* \param message The message to log. This supports the use of stream operators (>> and <<).
*/
#define WAKE_LOG(logger, level, message) \
{ \
	std::stringstream __wlog_ls_sstream; \
	(__wlog_ls_sstream << message); \
	W_LOG.Log(logger, level, __wlog_ls_sstream.str()); \
}

/**
* Log a message to a class logger.
*
* \param level The LogLevel to use.
* \param message The message to log. This supports the use of stream operators (>> and <<).
*/
#define WAKE_CLOG(level, message) WAKE_LOG(CLOG_INSTANCE, level, message)

#ifndef WAKE_LOG_NO_TRACE
#define CLOG_TRACE(message) WAKE_CLOG(LogLevel::Trace, message)
#define LOG_TRACE(logger, message) WAKE_LOG(logger, LogLevel::Trace, message)
#else
#define CLOG_TRACE(message)
#define LOG_TRACE(logger, message)
#endif

#ifndef WAKE_LOG_NO_DEBUG
#define CLOG_DEBUG(message) WAKE_CLOG(LogLevel::Debug, message)
#define LOG_DEBUG(logger, message) WAKE_LOG(logger, LogLevel::Debug, message)
#else
#define CLOG_DEBUG(message)
#define LOG_DEBUG(logger, message)
#endif

#ifndef WAKE_LOG_NO_INFO
#define CLOG_INFO(message) WAKE_CLOG(LogLevel::Info, message)
#define LOG_INFO(logger, message) WAKE_LOG(logger, LogLevel::Info, message)
#else
#define CLOG_INFO(message)
#define LOG_INFO(logger, message)
#endif

#ifndef WAKE_LOG_NO_WARN
#define CLOG_WARN(message) WAKE_CLOG(LogLevel::Warn, message)
#define LOG_WARN(logger, message) WAKE_LOG(logger, LogLevel::Warn, message)
#else
#define CLOG_WARN(message)
#define LOG_WARN(logger, message)
#endif

#ifndef WAKE_LOG_NO_ERROR
#define CLOG_ERROR(message) WAKE_CLOG(LogLevel::Error, message)
#define LOG_ERROR(logger, message) WAKE_LOG(logger, LogLevel::Error, message)
#else
#define CLOG_ERROR(message)
#define LOG_ERROR(logger, message)
#endif

#ifndef WAKE_LOG_NO_FATAL
#define CLOG_FATAL(message) WAKE_CLOG(LogLevel::Fatal, message)
#define LOG_FATAL(logger, message) WAKE_LOG(logger, LogLevel::Fatal, message)
#else
#define CLOG_FATAL(message)
#define LOG_FATAL(logger, message)
#endif