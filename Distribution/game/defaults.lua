-- This file is the default initialization for the engine. Usually, you'll want
-- to delete this file and do custom initialization.

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
