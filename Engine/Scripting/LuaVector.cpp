#include "LuaVector.h"

#include "LuaLibRegistry.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
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

glm::vec2* luaW_checkvector2(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == 2, 1, "table must be of length 2");

		lua_pushnumber(L, 1);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "first index of table must be a number");
		double X = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 2);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "second index of table must be a number");
		double Y = lua_tonumber(L, -1);
		PushLuaValue(L, glm::vec2(X, Y));
		return luaW_checkvector2(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, "Wake.Vector2");
	luaL_argcheck(L, Data != NULL, idx, "'Vector2' expected");
	return ((Vec2Container*)Data)->Vector;
}

static int l_vector2_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 0, 1 or 2 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::vec2());
		return 1;

	case 1:
		{
			glm::vec2* Vec = luaW_checkvector2(L, 1);
			PushLuaValue(L, *Vec);
			return 1;
		}

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
	auto& Vec = *luaW_checkvector2(L, 1);

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
	auto& Vec = *luaW_checkvector2(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index == 1 || Index == 2, 2, "must be between 1 and 2");
	lua_pushnumber(L, Vec[Index-1]);
	return 1;
}

static int l_vector2_set(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index == 1 || Index == 2, 2, "must be between 1 and 2");
	double Value = luaL_checknumber(L, 3);
	Vec[Index-1] = (float)Value;
	return 0;
}

static int l_vector2_setall(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	Vec[0] = (float)luaL_checknumber(L, 2);
	Vec[1] = (float)luaL_checknumber(L, 3);
	return 0;
}

