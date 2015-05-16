#include "LuaMatrix.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "LuaLibRegistry.h"

//
// Matrix Implementation
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
static int Mat_NewImpl(lua_State* L)
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
static int Mat_TableImpl(lua_State* L)
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
static int Mat_GetImpl(lua_State* L)
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
static int Mat_SetImpl(lua_State* L)
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
static int Mat_SetAllImpl(lua_State* L)
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
static int Mat_RowsImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Rows());
	return 1;
}

template<typename MatType>
static int Mat_ColumnsImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Columns());
	return 1;
}

template<typename MatType>
static int Mat_TransposeImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	PushMatrixImpl<typename MatType::transpose_type>(L, glm::transpose(Mat));
	return 1;
}

template<typename MatType>
static int Mat_ApplyImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	MatType Result;
	for (int i = 0; i < MatrixInfo<MatType>::Elements(); ++i)
	{
		lua_pushvalue(L, 2);
		lua_pushnumber(L, Mat[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()]);
		lua_pushnumber(L, i);
		lua_pushnumber(L, (i / MatrixInfo<MatType>::Columns()) + 1);
		lua_pushnumber(L, (i % MatrixInfo<MatType>::Columns()) + 1);
		lua_call(L, 4, 1);

		Result[i / MatrixInfo<MatType>::Columns()][i % MatrixInfo<MatType>::Columns()] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	PushMatrixImpl<MatType>(L, Result);
	return 1;
}

template<typename MatType>
static int Mat_M_GCImpl(lua_State* L)
{
	delete CheckMatrixImpl<MatType>(L, 1);
	return 0;
}

template<typename MatType>
static int Mat_M_EqualImpl(lua_State* L)
{
	auto& MatA = *CheckMatrixImpl<MatType>(L, 1);
	auto& MatB = *CheckMatrixImpl<MatType>(L, 2);
	lua_pushboolean(L, MatA == MatB);
	return 1;
}

template<typename MatType>
static int Mat_M_ToStringImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	lua_pushstring(L, glm::to_string(Mat).c_str());
	return 1;
}

template<typename MatType>
static int Mat_M_LengthImpl(lua_State* L)
{
	lua_pushnumber(L, MatrixInfo<MatType>::Elements());
	return 1;
}

//
// Vector Implementation
//

template<typename VecType>
struct VectorContainer
{
	VecType* Vector;
};

template<typename VecType>
static void PushVectorImpl(lua_State* L, const VecType& Value)
{
	auto* Container = (VectorContainer<VecType>*)lua_newuserdata(L, sizeof(VectorContainer<VecType>));
	Container->Vector = new VecType(Value);

	luaL_getmetatable(L, VectorInfo<VecType>::MetatableName());
	lua_setmetatable(L, -2);
}

template<typename VecType>
static VecType* CheckVectorImpl(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == VectorInfo<VecType>::Elements(), 1, "table is of incorrect length");

		VecType Vec;
		for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, idx);
			float Value = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			Vec[i] = Value;
		}

		PushVectorImpl<VecType>(L, Vec);
		return CheckVectorImpl<VecType>(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, VectorInfo<VecType>::MetatableName());
	luaL_argcheck(L, Data != nullptr, idx, "'Vector' or 'table' expected");
	return ((VectorContainer<VecType>*)Data)->Vector;
}

template<typename VecType>
static int Vec_NewImpl(lua_State* L)
{
	int ArgCount = lua_gettop(L);
	switch (ArgCount)
	{
	default:
		if (ArgCount == VectorInfo<VecType>::Elements())
		{
			VecType Vec;
			for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
			{
				float Value = (float)luaL_checknumber(L, i + 1);
				Vec[i] = Value;
			}

			PushVectorImpl<VecType>(L, Vec);
			return 1;
		}

		luaL_error(L, "expected 0, 1, or %d arguments", VectorInfo<VecType>::Elements());
		return 0;

	case 0:
		PushVectorImpl<VecType>(L, VecType());
		return 1;

	case 1:
	{
		auto* Vec = CheckVectorImpl<VecType>(L, 1);
		PushVectorImpl<VecType>(L, *Vec);
		return 1;
	}
	}
}

template<typename VecType>
static int Vec_TableImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_newtable(L);
	for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_pushnumber(L, Vec[i]);
		lua_settable(L, -3);
	}

	return 1;
}

template<typename VecType>
static int Vec_GetImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= VectorInfo<VecType>::Elements(), 2, "index out of range");
	lua_pushnumber(L, Vec[Index - 1]);
	return 1;
}

template<typename VecType>
static int Vec_SetImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= VectorInfo<VecType>::Elements(), 2, "index out of range");
	float Value = (float)luaL_checknumber(L, 3);
	Vec[Index - 1] = Value;
	return 0;
}

template<typename VecType>
static int Vec_SetAllImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		Vec[i] = (float)luaL_checknumber(L, i + 2);
	}
	return 0;
}

