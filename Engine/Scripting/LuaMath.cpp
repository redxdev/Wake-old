#include "LuaMath.h"
#include "LuaLibRegistry.h"
#include "LuaMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

static int l_lookAt(lua_State* L)
{
	auto& Eye = *luaW_checkvector3(L, 1);
	auto& Center = *luaW_checkvector3(L, 2);
	if (lua_gettop(L) >= 3)
	{
		auto& Up = *luaW_checkvector3(L, 3);
		PushLuaValue(L, glm::lookAt(Eye, Center, Up));
	}
	else
	{
		auto Up = glm::vec3(0, 1, 0);
		PushLuaValue(L, glm::lookAt(Eye, Center, Up));
	}

	return 1;
}

static int l_perspective(lua_State* L)
{
	float FieldOfView = (float)luaL_checknumber(L, 1);
	float AspectRatio = (float)luaL_checknumber(L, 2);
	float NearPlane = (float)luaL_checknumber(L, 3);
	float FarPlane = (float)luaL_checknumber(L, 4);
	PushLuaValue(L, glm::perspective(FieldOfView, AspectRatio, NearPlane, FarPlane));
	return 1;
}

static const struct luaL_reg mathlib_f[] = {
	{ "lookAt", l_lookAt },
	{ "perspective", l_perspective },
	{NULL, NULL}
};

int luaopen_math_ext(lua_State* L)
{
	lua_getglobal(L, "math");
	luaL_register(L, NULL, mathlib_f);

	return 1;
}

W_REGISTER_LUA_LIB(luaopen_math_ext);