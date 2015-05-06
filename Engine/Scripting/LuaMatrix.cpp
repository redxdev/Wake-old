#include "LuaMatrix.h"

#include "LuaLibRegistry.h"

#include <glm/glm.hpp>

struct Mat2x2Container
{
	glm::mat2x2* Matrix;
};

void PushLuaValue(lua_State* L, const glm::mat4x4& Value)
{
	Mat2x2Container* Data = (Mat2x2Container*)lua_newuserdata(L, sizeof(Mat2x2Container));
	Data->Matrix = new glm::mat2x2(Value);

	luaL_getmetatable(L, "Wake.Matrix2x2");
	lua_setmetatable(L, -2);
}

glm::mat2x2* luaW_checkmatrix2x2(lua_State* L, int idx)
{
	if (lua_istable(L, 1))
	{
		luaL_argcheck(L, lua_objlen(L, 4), 1, "table must be of length 4");

		lua_pushnumber(L, 1);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "index #1 of table must be a number");
		double X1 = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 2);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "index #2 of table must be a number");
		double Y1 = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 3);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "index #3 of table must be a number");
		double X2 = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 4);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "index #3 of table must be a number");
		double Y2 = lua_tonumber(L, -1);
		lua_pop(L, 1);

		PushLuaValue(L, glm::mat2x2(X1, Y1, X2, Y2));
		return luaW_checkmatrix2x2(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, "Wake.Matrix2x2");
	luaL_argcheck(L, Data != NULL, idx, "'Matrix2x2' expected");
	return ((Mat2x2Container*)Data)->Matrix;
}

static const luaL_reg matrix2x2_f[] = {
	{NULL, NULL}
};

static const luaL_reg matrix2x2_m[] = {
	{NULL, NULL}
};

int luaopen_matrix2x2(lua_State* L)
{
	luaL_newmetatable(L, "Wake.Matrix2x2");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, matrix2x2_f);

	luaL_register(L, "Matrix2x2", matrix2x2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_matrix2x2);