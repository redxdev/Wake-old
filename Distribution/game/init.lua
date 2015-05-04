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
	
	if key == 37 then -- internal code for escape
		engine.stop()
	end
end

window.keyPressed:bind(keyPressed)
window.keyReleased:bind(keyReleased)