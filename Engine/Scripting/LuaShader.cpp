#include "LuaShader.h"

#include "LuaLibRegistry.h"
#include "LuaMatrix.h"

#include "../Engine/Shader.h"

#include <sstream>

struct UniformContainer
{
	Uniform Uniform;
};

Uniform& luaW_checkuniform(lua_State* L, int idx)
{
	void* Data = luaL_checkudata(L, idx, W_MT_UNIFORM);
	luaL_argcheck(L, Data != NULL, idx, "'Uniform' expected");
	return ((UniformContainer*)Data)->Uniform;
}

void PushLuaValue(lua_State* L, Uniform Uniform)
{
	auto* UniformData = (UniformContainer*)lua_newuserdata(L, sizeof(UniformContainer));
	UniformData->Uniform = Uniform;

	luaL_getmetatable(L, W_MT_UNIFORM);
	lua_setmetatable(L, -2);
}

int l_uniform_set1f(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set1f((float)luaL_checknumber(L, 2));
	return 0;
}

int l_uniform_set2f(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set2f((float)luaL_checknumber(L, 2), (float)luaL_checknumber(L, 3));
	return 0;
}

int l_uniform_set3f(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set3f((float)luaL_checknumber(L, 2), (float)luaL_checknumber(L, 3), (float)luaL_checknumber(L, 4));
	return 0;
}

