#pragma once

#include "../Utility/luaext.h"

class Logger;

Logger* luaW_checklogger(lua_State* L);

int luaopen_log(lua_State* L);