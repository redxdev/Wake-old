#pragma once

#include "luaext.h"
#include "Event.h"

template<typename... Arguments>
class LuaCallable : public Callable<Arguments...>
{
public:
	LuaCallable(lua_State* L);
	~LuaCallable();

	virtual void Call(Arguments... Args) override;
	
	virtual bool Equals(Callable<Arguments...>* Other) const override;

private:
	lua_State* L;
	int FuncRef;
};

template<typename... T>
void PushLuaValue(lua_State* L, Event<T...>& E);

int luaopen_event(lua_State* L);