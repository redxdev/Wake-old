#include "ScriptManager.h"

#include "../Utility/luaext.h"
#include "../Logging/LuaLogging.h"
#include "../Engine/GEngine.h"

#include "LuaLibRegistry.h"

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

	luaL_openlibs(State);

	W_INT_LLREGISTRY.RegisterAll(State);

	SetPath(W_SCRIPT_PATH);

	return true;
}

bool ScriptManager::Shutdown()
{
	CLOG_DEBUG("ScriptManager Shutdown");

	lua_close(State);
	State = nullptr;

	return true;
}

bool ScriptManager::Init()
{
	CLOG_DEBUG("ScriptManager Init");

	CLOG_INFO("Loading configuration");
	if(!DoFile("config/cfg.lua"))
		return false;

	// Window Config
	lua_getglobal(State, "config");
	if (!lua_istable(State, -1))
	{
		CLOG_ERROR("config is not a table");
		return false;
	}

	CLOG_INFO("Running init scripts");
	if (!DoFile("lib/init.lua"))
		return false;

	if (!DoFile("game/init.lua"))
		return false;

	return true;
}

void ScriptManager::SetPath(const char* Path)
{
	CLOG_DEBUG("Setting lua path to " << Path);
	lua_getglobal(State, "package");
	lua_pushstring(State, Path);
	lua_setfield(State, -2, "path");
	lua_pop(State, 1);
}

bool ScriptManager::DoFile(const char* Path)
{
	CLOG_DEBUG("Executing script " << Path);
	if (luaL_loadfile(State, Path) != 0)
	{
		CLOG_ERROR("Unable to run script: " << lua_tostring(State, -1));
		return false;
	}

	if (lua_pcall(State, 0, 0, 0) != 0)
	{
		CLOG_ERROR("Unable to run script: " << lua_tostring(State, -1));
		return false;
	}

	return true;
}

bool ScriptManager::IsActive() const
{
	return State != nullptr;
}

lua_State* ScriptManager::GetState()
{
	return State;
}