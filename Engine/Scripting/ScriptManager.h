#pragma once

#include "../Utility/luaext.h"
#include "../Logging/LogMacros.h"

#define W_SCRIPT (ScriptManager::Get())
#define W_SCRIPT_PATH ("?;?.lua;game/?.lua;lib/?.lua")

class ScriptManager
{
	CLOG_LOGGER_DECL;

public:
	static ScriptManager& Get();

public:
	bool Startup();
	bool Shutdown();

	bool Init(); // called right before running

	void SetPath(const char* Path);

	bool DoFile(const char* Path);

private:
	ScriptManager();
	~ScriptManager();

	lua_State* State = nullptr;
};