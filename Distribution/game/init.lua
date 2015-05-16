-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

local a = Matrix3x4.new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)
