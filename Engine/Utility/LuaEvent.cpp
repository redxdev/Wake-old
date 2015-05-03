#include "LuaEvent.h"

template <typename ... Arguments>
LuaCallable<Arguments...>::LuaCallable(lua_State* L)
{
	if (!lua_isfunction(L, -1))
	{
		luaL_error(L, "Top of stack must be a function");
	}

	FuncRef = luaL_ref(L, LUA_REGISTRYINDEX);
	this->L = L;
}

template <typename ... Arguments>
LuaCallable<Arguments...>::~LuaCallable()
{
	luaL_unref(L, LUA_REGISTRYINDEX, FuncRef);
}

template<typename... Arguments>
void LuaCallable<Arguments...>::Call(Arguments... Args)
{
	if (!L)
		return;

	PushLuaValue(L, Args...);
}

template <typename ... Arguments>
bool LuaCallable<Arguments...>::Equals(Callable<Arguments...>* Other) const
{
	auto OtherPtr = dynamic_cast<const LuaCallable<Arguments...>*>(Other);
	if (!OtherPtr)
		return false;

	return OtherPtr->L == this->L && OtherPtr->FuncRef == this->FuncRef;
}

template<typename... T>
struct EventContainer
{
	EventContainer(Event<T...>& E)
	{
		this->E = E;
	}

	Event<T...>& E;
};

template <typename ... T>
void PushLuaValue(lua_State* L, Event<T...>& E)
{
	EventContainer<T...>* Container = (EventContainer<T...>*)lua_newuserdata(L, sizeof(EventContainer<T...>));
	Container->E = E;

	luaL_getmetatable(L, "Wake.event");
	lua_setmetatable(L, -2);
}

static const struct luaL_reg eventlib_f[] = {
	{NULL, NULL}
};

static const struct luaL_reg eventlib_m[] = {
	
	{NULL, NULL}
};

int luaopen_event(lua_State* L)
{
}