static int l_vector2_dot(lua_State* L)
{
	auto& VecA = *luaW_checkvector2(L, 1);
	auto& VecB = *luaW_checkvector2(L, 2);

	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

static int l_vector2_distance(lua_State* L)
{
	auto& VecA = *luaW_checkvector2(L, 1);
	auto& VecB = *luaW_checkvector2(L, 2);

	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

static int l_vector2_length(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

static int l_vector2_apply(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[0]);
	lua_call(L, 1, 1);

	float X = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_call(L, 1, 1);

	float Y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	PushLuaValue(L, glm::vec2(X, Y));
	return 1;
}

static int l_vector2_normalized(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	PushLuaValue(L, glm::normalize(Vec));
	return 1;
}

static int l_vector2_reflect(lua_State* L)
{
	auto& VecI = *luaW_checkvector2(L, 1);
	auto& VecN = *luaW_checkvector2(L, 2);
	PushLuaValue(L, glm::reflect(VecI, VecN));
	return 1;
}

static int l_vector2_refract(lua_State* L)
{
	auto& VecI = *luaW_checkvector2(L, 1);
	auto& VecN = *luaW_checkvector2(L, 2);
	float eta = (float)luaL_checknumber(L, 3);
	PushLuaValue(L, glm::refract(VecI, VecN, eta));
	return 1;
}

static int l_vector2_m_gc(lua_State* L)
{
	delete luaW_checkvector2(L, 1);
	return 0;
}

static int l_vector2_m_tostring(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	lua_pushstring(L, glm::to_string(Vec).c_str());

	return 1;
}

static int l_vector2_m_eq(lua_State* L)
{
	auto& VecA = *luaW_checkvector2(L, 1);
	auto& VecB = *luaW_checkvector2(L, 2);
	lua_pushboolean(L, VecA == VecB);
	return 1;
}

static int l_vector2_m_len(lua_State* L)
{
	luaW_checkvector2(L, 1);
	lua_pushnumber(L, 2);
	return 1;
}

static int l_vector2_m_unm(lua_State* L)
{
	auto& Vec = *luaW_checkvector2(L, 1);
	PushLuaValue(L, -Vec);
	return 1;
}

static int l_vector2_m_add(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector2(L, 2);
		PushLuaValue(L, Num + Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector2(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec + Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector2(L, 1);
		auto& VecB = *luaW_checkvector2(L, 2);
		PushLuaValue(L, VecA + VecB);
		return 1;
	}
}

static int l_vector2_m_sub(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector2(L, 2);
		PushLuaValue(L, Num - Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector2(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec - Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector2(L, 1);
		auto& VecB = *luaW_checkvector2(L, 2);
		PushLuaValue(L, VecA - VecB);
		return 1;
	}
}

static int l_vector2_m_mul(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector2(L, 2);
		PushLuaValue(L, Num * Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector2(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec * Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector2(L, 1);
		auto& VecB = *luaW_checkvector2(L, 2);
		PushLuaValue(L, VecA * VecB);
		return 1;
	}
}

static int l_vector2_m_div(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector2(L, 2);
		PushLuaValue(L, Num / Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector2(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec / Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector2(L, 1);
		auto& VecB = *luaW_checkvector2(L, 2);
		PushLuaValue(L, VecA / VecB);
		return 1;
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
	{ "apply", l_vector2_apply },
	{ "normalized", l_vector2_normalized },
	{ "reflect", l_vector2_reflect },
	{ "refract", l_vector2_refract },
	{NULL, NULL}
};

static const struct luaL_reg vector2_m[] = {
	{ "__gc", l_vector2_m_gc },
	{ "__eq", l_vector2_m_eq },
	{ "__tostring", l_vector2_m_tostring },
	{ "__len", l_vector2_m_len },
	{ "__unm", l_vector2_m_unm },
	{ "__add", l_vector2_m_add },
	{ "__sub", l_vector2_m_sub },
	{ "__mul", l_vector2_m_mul },
	{ "__div", l_vector2_m_div },
	{ "table", l_vector2_table },
	{ "get", l_vector2_get },
	{ "set", l_vector2_set },
	{ "setAll", l_vector2_setall },
	{ "dot", l_vector2_dot },
	{ "distance", l_vector2_distance },
	{ "length", l_vector2_length },
	{ "apply", l_vector2_apply },
	{ "normalized", l_vector2_normalized },
	{ "reflect", l_vector2_reflect },
	{ "refract", l_vector2_refract },
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

struct Vec3Container
{
	glm::vec3* Vector;
};

void PushLuaValue(lua_State* L, const glm::vec3& Value)
{
	Vec3Container* Data = (Vec3Container*)lua_newuserdata(L, sizeof(Vec3Container));
	Data->Vector = new glm::vec3(Value);

	luaL_getmetatable(L, "Wake.Vector3");
	lua_setmetatable(L, -2);
}

glm::vec3* luaW_checkvector3(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == 3, 1, "table must be of length 3");

		lua_pushnumber(L, 1);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "first index of table must be a number");
		double X = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 2);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "second index of table must be a number");
		double Y = lua_tonumber(L, -1);

		lua_pushnumber(L, 3);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "third index of table must be a number");
		double Z = lua_tonumber(L, -1);

		PushLuaValue(L, glm::vec3(X, Y, Z));
		return luaW_checkvector3(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, "Wake.Vector3");
	luaL_argcheck(L, Data != NULL, idx, "'Vector3' expected");
	return ((Vec3Container*)Data)->Vector;
}

static int l_vector3_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 0, 1 or 3 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::vec3());
		return 1;

	case 1:
		{
			glm::vec3* Vec = luaW_checkvector3(L, 1);
			PushLuaValue(L, *Vec);
			return 1;
		}

	case 3:
		{
			double X = luaL_checknumber(L, 1);
			double Y = luaL_checknumber(L, 2);
			double Z = luaL_checknumber(L, 3);
			PushLuaValue(L, glm::vec3(X, Y, Z));
			return 1;
		}
	}
}

static int l_vector3_table(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, Vec[0]);
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushnumber(L, Vec[2]);
	lua_settable(L, -3);

	return 1;
}

static int l_vector3_get(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= 3, 2, "must be between 1 and 3");
	lua_pushnumber(L, Vec[Index - 1]);
	return 1;
}

static int l_vector3_set(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= 3, 2, "must be between 1 and 3");
	double Value = luaL_checknumber(L, 3);
	Vec[Index - 1] = (float)Value;
	return 0;
}

static int l_vector3_setall(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	Vec[0] = (float)luaL_checknumber(L, 2);
	Vec[1] = (float)luaL_checknumber(L, 3);
	Vec[2] = (float)luaL_checknumber(L, 4);
	return 0;
}

static int l_vector3_dot(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);

	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

static int l_vector3_distance(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);

	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

static int l_vector3_length(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

static int l_vector3_apply(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[0]);
	lua_call(L, 1, 1);

	float X = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_call(L, 1, 1);

	float Y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[2]);
	lua_call(L, 1, 1);

	float Z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	PushLuaValue(L, glm::vec3(X, Y, Z));
	return 1;
}

static int l_vector3_normalized(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	PushLuaValue(L, glm::normalize(Vec));
	return 1;
}

