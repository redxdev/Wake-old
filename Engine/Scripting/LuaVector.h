#pragma once

#include "../Utility/LuaExt.h"

#include <glm/vec2.hpp>

// Pushes a copy, not the value
void PushLuaValue(lua_State* L, const glm::vec2& Value);

glm::vec2* luaW_checkvector2(lua_State* L, int idx);

int luaopen_vector2(lua_State* L);