--[[
	Copyright (c) 2015 Scott Lembcke and Howling Moon Software

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	TODO:
	* Print short function arguments as part of stack location.
	* Bug: sometimes doesn't advance to next line (same line event reported multiple times).
	* Do coroutines work as expected?
]]

-- Use ANSI color codes in the prompt by default.
-- You can disable it by making the strings empty.
local COLOR_RED = "\x1b[31m"
local COLOR_BLUE = "\x1b[34m"
local COLOR_RESET = "\x1b[0m"

if jit ~= nil and jit.os == "Windows" then
	COLOR_RED = ""
	COLOR_BLUE = ""
	COLOR_RESET = ""
end

local function pretty(obj, non_recursive)
	if type(obj) == "string" then
		-- Dump the string so that escape sequences are printed.
		return string.format("%q", obj)
	elseif type(obj) == "table" and not non_recursive then
		local str = "{"

		for k, v in pairs(obj) do
			local pair = pretty(k, true).." = "..pretty(v, true)
			str = str..(str == "{" and pair or ", "..pair)
		end

		return str.."}"
	else
		return tostring(obj)
	end
end

local help_message = [[
[return] - re-run last command
c(ontinue) - contiue execution
s(tep) - step forward by one line (into functions)
n(ext) - step forward by one line (skipping over functions)
p(rint) [expression] - execute the expression and print the result
f(inish) - step forward until exiting the current function
u(p) - move up the stack by one frame
d(own) - move down the stack by one frame
t(race) - print the stack trace
l(ocals) - print the function arguments, locals and upvalues.
h(elp) - print this message
]]

-- The stack level that cmd_* functions use to access locals or info
-- The structure of the code very carefully ensures this.
local LOCAL_STACK_LEVEL = 6

-- Extra stack frames to chop off.
-- Used for things like dbgcall() or the overridden assert/error functions
local stack_top = 0

-- The current stack frame index.
-- Changed using the up/down commands
local stack_offset = 0

-- Override if you don't want to use stdout.
local function dbg_write(str, ...)
	io.write(string.format(str, ...))
end

-- Override if you don't want to use stdin
local function dbg_read(prompt)
	dbg_write(prompt)
	return io.read()
end

local function dbg_writeln(str, ...)
	dbg_write((str or "").."\n", ...)
end

local function format_stack_frame_info(info)
	local fname = (info.name or string.format("<%s:%d>", info.short_src, info.linedefined))
	return string.format(COLOR_BLUE.."%s:%d"..COLOR_RESET.." in '%s'", info.short_src, info.currentline, fname)
end

local repl
local dbg

local function hook_factory(repl_threshold)
	return function(offset)
		return function(event, line)
			local info = debug.getinfo(2)

			if event == "call" and info.linedefined >= 0 then
				offset = offset + 1
			elseif event == "return" and info.linedefined >= 0 then
				if offset <= repl_threshold then
					-- TODO this is what causes the duplicated lines
					-- Don't remember why this is even here...
					--repl()
				else
					offset = offset - 1
				end
			elseif event == "line" and offset <= repl_threshold then
				repl()
			end
		end
	end
end

local hook_step = hook_factory(1)
local hook_next = hook_factory(0)
local hook_finish = hook_factory(-1)

local function table_merge(t1, t2)
	local tbl = {}
	for k, v in pairs(t1) do tbl[k] = v end
	for k, v in pairs(t2) do tbl[k] = v end

	return tbl
end

-- Create a table of all the locally accessible variables.
-- Globals are not included when running the locals command, but are when running the print command.
local function local_bindings(offset, include_globals)
	local level = stack_offset + offset + LOCAL_STACK_LEVEL
	local func = debug.getinfo(level).func
	local bindings = {}

	-- Retrieve the upvalues
	do local i = 1; repeat
		local name, value = debug.getupvalue(func, i)
		if name then bindings[name] = value end
		i = i + 1
	until name == nil end

	-- Retrieve the locals (overwriting any upvalues)
	do local i = 1; repeat
		local name, value = debug.getlocal(level, i)
		if name then bindings[name] = value end
		i = i + 1
	until name == nil end

	-- Retrieve the varargs. (works in Lua 5.2 and LuaJIT)
	local varargs = {}
	do local i = -1; repeat
		local name, value = debug.getlocal(level, i)
		table.insert(varargs, value)
		i = i - 1
	until name == nil end
	if #varargs ~= 0 then bindings["..."] = varargs end

	if include_globals then
		-- Merge the local bindings over the top of the environment table.
		-- In Lua 5.2, you have to get the environment table from the function's locals.
		local env = (_VERSION <= "Lua 5.1" and getfenv(func) or bindings._ENV)

		-- Finally, merge the tables and add a lookup for globals.
		return setmetatable(table_merge(env, bindings), {__index = _G})
	else
		return bindings
	end