static int l_vector3_reflect(lua_State* L)
{
	auto& VecI = *luaW_checkvector3(L, 1);
	auto& VecN = *luaW_checkvector3(L, 2);
	PushLuaValue(L, glm::reflect(VecI, VecN));
	return 1;
}

static int l_vector3_refract(lua_State* L)
{
	auto& VecI = *luaW_checkvector3(L, 1);
	auto& VecN = *luaW_checkvector3(L, 2);
	float eta = (float)luaL_checknumber(L, 3);
	PushLuaValue(L, glm::refract(VecI, VecN, eta));
	return 1;
}

static int l_vector3_cross(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);

	PushLuaValue(L, glm::cross(VecA, VecB));
	return 1;
}

static int l_vector3_m_gc(lua_State* L)
{
	delete luaW_checkvector3(L, 1);
	return 0;
}

static int l_vector3_m_eq(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);
	lua_pushboolean(L, VecA == VecB);
	return 1;
}

static int l_vector3_m_tostring(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	lua_pushstring(L, glm::to_string(Vec).c_str());

	return 1;
}

static int l_vector3_m_len(lua_State* L)
{
	luaW_checkvector3(L, 1);
	lua_pushnumber(L, 3);
	return 1;
}

static int l_vector3_m_unm(lua_State* L)
{
	auto& Vec = *luaW_checkvector3(L, 1);
	PushLuaValue(L, -Vec);
	return 1;
}

static int l_vector3_m_add(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector3(L, 2);
		PushLuaValue(L, Num + Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector3(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec + Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector3(L, 1);
		auto& VecB = *luaW_checkvector3(L, 2);
		PushLuaValue(L, VecA + VecB);
		return 1;
	}
}

static int l_vector3_m_sub(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector3(L, 2);
		PushLuaValue(L, Num - Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector3(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec - Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector3(L, 1);
		auto& VecB = *luaW_checkvector3(L, 2);
		PushLuaValue(L, VecA - VecB);
		return 1;
	}
}

static int l_vector3_m_mul(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector3(L, 2);
		PushLuaValue(L, Num * Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector3(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec * Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector3(L, 1);
		auto& VecB = *luaW_checkvector3(L, 2);
		PushLuaValue(L, VecA * VecB);
		return 1;
	}
}

static int l_vector3_m_div(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector3(L, 2);
		PushLuaValue(L, Num / Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector3(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec / Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector3(L, 1);
		auto& VecB = *luaW_checkvector3(L, 2);
		PushLuaValue(L, VecA / VecB);
		return 1;
	}
}

static const struct luaL_reg vector3_f[] = {
	{ "new", l_vector3_new },
	{ "table", l_vector3_table },
	{ "get", l_vector3_get },
	{ "set", l_vector3_set },
	{ "setAll", l_vector3_setall },
	{ "dot", l_vector3_dot },
	{ "distance", l_vector3_distance },
	{ "length", l_vector3_length },
	{ "apply", l_vector3_apply },
	{ "normalized", l_vector3_normalized },
	{ "reflect", l_vector3_reflect },
	{ "refract", l_vector3_refract },
	{ "cross", l_vector3_cross },
	{ NULL, NULL }
};

static const struct luaL_reg vector3_m[] = {
	{ "__gc", l_vector3_m_gc },
	{ "__eq", l_vector3_m_eq },
	{ "__tostring", l_vector3_m_tostring },
	{ "__len", l_vector3_m_len },
	{ "__unm", l_vector3_m_unm },
	{ "__add", l_vector3_m_add },
	{ "__sub", l_vector3_m_sub },
	{ "__mul", l_vector3_m_mul },
	{ "__div", l_vector3_m_div },
	{ "table", l_vector3_table },
	{ "get", l_vector3_get },
	{ "set", l_vector3_set },
	{ "setAll", l_vector3_setall },
	{ "dot", l_vector3_dot },
	{ "distance", l_vector3_distance },
	{ "length", l_vector3_length },
	{ "apply", l_vector3_apply },
	{ "normalized", l_vector3_normalized },
	{ "reflect", l_vector3_reflect },
	{ "refract", l_vector3_refract },
	{ "cross", l_vector3_cross },
	{ NULL, NULL }
};

int luaopen_vector3(lua_State* L)
{
	luaL_newmetatable(L, "Wake.Vector3");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, vector3_m);

	luaL_register(L, "Vector3", vector3_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector3);

struct Vec4Container
{
	glm::vec4* Vector;
};

void PushLuaValue(lua_State* L, const glm::vec4& Value)
{
	Vec4Container* Data = (Vec4Container*)lua_newuserdata(L, sizeof(Vec4Container));
	Data->Vector = new glm::vec4(Value);

	luaL_getmetatable(L, "Wake.Vector4");
	lua_setmetatable(L, -2);
}

glm::vec4* luaW_checkvector4(lua_State* L, int idx)
{
	if (lua_istable(L, idx))
	{
		luaL_argcheck(L, lua_objlen(L, idx) == 4, 1, "table must be of length 4");

		lua_pushnumber(L, 1);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "first index of table must be a number");
		double X = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushnumber(L, 2);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "second index of table must be a number");
		double Y = lua_tonumber(L, -1);

		lua_pushnumber(L, 3);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "third index of table must be a number");
		double Z = lua_tonumber(L, -1);

		lua_pushnumber(L, 4);
		lua_gettable(L, idx);
		luaL_argcheck(L, lua_isnumber(L, -1), 1, "fourth index of table must be a number");
		double W = lua_tonumber(L, -1);

		PushLuaValue(L, glm::vec4(X, Y, Z, W));
		return luaW_checkvector4(L, -1);
	}

	void* Data = luaL_checkudata(L, idx, "Wake.Vector4");
	luaL_argcheck(L, Data != NULL, idx, "'Vector4' expected");
	return ((Vec4Container*)Data)->Vector;
}

static int l_vector4_new(lua_State* L)
{
	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "expected 0, 1 or 4 arguments");
		return 0;

	case 0:
		PushLuaValue(L, glm::vec4());
		return 1;

	case 1:
	{
		glm::vec4* Vec = luaW_checkvector4(L, 1);
		PushLuaValue(L, *Vec);
		return 1;
	}

	case 4:
	{
		double X = luaL_checknumber(L, 1);
		double Y = luaL_checknumber(L, 2);
		double Z = luaL_checknumber(L, 3);
		double W = luaL_checknumber(L, 4);
		PushLuaValue(L, glm::vec4(X, Y, Z, W));
		return 1;
	}
	}
}

