-- Information library, primarily used to print out client info on startup

log.info(_VERSION)

if jit ~= nil then
	log.info(jit.version)
	log.info(jit.os .. " " .. jit.arch)
end