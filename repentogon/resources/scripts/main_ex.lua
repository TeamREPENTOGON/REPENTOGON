REPENTOGON = {
	["Real"] = true,
	["Name"] = "REPENTOGON", --I avoid having to do RegisterMod this way, sorry ;P
	["Version"] = "dev build",
	["Extras"] = { ["Misc"]={}, }, -- Tables containing additional REPENTOGON data structures, example: ChangeLog or StatsMenu
}

collectgarbage("generational")



local function MeetsVersion(targetVersion)
	if (REPENTOGON.Version == "dev build") then return true end
    local version = {}
    local target = {}
    
    for num in REPENTOGON.Version:gmatch("%d+") do
        table.insert(version, tonumber(num))
    end
    
    for num in targetVersion:gmatch("%d+") do
        table.insert(target, tonumber(num))
    end
    
    -- Compare each component of the version strings
    for i = 1, math.max(#version, #target) do
        local v = version[i] or 0
        local t = target[i] or 0
        
        if v < t then
            return false
        elseif v > t then
            return true
        end
    end
    
    return true
end

REPENTOGON.MeetsVersion = MeetsVersion;

local debug_getinfo = debug.getinfo

local callbackIDToName = {}
for k, v in pairs(ModCallbacks) do
	callbackIDToName[v] = k
end

local oldrequire = require

function require(modname)
	local ret = oldrequire(modname)
	return ret
end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- COLOR STUFF
----------------------------------------------------------------------------------------------------


rawset(getmetatable(Color), "EmberFade", Color(1, 1, 1, 1, 1, 0.514, 0.004))
Color.EmberFade:SetTint(0, 0, 0, 1.1)

local function quickRegisterColorPreset(name, R, G, B, A, RO, GO, BO, RC, GC, BC, CA)
	rawset(getmetatable(Color), name, Color(R, G, B, A or 1, RO or 0, GO or 0, BO or 0))
	Color[name]:SetColorize(RC or 0, GC or 0, BC or 0, CA or 0)
end

rawset(getmetatable(Vector), "__type", "Vector")

local colorPresets = {
	TearIpecac = {0.5, 0.9, 0.4},
	TearHoming = {0.4, 0.15, 0.38, 1, 71/255, 0, 116/255},
	TearTar = {0.95, 0.8, 0.6, 1, -150/255, -150/255, -150/255},
	TearSoy = {1.5, 2, 2},
	TearChocolate = {0.33, 0.18, 0.18, 1, 66/255, 40/255, 40/255},
	TearAlmond = {1.8, 1.7, 1},
	TearScorpio = {50/255, 1, 50/255},
	TearSerpentsKiss = {0.5, 0.97, 0.5},
	TearCommonCold = {0.4, 0.97, 0.5},
	TearCoal = {0.2, 0.09, 0.065},
	TearNumberOne = {1, 1, 0, 1, 45/255, 15/255, 0},

	ProjectileIpecac = {1, 1, 1, 1, 0, 0, 0, 0.4, 2, 0.5, 1},
	ProjectileHoming = {1, 1, 1, 1, 0.26, 0.05, 0.4, 0.8, 0.15, 1, 1},
	ProjectileTar	= {1, 1, 1, 1, 0, 0, 0, 0.5, 0.5, 0.5, 1},
	ProjectileSoy = {1, 1, 1, 1, 0.8, 0.7, 0.5, 1, 1, 1, 1},
	ProjectileFireWave = {1, 1, 1, 1, 1, 0.3, 0},

	ProjectileCageBlue = {1, 1, 1, 1, 0, 0, 0, 0.8, 1, 0.85, 1},
	ProjectileMegaSatanBlack = {1, 1, 1, 1, 0, 0, 0, 0.6, 0.6, 0.6, 1},
	ProjectileMegaSatanWhite = {1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 1},
	ProjectileHushBlue = {1, 1, 1, 1, 0, 0.2, 0.4},
	ProjectileHushYellow = {1, 1, 1, 1, 0.4, 0.2, 0},
	ProjectileHushGreen = {1, 1, 1, 1, 0.2, 0.2, 0},

	LaserIpecac = {1, 1, 1, 1, 0, 0, 0, 1.8, 3, 1, 1},
	LaserHoming = {1, 1, 1, 1, 0, 0, 0, 3, 1, 3.5, 1},
	LaserSoy = {1, 1, 1, 1, 0, 0, 0, 5.6, 5, 4.2, 1},
	LaserChocolate = {1, 1, 1, 1, 0, 0, 0, 3, 1.7, 1.7, 1},
	LaserAlmond = {1, 1, 1, 1, 0, 0, 0, 5.6, 5.2, 3.8, 1},
	LaserPoison = {1, 1, 1, 1, 0, 0, 0, 1.8, 4, 1, 1},
	LaserCoal = {3, 3, 3, 1, -0.5, -0.5, -0.5, 1.3, 1.2, 1.2, 1},
	LaserFireMind = {1, 1, 1, 1, 0, 0, 0, 5, 3, 1, 1},
	LaserNumberOne = {1, 1, 1, 1, 0, 0, 0, 5, 4.9, 1, 1},
	LaserMother = {1, 1, 1, 1, 0, 0, 0, 2, 2.2, 1, 1},

	ProjectileCorpsePink = {1, 1, 1, 1, 0, 0, 0, 4, 3.5, 3.2, 1},
	ProjectileCorpseWhite = {1, 1, 1, 1, 0, 0, 0, 2.7, 3, 2, 1},
	ProjectileCorpseGreen = {1, 1, 1, 1, 0, 0, 0, 1.5, 2, 1, 1},
	ProjectileCorpseYellow = {1, 1, 1, 1, 0, 0, 0, 3.5, 2.5, 1, 1},
	ProjectileCorpseClusterLight = {4, 4, 4, 1, 0, 0, 0, 0.63, 0.85, 0.32, 1},
	ProjectileCorpseClusterDark = {1, 1, 1, 1, 0, 0, 0, 0.63, 0.85, 0.32, 1},
}

for colorName, colorData in pairs(colorPresets) do
	quickRegisterColorPreset(colorName, table.unpack(colorData))
end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- TYPE CHECKING UTILITIES
----------------------------------------------------------------------------------------------------


-- I hate Luabridge, we can't have nice things.
local function GetMetatableType(ret)
	-- TODO directly pcall in here?

	-- Vector will CRASH THE GAME through a pcall if we try getting __name or __type from it, and getmetatable doesn't work on it.
	-- We have no choice but to check manually.
	if ret.X and ret.Y then
		return "Vector"
	else
		-- Directly trying __name or __type, too, will crash the game on occasion, even through a pcall.
		-- Absolute masterclass of an API here. Best in show, really.
		return getmetatable(ret).__name or getmetatable(ret).__type
	end
end

local function checkMetatable(mtType)
	return function(val)
		if GetMetatableType(val) ~= mtType then
			return "bad return type (" .. mtType .. " expected, got " .. GetMetatableType(val).__type .. ")"
		end
	end
end


local function checkInteger(val)
	if math.type(val) ~= "integer" then
		return "bad return type (number has no integer representation)"
	end
end

local function checkNumberGreaterThanFunction(minimum)
	return function(val)
		if val <= minimum then
			return "bad return value (number must be > " .. minimum .. ")"
		end
	end
end

local function checkNumberGreaterOrEqualFunction(minimum)
	return function(val)
		if val < minimum then
			return "bad return value (number must be >= " .. minimum .. ")"
		end
	end
end

local function checkTableSizeFunction(size)
	return function(val)
		local tablesize = 0
		for i, tabletype in pairs(val) do
			tablesize = tablesize + 1
		end

		if tablesize > size then
		
			return "bad return table length (" .. tostring(size) .. " expected, got " .. tostring(tablesize) .. ")"
		end
	end
end

local function checkTableIndexes(typestrings)
	return function(tbl)
		for i, param in pairs(tbl) do
			if not typestrings[i] then 
				return "return table has unexpected key `" .. tostring(i) .. "` with value type " .. type(param)
			end
		end
	end
end

local function checkTableSizeFunctionUpTo(size)
	return function(val)
		local tablesize = 0
		for i, tabletype in pairs(val) do
			tablesize = tablesize + 1
		end

		if tablesize > size then
			return "bad return table length (up to " .. tostring(size) .. " expected, got " .. tostring(tablesize) .. ")"
		end
	end
end

local function checkTableTypeFunction(typestrings)
	return function(tbl)
		local tablesize = 0
		for i, tabletype in pairs(tbl) do
			tablesize = tablesize + 1
		end

		for i, param in pairs(tbl) do
			if typestrings[i] then --no point in doing any of this if the thing doesnt even fucking exist
				local paramType = type(param)

				if paramType == "number" and math.type(param) == "integer" then
					paramType = "integer"
				end

				if paramType == "userdata" then
					paramType = GetMetatableType(param)
				end

				if paramType ~= typestrings[i] and not (paramType == "integer" and typestrings[i] == "number") then
					return "bad return type for table value with key `" ..
					tostring(i) .. "` (" .. typestrings[i] .. " expected, got " .. paramType .. ")"
				end
			end
		end
	end
end

local function backEnum(enum, id)
	for k, v in pairs(enum) do
		if v == id then
			return k
		end
	end
end

local typecheckFunctions = {
	[ModCallbacks.MC_PRE_ADD_COLLECTIBLE] = {
		["table"] = checkTableTypeFunction({ "integer", "integer", "boolean", "integer", "integer" }),
		["number"] = checkInteger,
		["boolean"] = true,
	},
	[ModCallbacks.MC_PRE_SFX_PLAY] = {
		["table"] = checkTableTypeFunction({ "integer", "number", "integer", "boolean", "number", "number" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_ROOM_ENTITY_SPAWN] = {
		["table"] = checkTableTypeFunction({ "integer", "integer", "integer" })
	},
	[ModCallbacks.MC_USE_ITEM] = {
		["table"] = true,
		["boolean"] = true,
	},
	[ModCallbacks.MC_GET_SHADER_PARAMS] = {
		["table"] = true
	},
	[ModCallbacks.MC_PRE_MUSIC_PLAY] = {
		["table"] = checkTableTypeFunction({ "integer", "number", "boolean" }),
		["boolean"] = true,
		["number"] = checkInteger
	},
	[ModCallbacks.MC_PRE_RENDER_PLAYER_HEAD] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_RENDER_PLAYER_BODY] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_ENTITY_THROW] = {
		["Vector"] = true
	},
	[ModCallbacks.MC_PRE_CHANGE_ROOM] = {
		["table"] = checkTableTypeFunction({ "integer", "integer" }),
	},
	[ModCallbacks.MC_PRE_PICKUP_MORPH] = {
		["table"] = checkTableTypeFunction({ "integer", "integer", "integer", "integer", "integer", "integer" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_NPC_MORPH] = {
		["table"] = checkTableTypeFunction({ "integer", "integer", "integer", "integer", "integer", "integer" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_FAMILIAR_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_NPC_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_PLAYER_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_PICKUP_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_TEAR_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_PROJECTILE_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_KNIFE_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_EFFECT_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_BOMB_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_SLOT_RENDER] = {
		["Vector"] = true,
		["boolean"] = true
	},
--[[
	// This seems to be eating the callback entirely
	[ModCallbacks.MC_PRE_GRID_ENTITY_SPAWN] = {
		["boolean"] = true,
		["GridEntityDesc"] = true,
		["table"] = checkTableTypeFunction({ "integer", "integer", "integer", "integer" })
	},
]]--
	[ModCallbacks.MC_PRE_REPLACE_SPRITESHEET] = {
		["table"] = checkTableTypeFunction({ "integer", "string" })
	},
	[ModCallbacks.MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY] = {
		["boolean"] = true,
		["number"] = checkInteger
	},
	[ModCallbacks.MC_PRE_PLANETARIUM_APPLY_ITEMS] = {
		["number"] = true,
	},
	[ModCallbacks.MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS] = {
		["number"] = true,
	},
	[ModCallbacks.MC_POST_PLANETARIUM_CALCULATE] = {
		["number"] = true,
	},
	[ModCallbacks.MC_PRE_DEVIL_APPLY_ITEMS] = {
		["number"] = true,
	},
	[ModCallbacks.MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS] = {
		["number"] = true,
	},
	[ModCallbacks.MC_POST_DEVIL_CALCULATE] = {
		["number"] = true,
	},
	[ModCallbacks.MC_PRE_ITEM_OVERLAY_SHOW] = {
		["boolean"] = true,
		["number"] = checkInteger
	},
	[ModCallbacks.MC_PRE_GET_LIGHTING_ALPHA] = {
		["number"] = true,
	},
	[ModCallbacks.MC_PRE_PLAYER_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_TEAR_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_FAMILIAR_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_BOMB_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_PICKUP_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_SLOT_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_KNIFE_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_PROJECTILE_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_NPC_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_ENTITY_TAKE_DMG] = {
		["boolean"] = true,
		["table"] = checkTableTypeFunction({ Damage="number", DamageFlags="integer", DamageCountdown="number" }),
	},
	[ModCallbacks.MC_PRE_PLAYER_ADD_CARD] = {
		["boolean"] = true,
		["number"] = checkInteger,
	},
	[ModCallbacks.MC_PRE_PLAYER_ADD_PILL] = {
		["boolean"] = true,
		["number"] = checkInteger,
	},
	[ModCallbacks.MC_CONSOLE_AUTOCOMPLETE] = {
	    ["table"] = true,
	},
}

local typecheckWarnFunctions = {
	[ModCallbacks.MC_INPUT_ACTION] = {
		["number"] = function(ret, retType, entity, hook)
			if hook ~= InputHook.GET_ACTION_VALUE then
				return "bad return type for hook " .. backEnum(InputHook, hook) .. " (boolean expected, got number)"
			elseif ret < -1 or ret > 1 then
				return "bad return value for hook GET_ACTION_VALUE (number must be within range -1 to 1)"
			end
		end,
		["boolean"] = function(ret, retType, entity, hook)
			if hook == InputHook.GET_ACTION_VALUE then
				return "bad return type for hook GET_ACTION_VALUE (number expected, got boolean)"
			end
		end
	},
	[ModCallbacks.MC_EXECUTE_CMD] = { -- returning any value causes the game to crash... but strings seem to work? docs aren't clear
		["string"] = true
	},
	[ModCallbacks.MC_PRE_ENTITY_SPAWN] = {
		["table"] = checkTableSizeFunction(4)
	},
	[ModCallbacks.MC_POST_PICKUP_SELECTION] = {
		["table"] = checkTableTypeFunction({ "integer", "integer", "boolean"})
	},
	[ModCallbacks.MC_PRE_ADD_COLLECTIBLE] = {
		["table"] = checkTableSizeFunctionUpTo(5),  --{ "integer", "integer", "boolean", "integer", "integer" }),
		["number"] = checkInteger,
		["boolean"] = true,
	},
	[ModCallbacks.MC_PRE_SFX_PLAY] = {
		["table"] = checkTableSizeFunctionUpTo(6),  --{ "integer", "number", "integer", "boolean", "number", "number" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_ROOM_ENTITY_SPAWN] = {
		["table"] = checkTableSizeFunctionUpTo(3),  --{ "integer", "integer", "integer" })
	},
	[ModCallbacks.MC_PRE_MUSIC_PLAY] = {
		["table"] = checkTableSizeFunctionUpTo(3),  --{ "integer", "number", "boolean" }),
		["boolean"] = true,
		["number"] = checkInteger,
	},
	[ModCallbacks.MC_PRE_CHANGE_ROOM] = {
		["table"] = checkTableSizeFunctionUpTo(2),  --{ "integer", "integer" }),
	},
	[ModCallbacks.MC_PRE_PICKUP_MORPH] = {
		["table"] = checkTableSizeFunctionUpTo(6), --{ "integer", "integer", "integer", "integer", "integer", "integer" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_NPC_MORPH] = {
		["table"] = checkTableSizeFunctionUpTo(6), --{ "integer", "integer", "integer", "integer", "integer", "integer" }),
		["boolean"] = true
	},
	[ModCallbacks.MC_PRE_REPLACE_SPRITESHEET] = {
		["table"] = checkTableSizeFunctionUpTo(2) --{ "integer", "string" })
	},
	[ModCallbacks.MC_PRE_PLAYER_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_TEAR_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_FAMILIAR_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_BOMB_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_PICKUP_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_SLOT_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_KNIFE_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_PROJECTILE_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_PRE_NPC_COLLISION] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Collide="boolean", SkipCollisionEffects="boolean" }),
	},
	[ModCallbacks.MC_ENTITY_TAKE_DMG] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Damage="number", DamageFlags="integer", DamageCountdown="number" }),
	},
	[ModCallbacks.MC_USE_ITEM] = {
		["boolean"] = true,
		["table"] = checkTableIndexes({ Discharge = "boolean", Remove = "boolean", ShowAnim = "boolean", }),
	},
	[ModCallbacks.MC_PRE_PLAYER_ADD_CARD] = {
		["boolean"] = true,
		["number"] = checkNumberGreaterThanFunction(0),
	},
	[ModCallbacks.MC_PRE_PLAYER_ADD_PILL] = {
		["boolean"] = true,
		["number"] = checkNumberGreaterThanFunction(0),
	},
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MAX_CHARGE] = {
		["number"] = checkNumberGreaterOrEqualFunction(0),
	},
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE] = {
		["number"] = checkNumberGreaterOrEqualFunction(0),
	},
}

