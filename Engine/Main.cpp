#include <iostream>

#include "WakeDefines.h"

#include "Logging/LogMacros.h"

#include "Engine/InputManager.h"
#include "Engine/GEngine.h"
#include "Engine/Bootstrap.h"

#include "World/World.h"

class TestActor : public Actor
{
public:
	TestActor(ActorID Id, bool StartActive)
		: Actor(Id, StartActive)
	{
	}

	virtual void Spawn() override
	{
		LOG_INFO(GlobalLogger, "TestActor spawned with an id of " << GetActorID() << "!");
	}

	virtual void Destroy() override
	{
		LOG_INFO(GlobalLogger, "TestActor is being destroyed with an id of " << GetActorID() << "!");
	}
};

void OnInput_Test(const Input& Input)
{
	LOG_INFO(GlobalLogger, "Hello!");
}

void OnInput_Exit(const Input& Input)
{
	W_ENGINE.Stop();
}

void Setup()
{
	W_INPUT.CreateBinding("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
	
	W_INPUT.CreateBinding("Test", INPUT_BIND(Keyboard, Pressed, Space));
	W_INPUT.Event("Test").Bind(&OnInput_Test);

	TestActor* A1 = W_WORLD.SpawnActor<TestActor>();
	TestActor* A2 = W_WORLD.SpawnActor<TestActor>();

	W_WORLD.Destroy(A1);
	W_WORLD.Destroy(A2->GetActorID());

	for (int i = 0; i < 10; ++i)
	{
		W_WORLD.SpawnActor<TestActor>(); // these would leak, except that the world cleans up everything on engine shutdown
	}
}

WAKE_CUSTOM_BOOTSTRAP(
	Main,
	WBS_OPT(WindowOptions.AntiAliasing, 4),
	Setup();
)

int main(int argc, char** argv)
{
	WAKE_CALL_BOOTSTRAP(Main, argc, argv);

#ifdef WAKE_EXIT_PAUSE
	std::cout << "Press return to exit..." << std::endl;
	getchar();
#endif
	return 0;
}