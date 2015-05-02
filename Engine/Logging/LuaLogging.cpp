#include "LuaLogging.h"
#include "LogMacros.h"

Logger ScriptLogger("sg");

static int l_trace(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_TRACE(ScriptLogger, Str);

	return 0;
}

static int l_debug(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_DEBUG(ScriptLogger, Str);

	return 0;
}

static int l_info(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_INFO(ScriptLogger, Str);

	return 0;
}

static int l_warn(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_WARN(ScriptLogger, Str);

	return 0;
}

static int l_error(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_ERROR(ScriptLogger, Str);

	return 0;
}

static int l_fatal(lua_State* L)
{
	const char* Str = luaL_checkstring(L, 1);
	LOG_FATAL(ScriptLogger, Str);

	return 0;
}

static const struct luaL_reg loglib[] = {
	{ "trace", l_trace },
	{ "debug", l_debug },
	{ "info", l_info },
	{ "warn", l_warn },
	{ "error", l_error },
	{ "fatal", l_fatal },
	{NULL, NULL}
};

int luaopen_log(lua_State* L)
{
	luaL_register(L, "log", loglib);
	return 1;
}