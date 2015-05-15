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

template<>
struct MatrixInfo<glm::mat2x3>
{
	static inline int Elements()
	{
		return 6;
	}

	static inline int Columns()
	{
		return 3;
	}

	static inline int Rows()
	{
		return 2;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix2x3";
	}

	static inline const char* TypeName()
	{
		return "Matrix2x3";
	}
};

template<>
struct MatrixInfo<glm::mat2x4>
{
	static inline int Elements()
	{
		return 8;
	}

	static inline int Columns()
	{
		return 4;
	}

	static inline int Rows()
	{
		return 2;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix2x4";
	}

	static inline const char* TypeName()
	{
		return "Matrix2x4";
	}
};

template<>
struct MatrixInfo<glm::mat3x2>
{
	static inline int Elements()
	{
		return 6;
	}

	static inline int Columns()
	{
		return 2;
	}

	static inline int Rows()
	{
		return 3;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix3x2";
	}

	static inline const char* TypeName()
	{
		return "Matrix3x2";
	}
};

template<>
struct MatrixInfo<glm::mat3x3>
{
	static inline int Elements()
	{
		return 9;
	}

	static inline int Columns()
	{
		return 3;
	}

	static inline int Rows()
	{
		return 3;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix3x3";
	}

	static inline const char* TypeName()
	{
		return "Matrix3x3";
	}
};

template<>
struct MatrixInfo<glm::mat3x4>
{
	static inline int Elements()
	{
		return 12;
	}

	static inline int Columns()
	{
		return 4;
	}

	static inline int Rows()
	{
		return 3;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix3x4";
	}

	static inline const char* TypeName()
	{
		return "Matrix3x4";
	}
};

template<>
struct MatrixInfo<glm::mat4x2>
{
	static inline int Elements()
	{
		return 8;
	}

	static inline int Columns()
	{
		return 2;
	}

	static inline int Rows()
	{
		return 4;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix4x2";
	}

	static inline const char* TypeName()
	{
		return "Matrix4x2";
	}
};

template<>
struct MatrixInfo<glm::mat4x3>
{
	static inline int Elements()
	{
		return 12;
	}

	static inline int Columns()
	{
		return 3;
	}

	static inline int Rows()
	{
		return 4;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix4x3";
	}

	static inline const char* TypeName()
	{
		return "Matrix4x3";
	}
};

template<>
struct MatrixInfo<glm::mat4x4>
{
	static inline int Elements()
	{
		return 16;
	}

	static inline int Columns()
	{
		return 4;
	}

	static inline int Rows()
	{
		return 4;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Matrix4x4";
	}

	static inline const char* TypeName()
	{
		return "Matrix4x4";
	}
};

void PushLuaValue(lua_State* L, const glm::mat2x2& Mat);
glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx);
int luaopen_matrix2x2(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat2x3& Mat);
glm::mat2x3* luaW_checkmatrix2x3(lua_State* L, int idx);
int luaopen_matrix2x3(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat2x4& Mat);
glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int idx);
int luaopen_matrix2x4(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat3x2& Mat);
glm::mat3x2* luaW_checkmatrix3x2(lua_State* L, int idx);
int luaopen_matrix3x2(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat3x3& Mat);
glm::mat3x3* luaW_checkmatrix3x3(lua_State* L, int idx);
int luaopen_matrix3x3(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat3x4& Mat);
glm::mat3x4* luaW_checkmatrix3x4(lua_State* L, int idx);
int luaopen_matrix3x4(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat4x2& Mat);
glm::mat4x2* luaW_checkmatrix4x2(lua_State* L, int idx);
int luaopen_matrix4x2(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat4x3& Mat);
glm::mat4x3* luaW_checkmatrix4x3(lua_State* L, int idx);
int luaopen_matrix4x3(lua_State* L);

void PushLuaValue(lua_State* L, const glm::mat4x4& Mat);
glm::mat4x4* luaW_checkmatrix4x4(lua_State* L, int idx);
int luaopen_matrix4x4(lua_State* L);