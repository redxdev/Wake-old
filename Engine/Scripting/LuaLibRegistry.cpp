#include "LuaLibRegistry.h"

LuaLibAutomator::LuaLibAutomator(lua_CFunction Func)
{
	W_INT_LLREGISTRY.AddToRegistry(Func);
}

LuaLibRegistry& LuaLibRegistry::Get()
{
	static LuaLibRegistry Instance;
	return Instance;
}

void LuaLibRegistry::AddToRegistry(lua_CFunction Func)
{
	Functions.push_back(Func);
}

void LuaLibRegistry::RegisterAll(lua_State* L)
{
	for (lua_CFunction Func : Functions)
	{
		lua_pushcfunction(L, Func);
		lua_call(L, 0, 0);
	}
}
