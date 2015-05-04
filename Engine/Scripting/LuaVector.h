#pragma once

#include "../Utility/LuaExt.h"

#include <glm/vec2.hpp>

// Pushes a copy
void PushLuaValue(lua_State* L, const glm::vec2& Value);

int luaopen_vector2(lua_State* L);