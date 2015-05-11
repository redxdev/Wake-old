-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Matrix2x4.new{8,7,6,5,4,3,2,1}
local b = Matrix2x4.new(1,2,3,4,5,6,7,8)

print(a)
print(a:apply(function(v) return v + 10 end))