static int l_vector4_table(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, Vec[0]);
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushnumber(L, Vec[2]);
	lua_settable(L, -3);

	lua_pushnumber(L, 4);
	lua_pushnumber(L, Vec[3]);
	lua_settable(L, -3);

	return 1;
}

static int l_vector4_get(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= 4, 2, "must be between 1 and 4");
	lua_pushnumber(L, Vec[Index - 1]);
	return 1;
}

static int l_vector4_set(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= 4, 2, "must be between 1 and 4");
	double Value = luaL_checknumber(L, 3);
	Vec[Index - 1] = (float)Value;
	return 0;
}

static int l_vector4_m_gc(lua_State* L)
{
	delete luaW_checkvector4(L, 1);
	return 0;
}

static int l_vector4_m_eq(lua_State* L)
{
	auto& VecA = *luaW_checkvector4(L, 1);
	auto& VecB = *luaW_checkvector4(L, 2);
	lua_pushboolean(L, VecA == VecB);
	return 1;
}

static int l_vector4_setall(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	Vec[0] = (float)luaL_checknumber(L, 2);
	Vec[1] = (float)luaL_checknumber(L, 3);
	Vec[2] = (float)luaL_checknumber(L, 4);
	Vec[3] = (float)luaL_checknumber(L, 5);
	return 0;
}

static int l_vector4_dot(lua_State* L)
{
	auto& VecA = *luaW_checkvector4(L, 1);
	auto& VecB = *luaW_checkvector4(L, 2);

	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

static int l_vector4_distance(lua_State* L)
{
	auto& VecA = *luaW_checkvector4(L, 1);
	auto& VecB = *luaW_checkvector4(L, 2);

	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

static int l_vector4_length(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

static int l_vector4_apply(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[0]);
	lua_call(L, 1, 1);

	float X = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[1]);
	lua_call(L, 1, 1);

	float Y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[2]);
	lua_call(L, 1, 1);

	float Z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushvalue(L, 2);
	lua_pushnumber(L, Vec[3]);
	lua_call(L, 1, 1);

	float W = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	PushLuaValue(L, glm::vec4(X, Y, Z, W));
	return 1;
}

static int l_vector4_normalized(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	PushLuaValue(L, glm::normalize(Vec));
	return 1;
}

