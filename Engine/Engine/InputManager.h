#pragma once

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <functional>

#include "../Logging/LogMacros.h"
#include "../Utility/Event.h"
#include "Input.h"

#define W_INPUT (Engine::InputManager::Get())

#define INPUT_BIND(Type, Mode, Code) (Engine::InputBinding((Engine::EInputType::Type), (Engine::EInputMode::Mode), (Engine::E##Type##Input::Code)))

namespace Engine
{
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
			case Engine::EInputType::Keyboard:
				return h ^ std::hash<uint8>()((uint8)Binding.Code.Keyboard << 2);

			case Engine::EInputType::Mouse:
				return h ^ std::hash<uint8>()((uint8)Binding.Code.Mouse << 2);
			}

			return h;
		}
	};

	class InputManager
	{
		CLOG_LOGGER_DECL;
	public:
		typedef Utility::Event<const Input&> InputEvent;

	public:
		static InputManager& Get();

		InputEvent OnRawInput;

		bool Startup();

		bool Shutdown();

		bool CreateBinding(const std::string& Name, const InputBinding& Binding);
		bool CreateRawBinding(const InputBinding& Binding);

		InputEvent& Event(const InputBinding& Binding);
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

		std::unordered_map<InputBinding, InputEvent, InputBindingHash> InputMap;
		std::unordered_map<std::string, InputBinding> BindingMap;

		InputEvent InvalidInputEvent;
	};
}