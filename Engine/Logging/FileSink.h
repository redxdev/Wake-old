#pragma once

#include <iostream>
#include <fstream>

#include "LogSink.h"

namespace Wake
{
	namespace Logging
	{
		/**
		* Logging sink for output to a file.
		*/
		class FileSink : public LogSink
		{
		public:
			/**
			* Constructor for a new FileSink instance.
			*
			* \param Filename The file to write.
			*/
			FileSink(const char* Filename);

			virtual ~FileSink();

			virtual void OpenSink();

			virtual void CloseSink();

			virtual void Append(const LogMessage& Message);

		private:
			const char* Filename;

			std::ofstream Stream;
		};
	}
}