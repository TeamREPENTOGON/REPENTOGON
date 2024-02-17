ffidll = ffi.load("zhlREPENTOGON")

package.path = package.path .. ';resources/scripts/ffi/?.lua'

ffichecks = {}

local lffi = ffi

local debug_getinfo = debug.getinfo

ffi.cdef[[
	typedef void (*LuaCallback)(int, int, void**, int, int*, void**);
	void RegisterCallback(LuaCallback);
	LuaCallback L_RunCallback;
	void L_Free(void* ptr);
]]

ffichecks.gettype = function(var)
	local t = type(var)
	if t == "cdata" then t = tostring(lffi.typeof(var)) end
	return t
end

ffichecks.checktype = function(index, val, typ, level)
	local t = type(val)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level or 2).name, typ, t), (level or 2)+1)
	end
end

ffichecks.istype = function(var, typ)
	return type(var) == typ
end

ffichecks.isnil = function(var) return ffichecks.istype(var, "nil") end
ffichecks.isnullptr = function(cdata) return cdata == nil end
ffichecks.isnumber = function(var) return ffichecks.istype(var, "number") end
ffichecks.isstring = function(var) return ffichecks.istype(var, "string") end
ffichecks.isboolean = function(var) return ffichecks.istype(var, "boolean") end
ffichecks.iscdata = function(var, ctype) return var and lffi.istype(ctype, var) end

ffichecks.checknumber = function(index, val, level) ffichecks.checktype(index, val, "number", (level or 2)+1) end
ffichecks.checkfunction = function(index, val, level) ffichecks.checktype(index, val, "function", (level or 2)+1) end
ffichecks.checkstring = function(index, val, level) ffichecks.checktype(index, val, "string", (level or 2)+1) end
ffichecks.checkboolean = function(index, val, level) ffichecks.checktype(index, val, "boolean", (level or 2)+1) end

ffichecks.checkcdata = function(idx, var, ctype, allownil, level)
	if not (ffichecks.iscdata(var, ctype) or (allownil and ffichecks.isnil(var))) then
		local t = type(var)
		if t == "cdata" then t = tostring(lffi.typeof(var)) end

		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug_getinfo(level or 2).name, tostring(ctype), t), (level or 2)+1)
	end
end

ffichecks.callcdatafunc = function(this, cdata, ctype, cfunc)
	ffichecks.checkcdata(2, cdata, ctype)
	cfunc(this, cdata)
end

-- if a function returns a null pointer, it's still cdata as far as ffi is concerned.
-- however, it will interpret nil as a null pointer if compared to a cdata pointer,
-- so do that comparison and replace the result with an actual nil if it's null
ffichecks.fixreturn = function(cdata)
	if not ffichecks.isnullptr(cdata) then
		return cdata
	end
	return nil
end

ffichecks.optnumber = function(var, opt)
	if ffichecks.isnumber(var) then
		return var
	end
	return opt
end
ffichecks.optboolean = function(var, opt)
	if ffichecks.isboolean(var) then
		return var
	end
	return opt
end
ffichecks.optstring = function(var, opt)
	if ffichecks.isstring(var) then
		return var
	end
	return opt
end
ffichecks.optcdata = function(var, ctype, opt)
	if ffichecks.isscdata(var, cdata) then
		return var
	end
	return opt
end

local free = ffidll.L_Free

function GC(obj)
	return lffi.gc(obj, free)
end

function GCString(str)
	return lffi.string(GC(str))
end


pcall(require("Isaac"))

print = function(...)
	local str = ""

	for i, v in ipairs({ ... }) do
		str = str .. tostring(v) .. " "
	end

	Isaac.DebugString(str)
	Isaac.ConsoleOutput(str .. "\n")
end

pcall(require("CppContainer"))
pcall(require("Options"))
pcall(require("Console"))
pcall(require("KColor"))
pcall(require("Color"))
pcall(require("Vector"))
pcall(require("RoomEntry"))
pcall(require("RoomSpawn"))
pcall(require("RoomConfig"))
pcall(require("RoomDescriptor"))
pcall(require("EntityList"))
pcall(require("GridEntity"))
pcall(require("EntityRef"))
pcall(require("Sprite"))
pcall(require("SFXManager"))
pcall(require("RNG"))
pcall(require("Room"))
pcall(require("Level"))
pcall(require("Entity"))
pcall(require("EntityBomb"))
pcall(require("EntityEffect"))
pcall(require("EntityFamiliar"))
pcall(require("EntityKnife"))
pcall(require("ProjectileParams"))
pcall(require("EntityNPC"))
pcall(require("EntityPickup"))
-- Entity_Player has a stub in EntityPickup
pcall(require("MusicManager"))
pcall(require("WeightedOutcomePicker"))
pcall(require("NightmareScene"))
pcall(require("Game"))
pcall(require("Ambush"))
pcall(require("RoomTransition"))
pcall(require("StageTransition"))
pcall(require("ItemConfigCostume"))
pcall(require("ItemConfigItem"))
pcall(require("ItemConfigCard"))
pcall(require("ItemConfigPillEffect"))
pcall(require("ItemConfig"))

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

---------------------------------------------------------------------------

package = nil
ffi = nil
ffidll = nil
