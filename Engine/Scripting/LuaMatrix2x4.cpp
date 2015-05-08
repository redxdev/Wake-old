#include "LuaMatrix2x4.h"

#include "LuaLibRegistry.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

struct Mat2x4Container
{
	glm::mat2x4* Matrix;
};

void PushLuaValue(lua_State* L, const glm::mat2x4& Value)
{
	Mat2x4Container* Data = (Mat2x4Container*)lua_newuserdata(L, sizeof(Mat2x4Container));
	Data->Matrix = new glm::mat2x4(Value);

	luaL_getmetatable(L, W_MT_MAT2X4);
	lua_setmetatable(L, -2);
}

glm::mat2x4* luaW_checkmatrix2x4(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == 8, 1, "table must be of length 8");

		glm::mat2x4 Mat;
		for (int i = 0; i < 8; ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, idx);
			luaL_argcheck(L, lua_isnumber(L, -1), 1, "all indices of table must be numbers");
			float Value = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
			Mat[i / 4][i % 4] = Value;
		}

		PushLuaValue(L, Mat);
		return luaW_checkmatrix2x4(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, W_MT_MAT2X4);
	luaL_argcheck(L, Data != NULL, idx, "'Matrix2x4' expected");
	return ((Mat2x4Container*)Data)->Matrix;
}

static int l_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 0, 1 or 8 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::mat2x4());
		return 1;

	case 1:
	{
		glm::mat2x4* Mat = luaW_checkmatrix2x4(L, 1);
		PushLuaValue(L, *Mat);
		return 1;
	}

	case 8:
	{
		glm::mat2x4 Mat;
		for (int i = 0; i < 8; ++i)
		{
			float Value = (float)luaL_checknumber(L, i + 1);
			Mat[i / 4][i % 4] = Value;
		}

		PushLuaValue(L, Mat);
		return 1;
	}
	}
}

static int l_m_gc(lua_State* L)
{
	delete luaW_checkmatrix2x4(L, 1);
	return 0;
}

static int l_m_eq(lua_State* L)
{
	auto& MatA = *luaW_checkmatrix2x4(L, 1);
	auto& MatB = *luaW_checkmatrix2x4(L, 2);
	lua_pushboolean(L, MatA == MatB);
	return 1;
}

static int l_m_tostring(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x4(L, 1);
	lua_pushstring(L, glm::to_string(Mat).c_str());
	return 1;
}

static int l_m_unm(lua_State* L)
{
	auto& Mat = *luaW_checkmatrix2x4(L, 1);
	PushLuaValue(L, -Mat);
	return 1;
}

static const luaL_reg matrix2x4_f[] = {
	{ "new", l_new },
	{ NULL, NULL }
};

static const luaL_reg matrix2x4_m[] = {
	{ "__gc", l_m_gc },
	{ "__eq", l_m_eq },
	{ "__tostring", l_m_tostring },
	{ "__unm", l_m_unm },
	{ NULL, NULL }
};

int luaopen_matrix2x4(lua_State* L)
{
	luaL_newmetatable(L, W_MT_MAT2X4);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, matrix2x4_m);

	luaL_register(L, "Matrix2x4", matrix2x4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x4);