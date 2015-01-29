#pragma once

#include <list>

namespace Logging
{
	class LogManager;

	/**
	* Handles log contexts and information specific to a single logger.
	*/
	class Logger
	{
	public:
		/**
		* Construct a new Logger instance.
		*
		* \param Name The name of this logger.
		*/
		Logger(const char* Name)
		{
			this->Name = Name;
		}

		/**
		* Get the name of this logger.
		*
		* \return The name of this logger.
		*/
		inline const char* GetName() const
		{
			return Name;
		}

	private:
		const char* Name;
	};

	/**
	* The global logger. Use this when creating a new logger wouldn't make sense.
	*/
	extern Logger GlobalLogger;
}