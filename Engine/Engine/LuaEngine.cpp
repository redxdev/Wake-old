#include "LuaEngine.h"

#include "GEngine.h"

#include "../Scripting/LuaLibRegistry.h"

static int l_stop(lua_State* L)
{
	W_ENGINE.Stop();
	return 0;
}

static int l_isRunning(lua_State* L)
{
	lua_pushboolean(L, W_ENGINE.IsRunning());
	return 1;
}

static int l_getDeltaTime(lua_State* L)
{
	lua_pushnumber(L, W_ENGINE.GetDeltaTime());
	return 1;
}

static const struct luaL_reg enginelib_f[] = {
	{ "stop", l_stop },
	{ "isRunning", l_isRunning },
	{ "getDeltaTime", l_getDeltaTime },
	{NULL, NULL}
};

int luaopen_engine(lua_State* L)
{
	luaL_register(L, "engine", enginelib_f);
	return 1;
}

W_REGISTER_LUA_LIB(luaopen_engine);