-- Input binding demo

local val = 0

local function printVal()
	print("The value is now " .. val)
end

local function increase(e)
	val = val + 1
	printVal()
end

-- You don't need to take the event argument if you don't use it
local function decrease()
	val = val - 1
	printVal()
end

local function stop()
	engine.stop()
end

input.event("Increase"):bind(increase)
input.event("Decrease"):bind(decrease)
input.event("Stop"):bind(stop)
