ffidll = ffi.load("zhlREPENTOGON")

package.path = package.path .. ';resources/scripts/ffi/?.lua'

ffichecks = {}

ffichecks.checktype = function(idx, var, typ)
	local t = type(var)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug.getinfo(3).name, typ, t))
	end
end

ffichecks.istype = function(idx, var, typ)
	return type(var) == typ
end

ffichecks.checknumber = function(idx, var) ffichecks.checktype(idx, var, "number") end
ffichecks.checkstring = function(idx, var) ffichecks.checktype(idx, var, "string") end
ffichecks.checkboolean = function(idx, var) ffichecks.checktype(idx, var, "boolean") end

ffichecks.isnumber = function(idx, var) return ffichecks.istype(idx, var, "number") end
ffichecks.isstring = function(idx, var) return ffichecks.istype(idx, var, "string") end
ffichecks.isboolean = function(idx, var) return ffichecks.istype(idx, var, "boolean") end

pcall(require("Isaac"))

print = function(str) 
	str = tostring(str)
	Isaac.DebugString(str)
	Isaac.ConsoleOutput(str .. "\n")
end

pcall(require("Options"))
pcall(require("Console"))
pcall(require("Game"))
pcall(require("Color"))
pcall(require("Vector"))
pcall(require("SFXManager"))


package = nil
ffi = nil
ffidll = nil