local boolCallbacks = {
	ModCallbacks.MC_PRE_USE_ITEM,
	ModCallbacks.MC_PRE_LASER_COLLISION,
	ModCallbacks.MC_PRE_NPC_UPDATE,
	ModCallbacks.MC_PRE_ENTITY_DEVOLVE,
	ModCallbacks.MC_PRE_SPAWN_CLEAN_AWARD,
	ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH,
	ModCallbacks.MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES,
	ModCallbacks.MC_PRE_USE_CARD,
	ModCallbacks.MC_PRE_USE_PILL,
	ModCallbacks.MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR,
	ModCallbacks.MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY,
	ModCallbacks.MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY,
	ModCallbacks.MC_PRE_SLOT_CREATE_EXPLOSION_DROPS,
	ModCallbacks.MC_PRE_DEVIL_APPLY_STAGE_PENALTY,
	ModCallbacks.MC_PRE_MEGA_SATAN_ENDING,
	ModCallbacks.MC_PRE_PLAYER_USE_BOMB,
	ModCallbacks.MC_PRE_PLAYER_TAKE_DMG,
	ModCallbacks.MC_PRE_PLAYER_COLLECT_CARD,
	ModCallbacks.MC_PRE_PLAYER_COLLECT_PILL,
	ModCallbacks.MC_PRE_PLAYER_GIVE_BIRTH_CAMBION,
	ModCallbacks.MC_PRE_PLAYER_GIVE_BIRTH_IMMACULATE,
}
for _, callback in ipairs(boolCallbacks) do
	typecheckFunctions[callback] = { ["boolean"] = true }
