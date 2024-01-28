ffidll = ffi.load("zhlREPENTOGON")

package.path = package.path .. ';resources/scripts/ffi/?.lua'

ffichecks = {}

ffichecks.checktype = function(idx, var, typ)
	local t = type(var)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug.getinfo(3).name, typ, t))
	end
end

ffichecks.checknumber = function(idx, var) ffichecks.checktype(idx, var, "number") end
ffichecks.checkstring = function(idx, var) ffichecks.checktype(idx, var, "string") end

pcall(require("Isaac"))
pcall(require("Console"))

print = function(str) 
	str = tostring(str)
	Isaac.DebugString(str)
	Isaac.ConsoleOutput(str)
end

package = nil
ffi = nil
ffidll = nil