/*#include "LuaMatrix.h"

#include "LuaLibRegistry.h"

#include "LuaVector.h"

//
// metatable functions
//

static const char* GetMTName(glm::mat2x2* Mat)
{
	return W_MT_MAT2X2;
}

//
// templated implementations
//

template<typename MatType>
static const char* GetMTName()
{
	return GetMTName((MatType*)nullptr);
}

template<typename VecType>
static const VecType& VecInstance()
{
	static VecType Inst;
	return Inst;
}

template<typename VecType>
static int VecLength()
{
	return VecInstance<VecType>().length();
}

template<typename MatType>
static const MatType& MatInstance()
{
	static MatType Inst;
	return Inst;
}

template<typename MatType>
static int MatLength()
{
	return VecLength<MatType::col_type>() * VecLength<MatType::row_type>();
}

template<typename MatType>
struct MatrixContainer
{
	MatType* Matrix;
};

template<typename MatType>
static void PushMatrixImpl(lua_State* L, const MatType& Mat)
{
	auto* Container = (MatrixContainer<MatType>*)lua_newuserdata(L, sizeof(MatrixContainer<MatType>));
	Container->Matrix = new MatType(Mat);

	luaL_getmetatable(L, GetMTName<MatType>());
	lua_setmetatable(L, -2);
}

template<typename MatType>
static MatType* CheckMatrixImpl(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == MatLength<MatType>(), 1, "table is of wrong length");

		MatType Mat;
		for (int i = 0; i < MatLength<MatType>(); ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, idx);
			luaL_argcheck(L, lua_isnumber(L, -1), 1, "all indices of table must be numbers");
			float Value = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			Mat[i / VecLength<MatType::row_type>()][i % VecLength<MatType::row_type>()] = Value;
		}

		PushLuaValue(L, Mat);
		return CheckMatrixImpl<MatType>(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, GetMTName<MatType>());
	luaL_argcheck(L, Data != NULL, idx, "'Matrix' or 'table' expected");
	return ((MatrixContainer<MatType>*)Data)->Matrix;
}

template<typename MatType>
static int NewImpl(lua_State* L)
{
	int ArgCount = lua_gettop(L);
	switch (ArgCount)
	{
	default:
		if (ArgCount == MatLength<MatType>())
		{
			MatType Mat;
			for (int i = 0; i < MatLength<MatType>(); ++i)
			{
				float Value = (float)luaL_checknumber(L, i + 1);
				Mat[i / VecLength<MatType::row_type>()][i % VecLength<MatType::row_type>()] = Value;
			}

			PushLuaValue(L, Mat);
			return 1;
		}
		else
		{
			luaL_error(L, "expected 0, 1 or %d arguments", MatLength<MatType>());
		}
		return 0;

	case 0:
		PushLuaValue(L, MatType());
		return 1;

	case 1:
	{
		auto* Mat = CheckMatrixImpl<MatType>(L, 1);
		PushLuaValue(L, *Mat);
		return 1;
	}
	}
}

template<typename MatType>
static int TableImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);

	lua_newtable(L);
	for (int i = 0; i < MatLength<MatType>(); ++i)
	{
		float Value = Mat[i / VecLength<MatType::row_type>()][i % VecLength<MatType::row_type>()];
		lua_pushnumber(L, i + 1);
		lua_pushnumber(L, Value);
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
		luaL_argcheck(L, Index >= 1 && Index <= VecLength<MatType::col_type>(), 2, "index out of range");
		PushLuaValue(L, Mat[Index - 1]);
		return 1;
	}

	case 3:
	{
		int Index1 = (int)luaL_checknumber(L, 2);
		luaL_argcheck(L, Index1 >= 1 && Index1 <= VecLength<MatType::col_type>(), 2, "index out of range");
		int Index2 = (int)luaL_checknumber(L, 3);
		luaL_argcheck(L, Index2 >= 1 && Index2 <= VecLength<MatType::row_type>(), 3, "index out of range");
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
	luaL_argcheck(L, Index1 >= 1 && Index1 <= VecLength<MatType::col_type>(), 2, "index out of range");
	int Index2 = (int)luaL_checknumber(L, 3);
	luaL_argcheck(L, Index2 >= 1 && Index2 <= VecLength<MatType::row_type>(), 3, "index out of range");

	Mat[Index1 - 1][Index2 - 1] = (float)luaL_checknumber(L, 4);
	return 0;
}

template<typename MatType>
static int SetAllImpl(lua_State* L)
{
	auto& Mat = *CheckMatrixImpl<MatType>(L, 1);
	for (int i = 0; i < MatLength<MatType>(); ++i)
	{
		float Value = (float)luaL_checknumber(L, i + 2);
		Mat[i / VecLength<MatType::row_type>()][i % VecLength<MatType::row_type>()] = Value;
	}

	return 0;
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
struct MatReg
{
	static const luaL_reg functions[] = {
		{ "new", NewImpl<MatType> },
		{ "table", TableImpl<MatType> },
		{ "get", GetImpl<MatType> },
		{ "set", SetImpl<MatType> },
		{ "setAll", SetAllImpl<MatType> },
		{ NULL, NULL }
	};

	static const luaL_reg metafunctions[] = {
		{ "__gc", M_GCImpl<MatType> },
		{ "__eq", M_EqualImpl<MatType> },
		{ "__tostring", M_ToStringImpl<MatType> },
		{ "table", TableImpl<MatType> },
		{ "get", GetImpl<MatType> },
		{ "set", SetImpl<MatType> },
		{ "setAll", SetAllImpl<MatType> },
		{ NULL, NULL }
	};
};

//
// Matrix2x2 implementation
//

void PushLuaValue(lua_State* L, const glm::mat2x2& Mat)
{
	PushMatrixImpl<glm::mat2x2>(L, Mat);
}

glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx)
{
	return CheckMatrixImpl<glm::mat2x2>(L, idx);
}

int luaopen_matrix2x2(lua_State* L)
{
	luaL_newmetatable(L, W_MT_MAT2X2);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, MatReg<glm::mat2x2>::functions);

	luaL_register(L, "Matrix2x2", MatReg<glm::mat2x2>::metafunctions);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x2);*/