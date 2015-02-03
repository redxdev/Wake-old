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



		return true;
	}

}