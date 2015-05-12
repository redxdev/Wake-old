#pragma once

#include <glm/mat2x2.hpp>

#include "../Utility/LuaExt.h"

template<typename MatType>
struct MatrixInfo
{
};

template<>
struct MatrixInfo<glm::mat2x2>
{
	static inline int Elements()
	{
		return 4;
	}

	static inline int Columns()
	{
		return 2;
	}

	static inline int Rows()
	{
		return 2;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix2x2";
	}

	static inline const char* TypeName()
	{
		return "Matrix2x2";
	}
};

void PushLuaValue(lua_State* L, const glm::mat2x2& Mat);
glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx);
int luaopen_matrix2x2(lua_State* L);