end

local intCallbacks = {
	ModCallbacks.MC_POST_CURSE_EVAL,
	ModCallbacks.MC_GET_CARD,
	ModCallbacks.MC_PRE_GET_COLLECTIBLE,
	ModCallbacks.MC_GET_PILL_COLOR,
	ModCallbacks.MC_GET_PILL_EFFECT,
	ModCallbacks.MC_GET_TRINKET,
	ModCallbacks.MC_GET_FOLLOWER_PRIORITY,
	ModCallbacks.MC_GET_SHOP_ITEM_PRICE,
	ModCallbacks.MC_PLAYER_GET_HEALTH_TYPE,
	ModCallbacks.MC_PLAYER_GET_ACTIVE_MAX_CHARGE,
	ModCallbacks.MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE,
	ModCallbacks.MC_PLAYER_GET_HEART_LIMIT,
	ModCallbacks.MC_PRE_SLOT_SET_PRIZE_COLLECTIBLE,
}
for _, callback in ipairs(intCallbacks) do
	typecheckFunctions[callback] = { ["number"] = checkInteger }
end

local function setExpectedTypes(typeTable)
	local validTypes = {}
	for typ, func in pairs(typeTable) do
		validTypes[#validTypes + 1] = typ
	end

	table.sort(validTypes)

	local expected = ""
	for i, typ in ipairs(validTypes) do
		if i ~= #validTypes then
			expected = expected .. typ .. ", "
		elseif #validTypes == 1 then
			expected = expected .. typ
		else
			expected = expected .. "or " .. typ
		end
	end

	if expected == "" then
		expected = "no return"
	end

	typeTable.expectedtypes = expected
end

for callback, typeTable in pairs(typecheckFunctions) do
	setExpectedTypes(typeTable)
end

for callback, typeTable in pairs(typecheckWarnFunctions) do
	setExpectedTypes(typeTable)
end

rawset(Isaac, "SetCallbackTypeCheck", function(callbackID, tbl, noSetExpected)
	typecheckFunctions[callbackID] = tbl
	if not noSetExpected then
		setExpectedTypes(tbl)
	end
end)


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- ERROR LOGGING / DISPLAY
----------------------------------------------------------------------------------------------------


local function cleanTraceback(level) -- similar to debug.traceback but breaks at xpcall, uses spaces instead of tabs, and doesn't call local functions upvalues
	level = level + 1
	local msg = "Stack Traceback:\n"
	while true do
		local info = debug_getinfo(level, "Sln")
		if not info then break end
		if info.what == "C" then
			if info.name == "xpcall" then break end
			msg = msg .. "  in method " .. tostring(info.name) .. "\n"
		else
			if info.name then
				msg = msg ..
				"  " ..
				tostring(info.short_src) ..
				":" .. tostring(info.currentline) .. ": in function '" .. tostring(info.name) .. "'\n"
			else
				msg = msg ..
				"  " ..
				tostring(info.short_src) ..
				":" .. tostring(info.currentline) .. ": in function at line " .. tostring(info.linedefined) .. "\n"
			end
		end

		level = level + 1
	end

	return msg
end

local err_input = 1
local err_decay = 1500
local err_hud_opacity = 1
local err_shader_opacity = 1
local err_color = 0
local err_shown = false
local err_bounding_x = nil

local ERR_INPUT_TARGET = 484
local INT_MIN = -2147483647

local err_main_font = Font()
local err_min_font = Font()


local function RenderErrText(name, opacity)
	local main_str = name .. " is causing errors!"
	local detail_str = "Click this message for more info."

	if not err_bounding_x then
		err_bounding_x = (Isaac.GetScreenWidth() / 2) -
		(math.max(err_main_font:GetStringWidth(main_str), err_main_font:GetStringWidth(detail_str)) / 2)
	end

	err_main_font:DrawString(main_str, (Isaac.GetScreenWidth() / 2) - (err_main_font:GetStringWidth(main_str) / 2), 0,
		KColor(1, err_color, err_color, opacity), 0, false)
	err_min_font:DrawString(detail_str, (Isaac.GetScreenWidth() / 2) - (err_min_font:GetStringWidth(detail_str) / 2), 7,
		KColor(1, err_color, err_color, opacity), 0, false)
end

local printedWarnings = {}

local function logWarning(callbackID, modName, warn)
	local cbName = callbackIDToName[callbackID] or callbackID
	for _, printedWarning in pairs(printedWarnings) do
		if warn == printedWarning then return end
	end

	table.insert(printedWarnings, warn)

	Console.PrintWarning('"' ..
	cbName ..
	'" from "' .. modName .. '" failed: ' .. warn .. "\n(This warning will not reappear until the next Lua reload.)")
	Isaac.DebugString('Error in "' ..
	cbName ..
	'" call from "' .. modName .. '": ' .. warn .. "(This warning will not reappear until the next Lua reload.)")
end

local err_modName = nil
local mouse = Sprite()
local mousecontrol_off = nil
local errdisp = nil

local UnlinkCallbacks -- Forward declaration

local function RenderErrHudCB() 
	if err_input < ERR_INPUT_TARGET then
		err_color = math.sin((err_input * 200) / err_decay) / 2 + .5
		err_input = err_input + 1
		err_decay = err_decay + 1
	else
		err_color = 0
	end

	if err_hud_opacity > 0.3 then err_hud_opacity = err_hud_opacity - (0.7 / ERR_INPUT_TARGET) end
	RenderErrText(err_modName, err_hud_opacity)


	mouse.Color = Color(1, 1, 1, err_hud_opacity)
	local mouse_pos = Isaac.WorldToScreen(Input.GetMousePosition(true))

	if mouse_pos.X > err_bounding_x and mouse_pos.X < Isaac.GetScreenWidth() - err_bounding_x and mouse_pos.Y < 18 then
		mouse.Color = Color(1, 0.5, 0.5, err_hud_opacity)
		if Input.IsMouseBtnPressed(Mouse.MOUSE_BUTTON_1) then
			ImGui.Show()
		end
	end

	if mousecontrol_off then
		mouse:Render(mouse_pos)
	end

	if ImGui.IsVisible() then UnlinkCallbacks() end
end

local function RenderErrTextCB()
	if err_shader_opacity > 0 then err_shader_opacity = err_shader_opacity - (1 / ERR_INPUT_TARGET) end
	RenderErrText(err_modName, err_shader_opacity)
end

UnlinkCallbacks = function()
	errdisp:RemoveCallback(ModCallbacks.MC_HUD_RENDER, RenderErrHudCB)
	errdisp:RemoveCallback(ModCallbacks.MC_GET_SHADER_PARAMS, RenderErrTextCB)
end


local function imGuiError(errortext)
	local windowId = "ErrorDisplayWindow"
	if not ImGui.ElementExists("ErrorDisplayMenu") then
		ImGui.CreateMenu("ErrorDisplayMenu", "REPENTOGON Error Display")
		ImGui.AddElement("ErrorDisplayMenu", "ErrorDisplayMenuEntry", ImGuiElement.MenuItem, "Error Display")
	
		ImGui.CreateWindow(windowId, "Error Display")
		ImGui.LinkWindowToElement(windowId, "ErrorDisplayMenuEntry")
	
		ImGui.AddText(windowId, "Below is the first error in the chain, which is likely (but not always) the one causing further problems.\nThis is **not** a replacement for a proper log file: it is simply an overview.\nAlways be sure to provide your full log file to mod developers.\n\n", true)
		ImGui.AddText(windowId, errortext, true, "ErrorDisplayText")
		ImGui.AddElement(windowId, "ErrorDisplayCopyToClipboard", ImGuiElement.Button, "Copy to Clipboard")
		ImGui.AddCallback("ErrorDisplayCopyToClipboard", ImGuiCallback.Clicked, function() Isaac.SetClipboard(errortext) end)
	else
		ImGui.UpdateText("ErrorDisplayText", errortext)
	end
	ImGui.SetVisible(windowId, true)
end

local err_dupecount = 1


local function logError(callbackID, modName, err)
	--[[ In order to squash multiple instances of an error into one line,
	     we want to get the *last* instance of "(x*)" for comparison here,
		 so we can remove our (x2) (x3) etc. from a comparison.

	     Unfortunately, Lua has no support for removing the last instance of a pattern,
		 but it very much has support for removing the *first* instance of a pattern.
		 For better or worse, we use a reversed version of both the target log and console history for comparison.
		 This is likely not the best for performance, but this only runs when there's an error anyways-
		 all bets are already off the table at that point.
	  ]]
	local ohistory = Console.GetHistory()
	local history = {}

	--[[ First, inverse the table, as console history is returned last-to-first.
	     Next, concatenate the table with \n so we can compare with our tracebacks.
	  ]]
	for i = #ohistory, 1, -1 do
		history[#history + 1] = ohistory[i]
	end

	history = table.concat(history, "\n"):reverse()

	local cbName = callbackIDToName[callbackID] or callbackID
	local consoleLog = '"' .. cbName .. '" from "' .. modName .. '" failed: ' .. err

	-- We add a \n to our comparison to account for the parsed history having one.
	local historyComparison = (consoleLog .. "\n"):reverse()

	local parsedHistory = history:gsub("%)(%d+)x%(%s", "", 1):sub(1, #historyComparison)

	if parsedHistory == historyComparison then
		--[[ Determine how many entries to remove.
		     Newlines automatically increment the amount of strings we need to remove by 1.
			 Since we previously concatenated history into a string with newlines, we can count
			 the amount of newlines, and this will return the amount to pop.
		]]
		err_dupecount = err_dupecount + 1
		Console.PopHistory(select(2, string.gsub(parsedHistory, "\n", "")))
		consoleLog = consoleLog .. " (x" .. err_dupecount .. ")"
	else
		err_dupecount = 1
	end

	Console.PrintError(consoleLog)
	Isaac.DebugString('Error in "' .. cbName .. '" call from "' .. modName .. '": ' .. err) -- this should be replaced with a proper log function so it can have the [INFO] header

	if not err_shown then
		errdisp = RegisterMod("REPENTOGON Error Display", 1)
		err_modName = modName;
		err_main_font:Load("font/pftempestasevencondensed.fnt")
		err_min_font:Load("font/luaminioutlined.fnt")

		mouse:Load("gfx/ui/cursor.anm2", true)
		mouse:Play("Idle")
		mousecontrol_off = not Options.MouseControl --i bet its expensive to straight up call this every frame

		imGuiError(consoleLog)

		errdisp:AddPriorityCallback(ModCallbacks.MC_HUD_RENDER, INT_MIN, RenderErrHudCB)

		errdisp:AddPriorityCallback(ModCallbacks.MC_GET_SHADER_PARAMS, INT_MIN, RenderErrTextCB)

		err_shown = true
	end
end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- TYPE CHECKING FOR CALLBACK RETURN VALUES
----------------------------------------------------------------------------------------------------


local function typeCheckCallback(callback, callbackID, ret, ...)
	local typeCheck = typecheckFunctions[callbackID]
	if typeCheck then
		local err
		local typ = type(ret)
		if typ == "userdata" then typ = GetMetatableType(ret) end

		if typeCheck[typ] == true then
		elseif typeCheck[typ] then
			err = typeCheck[typ](ret, typ, ...)
		else
			err = "bad return type (" ..
			typeCheck.expectedtypes .. " expected, got " .. tostring(typ) .. ")"
		end

		if err then
			local info = debug_getinfo(callback.Function, "S")
			logError(callbackID, callback.Mod.Name,
				info.short_src .. ": " .. info.linedefined .. ": " .. err)
			return err
		end
	end

	local typeCheckWarn = typecheckWarnFunctions[callbackID]
	if typeCheckWarn then
		local warn
		local typ = type(ret)
		if typeCheckWarn[typ] == true then
		elseif typeCheckWarn[typ] then
			warn = typeCheckWarn[typ](ret, typ, ...)
		else
			warn = "bad return type (" ..
			typeCheckWarn.expectedtypes .. " expected, got " .. tostring(typ) .. ")"
		end

		if warn then
			local info = debug_getinfo(callback.Function, "S")
			logWarning(callbackID, callback.Mod.Name,
				info.short_src .. ": " .. info.linedefined .. ": " .. warn)
		end
	end
end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- TYPE CHECKING FOR CALLBACK SYSTEM APIS (AddCallback, RemoveCallback, etc)
----------------------------------------------------------------------------------------------------


-- Function argument type checking for AddCallback, RemoveCallback, etc.
local CALLBACK_ID_TYPE = "Callback ID"
local function checkArgType(index, val, expectedType, level)
	level = (level or 2) + 1

	local actualType = type(val)
	local valid
	if expectedType == CALLBACK_ID_TYPE then
		-- Literally anything except nil is a valid callback ID.
		valid = (val ~= nil)
	else
		valid = (actualType == expectedType)
	end
	if not valid then
		if debug_getinfo(level+1).short_src == "resources/scripts/main.lua" then
			local name = debug_getinfo(level+1).name
			if name == "AddCallback" or name == "RemoveCallback" or name == "AddPriorityCallback" then
				-- Throw the error up an extra level for those mod table wrappers, to show the actual call site.
				-- IE, mod:AddCallback() / mod:AddPriorityCallback() / mod:RemoveCallback().
				level = level + 1
			end
		end
		error(string.format("Bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level).name, expectedType, actualType), level+1)
	end
end
local function checkNumberArg(index, val, level) checkArgType(index, val, "number", level) end
local function checkFunctionArg(index, val, level) checkArgType(index, val, "function", level) end
local function checkCallbackIdArg(index, val, level) checkArgType(index, val, CALLBACK_ID_TYPE, level) end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- CALLBACK SYSTEM CORE
----------------------------------------------------------------------------------------------------


local Callbacks = {}

-- The new callback structure does not use this at all.
-- However, it's kept for the legacy list of "ALL" callbacks to accomodate some niche custom callback usage.
local legacyCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

local function InitCallbackIfNeeded(callbackID)
	if not Callbacks[callbackID] then
		Callbacks[callbackID] = {
			NUM_ADDED = 0,
			ALL = setmetatable({}, legacyCallbackMeta),
			COMMON = {},
			PARAM = {},
		}
	end
end

-- Returns true if callback A should run before callback B.
local function CallbackComparator(a, b)
	if not a or not b then
		return a ~= nil
	elseif a.Priority == b.Priority then
		return a.AddOrder < b.AddOrder
	else
		return a.Priority < b.Priority
	end
end

-- Normally if you execute a callback with -1 as the param, it will run ALL added callbacks regardless of their specified param.
-- This behaviour exists for some vanilla callbacks that aren't intended to support optional params in the first place.
-- However, it can cause problems if a callback that DOES use params can get executed with -1 as the param, such as the first glitch item ID.
local RUN_CALLBACK_MINUS_ONE_PARAM_BLACKLIST = {
	[ModCallbacks.MC_PRE_USE_ITEM] = true,
	[ModCallbacks.MC_USE_ITEM] = true,
	[ModCallbacks.MC_PRE_ADD_COLLECTIBLE] = true,
	[ModCallbacks.MC_POST_ADD_COLLECTIBLE] = true,
	[ModCallbacks.MC_POST_TRIGGER_COLLECTIBLE_REMOVED] = true,
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MAX_CHARGE] = true,
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE] = true,
}

-- Returns an iterator function that takes advantage of how callbacks are now mapped by their optional params.
-- Allows other code to easily iterate over callbacks using a param, in the correct execution order.
local function GetCallbackIterator(callbackID, param)
	local callbackData = Callbacks[callbackID]

	if not callbackData then
		-- No callbacks to run. Return an empty iterator.
		return function() end
	end

	if param == -1 and not RUN_CALLBACK_MINUS_ONE_PARAM_BLACKLIST[callbackID] then
		-- If the callback is executed with -1 as the param, run ALL callbacks.
		-- This is sometimes used by the game for callbacks not intended to support optional params.
		local allCallback = callbackData.ALL[1]
		return function()
			local nextCallback

			-- Skip over removed callbacks if needed.
			repeat
				nextCallback = allCallback
				if allCallback then
					allCallback = allCallback.NextAll
				end
			until not nextCallback or not nextCallback.Removed

			return nextCallback
		end
	end

	local commonCallback = callbackData.COMMON[1]
	local paramCallback = param and callbackData.PARAM[param] and callbackData.PARAM[param][1]

	if not paramCallback then
		-- No parameterized callbacks to run, so just iterate over the common callbacks.
		return function()
			local nextCallback

			-- Skip over removed callbacks if needed.
			repeat
				nextCallback = commonCallback
				if commonCallback then
					commonCallback = commonCallback.NextParam
				end
			until not nextCallback or not nextCallback.Removed

			return nextCallback
		end
	end

	-- Simultaneously iterate over both the common callbacks and the relevant parameterized ones.
	-- By comparing the Priority of the callbacks and the order they were added, we can iterate in the correct order.
	return function()
		local nextCallback

		-- Skip over removed callbacks if needed.
		repeat
			if CallbackComparator(commonCallback, paramCallback) then
				nextCallback = commonCallback
				commonCallback = commonCallback.NextParam
			elseif paramCallback then
				nextCallback = paramCallback
				paramCallback = paramCallback.NextParam
			else
				nextCallback = nil
			end
		until not nextCallback or not nextCallback.Removed

		return nextCallback
	end
end

rawset(Isaac, "GetCallbackIterator", function(callbackID, param)
	checkCallbackIdArg(1, callbackID)
	return GetCallbackIterator(callbackID, param)
end)

-- Maintain legacy Isaac.GetCallbacks behaviour as it is accessible to mods and expects a table of ALL the callbacks, in execution order.
rawset(Isaac, "GetCallbacks", function(callbackID, createIfMissing)
	checkCallbackIdArg(1, callbackID)
	if createIfMissing then
		InitCallbackIfNeeded(callbackID)
	end
	return Callbacks[callbackID] and Callbacks[callbackID].ALL or {}
end)

-- Finds the correct spot to insert a new callback, taking into account CallbackPriority and the order they were added.
-- Utilizes Binary Search to improve performance when a lot of callbacks are added.
local function FindCallbackInsertPos(callbackList, newCallback)
	-- Check the front or back of the list first.
	if #callbackList == 0 or CallbackComparator(newCallback, callbackList[1]) then
		return 1
	elseif CallbackComparator(callbackList[#callbackList], newCallback) then
		return #callbackList + 1
	end

	-- Ok, binary search time.
	local minIdx = 2
	local maxIdx = #callbackList - 1
	local mid

	while true do
		mid = math.floor((maxIdx+minIdx)/2)
		local callback = callbackList[mid]
		if CallbackComparator(callback, newCallback) then
			minIdx = mid + 1
			if minIdx > maxIdx then
				return mid+1
			end
		elseif CallbackComparator(newCallback, callback) then
			maxIdx = mid - 1
			if minIdx > maxIdx then
				return mid
			end
		else
			return mid
		end
	end
end

local function AddToCallbackList(callbackList, newCallback, isAllList)
	local idx = FindCallbackInsertPos(callbackList, newCallback)
	table.insert(callbackList, idx, newCallback)

	local prevCallback = callbackList[idx-1]
	local nextCallback = callbackList[idx+1]

	-- Establish links to the previous/next callbacks in the list.
	-- This is done to make removing callbacks while the callback is running a little safer.
	if isAllList then
		if prevCallback then
			prevCallback.NextAll = newCallback
		end
		newCallback.NextAll = nextCallback
	else
		if prevCallback then
			prevCallback.NextParam = newCallback
		end
		newCallback.NextParam = nextCallback
	end
end

rawset(Isaac, "AddPriorityCallback", function(mod, callbackID, priority, fn, param)
	checkCallbackIdArg(2, callbackID)
	checkNumberArg(3, priority)
	checkFunctionArg(4, fn)

	InitCallbackIfNeeded(callbackID)

	local callbackData = Callbacks[callbackID]
	local wasEmpty = (#callbackData.ALL == 0)
	local numAdded = callbackData.NUM_ADDED + 1
	callbackData.NUM_ADDED = numAdded
	local newCallback = {Mod = mod, Function = fn, Priority = priority, AddOrder = numAdded, Param = param}

	AddToCallbackList(callbackData.ALL, newCallback, true)

	-- Callbacks with -1 as their param behave the same as those with no param.
	-- This aligns with the behaviour of the old implementation.
	if param and param ~= -1 then
		if not callbackData.PARAM[param] then
			callbackData.PARAM[param] = {}
		end
		AddToCallbackList(callbackData.PARAM[param], newCallback, false)
	else
		AddToCallbackList(callbackData.COMMON, newCallback, false)
	end

	if wasEmpty then
		if type(callbackID) == "number" then
			-- Enable this callback
			Isaac.SetBuiltInCallbackState(callbackID, true)
		end
	end
end)

rawset(Isaac, "AddCallback", function(mod, callbackID, fn, param)
	checkCallbackIdArg(2, callbackID)
	checkFunctionArg(3, fn)
	Isaac.AddPriorityCallback(mod, callbackID, CallbackPriority.DEFAULT, fn, param)
end)

local function RemoveCallbacksIf(callbackList, removeConditionFunc, isAllList)
	local removed = false
	for i=#callbackList,1,-1 do
		local callback = callbackList[i]
		if removeConditionFunc(callback) then
			-- Fix the iteration link of the previous callback.
			if callbackList[i-1] then
				if isAllList then
					callbackList[i-1].NextAll = callback.NextAll
				else
					callbackList[i-1].NextParam = callback.NextParam
				end
			end

			callback.Removed = true
			table.remove(callbackList, i)
			removed = true
		end
	end
	return removed
end

local function RemoveAllCallbacksIf(callbackID, removeConditionFunc)
	local callbackData = Callbacks[callbackID]

	if callbackData and RemoveCallbacksIf(callbackData.ALL, removeConditionFunc, true) then
		RemoveCallbacksIf(callbackData.COMMON, removeConditionFunc, false)
		for param, paramCallbacks in pairs(callbackData.PARAM) do
			RemoveCallbacksIf(paramCallbacks, removeConditionFunc, false)
		end
		if #callbackData.ALL == 0 and type(callbackID) == "number" then
			-- No more functions left, disable this callback
			Isaac.SetBuiltInCallbackState(callbackID, false)
		end
		return true
	end
end

rawset(Isaac, "RemoveCallback", function(mod, callbackID, fn)
	checkCallbackIdArg(2, callbackID)
	checkFunctionArg(3, fn)

	RemoveAllCallbacksIf(callbackID, function(callback) return callback.Function == fn end)
end)

-- Replacing _UnloadMod doesn't seem to allow this to get called, so instead
-- for now I'm calling it at the end of MC_PRE_MOD_UNLOAD.
local function RemoveAllCallbacksForMod(mod)
	for callbackID, callbackData in pairs(Callbacks) do
		RemoveAllCallbacksIf(callbackID, function(callback) return callback.Mod == mod end)
	end
end

-- Runs a single callback function and checks the results.
-- Only returns the value returned by the callback if no errors occured and the return value passed the type checks.
local function RunCallbackInternal(callbackID, callback, ...)
	local status, ret = xpcall(callback.Function, function(msg)
		return msg .. "\n" .. cleanTraceback(2)
	end, callback.Mod, ...)

	if not status then
		logError(callbackID, callback.Mod.Name, ret)
	elseif ret ~= nil and not typeCheckCallback(callback, callbackID, ret, ...) then
		return ret
	end
end


----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
---- RUNCALLBACK LOGIC
----------------------------------------------------------------------------------------------------


-- Default callback behaviour (first returned non-nil value terminates the callback).
local function DefaultRunCallbackLogic(callbackID, param, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, ...)
		if ret ~= nil then
			return ret
		end
	end
end

local function PreModUnloadCallbackLogic(callbackID, param, mod, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		RunCallbackInternal(callbackID, callback,  mod, ...)
	end

	-- I wasn't able to properly override _UnloadMod so I'm doing this here instead for now...
	RemoveAllCallbacksForMod(mod)
end

-- Basic "additive" callback behaviour. Values returned from a callback replace the value of the FIRST arg for subsequent callbacks.
-- Separate implementations are used depending on which arg is updated by the return value, because table.unpack tricks are slower.
local function RunAdditiveFirstArgCallback(callbackID, param, value, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, value, ...)
		if ret ~= nil then
			value = ret
		end
	end
	return value
end

local function RunAdditiveSecondArgCallback(callbackID, param, arg1, value, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, arg1, value, ...)
		if ret ~= nil then
			value = ret
		end
	end
	return value
end

local function RunAdditiveThirdArgCallback(callbackID, param, arg1, arg2, value, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, arg1, arg2, value, ...)
		if ret ~= nil then
			value = ret
		end
	end
	return value
end

local function RunAdditiveFourthArgCallback(callbackID, param, arg1, arg2, arg3, value, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, arg1, arg2, arg3, value, ...)
		if ret ~= nil then
			value = ret
		end
	end
	return value
end

-- Older paramless version of the additive callback logic, preserved because it was a global.
function _RunAdditiveCallback(callbackID, value, ...)
	RunAdditiveFirstArgCallback(callbackID, nil, value, ...)
end

local function RunPreAddCardPillCallback(callbackID, param, player, pillCard, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, player, pillCard, ...)
		if type(ret) == "boolean" and ret == false then
			return
		elseif type(ret) == "number" and ret > 0 then
			pillCard = ret
		end
	end
	return pillCard
end

-- Custom behaviour for pre-render callbacks (terminate on false, adds returned vectors to the render offset).
function _RunPreRenderCallback(callbackID, param, mt, value, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, mt, value, ...)
		if ret ~= nil then
			if type(ret) == "boolean" and ret == false then
				return false
			elseif ret.X and ret.Y then -- We're a Vector, checking it directly will crash the game... While we should always be a Vector at this point it doesn't hurt to check
				value = value + ret
			end
		end
	end
	return value
end

-- Custom handling for the MC_ENTITY_TAKE_DMG rewrite, so if a mod changes the damage amount etc that doesn't terminate the callback
-- and the updated values are shown to later callbacks. The callback also now ONLY terminates early if FALSE is returned.
function _RunEntityTakeDmgCallback(callbackID, param, entity, damage, damageFlags, source, damageCountdown)
	local combinedRet

	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, entity, damage, damageFlags, source, damageCountdown)
		if ret ~= nil then
			if type(ret) == "boolean" and ret == false then
				-- Only terminate the callback early if someone returns FALSE.
				-- RIP to returning true stopping the callback.
				return false
			elseif type(ret) == "table" then
				-- Set / update overrides to damage etc so that they are visible to later callbacks.
				if ret.Damage and type(ret.Damage) == "number" then
					damage = ret.Damage
				end
				if ret.DamageFlags and type(ret.DamageFlags) == "number" and ret.DamageFlags == math.floor(ret.DamageFlags) then
					damageFlags = ret.DamageFlags
				end
				if ret.DamageCountdown and type(ret.DamageCountdown) == "number" then
					damageCountdown = ret.DamageCountdown
				end
				if combinedRet then
					for k, v in pairs(ret) do
						combinedRet[k] = v
					end
				else
					combinedRet = ret
				end
			end
		end
	end

	return combinedRet
end

-- Custom handling for MC_PRE_TRIGGER_PLAYER_DEATH and MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES.
-- Terminate early if the player is revived by any means.
function _RunTriggerPlayerDeathCallback(callbackID, param, player, ...)
	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, player, ...)
		if ret == false or not player:IsDead() then
			return ret
		end
	end
	return true
end

-- Custom handling for MC_POST_PICKUP_SELECTION.
-- Terminate early if the table's 3rd argument is nil or false
function _RunPostPickupSelection(callbackID, param, pickup, variant, subType, ...)
	local recentRet = nil;

	for callback in GetCallbackIterator(callbackID, param) do
		local ret = RunCallbackInternal(callbackID, callback, pickup, variant, subType, ...)
		if type(ret) == "table" then
			if not ret[3] then
				return ret
			end

			if (math.type(ret[1]) == "integer" and math.type(ret[2]) == "integer") then
				recentRet = {ret[1], ret[2]}
				variant = ret[1]
				subType = ret[2]
			end

		end
	end

	return recentRet
end

-- I don't think we need these exposed anymore, but safer to just leave them alone since they were already exposed.
rawset(Isaac, "RunPreRenderCallback", _RunPreRenderCallback)
rawset(Isaac, "RunAdditiveCallback", _RunAdditiveCallback)
rawset(Isaac, "RunEntityTakeDmgCallback", _RunEntityTakeDmgCallback)
rawset(Isaac, "RunTriggerPlayerDeathCallback", _RunTriggerPlayerDeathCallback)


-- Defines non-default callback handling logic to be used for specific callbacks.
-- If a callback is not specified here, "DefaultRunCallbackLogic" will be called.
local CustomRunCallbackLogic = {
	[ModCallbacks.MC_PRE_MOD_UNLOAD] = PreModUnloadCallbackLogic,
	[ModCallbacks.MC_ENTITY_TAKE_DMG] = _RunEntityTakeDmgCallback,
	[ModCallbacks.MC_POST_PICKUP_SELECTION] = _RunPostPickupSelection,
	[ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH] = _RunTriggerPlayerDeathCallback,
	[ModCallbacks.MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES] = _RunTriggerPlayerDeathCallback,
	[ModCallbacks.MC_PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_PRE_DEVIL_APPLY_ITEMS] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_POST_DEVIL_CALCULATE] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_PRE_PLANETARIUM_APPLY_ITEMS] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_POST_PLANETARIUM_CALCULATE] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_EVALUATE_CUSTOM_CACHE] = RunAdditiveThirdArgCallback,
	[ModCallbacks.MC_EVALUATE_FAMILIAR_MULTIPLIER] = RunAdditiveFirstArgCallback,
	[ModCallbacks.MC_PRE_PLAYER_ADD_CARD] = RunPreAddCardPillCallback,
	[ModCallbacks.MC_PRE_PLAYER_ADD_PILL] = RunPreAddCardPillCallback,
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE] = RunAdditiveThirdArgCallback,
	[ModCallbacks.MC_PLAYER_GET_ACTIVE_MAX_CHARGE] = RunAdditiveFourthArgCallback,
}

