#include <iostream>

#include "Utility/Bootstrap.h"
#include "Logging/LogMacros.h"
#include "WakeDefines.h"

WAKE_BOOTSTRAP(Main)

int main(int argc, char** argv)
{
	WAKE_CALL_BOOTSTRAP(Main, argc, argv);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}