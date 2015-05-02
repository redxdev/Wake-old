#pragma once

#include "../Utility/luaext.h"

#include <list>

#define W_INT_LLREGISTRY (LuaLibRegistry::Get())

#define W_REGISTER_LUA_LIB(OpenFunc) static LuaLibAutomator __w_llautomator(OpenFunc)

class LuaLibAutomator
{
public:
	LuaLibAutomator(lua_CFunction Func);
};

class LuaLibRegistry
{
public:
	static LuaLibRegistry& Get();

public:
	void AddToRegistry(lua_CFunction Func);

	void RegisterAll(lua_State* L);

private:
	std::list<lua_CFunction> Functions;
};