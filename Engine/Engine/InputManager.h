#pragma once

#include <unordered_map>

#include "../Logging/LogMacros.h"
#include "../Utility/Event.h"
#include "Input.h"

#define W_INPUT (Engine::InputManager::Get())

namespace Engine
{
	struct InputBinding
	{
		EInputType Type;
		EInputMode Mode;
		InputCode Code;
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

	private:
		InputManager() {}
		~InputManager() {}

		void E_KeyPressed(EKeyboardInput Key);
		void E_KeyReleased(EKeyboardInput Key);

		void E_MouseButtonPressed(EMouseInput Button);
		void E_MouseButtonReleased(EMouseInput Button);

		void E_MouseWheelMoved(int32 Amount);

		std::unordered_map<InputBinding, InputEvent> InputMap;
		std::unordered_map<std::string, InputBinding> BindingMap;
	};
}