static int l_vector4_reflect(lua_State* L)
{
	auto& VecI = *luaW_checkvector4(L, 1);
	auto& VecN = *luaW_checkvector4(L, 2);
	PushLuaValue(L, glm::reflect(VecI, VecN));
	return 1;
}

static int l_vector4_refract(lua_State* L)
{
	auto& VecI = *luaW_checkvector4(L, 1);
	auto& VecN = *luaW_checkvector4(L, 2);
	float eta = (float)luaL_checknumber(L, 3);
	PushLuaValue(L, glm::refract(VecI, VecN, eta));
	return 1;
}

static int l_vector4_m_tostring(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	lua_pushstring(L, glm::to_string(Vec).c_str());

	return 1;
}

static int l_vector4_m_len(lua_State* L)
{
	luaW_checkvector4(L, 1);
	lua_pushnumber(L, 4);
	return 1;
}

static int l_vector4_m_unm(lua_State* L)
{
	auto& Vec = *luaW_checkvector4(L, 1);
	PushLuaValue(L, -Vec);
	return 1;
}

static int l_vector4_m_add(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector4(L, 2);
		PushLuaValue(L, Num + Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector4(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec + Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector4(L, 1);
		auto& VecB = *luaW_checkvector4(L, 2);
		PushLuaValue(L, VecA + VecB);
		return 1;
	}
}

static int l_vector4_m_sub(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector4(L, 2);
		PushLuaValue(L, Num - Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector4(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec - Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector4(L, 1);
		auto& VecB = *luaW_checkvector4(L, 2);
		PushLuaValue(L, VecA - VecB);
		return 1;
	}
}

static int l_vector4_m_mul(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector4(L, 2);
		PushLuaValue(L, Num * Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector4(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec * Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector4(L, 1);
		auto& VecB = *luaW_checkvector4(L, 2);
		PushLuaValue(L, VecA * VecB);
		return 1;
	}
}

static int l_vector4_m_div(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		float Num = (float)luaL_checknumber(L, 1);
		auto& Vec = *luaW_checkvector4(L, 2);
		PushLuaValue(L, Num / Vec);
		return 1;
	}
	else if (lua_isnumber(L, 2))
	{
		auto& Vec = *luaW_checkvector4(L, 1);
		float Num = (float)luaL_checknumber(L, 2);
		PushLuaValue(L, Vec / Num);
		return 1;
	}
	else
	{
		auto& VecA = *luaW_checkvector4(L, 1);
		auto& VecB = *luaW_checkvector4(L, 2);
		PushLuaValue(L, VecA / VecB);
		return 1;
	}
}

static const struct luaL_reg vector4_f[] = {
	{ "new", l_vector4_new },
	{ "table", l_vector4_table },
	{ "get", l_vector4_get },
	{ "set", l_vector4_set },
	{ "setAll", l_vector4_setall },
	{ "dot", l_vector4_dot },
	{ "distance", l_vector4_distance },
	{ "length", l_vector4_length },
	{ "apply", l_vector4_apply },
	{ "normalized", l_vector4_normalized },
	{ "reflect", l_vector4_reflect },
	{ "refract", l_vector4_refract },
	{ NULL, NULL }
};

static const struct luaL_reg vector4_m[] = {
	{ "__gc", l_vector4_m_gc },
	{ "__eq", l_vector4_m_eq },
	{ "__tostring", l_vector4_m_tostring },
	{ "__len", l_vector4_m_len },
	{ "__unm", l_vector4_m_unm },
	{ "__add", l_vector4_m_add },
	{ "__sub", l_vector4_m_sub },
	{ "__mul", l_vector4_m_mul },
	{ "__div", l_vector4_m_div },
	{ "new", l_vector4_new },
	{ "table", l_vector4_table },
	{ "get", l_vector4_get },
	{ "set", l_vector4_set },
	{ "setAll", l_vector4_setall },
	{ "dot", l_vector4_dot },
	{ "distance", l_vector4_distance },
	{ "length", l_vector4_length },
	{ "apply", l_vector4_apply },
	{ "normalized", l_vector4_normalized },
	{ "reflect", l_vector4_reflect },
	{ "refract", l_vector4_refract },
	{ NULL, NULL }
};

int luaopen_vector4(lua_State* L)
{
	luaL_newmetatable(L, "Wake.Vector4");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, vector4_m);

	luaL_register(L, "Vector4", vector4_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_vector4);