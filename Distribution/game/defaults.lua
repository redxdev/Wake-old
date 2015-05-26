-- This file is the default initialization for the engine. Usually, you'll want
-- to delete this file and do custom initialization.

local dbg = require "debugger"

-- Stop the engine if the window is closed
window.closed:bind(engine.stop)

-- Stop the engine if the "Stop" binding is called
input.event("Stop"):bind(engine.stop)

-- Break into the debugger
input.event("Debug"):bind(function() dbg() end)

-- Initialize the window for the first time
window.initialize(config.window)
