-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Vector4.new(1, 2, 3, 4)
local b = Vector4.new{5, 6, 7, 8}
print(a / 10)
print(10 / a)
print(a / b)
