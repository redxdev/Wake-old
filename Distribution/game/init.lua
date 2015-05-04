-- Game initialization
-- Add any code for game initialization here

log.info "Game init"

local i = 0

function tick()
	print "Tick!"
	i = i + 1

	if i > 100 then
		print "No more ticks!"
		engine.tick:unbind(tick)
	end
end

engine.tick:bind(tick)

function lostFocus()
	print "Lost focus!"
end

function gainedFocus()
	print "Gained focus!"
end

window.lostFocus:bind(lostFocus)
window.gainedFocus:bind(gainedFocus)

function keyPressed(key)
	print("Key pressed:" .. key)
end

function keyReleased(key)
	print("Key released:" .. key)
end

window.keyPressed:bind(keyPressed)
window.keyReleased:bind(keyReleased)

function mouseButtonPressed(button)
	print("Mouse button pressed:" .. button)
end

function mouseButtonReleased(button)
	print("Mouse button released:" .. button)
end

function mouseWheelMoved(amount)
	print("Mouse wheel moved: " .. amount)
end

window.mouseButtonPressed:bind(mouseButtonPressed)
window.mouseButtonReleased:bind(mouseButtonReleased)
window.mouseWheelMoved:bind(mouseWheelMoved)

function mouseMoved(x, y)
	print("Mouse " .. x .. "," .. y)
end

function mouseEntered()
	print "Mouse entered!"
end

function mouseLeft()
	print "Mouse left!"
end

--- window.mouseMoved:bind(mouseMoved)
window.mouseEntered:bind(mouseEntered)
window.mouseLeft:bind(mouseLeft)

function stopEngine(e)
	engine.stop()
end

input.bind("Quit", input.type.Keyboard, input.mode.Released, input.key.Escape)
input.bind("Quit", input.type.Keyboard, input.mode.Released, input.key.Return)
input.event("Quit"):bind(stopEngine)
