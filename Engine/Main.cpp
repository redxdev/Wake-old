#include <iostream>

#include "WakeDefines.h"
#include "Engine/Bootstrap.h"
#include "Game/Setup.h"

// Create the bootstrap in a separate scope since the bootstrap's shutdown
// happens when it gets destroyed (falls out of scope).
void RunEngine(int ArgC, char** ArgV)
{
	// Set some options
	BootstrapOptions Options;
	Options.DefaultLogSinks = true;
	Options.DontRunEngine = false;

	// Create the bootstrap
	Bootstrap Engine(ArgC, ArgV, Options);
	if (!Engine.Startup())
	{
		// Something went wrong :(
		return;
	}

	// Run our custom setup
	GameSetup();

	// Run the engine
	Engine.Run();

	// Everything is cleaned up when the bootstrap falls out of scope.
	GameTeardown();
}

int main(int ArgC, char** ArgV)
{
	// Run the engine
	RunEngine(ArgC, ArgV);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}