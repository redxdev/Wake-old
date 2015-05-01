#include "ScriptManager.h"

#include <lua.h>
#include <lauxlib.h>

CLOG_LOGGER_DEF(ScriptManager);

ScriptManager& ScriptManager::Get()
{
	static ScriptManager Instance;
	return Instance;
}

ScriptManager::ScriptManager()
{
	
}

ScriptManager::~ScriptManager()
{
	
}

bool ScriptManager::Startup()
{
	CLOG_DEBUG("ScriptManager Startup");

	State = luaL_newstate();

	return true;
}

bool ScriptManager::Shutdown()
{
	CLOG_DEBUG("ScriptManager Shutdown");

	lua_close(State);
	State = nullptr;
}

bool ScriptManager::Init()
{
	CLOG_DEBUG("ScriptManager Init");
}