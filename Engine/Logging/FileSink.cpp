#include "FileSink.h"

namespace Logging
{
	FileSink::FileSink(const char* Filename)
	{
		this->Filename = Filename;
	}

	FileSink::~FileSink()
	{
	}

	void FileSink::OpenSink()
	{
		Stream.open(Filename, std::ios::app);
	}

	void FileSink::CloseSink()
	{
		Stream.close();
	}

	void FileSink::Append(const LogMessage& Message)
	{
		Stream << Message.FormattedMessage << std::endl;
	}
}