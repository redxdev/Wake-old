#include "luaext.h"

#include <assert.h>

void PushLuaValue(lua_State* L)
{
}

void PushLuaValue(lua_State* L, float Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, uint8 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, uint16 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, uint32 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, uint64 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, int8 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, int16 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, int32 Number)
{
	lua_pushnumber(L, Number);
}

void PushLuaValue(lua_State* L, int64 Number)
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