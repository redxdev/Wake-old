#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

#include "Types.h"

template<typename T, typename... Arguments>
void PushLuaValue(lua_State* L, T First, Arguments... Args)
{
	PushLuaValue(L, First);
	PushLuaValue(L, Args...);
}

// do nothing
void PushLuaValue(lua_State* L);

// values
void PushLuaValue(lua_State* L, float Number);
void PushLuaValue(lua_State* L, uint8 Number);
void PushLuaValue(lua_State* L, uint16 Number);
void PushLuaValue(lua_State* L, uint32 Number);
void PushLuaValue(lua_State* L, uint64 Number);
void PushLuaValue(lua_State* L, int8 Number);
void PushLuaValue(lua_State* L, int16 Number);
void PushLuaValue(lua_State* L, int32 Number);
void PushLuaValue(lua_State* L, int64 Number);
void PushLuaValue(lua_State* L, const char* String);
void PushLuaValue(lua_State* L, const std::string& String);
void PushLuaValue(lua_State* L, bool Boolean);