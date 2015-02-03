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

		W_ENGINE.GetGameWindow().KeyPressed.Bind(this, &InputManager::E_KeyPressed);
		W_ENGINE.GetGameWindow().KeyReleased.Bind(this, &InputManager::E_KeyReleased);
		W_ENGINE.GetGameWindow().MouseButtonPressed.Bind(this, &InputManager::E_MouseButtonPressed);
		W_ENGINE.GetGameWindow().MouseButtonReleased.Bind(this, &InputManager::E_MouseButtonReleased);

		return true;
	}

	void InputManager::E_KeyPressed(EKeyboardInput Key)
	{
		Input KeyInput;
		KeyInput.Type = EInputType::Keyboard;
		KeyInput.Mode = EInputMode::Pressed;
		KeyInput.Keyboard = Key;

		OnRawInput.Call(KeyInput);
	}

	void InputManager::E_KeyReleased(EKeyboardInput Key)
	{
		Input KeyInput;
		KeyInput.Type = EInputType::Keyboard;
		KeyInput.Mode = EInputMode::Released;
		KeyInput.Keyboard = Key;

		OnRawInput.Call(KeyInput);
	}

	void InputManager::E_MouseButtonPressed(EMouseInput Button)
	{
		Input MouseInput;
		MouseInput.Type = EInputType::Mouse;
		MouseInput.Mode = EInputMode::Pressed;
		MouseInput.Mouse = Button;

		OnRawInput.Call(MouseInput);
	}

	void InputManager::E_MouseButtonReleased(EMouseInput Button)
	{
		Input MouseInput;
		MouseInput.Type = EInputType::Mouse;
		MouseInput.Mode = EInputMode::Released;
		MouseInput.Mouse = Button;

		OnRawInput.Call(MouseInput);
	}
}