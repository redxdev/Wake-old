#pragma once

#include "../Utility/LuaExt.h"
#include "LuaVector.h"

#include <glm/mat2x2.hpp>

#define W_MT_MAT2X2 ("Wake.Matrix2x2")

void PushLuaValue(lua_State* L, const glm::mat2x2& Value);
glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx);
int luaopen_matrix2x2(lua_State* L);