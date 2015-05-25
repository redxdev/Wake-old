-- Game initialization
-- Add any code for game initialization here

-- Stop the engine if the window is closed
local function gameWindowClosed()
  engine.stop()
end
window.closed:bind(gameWindowClosed)

-- Stop the engine if the "Stop" binding is called
local function stopBinding()
    engine.stop()
end
input.event("Stop"):bind(stopBinding)

-- Initialize the window for the first time
window.initialize(config.window)
