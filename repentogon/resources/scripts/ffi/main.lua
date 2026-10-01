-- Aw yeah, this is happening!
ffidll = ffi.load("zhlREPENTOGON")
ffichecks = {}
local lffi = ffi

local debug_getinfo = debug.getinfo

local ctypeCache = {}
local function resolveCtype(ctype)
	if type(ctype) ~= "string" then return ctype end
	local cached = ctypeCache[ctype]
	if cached then return cached end
	local resolved
	local ok, r = pcall(lffi.typeof, ctype)
	if ok then
		resolved = r
	else
		for _, pre in ipairs({ "struct ", "union ", "enum " }) do
			local ok2, r2 = pcall(lffi.typeof, pre .. ctype)
			if ok2 then resolved = r2; break end
		end
	end
	ctypeCache[ctype] = resolved
	return resolved
end

ffichecks.gettype = function(var)
	local t = type(var)
	if t == "cdata" or t == "userdata" then
		local ok, ct = pcall(lffi.typeof, var)
		if ok and ct then t = tostring(ct) end
	end
	return t
end

ffichecks.checktype = function(index, val, typ, level)
	local t = type(val)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level or 2).name, typ, t), (level or 2)+1)
	end
end

ffichecks.argerror = function(index, err, level)
	error(string.format("bad argument #%d to '%s' (%s)", index, debug_getinfo(level or 2).name, err), (level or 2) +1)
end

ffichecks.istype = function(var, typ)
	return type(var) == typ
end

ffichecks.isnil = function(var) return ffichecks.istype(var, "nil") end
ffichecks.isnullptr = function(cdata) return cdata == nil end
ffichecks.isnumber = function(var) return ffichecks.istype(var, "number") end
ffichecks.isstring = function(var) return ffichecks.istype(var, "string") end
ffichecks.isboolean = function(var) return ffichecks.istype(var, "boolean") end
ffichecks.istable = function(var) return ffichecks.istype(var, "table") end
ffichecks.iscdata = function(var, ctype)
	if not var then return false end
	local ct = resolveCtype(ctype)
	if ct and lffi.istype(ct, var) then
		return true
	end
	-- Also accept a pointer to the target type (reference cdata).
	if type(var) == "cdata" then
		local ptr = resolveCtype(ctype .. "*")
		if ptr and lffi.istype(ptr, var) then
			return true
		end
	end
	return false
end

ffichecks.checknumber = function(index, val, level) ffichecks.checktype(index, val, "number", (level or 2)+1) end
ffichecks.checkfunction = function(index, val, level) ffichecks.checktype(index, val, "function", (level or 2)+1) end
ffichecks.checkstring = function(index, val, level) ffichecks.checktype(index, val, "string", (level or 2)+1) end
ffichecks.checkboolean = function(index, val, level) ffichecks.checktype(index, val, "boolean", (level or 2)+1) end
ffichecks.checktable = function(index, val, level) ffichecks.checktype(index, val, "table", (level or 2)+1) end
ffichecks.checkinteger = function(index, val, level) 
	if math.type(val) ~= "integer" then
		error(string.format("bad argument #%d to '%s' (integer expected, got %s)", index, debug_getinfo(level or 2).name, type(val)), (level or 2)+1)
	end
end

ffichecks.checkcdata = function(idx, var, ctype, allownil, level)
	if not (ffichecks.iscdata(var, ctype) or (allownil and ffichecks.isnil(var))) then
		local t = ffichecks.gettype(var)

		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug_getinfo(level or 2).name, tostring(ctype), t), (level or 2)+1)
	end
end

ffichecks.callcdatafunc = function(this, cdata, ctype, cfunc)
	ffichecks.checkcdata(2, cdata, ctype)
	cfunc(this, cdata)
end

ffichecks.optnumber = function(var, opt)
	if ffichecks.isnumber(var) then
		return var
	end
	return opt
end
ffichecks.optboolean = function(var, opt)
	if var == nil then
		return opt
	end
	-- Cheap enough coercion for me
	return not not var
end
ffichecks.optstring = function(var, opt)
	if ffichecks.isstring(var) then
		return var
	end
	return opt
end
ffichecks.optcdata = function(var, cdata, opt)
	if ffichecks.iscdata(var, cdata) then
		return var
	end
	return opt
end
ffichecks.wrap = function(ptr, ctype)
    if ptr == nil or ffi.cast("void*", ptr) == nil then return nil end
    local o = ctype()
    ffi.copy(o, ptr, ffi.sizeof(ctype))
    return o
end