for _, callback in ipairs({
	ModCallbacks.MC_PRE_FAMILIAR_RENDER,
	ModCallbacks.MC_PRE_NPC_RENDER,
	ModCallbacks.MC_PRE_PLAYER_RENDER,
	ModCallbacks.MC_PRE_PICKUP_RENDER,
	ModCallbacks.MC_PRE_TEAR_RENDER,
	ModCallbacks.MC_PRE_PROJECTILE_RENDER,
	ModCallbacks.MC_PRE_KNIFE_RENDER,
	ModCallbacks.MC_PRE_EFFECT_RENDER,
	ModCallbacks.MC_PRE_BOMB_RENDER,
	ModCallbacks.MC_PRE_SLOT_RENDER,
}) do
	CustomRunCallbackLogic[callback] = _RunPreRenderCallback
end


function _RunCallback(callbackID, param, ...)
	-- Check for custom logic.
	local runCallbackLogic = CustomRunCallbackLogic[callbackID] or DefaultRunCallbackLogic

	return runCallbackLogic(callbackID, param, ...)
end

Isaac.RunCallbackWithParam = _RunCallback

function Isaac.RunCallback(callbackID, ...)
	return Isaac.RunCallbackWithParam(callbackID, nil, ...)
end


-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
---- MISC STUFF
-----------------------------------------------------------------------------------------------------


