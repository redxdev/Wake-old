#include "LuaLibRegistry.h"

LuaLibAutomator::LuaLibAutomator(lua_CFunction Func, int Priority)
{
	W_INT_LLREGISTRY.AddToRegistry(Func, Priority);
}

LuaLibRegistry& LuaLibRegistry::Get()
{
	static LuaLibRegistry Instance;
	return Instance;
}

void LuaLibRegistry::AddToRegistry(lua_CFunction Func, int Priority)
{
	Libraries.push_back(std::make_tuple(Priority, Func));
}

void LuaLibRegistry::RegisterAll(lua_State* L)
{
	Libraries.sort([](std::tuple<int, lua_CFunction> A, std::tuple<int, lua_CFunction> B){return std::get<0>(A) > std::get<0>(B);});

	for (auto Entry : Libraries)
	{
		lua_pushcfunction(L, std::get<1>(Entry));
		lua_call(L, 0, 0);
	}
}
