#include "LuaMatrix.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "LuaLibRegistry.h"
#include "LuaVector.h"

//
// Generic functions
//

template<typename MatType>
struct MatrixContainer
{
	MatType* Matrix;
};

template<typename MatType>
static void PushMatrixImpl(lua_State* L, const MatType& Value)
{
	auto* Container = (MatrixContainer<MatType>*)lua_newuserdata(L, sizeof(MatrixContainer<MatType>));
	Container->Matrix = new MatType(Value);

	luaL_getmetatable(L, MatrixInfo<MatType>::MetatableName());
	lua_setmetatable(L, -2);
}

template<typename MatType>
static MatType* CheckMatrixImpl(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == MatrixInfo<MatType>::Elements(), 1, "table is of incorrect length");

		MatType Mat;
		for (int i = 0; i < MatrixInfo<MatType>::Elements(); ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, idx);
			float Value = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			Mat[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()] = Value;
		}

		PushMatrixImpl<MatType>(L, Mat);
		return CheckMatrixImpl<MatType>(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, MatrixInfo<MatType>::MetatableName());
	luaL_argcheck(L, Data != nullptr, idx, "'Matrix' or 'table' expected");
	return ((MatrixContainer<MatType>*)Data)->Matrix;
}

template<typename MatType>
static int NewImpl(lua_State* L)
{
	int ArgCount = lua_gettop(L);
	switch (ArgCount)
	{
	default:
		if (ArgCount == MatrixInfo<MatType>::Elements())
		{
			MatType Mat;
			for (int i = 0; i < MatrixInfo<MatType>::Elements(); ++i)
			{
				float Value = (float)luaL_checknumber(L, i + 1);
				Mat[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()] = Value;
			}

			PushMatrixImpl<MatType>(L, Mat);
			return 1;
		}

		luaL_error(L, "expected 0, 1, or %d arguments", MatrixInfo<MatType>::Elements());
		return 0;

	case 0:
		PushMatrixImpl<MatType>(L, MatType());
		return 1;

	case 1:
	{
		auto* Mat = CheckMatrixImpl<MatType>(L, 1);
		PushMatrixImpl<MatType>(L, *Mat);
		return 1;
	}
	}
}

template<typename MatType>
static int TableImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	lua_newtable(L);
	for (int i = 0; i < MatrixInfo<MatType>::Elements(); ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_pushnumber(L, Mat[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()]);
		lua_settable(L, -3);
	}

	return 1;
}

template<typename MatType>
static int GetImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 2 or 3 arguments");
		return 0;

	case 2:
	{
		int Index = (int)luaL_checknumber(L, 2);
		luaL_argcheck(L, Index >= 1 && Index <= MatrixInfo<MatType>::Rows(), 2, "row index out of range");
		PushLuaValue(L, Mat[Index - 1]);
		return 1;
	}

	case 3:
	{
		int Index1 = (int)luaL_checknumber(L, 2);
		luaL_argcheck(L, Index1 >= 1 && Index1 <= MatrixInfo<MatType>::Rows(), 2, "row index out of range");
		int Index2 = (int)luaL_checknumber(L, 3);
		luaL_argcheck(L, Index2 >= 1 && Index2 <= MatrixInfo<MatType>::Columns(), 3, "column index out of range");
		lua_pushnumber(L, Mat[Index1 - 1][Index2 - 1]);
		return 1;
	}
	}
}

template<typename MatType>
static int SetImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	int Index1 = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index1 >= 1 && Index1 <= MatrixInfo<MatType>::Rows(), 2, "row index out of range");
	int Index2 = (int)luaL_checknumber(L, 3);
	luaL_argcheck(L, Index2 >= 1 && Index2 <= MatrixInfo<MatType>::Columns(), 3, "column index out of range");

	Mat[Index1 - 1][Index2 - 1] = (float)luaL_checknumber(L, 4);
	return 0;
}

template<typename MatType>
static int SetAllImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	for (int i = 0; i < MatrixInfo<MatType>::Elements(); ++i)
	{
		float Value = (float)luaL_checknumber(L, i + 2);
		Mat[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()] = Value;
	}

	return 0;
}

template<typename MatType>
static int RowsImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Rows());
	return 1;
}

template<typename MatType>
static int ColumnsImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Columns());
	return 1;
}

template<typename MatType>
static int M_GCImpl(lua_State* L)
{
	delete CheckMatrixImpl<MatType>(L, 1);
	return 0;
}

template<typename MatType>
static int M_EqualImpl(lua_State* L)
{
	auto& MatA = *CheckMatrixImpl<MatType>(L, 1);
	auto& MatB = *CheckMatrixImpl<MatType>(L, 2);
	lua_pushboolean(L, MatA == MatB);
	return 1;
}

template<typename MatType>
static int M_ToStringImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	lua_pushstring(L, glm::to_string(Mat).c_str());
	return 1;
}

template<typename MatType>
static int M_LengthImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Elements());
	return 1;
}

//
// Registration
//

