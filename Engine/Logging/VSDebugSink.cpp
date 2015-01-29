#ifdef _MSC_VER

#include "VSDebugSink.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace Logging
{
	VSDebugSink::~VSDebugSink()
	{
	}

	void VSDebugSink::Append(const LogMessage& Message)
	{
		std::wstring wide = std::wstring(Message.FormattedMessage.begin(), Message.FormattedMessage.end());
		OutputDebugString((wide + L"\n").c_str());
	}
}

#endif