-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

local a = Matrix3x2.new(1, 2, 3, 4, 5, 6)
local b = Matrix3x2.new(6, 5, 4, 3, 2, 1)
local c = Matrix3x2.new(a)
local d = Matrix3x2.new()

print(a)
print(b)
print(c)
print(d)

print(a == b)
print(a == c)
