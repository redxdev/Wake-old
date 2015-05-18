-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

print(Matrix3x3.new(1, 2, 3, 4, 5, 6, 7, 8, 9) * 10)
print(Matrix3x3.new(1, 2, 3, 4, 5, 6, 7, 8, 9) * Vector3.new(1, 2, 3))
