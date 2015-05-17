-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

print(Matrix2x3.new(1, 2, 3, 4, 5, 6) - 10)
print(Matrix2x3.new(1, 2, 3, 4, 5, 6) - Matrix2x3.new(1, 2, 3, 4, 5, 6))
