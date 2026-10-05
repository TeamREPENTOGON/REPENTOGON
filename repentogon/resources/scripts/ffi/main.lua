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
ffichecks.checkstring = function(index, val, level)
	if type(val) == "number" then return tostring(val) end
	ffichecks.checktype(index, val, "string", (level or 2)+1)
	return val
end
ffichecks.checkboolean = function(index, val, level)
	if val == nil then ffichecks.checktype(index, val, "boolean", (level or 2)+1) end
	return not not val
end
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
    if ptr == nil or lffi.cast("void*", ptr) == nil then return nil end
    local o = ctype()
    lffi.copy(o, ptr, lffi.sizeof(ctype))
    return o
end

ffichecks.vectorsize = function(first, last, elemSize)
	if first == nil then return 0 end
	return tonumber(lffi.cast("const char*", last) - lffi.cast("const char*", first)) // elemSize
end
ffichecks.vectortotable = function(first, last, elemSize)
	local result = {}
	for i = 0, ffichecks.vectorsize(first, last, elemSize) - 1 do
		result[i + 1] = first[i]
	end
	return result
end

lffi.cdef [[
	struct StdString {
		private char Storage[0x18];
	} : 0x18;

	const char* L_StdString_CStr(const struct StdString*);
	void L_StdString_Assign(struct StdString*, const char*);
]]

local repentogon = ffidll
ffichecks.stdstring = function(str)
	return lffi.string(repentogon.L_StdString_CStr(str))
end

ffichecks.copyvector = function(vector)
	return Vector(vector.X, vector.Y)
end

local function loadmodule(name)
	local ok, err = pcall(require, "ffi." .. name)
	if not ok then
		Isaac.DebugString(string.format("[ERROR] Failed to load %s: %s\n", name, tostring(err)))
	end
end

