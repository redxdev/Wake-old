-- Input bindings

local type = input.type
local mode = input.mode
local key = input.key
local mouse = input.mouse

input.bind("Stop", type.Keyboard, mode.Released, key.Escape)
input.bind("Debug", type.Keyboard, mode.Released, key.BackTick)
