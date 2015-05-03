#pragma once

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

template<typename T, typename... Arguments>
void PushLuaValue(lua_State* L, T First, Arguments Args);

void PushLuaValue(lua_State* L, float Number);
void PushLuaValue(lua_State* L, int Number);
void PushLuaValue(lua_State* L, const char* String);
void PushLuaValue(lua_State* L, std::string String);
void PushLuaValue(lua_State* L, bool Boolean);