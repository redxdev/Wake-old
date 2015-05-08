#pragma once

#include "../Utility/LuaExt.h"
#include "LuaVector.h"

#include <glm/mat2x4.hpp>

#define W_MT_MAT2X4 ("Wake.Matrix2x4")

void PushLuaValue(lua_State* L, const glm::mat2x4& Value);
glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int idx);
int luaopen_matrix2x3(lua_State* L);