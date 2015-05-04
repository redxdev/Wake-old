-- Input bindings

local type = input.type
local mode = input.mode
local key = input.key
local mouse = input.mouse

input.bind("Increase", type.Keyboard, mode.Released, key.Up)
input.bind("Decrease", type.Keyboard, mode.Released, key.Down)
input.bind("Stop", type.Keyboard, mode.Released, key.Escape)
