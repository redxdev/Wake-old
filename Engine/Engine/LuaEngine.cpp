#include "LuaEngine.h"

#include "GEngine.h"

#include "../Scripting/LuaLibRegistry.h"

static int l_stop(lua_State* L)
{
	W_ENGINE.Stop();
	return 0;
}

static int l_isRunning(lua_State* L)
{
	lua_pushboolean(L, W_ENGINE.IsRunning());
	return 1;
}

static int l_getDeltaTime(lua_State* L)
{
	lua_pushnumber(L, W_ENGINE.GetDeltaTime());
	return 1;
}

static const struct luaL_reg enginelib_f[] = {
	{ "stop", l_stop },
	{ "isRunning", l_isRunning },
	{ "getDeltaTime", l_getDeltaTime },
	{NULL, NULL}
};

int luaopen_engine(lua_State* L)
{
	luaL_register(L, "engine", enginelib_f);
	return 1;
}

W_REGISTER_LUA_LIB(luaopen_engine);

static int readwopt_int(lua_State* L, const char* Key)
{
	lua_pushstring(L, Key);
	lua_gettable(L, -2);
	if (!lua_isnumber(L, -1))
		luaL_error(L, "%s must be a number", Key);

	int result = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

static const char* readwopt_str(lua_State* L, const char* Key)
{
	lua_pushstring(L, Key);
	lua_gettable(L, -2);
	if (!lua_isstring(L, -1))
		luaL_error(L, "%s must be a string", Key);

	const char* result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

static bool readwopt_bool(lua_State* L, const char* Key)
{
	lua_pushstring(L, Key);
	lua_gettable(L, -2);
	if (!lua_isboolean(L, -1))
		luaL_error(L, "%s must be a boolean", Key);

	bool result = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

static int l_window_initialize(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	WindowOptions WOpt;
	WOpt.Width = readwopt_int(L, "width");
	WOpt.Height = readwopt_int(L, "height");
	WOpt.DepthBits = readwopt_int(L, "depthBits");
	WOpt.StencilBits = readwopt_int(L, "stencilBits");
	WOpt.AntiAliasing = readwopt_int(L, "antiAliasing");
	WOpt.BitsPerPixel = readwopt_int(L, "bitsPerPixel");
	WOpt.OGL_Major = readwopt_int(L, "oglMajor");
	WOpt.OGL_Minor = readwopt_int(L, "oglMinor");
	WOpt.Title = readwopt_str(L, "title");
	WOpt.Fullscreen = readwopt_bool(L, "fullscreen");
	WOpt.VerticalSync = readwopt_bool(L, "verticalSync");

	lua_pushboolean(L, W_ENGINE.GetGameWindow().Initialize(WOpt));
	return 1;
}

static int l_window_isOpen(lua_State* L)
{
	lua_pushboolean(L, W_ENGINE.GetGameWindow().IsOpen());
	return 1;
}

static int l_window_getWidth(lua_State* L)
{
	lua_pushnumber(L, W_ENGINE.GetGameWindow().GetWidth());
	return 1;
}

static int l_window_getHeight(lua_State* L)
{
	lua_pushnumber(L, W_ENGINE.GetGameWindow().GetHeight());
	return 1;
}

static int l_window_setTitle(lua_State* L)
{
	W_ENGINE.GetGameWindow().SetTitle(luaL_checkstring(L, 1));
	return 0;
}

static const struct luaL_reg windowlib_f[] = {
	{ "initialize", l_window_initialize },
	{ "isOpen", l_window_isOpen },
	{ "getWidth", l_window_getWidth },
	{ "getHeight", l_window_getHeight },
	{NULL, NULL}
};

int luaopen_window(lua_State* L)
{
	luaL_register(L, "window", windowlib_f);
	return 1;
}

W_REGISTER_LUA_LIB(luaopen_window);