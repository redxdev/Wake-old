-- Input helper functions
local logger = Logger.new("InputHelper")

-- Sets up bindings based on a table. The table should be an array with
-- each entry being another table with arguments to pass to input.bind.
function input.bindAll(t)
	logger:debug("Creating " .. #t .. " bindings")
	for _,v in ipairs(t) do
		input.bind(v[1], v[2], v[3], v[4])
	end
end