template<typename VecType>
static int Vec_DotImpl(lua_State* L)
{
	auto& VecA = *CheckVectorImpl<VecType>(L, 1);
	auto& VecB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

template<typename VecType>
static int Vec_DistanceImpl(lua_State* L)
{
	auto& VecA = *CheckVectorImpl<VecType>(L, 1);
	auto& VecB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

template<typename VecType>
static int Vec_LengthImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

template<typename VecType>
static int Vec_ApplyImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	VecType Result;
	for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		lua_pushvalue(L, 2);
		lua_pushnumber(L, Vec[i]);
		lua_pushnumber(L, i);
		lua_call(L, 2, 1);

		Result[i] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	PushVectorImpl<VecType>(L, Result);
	return 1;
}

template<typename VecType>
static int Vec_NormalizeImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	PushVectorImpl<VecType>(L, glm::normalize(Vec));
	return 1;
}

template<typename VecType>
static int Vec_ReflectImpl(lua_State* L)
{
	auto& VecI = *CheckVectorImpl<VecType>(L, 1);
	auto& VecN = *CheckVectorImpl<VecType>(L, 2);
	PushVectorImpl<VecType>(L, glm::reflect(VecI, VecN));
	return 1;
}

template<typename VecType>
static int Vec_RefractImpl(lua_State* L)
{
	auto& VecI = *CheckVectorImpl<VecType>(L, 1);
	auto& VecN = *CheckVectorImpl<VecType>(L, 1);
	float eta = (float)luaL_checknumber(L, 3);
	PushVectorImpl<VecType>(L, glm::refract(VecI, VecN, eta));
	return 1;
}

template<typename VecType>
static int Vec_M_GCImpl(lua_State* L)
{
	delete CheckVectorImpl<VecType>(L, 1);
	return 0;
}

template<typename VecType>
static int Vec_M_EqualImpl(lua_State* L)
{
	auto& VecA = *CheckVectorImpl<VecType>(L, 1);
	auto& VecB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushboolean(L, VecA == VecB);
	return 1;
}

template<typename VecType>
static int Vec_M_ToStringImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_pushstring(L, glm::to_string(Vec).c_str());
	return 1;
}

template<typename VecType>
static int Vec_M_LengthImpl(lua_State* L)
{
	lua_pushnumber(L, VectorInfo<VecType>::Elements());
	return 1;
}

template<typename VecType>
static int Vec_M_UnaryMinusImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	PushVectorImpl<VecType>(L, -Vec);
	return 1;
}

template<typename VecType>
static int Vec_M_AddImpl(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, Num + Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *CheckVectorImpl<VecType>(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushVectorImpl<VecType>(L, Vec + Num);
		return 1;
	}
	else
	{
		auto& VecA = *CheckVectorImpl<VecType>(L, 1);
		auto& VecB = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, VecA + VecB);
		return 1;
	}
}

template<typename VecType>
static int Vec_M_SubImpl(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, Num - Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *CheckVectorImpl<VecType>(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushVectorImpl<VecType>(L, Vec - Num);
		return 1;
	}
	else
	{
		auto& VecA = *CheckVectorImpl<VecType>(L, 1);
		auto& VecB = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, VecA - VecB);
		return 1;
	}
}

template<typename VecType>
static int Vec_M_MulImpl(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, Num * Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *CheckVectorImpl<VecType>(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushVectorImpl<VecType>(L, Vec * Num);
		return 1;
	}
	else
	{
		auto& VecA = *CheckVectorImpl<VecType>(L, 1);
		auto& VecB = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, VecA * VecB);
		return 1;
	}
}

template<typename VecType>
static int Vec_M_DivImpl(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, Num / Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *CheckVectorImpl<VecType>(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushVectorImpl<VecType>(L, Vec / Num);
		return 1;
	}
	else
	{
		auto& VecA = *CheckVectorImpl<VecType>(L, 1);
		auto& VecB = *CheckVectorImpl<VecType>(L, 2);
		PushVectorImpl<VecType>(L, VecA / VecB);
		return 1;
	}
}

static int Vec3_CrossImpl(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);

	PushLuaValue(L, glm::cross(VecA, VecB));
	return 1;
}

//
// Shared Implementations
//

static bool CheckMetatable(lua_State* L, int idx, const char* MTName)
{
	lua_getmetatable(L, idx);
	luaL_getmetatable(L, MTName);
	bool Result = lua_equal(L, -1, -2);
	lua_pop(L, 2);
	return Result;
}

//
// Matrix Registration
//

