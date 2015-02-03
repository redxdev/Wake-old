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

	private:
		InputManager() {}
		~InputManager() {}

		void W_KeyPressed(EKeyboardInput Key);
		void W_KeyReleased(EKeyboardInput Key);
	};
}