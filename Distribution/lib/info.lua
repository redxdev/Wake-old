-- Information library, used to print out client info on startup
local logger = log.new("ClientInfo")

logger:info(_VERSION)

if jit ~= nil then
	logger:info(jit.version)
	logger:info(jit.os .. " " .. jit.arch)
end
