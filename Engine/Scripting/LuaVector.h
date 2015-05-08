#pragma once

#include "../Utility/LuaExt.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define W_MT_VEC2 ("Wake.Vector2")
#define W_MT_VEC3 ("Wake.Vec3")
#define W_MT_VEC4 ("Wake.Vec4")

void PushLuaValue(lua_State* L, const glm::vec2& Value);
glm::vec2* luaW_checkvector2(lua_State* L, int idx);
int luaopen_vector2(lua_State* L);

void PushLuaValue(lua_State* L, const glm::vec3& Value);
glm::vec3* luaW_checkvector3(lua_State* L, int idx);
int luaopen_vector3(lua_State* L);

void PushLuaValue(lua_State* L, const glm::vec4& Value);
glm::vec4* luaW_checkvector4(lua_State* L, int idx);
int luaopen_vector4(lua_State* L);