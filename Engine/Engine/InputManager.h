#pragma once

#include "../Logging/LogMacros.h"
#include "../Utility/Event.h"
#include "Input.h"

#define W_INPUT (Engine::InputManager::Get())

namespace Engine
{
	class InputManager
	{
		CLOG_LOGGER_DECL;
	public:
		static InputManager& Get();

		W_EVENT(OnRawInput, const Input&);

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
	};
}