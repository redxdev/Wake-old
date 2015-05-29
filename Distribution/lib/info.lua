-- Prints the client's information to the log on startup, useful when debugging
-- from logs.

local log = Logger.new("ClientInfo")

log:info(_VERSION)

if jit ~= nil then
	log:info(jit.version)
	log:info(jit.os .. " " .. jit.arch)
else
	log:info("No LuaJIT")
end
