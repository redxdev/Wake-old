#include <iostream>

#include "Utility/Bootstrap.h"
#include "Logging/LogMacros.h"
#include "WakeDefines.h"
#include "Engine/InputManager.h"
#include "Engine/GEngine.h"

void OnRawInput(const Engine::Input& Input)
{
	switch (Input.Type)
	{
	case Engine::EInputType::Keyboard:
		LOG_INFO(Logging::GlobalLogger, "Keyboard " << Engine::ConvertInputModeToString(Input.Mode) << " " << Engine::ConvertKeyboardToString(Input.Code.Keyboard));
		break;

	case Engine::EInputType::Mouse:
		LOG_INFO(Logging::GlobalLogger, "Mouse " << Engine::ConvertInputModeToString(Input.Mode) << " " << Engine::ConvertMouseToString(Input.Code.Mouse));
		if (Input.Mode == Engine::EInputMode::Value)
		{
			LOG_INFO(Logging::GlobalLogger, "Value - " << Input.Value);
		}
		break;
	}
}

void OnInput_Test(const Engine::Input& Input)
{
	LOG_INFO(Logging::GlobalLogger, "Hello!");
}

void OnInput_Exit(const Engine::Input& Input)
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