#include <iostream>

#include "Logging/LogMacros.h"
#include "WakeDefines.h"
#include "Engine/InputManager.h"
#include "Engine/GEngine.h"
#include "Engine/Bootstrap.h"

void OnRawInput(const Input& Input)
{
	switch (Input.Type)
	{
	case EInputType::Keyboard:
		LOG_INFO(GlobalLogger, "Keyboard " << ConvertInputModeToString(Input.Mode) << " " << ConvertKeyboardToString(Input.Code.Keyboard));
		break;

	case EInputType::Mouse:
		LOG_INFO(GlobalLogger, "Mouse " << ConvertInputModeToString(Input.Mode) << " " << ConvertMouseToString(Input.Code.Mouse));
		if (Input.Mode == EInputMode::Value)
		{
			LOG_INFO(GlobalLogger, "Value - " << Input.Value);
		}
		break;
	}
}

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
	W_INPUT.OnRawInput.Bind(&OnRawInput);

	W_INPUT.CreateBinding("Exit", INPUT_BIND(Keyboard, Pressed, Escape));
	W_INPUT.Event("Exit").Bind(&OnInput_Exit);
	
	W_INPUT.CreateBinding("Test", INPUT_BIND(Keyboard, Pressed, Space));
	W_INPUT.Event("Test").Bind(&OnInput_Test);
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