end

-- Compile an expression with the given variable bindings.
local function compile_chunk(expr, env)
	if _VERSION <= "Lua 5.1" then
		local chunk = loadstring("return "..expr, "<debugger repl>")
		if chunk then setfenv(chunk, env) end
		return chunk
	else
		-- The Lua 5.2 way is a bit cleaner
		return load("return "..expr, "<debugger repl>", "t", env)
	end
end

local function cmd_print(expr)
	local env = local_bindings(1, true)
	local chunk = compile_chunk(expr, env)
	if chunk == nil then
		dbg_writeln(COLOR_RED.."Error: Could not evaluate expression."..COLOR_RESET)
		return false
	end

	-- Call the chunk and collect the results.
	local results = {pcall(chunk, unpack(env["..."] or {}))}

	-- The first result is the pcall error.
	if not results[1] then
		dbg_writeln(COLOR_RED.."Error:"..COLOR_RESET.." %s", results[2])
	elseif #results == 1 then
		dbg_writeln(COLOR_BLUE..expr..COLOR_RED.." => "..COLOR_BLUE.."<no result>"..COLOR_RESET)
	else
		local result = ""
		for i = 2, #results do
			result = result..(i ~= 2 and ", " or "")..pretty(results[i])
		end

		dbg_writeln(COLOR_BLUE..expr..COLOR_RED.." => "..COLOR_RESET..result)
	end

	return false
end

local function cmd_up()
	local info = debug.getinfo(stack_offset + LOCAL_STACK_LEVEL + 1)

	if info then
		stack_offset = stack_offset + 1
	else
		dbg_writeln(COLOR_BLUE.."Already at the top of the stack."..COLOR_RESET)
	end

	dbg_writeln("Inspecting frame: "..format_stack_frame_info(debug.getinfo(stack_offset + LOCAL_STACK_LEVEL)))
	return false
end

local function cmd_down()
	if stack_offset > stack_top then
		stack_offset = stack_offset - 1

		local info = debug.getinfo(stack_offset + LOCAL_STACK_LEVEL)
	else
		dbg_writeln(COLOR_BLUE.."Already at the bottom of the stack."..COLOR_RESET)
	end

	dbg_writeln("Inspecting frame: "..format_stack_frame_info(debug.getinfo(stack_offset + LOCAL_STACK_LEVEL)))
	return false
end

local function cmd_trace()
	local location = format_stack_frame_info(debug.getinfo(stack_offset + LOCAL_STACK_LEVEL))
	local offset = stack_offset - stack_top
	local message = string.format("Inspecting frame: %d - (%s)", offset, location)
	local str = debug.traceback(message, LOCAL_STACK_LEVEL)

	-- Iterate the lines of the stack trace so we can highlight the current one.
	local line_num = -2
	while str and #str ~= 0 do
		local line, rest = string.match(str, "([^\n]*)\n?(.*)")
		str = rest

		if line_num >= 0 then line = tostring(line_num)..line end
		dbg_writeln((line_num == stack_offset) and COLOR_BLUE..line..COLOR_RESET or line)
		line_num = line_num + 1
	end

	return false
end

local function cmd_locals()
	local bindings = local_bindings(1, false)

	-- Get all the variable binding names and sort them
	local keys = {}
	for k, v in pairs(bindings) do table.insert(keys, k) end
	table.sort(keys)

	for i, k in ipairs(keys) do
		local v = bindings[k]

		-- Skip the debugger object itself, temporaries and Lua 5.2's _ENV object.
		if not rawequal(v, dbg) and k ~= "_ENV" and k ~= "(*temporary)" then
			dbg_writeln("\t"..COLOR_BLUE.."%s "..COLOR_RED.."=>"..COLOR_RESET.." %s", k, pretty(v))
		end
	end

	return false
end

local last_cmd = false

