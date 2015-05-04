#include "LuaEvent.h"

#include "LuaLibRegistry.h"

static ILuaEventWrapper* check_event(lua_State* L)
{
	void* DataPtr = luaL_checkudata(L, 1, "Wake.event");
	luaL_argcheck(L, DataPtr != NULL, 1, "'event' expected");
	LEWContainer* LEC = (LEWContainer*)DataPtr;
	ILuaEventWrapper* Container = LEC->Wrapper;
	return Container;
}

static int l_bind(lua_State* L)
{
	auto Container = check_event(L);

	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
	lua_pushvalue(L, 2);
	Container->Bind(L);

	return 0;
}

static int l_unbind(lua_State* L)
{
	auto Container = check_event(L);

	luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
	lua_pushvalue(L, 2);
	Container->Unbind(L);

	return 0;
}

static int l_clear(lua_State* L)
{
	auto Container = check_event(L);
	Container->Clear();

	return 0;
}

static int l_m_gc(lua_State* L)
{
	delete check_event(L);
	return 0;
}

static int l_m_tostring(lua_State* L)
{
	auto Container = check_event(L);
	lua_pushstring(L, "Event<>");
	return 1;
}

static const struct luaL_reg eventlib_f[] = {
	{ "bind", l_bind },
	{ "unbind", l_unbind },
	{ "clear", l_clear },
	{NULL, NULL}
};

static const struct luaL_reg eventlib_m[] = {
	{ "__gc", l_m_gc },
	{ "__tostring", l_m_tostring },
	{ "bind", l_bind },
	{ "unbind", l_unbind },
	{ "clear", l_clear },
	{NULL, NULL}
};

int luaopen_event(lua_State* L)
{
	luaL_newmetatable(L, "Wake.event");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, eventlib_m);

	luaL_register(L, "event", eventlib_f);
	return 1;
}

// Must exist before libraries start registering events
W_REGISTER_LUA_LIB_PRIORITY(luaopen_event, 100);