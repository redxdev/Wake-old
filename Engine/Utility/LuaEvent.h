#pragma once

#include "LuaExt.h"
#include "Event.h"
#include "../Scripting/ScriptManager.h"

template<typename... Arguments>
class LuaCallable : public Callable<Arguments...>
{
private:
	const int ArgCount = sizeof...(Arguments);

public:
	LuaCallable()
	{
		lua_State* L = W_SCRIPT.GetState();
		if (!lua_isfunction(L, -1))
		{
			luaL_error(L, "Top of stack must be a function");
		}

		FuncRef = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	virtual ~LuaCallable()
	{
		if (W_SCRIPT.IsActive())
			luaL_unref(W_SCRIPT.GetState(), LUA_REGISTRYINDEX, FuncRef);
	}

	virtual void Call(Arguments... Args) override
	{
		lua_State* L = W_SCRIPT.GetState();
		lua_rawgeti(L, LUA_REGISTRYINDEX, FuncRef);
		PushLuaValue(L, Args...);
		if (lua_pcall(L, ArgCount, 0, 0) != 0)
		{
			LOG_ERROR(GlobalLogger, "Unable to call lua function from event: " << lua_tostring(L, -1));
		}
	}
	
	virtual bool Equals(Callable<Arguments...>* Other) const override
	{
		auto OtherPtr = dynamic_cast<const LuaCallable<Arguments...>*>(Other);
		if (!OtherPtr)
			return false;

		lua_State* L = W_SCRIPT.GetState();
		lua_rawgeti(L, LUA_REGISTRYINDEX, FuncRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, OtherPtr->FuncRef);
		bool Result = lua_equal(L, -1, -2) != 0;
		lua_pop(L, 2);

		return Result;
	}

private:
	int FuncRef;
};

class ILuaEventWrapper
{
public:
	virtual ~ILuaEventWrapper() {}

	virtual void Bind(lua_State* L) = 0;
	virtual void Unbind(lua_State* L) = 0;
	virtual void Clear() = 0;
};

template<typename... Arguments>
class LuaEventWrapper : public ILuaEventWrapper
{
public:
	LuaEventWrapper(Event<Arguments...>& Event)
		: EnclosedEvent(Event)
	{
	}

	virtual ~LuaEventWrapper()
	{
		
	}

	virtual void Bind(lua_State* L) override
	{
		this->EnclosedEvent.Bind(new LuaCallable<Arguments...>());
	}

	virtual void Unbind(lua_State* L) override
	{
		this->EnclosedEvent.Unbind(new LuaCallable<Arguments...>());
	}

	virtual void Clear() override
	{
		this->EnclosedEvent.Clear();
	}

private:
	Event<Arguments...>& EnclosedEvent;
};

struct LEWContainer
{
	ILuaEventWrapper* Wrapper;
};

template<typename... T>
void PushLuaValue(lua_State* L, Event<T...>& E)
{
	auto Container = (LEWContainer*)lua_newuserdata(L, sizeof(LEWContainer));
	Container->Wrapper = new LuaEventWrapper<T...>(E);

	luaL_getmetatable(L, "Wake.event");
	lua_setmetatable(L, -2);
}

int luaopen_event(lua_State* L);