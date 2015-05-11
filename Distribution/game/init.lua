-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Matrix2x2.new(1, 2, 3, 4)

print(a)
a:setAll(10, 20, 30, 40)
print(a)