int l_uniform_set4f(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set4f(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

int l_uniform_set1i(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set1i(luaL_checkinteger(L, 2));
	return 0;
}

int l_uniform_set2i(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set2i(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
	return 0;
}

int l_uniform_set3i(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set3i(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
	return 0;
}

int l_uniform_set4i(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set4i(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
	return 0;
}

int l_uniform_set1ui(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set1ui(luaL_checkinteger(L, 2));
	return 0;
}

int l_uniform_set2ui(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set2ui(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
	return 0;
}

int l_uniform_set3ui(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set3ui(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
	return 0;
}

int l_uniform_set4ui(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.Set4ui(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
	return 0;
}

int l_uniform_setVec2(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetVec2(*luaW_checkvector2(L, 2));
	return 0;
}

int l_uniform_setVec3(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetVec3(*luaW_checkvector3(L, 2));
	return 0;
}

int l_uniform_setVec4(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetVec4(*luaW_checkvector4(L, 2));
	return 0;
}

int l_uniform_setMatrix2(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix2(*luaW_checkmatrix2x2(L, 2));
	return 0;
}

int l_uniform_setMatrix2x3(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix2x3(*luaW_checkmatrix2x3(L, 2));
	return 0;
}

int l_uniform_setMatrix2x4(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix2x4(*luaW_checkmatrix2x4(L, 2));
	return 0;
}

int l_uniform_setMatrix3x2(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix3x2(*luaW_checkmatrix3x2(L, 2));
	return 0;
}

int l_uniform_setMatrix3(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix3(*luaW_checkmatrix3x3(L, 2));
	return 0;
}

int l_uniform_setMatrix3x4(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix3x4(*luaW_checkmatrix3x4(L, 2));
	return 0;
}

int l_uniform_setMatrix4x2(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix4x2(*luaW_checkmatrix4x2(L, 2));
	return 0;
}

int l_uniform_setMatrix4x3(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix4x3(*luaW_checkmatrix4x3(L, 2));
	return 0;
}

int l_uniform_setMatrix4(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	Uniform.SetMatrix4(*luaW_checkmatrix4x4(L, 2));
	return 0;
}

int l_uniform_m_tostring(lua_State* L)
{
	auto& Uniform = luaW_checkuniform(L, 1);
	std::stringstream ss;
	ss << "Uniform(" << Uniform.GetProgram() << ", " << Uniform.GetLocation() << ")";
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

static const struct luaL_reg uniformlib_m[] = { // no __gc method needed, see UniformContainer definition
	{ "__tostring", l_uniform_m_tostring },
	{ "set1f", l_uniform_set1f },
	{ "set2f", l_uniform_set2f },
	{ "set3f", l_uniform_set3f },
	{ "set4f", l_uniform_set4f },
	{ "set1i", l_uniform_set1i },
	{ "set2i", l_uniform_set2i },
	{ "set3i", l_uniform_set3i },
	{ "set4i", l_uniform_set4i },
	{ "set1ui", l_uniform_set1ui },
	{ "set2ui", l_uniform_set2ui },
	{ "set3ui", l_uniform_set3ui },
	{ "set4ui", l_uniform_set4ui },
	{ "setVec2", l_uniform_setVec2 },
	{ "setVec3", l_uniform_setVec3 },
	{ "setVec4", l_uniform_setVec4 },
	{ "setMatrix2", l_uniform_setMatrix2 },
	{ "setMatrix2x4", l_uniform_setMatrix2x3 },
	{ "setMatrix2x3", l_uniform_setMatrix2x4 },
	{ "setMatrix3x2", l_uniform_setMatrix3x2 },
	{ "setMatrix3", l_uniform_setMatrix3 },
	{ "setMatrix3x4", l_uniform_setMatrix3x4 },
	{ "setMatrix4x2", l_uniform_setMatrix4x2 },
	{ "setMatrix4x3", l_uniform_setMatrix4x3 },
	{ "setMatrix4", l_uniform_setMatrix4 },
	{NULL, NULL}
};

int luaopen_uniform(lua_State* L)
{
	luaL_newmetatable(L, W_MT_UNIFORM);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, uniformlib_m);
	return 0;
}

W_REGISTER_LUA_LIB(luaopen_uniform);

struct ShaderContainer
{
	ShaderProgram* Shader;
};

ShaderProgram* luaW_checkshader(lua_State* L, int idx)
{
	void* Data = luaL_checkudata(L, idx, W_MT_SHADER);
	luaL_argcheck(L, Data != NULL, idx, "'Shader' expected");
	return ((ShaderContainer*)Data)->Shader;
}

void PushLuaValue(lua_State* L, ShaderProgram* Shader)
{
	auto* ShaderData = (ShaderContainer*)lua_newuserdata(L, sizeof(ShaderContainer));
	ShaderData->Shader = Shader;

	luaL_getmetatable(L, W_MT_SHADER);
	lua_setmetatable(L, -2);
}

int l_shader_load(lua_State* L)
{
	const char* VertexPath = luaL_checkstring(L, 1);
	const char* FragmentPath = luaL_checkstring(L, 2);

	GLuint VertexShader = ShaderProgram::LoadFile(VertexPath, GL_VERTEX_SHADER);
	if (VertexShader == 0)
	{
		lua_pushnil(L);
		lua_pushstring(L, "Failed to load vertex shader.");
		return 2;
	}

	GLuint FragmentShader = ShaderProgram::LoadFile(FragmentPath, GL_FRAGMENT_SHADER);
	if (FragmentShader == 0)
	{
		glDeleteShader(VertexShader);
		lua_pushnil(L);
		lua_pushstring(L, "Failed to load fragment shader.");
		return 2;
	}

	GLuint Program = ShaderProgram::LoadProgram(VertexShader, FragmentShader);
	if (Program == 0)
	{
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		lua_pushnil(L);
		lua_pushstring(L, "Failed to link shader program.");
		return 2;
	}

	ShaderProgram* Shader = new ShaderProgram(Program);
	PushLuaValue(L, Shader);
	return 1;
}

int l_shader_getUniform(lua_State* L)
{
	auto* Shader = luaW_checkshader(L, 1);
	auto Uniform = Shader->GetUniform(luaL_checkstring(L, 2));
	if (Uniform.GetLocation() == -1)
	{
		lua_pushnil(L);
		return 1;
	}

	PushLuaValue(L, Uniform);
	return 1;
}

int l_shader_m_gc(lua_State* L)
{
	delete luaW_checkshader(L, 1);
	return 0;
}

int l_shader_m_tostring(lua_State* L)
{
	auto* Shader = luaW_checkshader(L, 1);
	std::stringstream ss;
	ss << "ShaderProgram(" << Shader->GetProgram() << ")";
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

static const struct luaL_reg shaderlib_f[] = {
	{ "load", l_shader_load },
	{ "getUniform", l_shader_getUniform },
	{NULL, NULL}
};

static const struct luaL_reg shaderlib_m[] = {
	{ "__gc", l_shader_m_gc },
	{ "__tostring", l_shader_m_tostring },
	{ "getUniform", l_shader_getUniform },
	{NULL, NULL}
};

int luaopen_shader(lua_State* L)
{
	luaL_newmetatable(L, W_MT_SHADER);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, shaderlib_m);

	luaL_register(L, "Shader", shaderlib_f);
	return 1;
}

W_REGISTER_LUA_LIB(luaopen_shader);