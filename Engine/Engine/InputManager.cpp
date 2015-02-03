#include "InputManager.h"
#include "GEngine.h"
#include "Window.h"

namespace Engine
{
	CLOG_LOGGER_DEF(Engine::InputManager);

	InputManager& InputManager::Get()
	{
		static InputManager Instance;
		return Instance;
	}

	bool InputManager::Startup()
	{
		CLOG_INFO("InputManager Startup");

		W_ENGINE.GetGameWindow().KeyPressed.Bind(this, &InputManager::W_KeyPressed);
		W_ENGINE.GetGameWindow().KeyReleased.Bind(this, &InputManager::W_KeyReleased);

		return true;
	}

	void InputManager::W_KeyPressed(EKeyboardInput Key)
	{
		Input KeyInput;
		KeyInput.Type = EInputType::Keyboard;
		KeyInput.Mode = EInputMode::Pressed;
		KeyInput.Keyboard = Key;

		OnRawInput.Call(KeyInput);
	}

	void InputManager::W_KeyReleased(EKeyboardInput Key)
	{
		Input KeyInput;
		KeyInput.Type = EInputType::Keyboard;
		KeyInput.Mode = EInputMode::Released;
		KeyInput.Keyboard = Key;

		OnRawInput.Call(KeyInput);
	}

}