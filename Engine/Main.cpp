#include <iostream>

#include "Utility/Bootstrap.h"
#include "Logging/LogMacros.h"
#include "WakeDefines.h"
#include "Engine/InputManager.h"

void OnRawInput(const Engine::Input& Input)
{
	switch (Input.Type)
	{
	case Engine::EInputType::Keyboard:
		LOG_INFO(Logging::GlobalLogger, "Keyboard " << Engine::ConvertInputModeToString(Input.Mode) << " " << Engine::ConvertKeyboardToString(Input.Keyboard));
		break;

	case Engine::EInputType::Mouse:
		LOG_INFO(Logging::GlobalLogger, "Mouse " << Engine::ConvertInputModeToString(Input.Mode) << " " << Engine::ConvertMouseToString(Input.Mouse));
		break;
	}
}

void Setup()
{
	W_INPUT.OnRawInput.Bind(&OnRawInput);
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