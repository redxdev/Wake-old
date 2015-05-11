-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Vector2.new(1, 2)
local b = Vector2.new{4, 3}

print(a)
print(a / 2)
print(2 / a)
print(a / b)
