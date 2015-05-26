#include "LuaEvent.h"

#include "LuaLibRegistry.h"

ILuaEventWrapper* luaW_checkevent(lua_State* L)
{
	void* DataPtr = luaL_checkudata(L, 1, W_MT_EVENT);
	luaL_argcheck(L, DataPtr != NULL, 1, "'event' expected");
	LEWContainer* LEC = (LEWContainer*)DataPtr;
	ILuaEventWrapper* Container = LEC->Wrapper;
	return Container;
}

static int l_bind(lua_State* L)
{
	auto Container = luaW_checkevent(L);

	switch (lua_gettop(L))
	{
		default:
			luaL_error(L, "bind requires 2 or 3 arguments");
			break;

		case 2:
			luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
			lua_pushvalue(L, 2);
			Container->Bind(L);
			break;

		case 3:
			luaL_argcheck(L, lua_istable(L, 2), 2, "'table' expected");
			lua_pushvalue(L, 2);
			luaL_argcheck(L, lua_isfunction(L, 3), 3, "'function' expected");
			lua_pushvalue(L, 3);
			Container->BindTable(L);
			break;
	}

	return 0;
}

static int l_unbind(lua_State* L)
{
	auto Container = luaW_checkevent(L);

	switch (lua_gettop(L))
	{
	default:
		luaL_error(L, "bind requires 2 or 3 arguments");
		break;

	case 2:
		luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
		lua_pushvalue(L, 2);
		Container->Unbind(L);
		break;

	case 3:
		luaL_argcheck(L, lua_istable(L, 2), 2, "'table' expected");
		lua_pushvalue(L, 2);
		luaL_argcheck(L, lua_isfunction(L, 3), 3, "'function' expected");
		lua_pushvalue(L, 3);
		Container->UnbindTable(L);
		break;
	}

	return 0;
}

static int l_clear(lua_State* L)
{
	auto Container = luaW_checkevent(L);
	Container->Clear();

	return 0;
}

static int l_m_gc(lua_State* L)
{
	delete luaW_checkevent(L);
	return 0;
}

static int l_m_tostring(lua_State* L)
{
	auto Container = luaW_checkevent(L);
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
	luaL_newmetatable(L, W_MT_EVENT);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_register(L, NULL, eventlib_m);

	luaL_register(L, "event", eventlib_f);
	return 1;
}

// Must exist before libraries start registering events
W_REGISTER_LUA_LIB_PRIORITY(luaopen_event, 100);