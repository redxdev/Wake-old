#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../Utility/LuaExt.h"

template<typename VecType>
struct VectorInfo
{
};

template<>
struct VectorInfo<glm::vec2>
{
	static inline int Elements()
	{
		return 2;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Vector2";
	}

	static inline const char* TypeName()
	{
		return "Vector2";
	}
};

template<>
struct VectorInfo<glm::vec3>
{
	static inline int Elements()
	{
		return 3;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Vector3";
	}

	static inline const char* TypeName()
	{
		return "Vector3";
	}
};

template<>
struct VectorInfo<glm::vec4>
{
	static inline int Elements()
	{
		return 4;
	}

	static inline const char* MetatableName()
	{
		return "Wake.Vector4";
	}

	static inline const char* TypeName()
	{
		return "Vector4";
	}
};

void PushLuaValue(lua_State* L, const glm::vec2& Value);
glm::vec2* luaW_checkvector2(lua_State* L, int idx);
int luaopen_vector2(lua_State* L);

void PushLuaValue(lua_State* L, const glm::vec3& Value);
glm::vec3* luaW_checkvector3(lua_State* L, int idx);
int luaopen_vector3(lua_State* L);

void PushLuaValue(lua_State* L, const glm::vec4& Value);
glm::vec4* luaW_checkvector4(lua_State* L, int idx);
int luaopen_vector4(lua_State* L);