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

void InputManager::Bind(const std::string& Name, const InputBinding& Binding)
{
	auto EventItr = EventMap.find(Name);
	if (EventItr == EventMap.end())
	{
		EventMap.emplace(Name, new InputEvent());
		EventItr = EventMap.find(Name); // TODO: fixme
	}

	auto InputItr = InputMap.find(Binding);
	if (InputItr == InputMap.end())
	{
		InputMap.emplace(Binding, std::list<InputEvent*>());
		InputItr = InputMap.find(Binding);
	}

	InputItr->second.push_back(EventItr->second);
}

InputManager::InputEvent& InputManager::Event(const std::string& Name)
{
	auto found = EventMap.find(Name);
	if (found == EventMap.end())
	{
		CLOG_WARN("Event(\"" << Name << "\") called but binding doesn't exist, using empty event");
		return InvalidInputEvent;
	}

	return *found->second;
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
		for (auto Event : found->second)
		{
			Event->Call(RawInput);
		}
	}
}