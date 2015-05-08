#pragma once

#include "../Utility/LuaExt.h"
#include "LuaVector.h"

#include <glm/mat2x3.hpp>

#define W_MT_MAT2X3 ("Wake.Matrix2x3")

void PushLuaValue(lua_State* L, const glm::mat2x3& Value);
glm::mat2x3* luaW_checkmatrix2x2(lua_State* L, int idx);
int luaopen_matrix2x3(lua_State* L);