--Menuman Hub
MenuManager.MainMenu = MainMenu
MenuManager.CharacterMenu = CharacterMenu
MenuManager.StatsMenu = StatsMenu

-- ImGui alias functions
--local ImGui = Isaac.GetImGui()
ImGui.ImGuiToWorld = function (position) return Isaac.ScreenToWorld(position) end
ImGui.WorldToImGui = function (position) return Isaac.WorldToScreen(position) * Isaac.GetScreenPointScale() end

if not _LUADEBUG then
	debug = nil
	os = {
		date = os.date,
		time = os.time,
		clock = os.clock,
		difftime = os.difftime
	}
end

--Object Cache stuff
local gaymeinstance
local oldgayme = Game
function Game()
	if not gaymeinstance then
		gaymeinstance = oldgayme()
	end
	return gaymeinstance
end


local oldregmod = RegisterMod

function RegisterMod(name, ver)
	local out = oldregmod(name, ver)
	out.Repentogon = REPENTOGON
	return out
end


rawset(Isaac, "GetEntityConfig", function() return EntityConfig end)
RoomConfigHolder = RoomConfig  -- Backwards compatability

-- Reset Imgui Data after reload of all mods
ImGui.Reset()

--resource load error, used in changelog but it's not strictly a changelog thing so i'm putting it here
local res_error_font=Font()
local loaded_reserror_font=""

