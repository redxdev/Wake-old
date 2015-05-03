#include "luaext.h"

#include <assert.h>

template<typename T, typename... Arguments>
void PushLuaValue(lua_State* L, T First, Arguments Args)
{
	PushLuaValue(L, First);
	PushLuaValue(L, Args...);
}

void PushLuaValue(lua_State* L, float Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, int Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, const char* String)
{
	lua_pushstring(L, String);
}

void PushLuaValue(lua_State* L, std::string String)
{
	lua_pushstring(L, String.c_str());
}

void PushLuaValue(lua_State* L, bool Boolean)
{
	lua_pushboolean(L, Boolean);
}