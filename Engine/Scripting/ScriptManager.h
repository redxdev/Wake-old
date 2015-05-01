#pragma once

#include <lua.h>

#include "../Logging/LogMacros.h"

#define W_SCRIPT (ScriptManager::Get())

class ScriptManager
{
	CLOG_LOGGER_DECL;

public:
	static ScriptManager& Get();

public:
	bool Startup();
	bool Shutdown();

	bool Init(); // called right before running

private:
	ScriptManager();
	~ScriptManager();

	lua_State* State = nullptr;
};