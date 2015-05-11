#include "LuaVector.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "LuaLibRegistry.h"

//
// Generic functions
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
static int NewImpl(lua_State* L)
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
static int TableImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_newtable(L);
	for (unsigned int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_pushnumber(L, Vec[i]);
		lua_settable(L, -3);
	}

	return 1;
}

template<typename VecType>
static int GetImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= VectorInfo<VecType>::Elements(), 2, "index out of range");
	lua_pushnumber(L, Vec[Index - 1]);
	return 1;
}

template<typename VecType>
static int SetImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	int Index = (int)luaL_checknumber(L, 2);
	luaL_argcheck(L, Index >= 1 && Index <= VectorInfo<VecType>::Elements(), 2, "index out of range");
	float Value = (float)luaL_checknumber(L, 3);
	Vec[Index - 1] = Value;
	return 0;
}

template<typename VecType>
static int SetAllImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		Vec[i] = (float)luaL_checknumber(L, i + 2);
	}
	return 0;
}

template<typename VecType>
static int DotImpl(lua_State* L)
{
	auto& VecA = *CheckVectorImpl<VecType>(L, 1);
	auto& VecB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushnumber(L, glm::dot(VecA, VecB));
	return 1;
}

template<typename VecType>
static int DistanceImpl(lua_State* L)
{
	auto& VecA = *CheckVectorImpl<VecType>(L, 1);
	auto& VecB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushnumber(L, glm::distance(VecA, VecB));
	return 1;
}

template<typename VecType>
static int LengthImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_pushnumber(L, glm::length(Vec));
	return 1;
}

template<typename VecType>
static int ApplyImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

	VecType Result;
	for (int i = 0; i < VectorInfo<VecType>::Elements(); ++i)
	{
		lua_pushvalue(L, 2);
		lua_pushnumber(L, Vec[i]);
		lua_call(L, 1, 1);

		Result[i] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	PushVectorImpl<VecType>(L, Result);
	return 1;
}

template<typename VecType>
static int NormalizeImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	PushVectorImpl<VecType>(L, glm::normalize(Vec));
	return 1;
}

template<typename VecType>
static int ReflectImpl(lua_State* L)
{
	auto& VecI = *CheckVectorImpl<VecType>(L, 1);
	auto& VecN = *CheckVectorImpl<VecType>(L, 2);
	PushVectorImpl<VecType>(L, glm::reflect(VecI, VecN));
	return 1;
}

template<typename VecType>
static int RefractImpl(lua_State* L)
{
	auto& VecI = *CheckVectorImpl<VecType>(L, 1);
	auto& VecN = *CheckVectorImpl<VecType>(L, 1);
	float eta = (float)luaL_checknumber(L, 3);
	PushVectorImpl<VecType>(L, glm::refract(VecI, VecN, eta));
	return 1;
}

template<typename VecType>
static int M_GCImpl(lua_State* L)
{
	delete CheckVectorImpl<VecType>(L, 1);
	return 0;
}

template<typename VecType>
static int M_EqualImpl(lua_State* L)
{
	auto& MatA = *CheckVectorImpl<VecType>(L, 1);
	auto& MatB = *CheckVectorImpl<VecType>(L, 2);
	lua_pushboolean(L, MatA == MatB);
	return 1;
}

template<typename VecType>
static int M_ToStringImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	lua_pushstring(L, glm::to_string(Vec).c_str());
	return 1;
}

template<typename VecType>
static int M_LengthImpl(lua_State* L)
{
	CheckVectorImpl<VecType>(L, 1);
	lua_pushnumber(L, VectorInfo<VecType>::Elements());
	return 1;
}

template<typename VecType>
static int M_UnaryMinusImpl(lua_State* L)
{
	auto& Vec = *CheckVectorImpl<VecType>(L, 1);
	PushVectorImpl<VecType>(L, -Vec);
	return 1;
}

template<typename VecType>
static int M_AddImpl(lua_State* L)
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
static int M_SubImpl(lua_State* L)
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
static int M_MulImpl(lua_State* L)
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
static int M_DivImpl(lua_State* L)
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

//
// Specialized functions
//

static int Vec3_CrossImpl(lua_State* L)
{
	auto& VecA = *luaW_checkvector3(L, 1);
	auto& VecB = *luaW_checkvector3(L, 2);

	PushLuaValue(L, glm::cross(VecA, VecB));
	return 1;
}

//
// Registration
//

#define VECTOR_LIB_F(Name, Type, ...) \
static const luaL_reg Name##_f[] = { \
	{ "new", NewImpl<Type> }, \
	{ "table", TableImpl<Type> }, \
	{ "get", GetImpl<Type> }, \
	{ "set", SetImpl<Type> }, \
	{ "setAll", SetAllImpl<Type> }, \
	{ "dot", DotImpl<Type> }, \
	{ "distance", DistanceImpl<Type> }, \
	{ "length", LengthImpl<Type> }, \
	{ "apply", ApplyImpl<Type> }, \
	{ "normalize", NormalizeImpl<Type> }, \
	{ "reflect", ReflectImpl<Type> }, \
	{ "refract", RefractImpl<Type> }, \
	__VA_ARGS__, \
	{ NULL, NULL } \
}

#define VECTOR_LIB_M(Name, Type, ...) \
static const luaL_reg Name##_m[] = { \
	{ "__gc", M_GCImpl<Type> }, \
	{ "__eq", M_EqualImpl<Type> }, \
	{ "__tostring", M_ToStringImpl<Type> }, \
	{ "__len", M_LengthImpl<Type> }, \
	{ "__unm", M_UnaryMinusImpl<Type> }, \
	{ "__add", M_AddImpl<Type> }, \
	{ "__sub", M_SubImpl<Type> }, \
	{ "__mul", M_MulImpl<Type> }, \
	{ "__div", M_DivImpl<Type> }, \
	{ "table", TableImpl<Type> }, \
	{ "get", GetImpl<Type> }, \
	{ "set", SetImpl<Type> }, \
	{ "setAll", SetAllImpl<Type> }, \
	{ "dot", DotImpl<Type> }, \
	{ "distance", DistanceImpl<Type> }, \
	{ "length", LengthImpl<Type> }, \
	{ "apply", ApplyImpl<Type> }, \
	{ "normalize", NormalizeImpl<Type> }, \
	{ "reflect", ReflectImpl<Type> }, \
	{ "refract", RefractImpl<Type> }, \
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