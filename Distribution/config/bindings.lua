-- Input bindings

-- For input.bindAll
require "lib/inputhelper"

local type = input.type
local mode = input.mode
local key = input.key
local mouse = input.mouse

local bindings = {
	{"Stop", type.Keyboard, mode.Released, key.Escape},
	{"Debug", type.Keyboard, mode.Released, key.BackTick}
}

input.bindAll(bindings)
