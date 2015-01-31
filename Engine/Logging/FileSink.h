#pragma once

#include <iostream>
#include <fstream>

#include "LogSink.h"

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

		virtual void OpenSink() override;

		virtual void CloseSink() override;

		virtual void Append(const LogMessage& Message) override;

	private:
		const char* Filename;

		std::ofstream Stream;
	};
}