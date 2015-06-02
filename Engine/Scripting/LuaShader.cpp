#include "LuaShader.h"

#include "LuaLibRegistry.h"

#include "../Engine/Shader.h"

#include <sstream>

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

int l_m_load(lua_State* L)
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

int l_m_gc(lua_State* L)
{
	delete luaW_checkshader(L, 1);
	return 0;
}

int l_m_tostring(lua_State* L)
{
	auto* Shader = luaW_checkshader(L, 1);
	std::stringstream ss;
	ss << "ShaderProgram(" << Shader->GetProgram() << ")";
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

static const struct luaL_reg shaderlib_f[] = {
	{ "load", l_m_load },
	{NULL, NULL}
};

static const struct luaL_reg shaderlib_m[] = {
	{ "__gc", l_m_gc },
	{ "__tostring", l_m_tostring },
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