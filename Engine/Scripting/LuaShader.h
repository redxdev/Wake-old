#pragma once

#include "../Utility/LuaExt.h"

#define W_MT_UNIFORM ("Wake.Uniform")
#define W_MT_SHADER ("Wake.Shader")

class Uniform;

Uniform& luaW_checkuniform(lua_State* L, int idx);
void PushLuaValue(lua_State* L, Uniform Uniform);

int luaopen_uniform(lua_State* L);

class ShaderProgram;

ShaderProgram* luaW_checkshader(lua_State* L, int idx);
void PushLuaValue(lua_State* L, ShaderProgram* Shader);

int luaopen_shader(lua_State* L);