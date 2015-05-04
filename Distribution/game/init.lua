-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local vec = Vector2.new(10, 20)
print(tostring(vec))
print(vec:get(1))
print(vec:get(2))
print(#vec)
vec:set(1, 2)
print(vec:get(1))
vec:setAll(3.5, 88)
print(vec:get(1))
print(vec:get(2))