local metatypes = {}
local ffi_metatype = lffi.metatype
lffi.metatype = function(ct, mt)
	metatypes[#metatypes + 1] = mt
	return ffi_metatype(ct, mt)
end

local entityBridges = {
	EntityAddress = __Lua_Entity_EntityAddress,
	PlayerAddress = __Lua_Entity_PlayerAddress,
	PushEntity = __Lua_Entity_PushEntity,
	PushPlayer = __Lua_Entity_PushPlayer,
	PushNPC = __Lua_Entity_PushNPC,
	PushEffect = __Lua_Entity_PushEffect,
	PushResults = __Lua_Entity_PushResults,
}
local voidptr = lffi.typeof("void*")
local uintptr = lffi.typeof("uintptr_t")

ffichecks.entitytopointer = function(entity)
	if entity == nil then
		return nil
	end
	return lffi.cast(voidptr, entityBridges.EntityAddress(entity))
end
ffichecks.playertopointer = function(player)
	if player == nil then
		return nil
	end
	return lffi.cast(voidptr, entityBridges.PlayerAddress(player))
end

local function pointertouserdata(push)
	return function(pointer)
		if pointer == nil then
			return nil
		end
		return push(tonumber(lffi.cast(uintptr, pointer)))
	end
end
ffichecks.pointertoentity = pointertouserdata(entityBridges.PushEntity)
ffichecks.pointertoplayer = pointertouserdata(entityBridges.PushPlayer)
ffichecks.pointertonpc = pointertouserdata(entityBridges.PushNPC)
ffichecks.pointertoeffect = pointertouserdata(entityBridges.PushEffect)
ffichecks.entityresults = entityBridges.PushResults

loadmodule("Vector")
loadmodule("VectorList")
loadmodule("GridEntityDesc")
loadmodule("KColor")
loadmodule("Color")
loadmodule("PosVel")
loadmodule("BitSet128")
loadmodule("RNG")
loadmodule("ItemConfigCostume")
loadmodule("ItemConfigItem")
loadmodule("TearParams")
loadmodule("ProjectileParams")
loadmodule("ActiveItemDesc")
loadmodule("QueueItemData")
loadmodule("TemporaryEffect")
loadmodule("TemporaryEffects")
loadmodule("AnimationFrame")
loadmodule("AnimationLayer")
loadmodule("AnimationData")
loadmodule("Shader")
loadmodule("DestinationQuad")
loadmodule("SourceQuad")
loadmodule("Image")
loadmodule("NullFrame")
loadmodule("BlendMode")
loadmodule("LayerState")
loadmodule("AnimationState")
loadmodule("Sprite")
loadmodule("EntityRef")	
loadmodule("GridEntity.GridEntity")
loadmodule("GridEntity.GridEntityDecoration")
loadmodule("GridEntity.GridEntityDoor")
loadmodule("GridEntity.GridEntityFire")
loadmodule("GridEntity.GridEntityGravity")
loadmodule("GridEntity.GridEntityLock")
loadmodule("GridEntity.GridEntityPit")
loadmodule("GridEntity.GridEntityPoop")
loadmodule("GridEntity.GridEntityPressurePlate")
loadmodule("GridEntity.GridEntityRock")
loadmodule("GridEntity.GridEntitySpikes")
loadmodule("GridEntity.GridEntityStairs")
loadmodule("GridEntity.GridEntityStatue")
loadmodule("GridEntity.GridEntityTeleporter")
loadmodule("GridEntity.GridEntityTNT")	
loadmodule("GridEntity.GridEntityTrapDoor")
loadmodule("GridEntity.GridEntityWall")
loadmodule("GridEntity.GridEntityWeb")
loadmodule("Camera")
loadmodule("RailManager")
loadmodule("ColorModifier")
loadmodule("Room.LRoomAreaDesc")
loadmodule("Room.LRoomTileDesc")
loadmodule("Backdrop")
loadmodule("FXParams")
loadmodule("FXLayers")
loadmodule("Room.RoomConfigEntry")
loadmodule("Room.RoomConfigSpawn")
loadmodule("Room.RoomConfigRoom")
loadmodule("Room.RoomConfigSet")
loadmodule("Room.RoomConfigStage")
loadmodule("Room.RoomConfig")
loadmodule("Room.RoomDescriptor")
loadmodule("Room.RoomTransition")
loadmodule("Room.Room")
loadmodule("Input")
loadmodule("SFXManager")
loadmodule("MusicManager")
loadmodule("Ambush")
loadmodule("Menus.MenuManager")
loadmodule("Menus.BestiaryMenu")
loadmodule("Menus.ChallengeMenu")
loadmodule("Menus.CharacterMenu")
loadmodule("Menus.CollectionMenu")
loadmodule("Menus.ControllerSelectMenu")
loadmodule("Menus.CustomChallengeMenu")
loadmodule("Menus.CutscenesMenu")
loadmodule("Menus.DailyChallengeMenu")
loadmodule("Menus.KeyConfigMenu")
loadmodule("Menus.MainMenu")
loadmodule("Menus.ModsMenu")
loadmodule("Menus.OptionsMenu")
loadmodule("Menus.PauseMenu")
loadmodule("Menus.SaveMenu")
loadmodule("Menus.SpecialSeedsMenu")
loadmodule("Menus.StatsMenu")
loadmodule("Menus.TitleMenu")
loadmodule("EntityDesc")
loadmodule("ChallengeParam")
loadmodule("HUD.Minimap")
loadmodule("HUD.ScoreSheet")
loadmodule("StageTransition")
loadmodule("NightmareScene")
loadmodule("ItemOverlay")
loadmodule("Console")
loadmodule("PersistentGameData")
loadmodule("DailyChallenge")
loadmodule("Capsule")
loadmodule("Shape")
loadmodule("DebugRenderer")
loadmodule("ColorParams")
loadmodule("LootListEntry")
loadmodule("LootList")
loadmodule("WeightedOutcomePicker")
loadmodule("MultiShotParams")
loadmodule("History")
loadmodule("PocketItem")
loadmodule("ProceduralItems.ProceduralEffect")
loadmodule("ProceduralItems.ProceduralItem")
loadmodule("ProceduralItems.ProceduralItemManager")
loadmodule("LevelGenerator.LevelGeneratorRoom")
loadmodule("LevelGenerator.LevelGeneratorEntry")
loadmodule("LevelGenerator.LevelGenerator")
loadmodule("BossPool")
loadmodule("CostumeSpriteDesc")
loadmodule("GenericPrompt")
loadmodule("BeamRenderer")
loadmodule("HUD.PlayerHUDHeart")
loadmodule("HUD.PlayerHUD")
loadmodule("HUD.HUDMessage")
loadmodule("HUD.HistoryHUD")
loadmodule("HUD.MinimapConfig")
loadmodule("HUD.HUD")
loadmodule("Weapon")
loadmodule("EntitySaveState.EntitySaveState")
loadmodule("EntitySaveState.EntitiesSaveStateVector")
loadmodule("EntitySaveState.GridEntitiesSaveStateVector")
loadmodule("EntityConfig.EntityConfigEntity")
loadmodule("EntityConfig.EntityConfigPlayer")
loadmodule("EntityConfig.EntityConfigBaby")
loadmodule("EntityConfig.EntityConfig")
loadmodule("ImGui")
loadmodule("Renderer.Transformer")
loadmodule("Renderer.SurfaceRenderController")
loadmodule("Renderer.Renderer")
loadmodule("Seeds")
loadmodule("Font.FontRenderSettings")
loadmodule("Font.Font")
loadmodule("ItemConfig.ItemConfigCard")
loadmodule("ItemConfig.ItemConfigPillEffect")
loadmodule("ItemConfig.ItemConfigList")
loadmodule("ItemConfig.CardConfigList")
loadmodule("ItemConfig.PillConfigList")
loadmodule("ItemConfig.ItemConfig")
loadmodule("PlayerManager")
loadmodule("ItemPool")
loadmodule("Level")
loadmodule("Game")
loadmodule("Isaac")

lffi.metatype = ffi_metatype

-- Thank you APIOverride, very cool!
local function IndexCall(t, _, k)
	return t[k]
end
for _, mt in ipairs(metatypes) do
	local index = rawget(mt, "__index")
	if type(index) == "table" then
		local meta = getmetatable(index)
		if meta == nil then
			setmetatable(index, { __call = IndexCall })
		elseif rawget(meta, "__call") == nil then
			meta.__call = IndexCall
		end
	end
end

local function InstallPropertyTable(mt, isSet)
	local key = isSet and "__propset" or "__propget"
	if rawget(mt, key) ~= nil then
		return
	end

	local handlerKey = isSet and "__newindex" or "__index"
	local original = rawget(mt, handlerKey)
	if type(original) ~= "function" then
		original = nil
	end

	local hooked = false
	local properties
	properties = setmetatable({}, {
		__index = function(_, name)
			if isSet then
				return function(self, value)
					if original then
						return original(self, name, value)
					end
					self[name] = value
				end
			end
			return function(self)
				if original then
					return original(self, name)
				end
				return self[name]
			end
		end,
		__newindex = function(t, name, accessor)
			rawset(t, name, accessor)
			if original and not hooked then
				hooked = true
				if isSet then
					rawset(mt, handlerKey, function(self, k, v)
						local accessor = rawget(properties, k)
						if accessor then
							return accessor(self, v)
						end
						return original(self, k, v)
					end)
				else
					rawset(mt, handlerKey, function(self, k)
						local accessor = rawget(properties, k)
						if accessor then
							return accessor(self)
						end
						return original(self, k)
					end)
				end
			end
		end,
	})
	rawset(mt, key, properties)
end

for _, mt in ipairs(metatypes) do
	InstallPropertyTable(mt, false)
	InstallPropertyTable(mt, true)
end

__Lua_Entity_EntityAddress = nil
__Lua_Entity_PlayerAddress = nil
__Lua_Entity_PushEntity = nil
__Lua_Entity_PushPlayer = nil
__Lua_Entity_PushNPC = nil
__Lua_Entity_PushEffect = nil
__Lua_Entity_PushResults = nil

ffi = nil
ffidll = nil
