-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"
require "countdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)
