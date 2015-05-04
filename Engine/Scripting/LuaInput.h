#pragma once

#include "../Utility/LuaExt.h"
#include "../Engine/Input.h"

int luaopen_input(lua_State* L);

void PushLuaValue(lua_State* L, EKeyboardInput Key);