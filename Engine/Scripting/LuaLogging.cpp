#include "LuaLogging.h"

#include "../Logging/LogMacros.h"

#include "LuaLibRegistry.h"

Logger ScriptLogger("sg");

struct LuaLogger
{
	Logger* Logger;
};

Logger* luaW_checklogger(lua_State* L)
{
	void* Data = luaL_checkudata(L, 1, W_MT_LOGGER);
	luaL_argcheck(L, Data != NULL, 1, "'log' expected");
	return ((LuaLogger*)Data)->Logger;
}

static void log_impl(lua_State* L, LogLevel Level)
{
	if (lua_type(L, 1) == LUA_TUSERDATA)
	{
		Logger* Log = luaW_checklogger(L);
		const char* Str = luaL_checkstring(L, 2);
		WAKE_LOG(*Log, Level, Str);
	}
	else
	{
		const char* Str = luaL_checkstring(L, 1);
		WAKE_LOG(ScriptLogger, Level, Str);
	}
}

static int l_new(lua_State* L)
{
	LuaLogger* LogData = (LuaLogger*)lua_newuserdata(L, sizeof(LuaLogger));
	LogData->Logger = new Logger(luaL_checkstring(L, 1));

	luaL_getmetatable(L, W_MT_LOGGER);
	lua_setmetatable(L, -2);

	return 1;
}

static int l_trace(lua_State* L)
{
	log_impl(L, LogLevel::Trace);

	return 0;
}

static int l_debug(lua_State* L)
{
	log_impl(L, LogLevel::Debug);

	return 0;
}

static int l_info(lua_State* L)
{
	log_impl(L, LogLevel::Info);

	return 0;
}

static int l_warn(lua_State* L)
{
	log_impl(L, LogLevel::Warn);

	return 0;
}

static int l_error(lua_State* L)
{
	log_impl(L, LogLevel::Error);

	return 0;
}

static int l_fatal(lua_State* L)
{
	log_impl(L, LogLevel::Fatal);

	return 0;
}

static int l_m_gc(lua_State* L)
{
	delete luaW_checklogger(L);
	return 0;
}

static int l_m_tostring(lua_State* L)
{
	Logger* Log = luaW_checklogger(L);
	lua_pushstring(L, Log->GetName());
	return 1;
}

static const struct luaL_reg loglib_f[] = {
	{ "trace", l_trace },
	{ "debug", l_debug },
	{ "info", l_info },
	{ "warn", l_warn },
	{ "error", l_error },
	{ "fatal", l_fatal },
	{ "new", l_new },
	{NULL, NULL}
};

static const struct luaL_reg loglib_m[] = {
	{ "__gc", l_m_gc },
	{ "__tostring", l_m_tostring },
	{ "trace", l_trace },
	{ "debug", l_debug },
	{ "info", l_info },
	{ "warn", l_warn },
	{ "error", l_error },
	{ "fatal", l_fatal },
	{NULL, NULL}
};

int luaopen_logger(lua_State* L)
{
	luaL_newmetatable(L, W_MT_LOGGER);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, loglib_m);

	luaL_register(L, "Logger", loglib_f);
	return 1;
}

W_REGISTER_LUA_LIB(luaopen_logger);