local function loadmodule(name)
	local ok, err = pcall(require, name)
	if not ok then
		Isaac.DebugString(string.format("[ERROR] Failed to load %s: %s\n", name, tostring(err)))
	end
end

loadmodule("ffi.Vector")
loadmodule("ffi.VectorList")
loadmodule("ffi.GridEntityDesc")
loadmodule("ffi.KColor")
loadmodule("ffi.Color")
loadmodule("ffi.PosVel")
loadmodule("ffi.BitSet128")
loadmodule("ffi.RNG")
loadmodule("ffi.ItemConfigCostume")
loadmodule("ffi.ItemConfigItem")
loadmodule("ffi.TearParams")
loadmodule("ffi.ProjectileParams")
loadmodule("ffi.ActiveItemDesc")
loadmodule("ffi.QueueItemData")
loadmodule("ffi.TemporaryEffect")
loadmodule("ffi.TemporaryEffects")
loadmodule("ffi.AnimationFrame")
loadmodule("ffi.AnimationLayer")
loadmodule("ffi.AnimationData")
loadmodule("ffi.Shader")
loadmodule("ffi.DestinationQuad")
loadmodule("ffi.SourceQuad")
loadmodule("ffi.Image")
loadmodule("ffi.NullFrame")
loadmodule("ffi.BlendMode")
loadmodule("ffi.LayerState")
loadmodule("ffi.AnimationState")
loadmodule("ffi.Sprite")
loadmodule("ffi.EntityRef")	
loadmodule("ffi.GridEntity.GridEntity")
loadmodule("ffi.GridEntity.GridEntityDecoration")
loadmodule("ffi.GridEntity.GridEntityDoor")
loadmodule("ffi.GridEntity.GridEntityFire")
loadmodule("ffi.GridEntity.GridEntityGravity")
loadmodule("ffi.GridEntity.GridEntityLock")
loadmodule("ffi.GridEntity.GridEntityPit")
loadmodule("ffi.GridEntity.GridEntityPoop")
loadmodule("ffi.GridEntity.GridEntityPressurePlate")
loadmodule("ffi.GridEntity.GridEntityRock")
loadmodule("ffi.GridEntity.GridEntitySpikes")
loadmodule("ffi.GridEntity.GridEntityStairs")
loadmodule("ffi.GridEntity.GridEntityStatue")
loadmodule("ffi.GridEntity.GridEntityTeleporter")
loadmodule("ffi.GridEntity.GridEntityTNT")	
loadmodule("ffi.GridEntity.GridEntityTrapDoor")
loadmodule("ffi.GridEntity.GridEntityWall")
loadmodule("ffi.GridEntity.GridEntityWeb")
loadmodule("ffi.Camera")
loadmodule("ffi.RailManager")
loadmodule("ffi.ColorModifier")
loadmodule("ffi.Room.LRoomAreaDesc")
loadmodule("ffi.Room.LRoomTileDesc")
loadmodule("ffi.Backdrop")
loadmodule("ffi.FXParams")
loadmodule("ffi.FXLayers")
loadmodule("ffi.Room.RoomConfigEntry")
loadmodule("ffi.Room.RoomConfigSpawn")
loadmodule("ffi.Room.RoomConfigRoom")
loadmodule("ffi.Room.RoomDescriptor")
loadmodule("ffi.Room.Room")
loadmodule("ffi.Input")
loadmodule("ffi.SFXManager")
loadmodule("ffi.MusicManager")
loadmodule("ffi.Ambush")
loadmodule("ffi.Menus.MenuManager")
loadmodule("ffi.Menus.BestiaryMenu")
loadmodule("ffi.Menus.ChallengeMenu")
loadmodule("ffi.Menus.CharacterMenu")
loadmodule("ffi.Menus.CollectionMenu")
loadmodule("ffi.Menus.ControllerSelectMenu")
loadmodule("ffi.Menus.CustomChallengeMenu")
loadmodule("ffi.Menus.CutscenesMenu")
loadmodule("ffi.Menus.DailyChallengeMenu")
loadmodule("ffi.Menus.KeyConfigMenu")
loadmodule("ffi.Menus.MainMenu")
loadmodule("ffi.Menus.ModsMenu")
loadmodule("ffi.Menus.OptionsMenu")
loadmodule("ffi.Menus.PauseMenu")
loadmodule("ffi.Menus.SaveMenu")
loadmodule("ffi.Menus.SpecialSeedsMenu")
loadmodule("ffi.Menus.StatsMenu")
loadmodule("ffi.Menus.TitleMenu")

ffi = nil
ffidll = nil