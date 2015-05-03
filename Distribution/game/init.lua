-- Game initialization
-- Add any code for game initialization here

log.info "Game init"

local i = 0

function tick()
	print(i)
	i = i + 1
	
	if i > 100 then
		engine.tick:unbind(tick)
	end
end

engine.tick:bind(tick)

function lostFocus()
	print "Bye!"
end

function gainedFocus()
	print "Hi!"
end

window.lostFocus:bind(lostFocus)
window.gainedFocus:bind(gainedFocus)