local curlocale="default"
local reserror_text={
	["en"]={"Unable to load REPENTOGON",
				"resources. Please make sure",
				"that all files are copied",
				"correctly and that mods are",
				"enabled in the mods menu!",},
	["ru"]={"Не удалось загрузить ресурсы",
			"REPENTOGON-а. Просьба проверить",
			"наличие всех файлов и убедиться,",
			"что моды включены в меню модов!",},
	["zh"]={"无法加载 REPENTOGON 资源。",
			"请确保正确复制了所有文件，并在 mod",
			"菜单中启用了 mod!",},
}

local reserror_fonts={
	["en"]={"font/pftempestasevencondensed.fnt",0,0}, --font name, hori and vert offsets relative to en replacement
	["ru"]={"font/pftempestasevencondensed.fnt",-30,0},
	["zh"]={"font/cjk/lanapixel.fnt",-15,0},
}


local function NoRepentogonFolderErrRender()
--	if MenuManager:GetActiveMenu()==MainMenuType.TITLE then
		curlocale=Options.Language
		if not reserror_text[curlocale] then curlocale="en" end
		if loaded_reserror_font~=reserror_fonts[curlocale][1] or (not res_error_font:IsLoaded()) then res_error_font:Load(reserror_fonts[curlocale][1]) loaded_reserror_font=reserror_fonts[curlocale][1] end
		local rendercoords=Isaac.WorldToMenuPosition(MainMenuType.TITLE,Vector(260+reserror_fonts[curlocale][2],180+reserror_fonts[curlocale][3]))
		for i,line in ipairs(reserror_text[curlocale]) do
			res_error_font:DrawStringUTF8(line,rendercoords.X,rendercoords.Y + 12*(i-1) ,KColor(0.7,0.1,0.1,1.0),0,false)
		end
