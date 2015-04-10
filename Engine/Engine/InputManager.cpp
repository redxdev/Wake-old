#include "InputManager.h"
#include "GEngine.h"
#include "Window.h"

CLOG_LOGGER_DEF(InputManager);

InputManager& InputManager::Get()
{
	static InputManager Instance;
	return Instance;
}

bool InputManager::Startup()
{
	CLOG_DEBUG("InputManager Startup");

	W_ENGINE.GetGameWindow().KeyPressed.Bind(this, &InputManager::E_KeyPressed);
	W_ENGINE.GetGameWindow().KeyReleased.Bind(this, &InputManager::E_KeyReleased);
	W_ENGINE.GetGameWindow().MouseButtonPressed.Bind(this, &InputManager::E_MouseButtonPressed);
	W_ENGINE.GetGameWindow().MouseButtonReleased.Bind(this, &InputManager::E_MouseButtonReleased);
	W_ENGINE.GetGameWindow().MouseWheelMoved.Bind(this, &InputManager::E_MouseWheelMoved);

	return true;
}

bool InputManager::Shutdown()
{
	CLOG_DEBUG("InputManager Shutdown");
	OnRawInput.Clear();

	return true;
}

bool InputManager::CreateBinding(const std::string& Name, const InputBinding& Binding)
{
	auto result = BindingMap.emplace(Name, Binding);
	if (!result.second)
	{
		CLOG_INFO("Binding " << Name << " already exists");
		return false;
	}

	return CreateRawBinding(Binding);
}

bool InputManager::CreateRawBinding(const InputBinding& Binding)
{
	return !InputMap.emplace(Binding, InputEvent()).second;
}

InputManager::InputEvent& InputManager::Event(const InputBinding& Binding)
{
	auto found = InputMap.find(Binding);
	if (found != InputMap.end())
		return found->second;

	return Event(Binding);
}

InputManager::InputEvent& InputManager::Event(const std::string& Name)
{
	auto found = BindingMap.find(Name);
	if (found == BindingMap.end())
	{
		CLOG_WARN("Event(\"" << Name << "\") called but binding doesn't exist, using empty event");
		return InvalidInputEvent;
	}

	return Event(found->second);
}

void InputManager::E_KeyPressed(EKeyboardInput Key)
{
	Input KeyInput;
	KeyInput.Type = EInputType::Keyboard;
	KeyInput.Mode = EInputMode::Pressed;
	KeyInput.Code.Keyboard = Key;

	CallRawInput(KeyInput);
}

void InputManager::E_KeyReleased(EKeyboardInput Key)
{
	Input KeyInput;
	KeyInput.Type = EInputType::Keyboard;
	KeyInput.Mode = EInputMode::Released;
	KeyInput.Code.Keyboard = Key;

	CallRawInput(KeyInput);
}

void InputManager::E_MouseButtonPressed(EMouseInput Button)
{
	Input MouseInput;
	MouseInput.Type = EInputType::Mouse;
	MouseInput.Mode = EInputMode::Pressed;
	MouseInput.Code.Mouse = Button;

	CallRawInput(MouseInput);
}

void InputManager::E_MouseButtonReleased(EMouseInput Button)
{
	Input MouseInput;
	MouseInput.Type = EInputType::Mouse;
	MouseInput.Mode = EInputMode::Released;
	MouseInput.Code.Mouse = Button;

	CallRawInput(MouseInput);
}

void InputManager::E_MouseWheelMoved(int32 Amount)
{
	Input MouseInput;
	MouseInput.Type = EInputType::Mouse;
	MouseInput.Mode = EInputMode::Value;
	MouseInput.Code.Mouse = (Amount >= 0 ? EMouseInput::WheelUp : EMouseInput::WheelDown);
	MouseInput.Value = (float)Amount;

	CallRawInput(MouseInput);
}

void InputManager::CallRawInput(const Input& RawInput)
{
	OnRawInput.Call(RawInput);

	InputBinding Binding(RawInput.Type, RawInput.Mode, RawInput.Code);
	auto found = InputMap.find(Binding);
	if (found != InputMap.end())
	{
		found->second.Call(RawInput);
	}
}