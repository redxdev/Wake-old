-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Vector3.new(1, 2, 3)
local b = Vector3.new({1, 3, 5})
