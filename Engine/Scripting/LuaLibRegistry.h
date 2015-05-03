#pragma once

#include "../Utility/luaext.h"

#include <list>
#include <tuple>

#define W_INT_LLREGISTRY (LuaLibRegistry::Get())

#define W_REGISTER_LUA_LIB_PRIORITY(OpenFunc, Priority) static LuaLibAutomator __w_llautomator_##OpenFunc(OpenFunc, Priority)
#define W_REGISTER_LUA_LIB(OpenFunc) W_REGISTER_LUA_LIB_PRIORITY(OpenFunc, 0)

class LuaLibAutomator
{
public:
	LuaLibAutomator(lua_CFunction Func, int Priority);
};

class LuaLibRegistry
{
public:
	static LuaLibRegistry& Get();

public:
	void AddToRegistry(lua_CFunction Func, int Priority = 0);

	void RegisterAll(lua_State* L);

private:
	std::list<std::tuple<int, lua_CFunction>> Libraries;
};