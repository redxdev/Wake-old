#include "LuaVector.h"

#include "LuaLibRegistry.h"

#include <glm/glm.hpp>
#include <sstream>

struct Vec2Container
{
	glm::vec2* Vector;
};

void PushLuaValue(lua_State* L, const glm::vec2& Value)
{
	Vec2Container* Data = (Vec2Container*)lua_newuserdata(L, sizeof(Vec2Container));
	Data->Vector = new glm::vec2(Value);

	luaL_getmetatable(L, "Wake.Vector2");
	lua_setmetatable(L, -2);
}

static glm::vec2* check_vec2(lua_State* L, int idx)
{
	void* Data = luaL_checkudata(L, idx, "Wake.Vector2");
	luaL_argcheck(L, Data != NULL, idx, "'Vector2' expected");
	return ((Vec2Container*)Data)->Vector;
}

static int l_vector2_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "Expected 1 or 2 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::vec2());
		return 1;

	case 1:
		switch (lua_type(L, 1))
		{
		default:
			luaL_error(L, "Argument #1 must be a table or a Vector2");
			return 0;

		case LUA_TUSERDATA:
			{
				glm::vec2* Vec = check_vec2(L, 1);
				PushLuaValue(L, *Vec);
				return 1;
			}

		case LUA_TTABLE:
			{
				lua_pushnumber(L, 1);
				lua_gettable(L, 1);
				if (!lua_isnumber(L, -1))
				{
					luaL_error(L, "First index must be a number");
				}
				double X = lua_tonumber(L, -1);
				lua_pop(L, 1);

				lua_pushnumber(L, 2);
				lua_gettable(L, 1);
				if (!lua_isnumber(L, -1))
				{
					luaL_error(L, "Second index must be a number");
				}
				double Y = lua_tonumber(L, -1);
				lua_pop(L, 1);

				PushLuaValue(L, glm::vec2(X, Y));
				return 1;
			}
		}
		break;

	case 2:
		{
			double X = luaL_checknumber(L, 1);
			double Y = luaL_checknumber(L, 2);
			PushLuaValue(L, glm::vec2(X, Y));
			return 1;
		}
	}
}

static int l_vector2_table(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);

	lua_newtable(L);
	
	lua_pushnumber(L, 1);
	lua_pushnumber(L, Vec[0]);
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_settable(L, -3);

	return 1;
}

static int l_vector2_get(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index == 1 || Index == 2, 2, "Must be between 1 and 2");
	lua_pushnumber(L, Vec[Index-1]);
	return 1;
}

static int l_vector2_set(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index == 1 || Index == 2, 2, "Must be between 1 and 2");
	double Value = luaL_checknumber(L, 3);
	Vec[Index-1] = Value;
	return 0;
}

static int l_vector2_setall(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);
	Vec[0] = luaL_checknumber(L, 2);
	Vec[1] = luaL_checknumber(L, 3);
	return 0;
}

static int l_vector2_dot(lua_State* L)
{
	auto& VecA = *check_vec2(L, 1);
	auto& VecB = *check_vec2(L, 2);

	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

static int l_vector2_distance(lua_State* L)
{
	auto& VecA = *check_vec2(L, 1);
	auto& VecB = *check_vec2(L, 2);

	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

static int l_vector2_length(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

static int l_vector2_m_gc(lua_State* L)
{
	delete check_vec2(L, 1);
	return 0;
}

static int l_vector2_m_tostring(lua_State* L)
{
	auto& Vec = *check_vec2(L, 1);
	std::stringstream ss;
	ss << "(" << Vec[0] << "," << Vec[1] << ")";
	lua_pushstring(L, ss.str().c_str());

	return 1;
}

static int l_vector2_m_eq(lua_State* L)
{
	auto& VecA = *check_vec2(L, 1);
	auto& VecB = *check_vec2(L, 2);
	lua_pushboolean(L, VecA == VecB);
	return 1;
}

static int l_vector2_m_len(lua_State* L)
{
	check_vec2(L, 1);
	lua_pushnumber(L, 2);
	return 1;
}

static int l_vector2_m_mul(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = luaL_checknumber(L, 1);
		auto& Vec = *check_vec2(L, 2);
		PushLuaValue(L, Num * Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *check_vec2(L, 1);
		float Num = luaL_checknumber(L, 2);
		PushLuaValue(L, Vec * Num);
		return 1;
	}
	else
	{
		return l_vector2_dot(L);
	}
}

static const struct luaL_reg vector2_f[] = {
	{ "new", l_vector2_new },
	{ "table", l_vector2_table },
	{ "get", l_vector2_get },
	{ "set", l_vector2_set },
	{ "setAll", l_vector2_setall },
	{ "dot", l_vector2_dot },
	{ "distance", l_vector2_distance },
	{ "length", l_vector2_length },
	{NULL, NULL}
};

static const struct luaL_reg vector2_m[] = {
	{ "__gc", l_vector2_m_gc },
	{ "__eq", l_vector2_m_eq },
	{ "__tostring", l_vector2_m_tostring },
	{ "__len", l_vector2_m_len },
	{ "__mul", l_vector2_m_mul },
	{ "table", l_vector2_table },
	{ "get", l_vector2_get },
	{ "set", l_vector2_set },
	{ "setAll", l_vector2_setall },
	{ "dot", l_vector2_dot },
	{ "distance", l_vector2_distance },
	{ "length", l_vector2_length },
	{ NULL, NULL }
};

int luaopen_vector2(lua_State* L)
{
	luaL_newmetatable(L, "Wake.Vector2");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, vector2_m);

	luaL_register(L, "Vector2", vector2_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector2);