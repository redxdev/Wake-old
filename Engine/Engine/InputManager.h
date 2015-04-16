#pragma once

#include <unordered_map>

#include "../Logging/LogMacros.h"
#include "../Utility/Event.h"
#include "Input.h"

#define W_INPUT (InputManager::Get())

/**
 * Shorthand for creating an InputBinding.
 */
#define INPUT_BIND(Type, Mode, Code) (InputBinding((EInputType::Type), (EInputMode::Mode), (E##Type##Input::Code)))

/**
 * Represents a type of input, used by the input manager.
 */
struct InputBinding
{
	InputBinding()
	{
	}

	InputBinding(EInputType Type, EInputMode Mode, InputCode Code)
		: Type(Type), Mode(Mode), Code(Code)
	{
	}
	
	EInputType Type;
	EInputMode Mode;
	InputCode Code;

	bool operator==(const InputBinding& Other) const
	{
		return Type == Other.Type && Mode == Other.Mode &&
			(Type == EInputType::Keyboard ? Code.Keyboard == Other.Code.Keyboard : Code.Mouse == Other.Code.Mouse);
	}
};

struct InputBindingHash
{
	std::size_t operator()(const InputBinding& Binding)
	{
		std::size_t h = std::hash<uint8>()((uint8)Binding.Type) ^ std::hash<uint8>()((uint8)Binding.Mode << 1);
		switch (Binding.Type)
		{
		case EInputType::Keyboard:
			return h ^ std::hash<uint8>()((uint8)Binding.Code.Keyboard << 2);

		case EInputType::Mouse:
			return h ^ std::hash<uint8>()((uint8)Binding.Code.Mouse << 2);
		}

		return h;
	}
};

class InputManager
{
	CLOG_LOGGER_DECL;
public:
	typedef W_EVENT(InputEvent, const Input&);

public:
	static InputManager& Get();

	InputEvent OnRawInput;

	bool Startup();

	bool Shutdown();

	/**
	 * Create a new named binding, or add to an existing one.
	 */
	void Bind(const std::string& Name, const InputBinding& Binding);

	/**
	 * Retrieve the event for a named binding.
	 */
	InputEvent& Event(const std::string& Name);

private:
	InputManager() {}
	~InputManager() {}

	void E_KeyPressed(EKeyboardInput Key);
	void E_KeyReleased(EKeyboardInput Key);

	void E_MouseButtonPressed(EMouseInput Button);
	void E_MouseButtonReleased(EMouseInput Button);

	void E_MouseWheelMoved(int32 Amount);

	void CallRawInput(const Input& RawInput);

	std::unordered_map<std::string, InputEvent*> EventMap;
	std::unordered_map<InputBinding, std::list<InputEvent*>, InputBindingHash> InputMap;

	InputEvent InvalidInputEvent;
};