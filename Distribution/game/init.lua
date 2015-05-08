-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Matrix2x3.new(1, 2, 3, 4, 5, 6)
local b = Matrix2x3.new{1, 2, 3, 4, 5, 6}