--	end
end
REPENTOGON.Extras.Misc.NoRPTGNFldrErr=NoRepentogonFolderErrRender

local SpriteMT=getmetatable(Sprite)
local OldSprConstructor=SpriteMT.__call

function SpriteMT.__call(_,ANM2Path,LoadGraphics)
	local out,isloaded=OldSprConstructor(),false
	if LoadGraphics==nil then LoadGraphics=true end

	if ANM2Path and type(ANM2Path)=="string" then
		out:Load(ANM2Path,LoadGraphics)
		isloaded=out:GetLayerCount()>0
	end

	return out,isloaded
end


local FontMT=getmetatable(Font)
local OldFontConstructor=FontMT.__call

function FontMT.__call(_,FontPath)
	local out,isloaded=OldFontConstructor(),false

	if FontPath and type(FontPath)=="string" then
		out:Load(FontPath)
		isloaded=out:IsLoaded()
	end

	return out,isloaded
end

-- Gets rid of the old `scripts/main.lua` wrappers for some re-implemented function hooks.
-- This can allow us to circumvent how the old wrappers handled certain inputs, as well as make
-- errors report the actual call site instead of the line in `scripts/main.lua`.
-- Make sure you understand what the old wrapper was doing, and that the new hook maintains any features like optional args.
local LuaWrappersToRemove = {
	{ ItemPool, {"GetCollectible"} },
	{
		EntityPlayer, {
			"AddCollectible",
			"FullCharge",
			"GetMultiShotParams",
			"GetActiveItem",
			"GetActiveCharge",
			"GetActiveSubCharge",
			"GetBatteryCharge",
			"NeedsCharge",
			"SetActiveCharge",
			"DischargeActiveItem",
			"SetPocketActiveItem",
		}
	},
}
for _, tab in ipairs(LuaWrappersToRemove) do
	local class = tab[1]

	local mt = getmetatable(class).__class
	local oldIndex = mt.__index
	local newIndex = {}

	for _, funcName in ipairs(tab[2]) do
		newIndex[funcName] = mt[funcName]
	end

	rawset(mt, "__index", function(self, k)
		return newIndex[k] or oldIndex(self, k)
	end)
end
LuaWrappersToRemove = nil

--res load error stuff end

pcall(require("repentogon_extras/changelog"))
pcall(require("repentogon_extras/daily_stats"))
pcall(require("repentogon_extras/stats_menu"))
pcall(require("repentogon_extras/bestiary_menu"))
-- pcall(require("repentogon_extras/onlinestub")) let's not load it
pcall(require("repentogon_extras/mods_menu_tweaks"))