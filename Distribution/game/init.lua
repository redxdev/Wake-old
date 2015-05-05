-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Vector2.new(10, 20)
local b = Vector2.new(5, 6)
local c = Vector2.new(10, 20)
print(a:length())
print(Vector2.length(a))
print(tostring(a * b))
print(tostring(a == c))