#define MATRIX_LIB_F(Name, Type, ...) \
static const luaL_reg Name##_f[] = { \
	{ "new", NewImpl<Type> }, \
	{ "table", TableImpl<Type> }, \
	{ "get", GetImpl<Type> }, \
	{ "set", SetImpl<Type> }, \
	{ "setAll", SetAllImpl<Type> }, \
	{ "rows", RowsImpl<Type> }, \
	{ "columns", ColumnsImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

#define MATRIX_LIB_M(Name, Type, ...) \
static const luaL_reg Name##_m[] = { \
	{ "__gc", M_GCImpl<Type> }, \
	{ "__eq", M_EqualImpl<Type> }, \
	{ "__tostring", M_ToStringImpl<Type> }, \
	{ "__len", M_LengthImpl<Type> }, \
	{ "table", TableImpl<Type> }, \
	{ "get", GetImpl<Type> }, \
	{ "set", SetImpl<Type> }, \
	{ "setAll", SetAllImpl<Type> }, \
	{ "rows", RowsImpl<Type> }, \
	{ "columns", ColumnsImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

MATRIX_LIB_F(matrix2x2, glm::mat2x2);
MATRIX_LIB_M(matrix2x2, glm::mat2x2);

void PushLuaValue(lua_State* L, const glm::mat2x2& Value)
{
	PushMatrixImpl<glm::mat2x2>(L, Value);
}

glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat2x2>(L, idx);
}

int luaopen_matrix2x2(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat2x2>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix2x2_m);

	luaL_register(L, MatrixInfo<glm::mat2x2>::TypeName(), matrix2x2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x2);

MATRIX_LIB_F(matrix2x3, glm::mat2x3);
MATRIX_LIB_M(matrix2x3, glm::mat2x3);

void PushLuaValue(lua_State* L, const glm::mat2x3& Value)
{
	PushMatrixImpl<glm::mat2x3>(L, Value);
}

glm::mat2x3* luaW_checkmatrix2x3(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat2x3>(L, idx);
}

int luaopen_matrix2x3(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat2x3>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix2x3_m);

	luaL_register(L, MatrixInfo<glm::mat2x3>::TypeName(), matrix2x3_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x3);

MATRIX_LIB_F(matrix2x4, glm::mat2x4);
MATRIX_LIB_M(matrix2x4, glm::mat2x4);

void PushLuaValue(lua_State* L, const glm::mat2x4& Value)
{
	PushMatrixImpl<glm::mat2x4>(L, Value);
}

glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat2x4>(L, idx);
}

int luaopen_matrix2x4(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat2x4>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix2x4_m);

	luaL_register(L, MatrixInfo<glm::mat2x4>::TypeName(), matrix2x4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x4);

MATRIX_LIB_F(matrix3x2, glm::mat3x2);
MATRIX_LIB_M(matrix3x2, glm::mat3x2);

void PushLuaValue(lua_State* L, const glm::mat3x2& Value)
{
	PushMatrixImpl<glm::mat3x2>(L, Value);
}

glm::mat3x2* luaW_checkmatrix3x2(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat3x2>(L, idx);
}

int luaopen_matrix3x2(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat3x2>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix3x2_m);

	luaL_register(L, MatrixInfo<glm::mat3x2>::TypeName(), matrix3x2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix3x2);

MATRIX_LIB_F(matrix3x3, glm::mat3x3);
MATRIX_LIB_M(matrix3x3, glm::mat3x3);

void PushLuaValue(lua_State* L, const glm::mat3x3& Value)
{
	PushMatrixImpl<glm::mat3x3>(L, Value);
}

glm::mat3x3* luaW_checkmatrix3x3(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat3x3>(L, idx);
}

int luaopen_matrix3x3(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat3x3>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix3x3_m);

	luaL_register(L, MatrixInfo<glm::mat3x3>::TypeName(), matrix3x3_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix3x3);

MATRIX_LIB_F(matrix3x4, glm::mat3x4);
MATRIX_LIB_M(matrix3x4, glm::mat3x4);

void PushLuaValue(lua_State* L, const glm::mat3x4& Value)
{
	PushMatrixImpl<glm::mat3x4>(L, Value);
}

glm::mat3x4* luaW_checkmatrix3x4(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat3x4>(L, idx);
}

int luaopen_matrix3x4(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat3x4>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix3x4_m);

	luaL_register(L, MatrixInfo<glm::mat3x4>::TypeName(), matrix3x4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix3x4);

MATRIX_LIB_F(matrix4x2, glm::mat4x2);
MATRIX_LIB_M(matrix4x2, glm::mat4x2);

void PushLuaValue(lua_State* L, const glm::mat4x2& Value)
{
	PushMatrixImpl<glm::mat4x2>(L, Value);
}

glm::mat4x2* luaW_checkmatrix4x2(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat4x2>(L, idx);
}

int luaopen_matrix4x2(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat4x2>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix4x2_m);

	luaL_register(L, MatrixInfo<glm::mat4x2>::TypeName(), matrix4x2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix4x2);

MATRIX_LIB_F(matrix4x3, glm::mat4x3);
MATRIX_LIB_M(matrix4x3, glm::mat4x3);

void PushLuaValue(lua_State* L, const glm::mat4x3& Value)
{
	PushMatrixImpl<glm::mat4x3>(L, Value);
}

glm::mat4x3* luaW_checkmatrix4x3(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat4x3>(L, idx);
}

int luaopen_matrix4x3(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat4x3>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix4x3_m);

	luaL_register(L, MatrixInfo<glm::mat4x3>::TypeName(), matrix4x3_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix4x3);

MATRIX_LIB_F(matrix4x4, glm::mat4x4);
MATRIX_LIB_M(matrix4x4, glm::mat4x4);

void PushLuaValue(lua_State* L, const glm::mat4x4& Value)
{
	PushMatrixImpl<glm::mat4x4>(L, Value);
}

glm::mat4x4* luaW_checkmatrix4x4(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat4x4>(L, idx);
}

int luaopen_matrix4x4(lua_State* L)
{
	luaL_newmetatable(L, MatrixInfo<glm::mat4x4>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, matrix4x4_m);

	luaL_register(L, MatrixInfo<glm::mat4x4>::TypeName(), matrix4x4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix4x4);

#undef MATRIX_LIB_F
#undef MATRIX_LIB_M