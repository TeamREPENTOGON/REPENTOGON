ffidll = ffi.load("zhlREPENTOGON")

package.path = package.path .. ';resources/scripts/ffi/?.lua'

ffichecks = {}

local lffi = ffi

ffichecks.checktype = function(idx, var, typ)
	local t = type(var)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug.getinfo(2).name, typ, t))
	end
end

ffichecks.istype = function(idx, var, typ)
	return type(var) == typ
end

ffichecks.checknumber = function(idx, var) ffichecks.checktype(idx, var, "number") end
ffichecks.checkstring = function(idx, var) ffichecks.checktype(idx, var, "string") end
ffichecks.checkboolean = function(idx, var) ffichecks.checktype(idx, var, "boolean") end
ffichecks.checkcdata = function(idx, var, ctype)
	if not var or not lffi.istype(ctype, var) then
		local t = type(var)
		if t == "cdata" then t = tostring(lffi.typeof(var)) end

		error(string.format("bad argument #%d to '%s' (%s expected, got %s)"), idx, debug.getinfo(2).name, tostring(ctype), t)
	end
end

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
pcall(require("RNG"))
pcall(require("MusicManager"))


package = nil
ffi = nil
ffidll = nil

---------------------------------------------------------------------------

pcall(require("callbacks"))

function RegisterMod(modname, apiversion)
	local mod = {
		Name = modname,
		AddCallback = function(self, callbackId, fn, param)
			Isaac.AddCallback(self, callbackId, fn, param)
		end,
		AddPriorityCallback = function(self, callbackId, priority, fn, param)
			Isaac.AddPriorityCallback(self, callbackId, priority, fn, param)
		end,
		-- TODO
		RemoveCallback = function(self, callbackId, fn)
			Isaac.RemoveCallback(self, callbackId, fn)
		end,
		-- TODO
		SaveData = function(self, data)
			Isaac.SaveModData(self, data)
		end,
		-- TODO
		LoadData = function(self)
			return Isaac.LoadModData(self)
		end,
		-- TODO
		HasData = function(self)
			return Isaac.HasModData(self)
		end,
		-- TODO
		RemoveData = function(self)
			Isaac.RemoveModData(self)
		end
	}
	-- In the old API this also called Isaac.RegisterMod.
	-- I don't think that needs to exist anymore.
	--Isaac.RegisterMod(mod, modname, apiversion)
	return mod
end