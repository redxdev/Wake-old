-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

local a = Matrix2x4.new(1, 2, 3, 4, 5, 6, 7, 8)
local b = Matrix2x4.new(8, 7, 6, 5, 4, 3, 2, 1)
local c = Matrix2x4.new(a)
local d = Matrix2x4.new()

print(a:rows())
print(a:columns())
print(#a)
print(Matrix2x4.rows())
print(Matrix2x4.columns())
