-- Prints the client's information to the log on startup, useful when debugging
-- from logs.

local logger = Logger.new("ClientInfo")

logger:info(_VERSION)

if jit ~= nil then
	logger:info(jit.version)
	logger:info(jit.os .. " " .. jit.arch)
else
	logger:info("No LuaJIT")
end