local function match_command(line)
	local commands = {
		["c"] = function() return true end,
		["s"] = function() return true, hook_step end,
		["n"] = function() return true, hook_next end,
		["f"] = function() return true, hook_finish end,
		["p%s?(.*)"] = cmd_print,
		["u"] = cmd_up,
		["d"] = cmd_down,
		["t"] = cmd_trace,
		["l"] = cmd_locals,
		["h"] = function() dbg_writeln(help_message); return false end,
	}

	for cmd, cmd_func in pairs(commands) do
		local matches = {string.match(line, "^("..cmd..")$")}
		if matches[1] then
			return cmd_func, select(2, unpack(matches))
		end
	end
end

-- Run a command line
-- Returns true if the REPL should exit and the hook function factory
local function run_command(line)
	-- Continue without caching the command if you hit control-d.
	if line == nil then
		dbg_writeln()
		return true
	end

	-- Re-execute the last command if you press return.
	if line == "" then
		if last_cmd then line = last_cmd else return false end
	else
		last_cmd = line
	end

	command, command_arg = match_command(line)
	if command then
		return unpack({command(command_arg)})
	else
		dbg_writeln(COLOR_RED.."Error:"..COLOR_RESET.." command '%s' not recognized", line)
		return false
	end
end

repl = function()
	dbg_writeln(format_stack_frame_info(debug.getinfo(LOCAL_STACK_LEVEL - 3 + stack_top)))

	repeat
		local success, done, hook = pcall(run_command, dbg_read(COLOR_RED.."debugger.lua> "..COLOR_RESET))
		if success then
			debug.sethook(hook and hook(0), "crl")
		else
			local message = string.format(COLOR_RED.."INTERNAL DEBUGGER.LUA ERROR. ABORTING\n:"..COLOR_RESET.." %s", done)
			dbg_writeln(message)
			error(message)
		end
	until done
end

-- Make the debugger object callable like a function.
dbg = setmetatable({}, {
	__call = function(self, condition, offset)
		if condition then return end

		offset = (offset or 0)
		stack_offset = offset
		stack_top = offset

		debug.sethook(hook_next(1), "crl")
		return
	end,
})

-- Expose the debugger's printing functions.
dbg.write = dbg_write
dbg.writeln = dbg_writeln
dbg.pretty = pretty

-- Works like error(), but invokes the debugger.
function dbg.error(err, level)
	level = level or 1
	dbg_writeln(COLOR_RED.."Debugger stopped on error:"..COLOR_RESET.."(%s)", pretty(err))
	dbg(false, level)

	error(err, level)
end

-- Works like assert(), but invokes the debugger on a failure.
function dbg.assert(condition, message)
	if not condition then
		dbg_writeln(COLOR_RED.."Debugger stopped on "..COLOR_RESET.."assert(..., %s)", message)
		dbg(false, 1)
	end

	assert(condition, message)
end

-- Works like pcall(), but invokes the debugger on an error.
function dbg.call(f, l)
	return (xpcall(f, function(err)
		dbg_writeln(COLOR_RED.."Debugger stopped on error: "..COLOR_RESET..pretty(err))
		dbg(false, (l or 0) + 1)

		-- Prevent a tail call to dbg().
		return
	end))
end

local function luajit_load_readline_support()
	local ffi = require("ffi")

	ffi.cdef[[
		void free(void *ptr);

		char *readline(const char *);
		int add_history(const char *);
	]]

	local readline = ffi.load("readline")

	dbg_read = function(prompt)
		local cstr = readline.readline(prompt)

		if cstr ~= nil then
			local str = ffi.string(cstr)

			if string.match(str, "[^%s]+") then
				readline.add_history(cstr)
			end

			ffi.C.free(cstr)
			return str
		else
			return nil
		end
	end

	dbg_writeln(COLOR_RED.."debugger.lua: Readline support enabled.")
end

-- Detect Lua version.
if jit then
	-- LuaJIT
	dbg_writeln(COLOR_RED.."debugger.lua: Loaded for "..jit.version..COLOR_RESET)
	pcall(luajit_load_readline_support)
elseif _VERSION == "Lua 5.2" or _VERSION == "Lua 5.1" then
	dbg_writeln(COLOR_RED.."debugger.lua: Loaded for ".._VERSION..COLOR_RESET)
else
	dbg_writeln("debugger.lua: Not tested against ".._VERSION)
	dbg_writeln("Please send me feedback!")
end

return dbg