#define MATRIX_LIB_F(Name, Type, ...) \
static const luaL_reg Name##_f[] = { \
	{ "new", Mat_NewImpl<Type> }, \
	{ "table", Mat_TableImpl<Type> }, \
	{ "get", Mat_GetImpl<Type> }, \
	{ "set", Mat_SetImpl<Type> }, \
	{ "setAll", Mat_SetAllImpl<Type> }, \
	{ "rows", Mat_RowsImpl<Type> }, \
	{ "columns", Mat_ColumnsImpl<Type> }, \
	{ "transpose", Mat_TransposeImpl<Type> }, \
	{ "apply", Mat_ApplyImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

#define MATRIX_LIB_M(Name, Type, ...) \
static const luaL_reg Name##_m[] = { \
	{ "__gc", Mat_M_GCImpl<Type> }, \
	{ "__eq", Mat_M_EqualImpl<Type> }, \
	{ "__tostring", Mat_M_ToStringImpl<Type> }, \
	{ "__len", Mat_M_LengthImpl<Type> }, \
	{ "table", Mat_TableImpl<Type> }, \
	{ "get", Mat_GetImpl<Type> }, \
	{ "set", Mat_SetImpl<Type> }, \
	{ "setAll", Mat_SetAllImpl<Type> }, \
	{ "rows", Mat_RowsImpl<Type> }, \
	{ "columns", Mat_ColumnsImpl<Type> }, \
	{ "transpose", Mat_TransposeImpl<Type> }, \
	{ "apply", Mat_ApplyImpl<Type> }, \
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

//
// Vector Registration
//

#define VECTOR_LIB_F(Name, Type, ...) \
static const luaL_reg Name##_f[] = { \
	{ "new", Vec_NewImpl<Type> }, \
	{ "table", Vec_TableImpl<Type> }, \
	{ "get", Vec_GetImpl<Type> }, \
	{ "set", Vec_SetImpl<Type> }, \
	{ "setAll", Vec_SetAllImpl<Type> }, \
	{ "dot", Vec_DotImpl<Type> }, \
	{ "distance", Vec_DistanceImpl<Type> }, \
	{ "length", Vec_LengthImpl<Type> }, \
	{ "apply", Vec_ApplyImpl<Type> }, \
	{ "normalize", Vec_NormalizeImpl<Type> }, \
	{ "reflect", Vec_ReflectImpl<Type> }, \
	{ "refract", Vec_RefractImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

#define VECTOR_LIB_M(Name, Type, ...) \
static const luaL_reg Name##_m[] = { \
	{ "__gc", Vec_M_GCImpl<Type> }, \
	{ "__eq", Vec_M_EqualImpl<Type> }, \
	{ "__tostring", Vec_M_ToStringImpl<Type> }, \
	{ "__len", Vec_M_LengthImpl<Type> }, \
	{ "__unm", Vec_M_UnaryMinusImpl<Type> }, \
	{ "__add", Vec_M_AddImpl<Type> }, \
	{ "__sub", Vec_M_SubImpl<Type> }, \
	{ "__mul", Vec_M_MulImpl<Type> }, \
	{ "__div", Vec_M_DivImpl<Type> }, \
	{ "table", Vec_TableImpl<Type> }, \
	{ "get", Vec_GetImpl<Type> }, \
	{ "set", Vec_SetImpl<Type> }, \
	{ "setAll", Vec_SetAllImpl<Type> }, \
	{ "dot", Vec_DotImpl<Type> }, \
	{ "distance", Vec_DistanceImpl<Type> }, \
	{ "length", Vec_LengthImpl<Type> }, \
	{ "apply", Vec_ApplyImpl<Type> }, \
	{ "normalize", Vec_NormalizeImpl<Type> }, \
	{ "reflect", Vec_ReflectImpl<Type> }, \
	{ "refract", Vec_RefractImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

VECTOR_LIB_F(vector2, glm::vec2);
VECTOR_LIB_M(vector2, glm::vec2);

void PushLuaValue(lua_State* L, const glm::vec2& Value)
{
	PushVectorImpl<glm::vec2>(L, Value);
}

glm::vec2* luaW_checkvector2(lua_State* L, int idx)
{
	return CheckVectorImpl<glm::vec2>(L, idx);
}

int luaopen_vector2(lua_State* L)
{
	luaL_newmetatable(L, VectorInfo<glm::vec2>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, vector2_m);

	luaL_register(L, VectorInfo<glm::vec2>::TypeName(), vector2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector2);

VECTOR_LIB_F(vector3, glm::vec3,
{ "cross", Vec3_CrossImpl }
);

VECTOR_LIB_M(vector3, glm::vec3,
{ "cross", Vec3_CrossImpl }
);

void PushLuaValue(lua_State* L, const glm::vec3& Value)
{
	PushVectorImpl<glm::vec3>(L, Value);
}

glm::vec3* luaW_checkvector3(lua_State* L, int idx)
{
	return CheckVectorImpl<glm::vec3>(L, idx);
}

int luaopen_vector3(lua_State* L)
{
	luaL_newmetatable(L, VectorInfo<glm::vec3>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, vector3_m);

	luaL_register(L, VectorInfo<glm::vec3>::TypeName(), vector3_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector3);

VECTOR_LIB_F(vector4, glm::vec4);

VECTOR_LIB_M(vector4, glm::vec4);

void PushLuaValue(lua_State* L, const glm::vec4& Value)
{
	PushVectorImpl<glm::vec4>(L, Value);
}

glm::vec4* luaW_checkvector4(lua_State* L, int idx)
{
	return CheckVectorImpl<glm::vec4>(L, idx);
}

int luaopen_vector4(lua_State* L)
{
	luaL_newmetatable(L, VectorInfo<glm::vec4>::MetatableName());
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_register(L, NULL, vector4_m);

	luaL_register(L, VectorInfo<glm::vec4>::TypeName(), vector4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector4);

#undef VECTOR_LIB_F
#undef VECTOR_LIB_M