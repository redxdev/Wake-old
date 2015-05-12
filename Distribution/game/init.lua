-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Matrix2x2.new(1, 2, 3, 4)
local b = Matrix2x2.new{5, 6, 7, 8}
local c = Matrix2x2.new(a)
local d = Matrix2x2.new()

for _,v in ipairs(a:table()) do
    print(v)
end
