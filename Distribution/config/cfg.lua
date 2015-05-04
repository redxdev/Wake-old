------------------------
-- Wake Configuration --
------------------------

-- Config *should* be the only global variable defined
config = {}

-- It is recommended that you don't put any configuration here. Instead, require all the config files
-- you need from this file.

-- A few standards:
-- Graphics settings go in window.lua
-- Key bindings go in bindings.lua

require "config/window"
require "config/bindings"
