#pragma once

#include "../Utility/luaext.h"

#define W_MT_LOGGER ("Wake.Logger")

class Logger;

Logger* luaW_checklogger(lua_State* L);

int luaopen_logger(lua_State* L);