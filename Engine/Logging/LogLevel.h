#pragma once
#include "../Utility/Types.h"

/**
 * Represents the log level.
 */
enum class LogLevel : uint8
{
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

/**
 * Convert a LogLevel enum to a string.
 *
 * \param Level The LogLevel to convert.
 */
const char* LogLevelToString(LogLevel Level);