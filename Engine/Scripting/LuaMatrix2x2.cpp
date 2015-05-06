#include "LuaMatrix2x2.h"

#include "LuaLibRegistry.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct Mat2x2Container
{
	glm::mat2x2* Matrix;
};

void PushLuaValue(lua_State* L, const glm::mat2x2& Value)
{
	Mat2x2Container* Data = (Mat2x2Container*)lua_newuserdata(L, sizeof(Mat2x2Container));
	Data->Matrix = new glm::mat2x2(Value);

	luaL_getmetatable(L, "Wake.Matrix2x2");
	lua_setmetatable(L, -2);
}

glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == 4, 1, "table must be of length 4");

		glm::mat2x2 Mat;
		for (int i = 0; i < 4; ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, idx);
			luaL_argcheck(L, lua_isnumber(L, -1), 1, "all indices of table must be numbers");
			float Value = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			Mat[i / 2][i % 2] = Value;
		}

		PushLuaValue(L, Mat);
		return luaW_checkmatrix2x2(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, "Wake.Matrix2x2");
	luaL_argcheck(L, Data != NULL, idx, "'Matrix2x2' expected");
	return ((Mat2x2Container*)Data)->Matrix;
}

static int l_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 0, 1 or 4 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::mat2x2());
		return 1;

	case 1:
	{
		glm::mat2x2* Mat = luaW_checkmatrix2x2(L, 1);
		PushLuaValue(L, *Mat);
		return 1;
	}

	case 4:
	{
		glm::mat2x2 Mat;
		for (int i = 0; i < 4; ++i)
		{
			float Value = (float)luaL_checknumber(L, i + 1);
			Mat[i / 2][i % 2] = Value;
		}

		PushLuaValue(L, Mat);
		return 1;
	}
	}
}

static int l_table(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);

	lua_newtable(L);
	for (int i = 0; i < 4; ++i)
	{
		float Value = Mat[i / 2][i % 2];
		lua_pushnumber(L, i + 1);
		lua_pushnumber(L, Value);
		lua_settable(L, -3);
	}

	return 1;
}

static int l_get(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 2 or 3 arguments");
		return 0;

	case 2:
	{
		int Index = (int)luaL_checknumber(L, 2);
		luaL_argcheck(L, Index >= 1 && Index <= 2, 2, "must be >= 1 and <= 2");
		PushLuaValue(L, Mat[Index-1]);
		return 1;
	}

	case 3:
	{
		int Index1 = (int)luaL_checknumber(L, 2);
		luaL_argcheck(L, Index1 >= 1 && Index1 <= 2, 2, "must be >= 1 and <= 2");
		int Index2 = (int)luaL_checknumber(L, 3);
		luaL_argcheck(L, Index2 >= 1 && Index2 <= 2, 3, "must be >= 1 and <= 2");
		lua_pushnumber(L, Mat[Index1-1][Index2-1]);
		return 1;
	}
	}
}

static int l_set(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);
	int Index1 = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index1 >= 1 && Index1 <= 2, 2, "must be >= 1 and <= 2");
	int Index2 = (int)luaL_checknumber(L, 3);
	luaL_argcheck(L, Index2 >= 1 && Index2 <= 2, 3, "must be >= 1 and <= 2");

	Mat[Index1 - 1][Index2 - 1] = luaL_checknumber(L, 4);
	return 0;
}

static int l_setall(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);
	for (int i = 0; i < 4; ++i)
	{
		float Value = (float)luaL_checknumber(L, i + 2);
		Mat[i / 2][i % 2] = Value;
	}

	return 0;
}

static int l_apply(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	glm::mat2x2 NewMat;
	for (int i = 0; i < 4; ++i)
	{
		lua_pushvalue(L, 2);
		lua_pushnumber(L, Mat[i / 2][i % 2]);
		lua_call(L, 1, 1);

		float Value = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		NewMat[i / 2][i % 2] = Value;
	}

	PushLuaValue(L, NewMat);
	return 1;
}

static int l_m_gc(lua_State* L)
{
	delete luaW_checkmatrix2x2(L, 1);
	return 0;
}

static int l_m_eq(lua_State* L)
{
	auto& MatA = *luaW_checkmatrix2x2(L, 1);
	auto& MatB = *luaW_checkmatrix2x2(L, 2);
	lua_pushboolean(L, MatA == MatB);
	return 1;
}

static int l_m_tostring(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x2(L, 1);
	lua_pushstring(L, glm::to_string(Mat).c_str());
	return 1;
}

static const luaL_reg matrix2x2_f[] = {
	{ "new", l_new },
	{ "table", l_table },
	{ "get", l_get },
	{ "set", l_set },
	{ "setAll", l_setall },
	{ "apply", l_apply },
	{NULL, NULL}
};

static const luaL_reg matrix2x2_m[] = {
	{ "__gc", l_m_gc },
	{ "__eq", l_m_eq },
	{ "__tostring", l_m_tostring },
	{ "table", l_table },
	{ "get", l_get },
	{ "set", l_set },
	{ "setAll", l_setall },
	{ "apply", l_apply },
	{NULL, NULL}
};

int luaopen_matrix2x2(lua_State* L)
{
	luaL_newmetatable(L, "Wake.Matrix2x2");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, matrix2x2_m);

	luaL_register(L, "Matrix2x2", matrix2x2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x2);