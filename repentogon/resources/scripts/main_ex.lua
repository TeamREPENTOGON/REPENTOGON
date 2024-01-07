REPENTOGON = {
	["Real"] = true,
	["Name"] = "REPENTOGON", --I avoid having to do RegisterMod this way, sorry ;P
	["Version"] = "dev build",
	["RESOURCEPATH"] = "../../../resources-repentogon",
	["Extras"] = { ["Misc"]={}, }, -- Tables containing additional REPENTOGON data structures, example: ChangeLog or StatsMenu
}

collectgarbage("generational")

local defaultCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

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

local function checkTableSizeFunction(size)
	return function(val)
		local tablesize = 0
		for i, tabletype in pairs(val) do
			tablesize = i
		end

		if tablesize > size then
			return "bad return table length (" .. tostring(size) .. " expected, got " .. tostring(tablesize) .. ")"
		end
	end
end

local function checkTableTypeFunction(typestrings)
	return function(tbl)
		local tablesize = 0
		for i, tabletype in pairs(tbl) do
			tablesize = tablesize + 1
		end

		local numtypestrings = 0
		for i, typestr in pairs(typestrings) do
			numtypestrings = numtypestrings + 1
		end

		if tablesize > numtypestrings then
			return "bad return table length (up to " .. tostring(numtypestrings) .. " expected, got " .. tostring(tablesize) .. ")"
		end

		for i, param in pairs(tbl) do
			local paramType = type(param)

			if paramType == "number" and math.type(param) == "integer" then
				paramType = "integer"
			end

			if paramType == "userdata" then
				paramType = GetMetatableType(param)
			end

			if not typestrings[i] then
				return "return table has unexpected key `" .. tostring(i) .. "` with value type " .. paramType
			end

			if paramType ~= typestrings[i] and not (paramType == "integer" and typestrings[i] == "number") then
				return "bad return type for table value with key `" ..
				tostring(i) .. "` (" .. typestrings[i] .. " expected, got " .. paramType .. ")"
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
	[ModCallbacks.MC_PRE_ENTITY_SPAWN] = {
		["table"] = checkTableSizeFunction(4)
	},
	[ModCallbacks.MC_POST_PICKUP_SELECTION] = {
		["table"] = checkTableSizeFunction(2)
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
	[ModCallbacks.MC_PRE_GRID_ENTITY_SPAWN] = {
		["boolean"] = true,
		["number"] = checkInteger
	},
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
}

local boolCallbacks = {
	ModCallbacks.MC_PRE_USE_ITEM,
	ModCallbacks.MC_PRE_LASER_COLLISION,
	ModCallbacks.MC_PRE_NPC_UPDATE,
	ModCallbacks.MC_PRE_ENTITY_DEVOLVE,
	ModCallbacks.MC_PRE_SPAWN_CLEAN_AWARD,
	ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH,
	ModCallbacks.MC_PRE_USE_CARD,
	ModCallbacks.MC_PRE_USE_PILL,
	ModCallbacks.MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR,
	ModCallbacks.MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY,
	ModCallbacks.MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY,
	ModCallbacks.MC_PRE_SLOT_CREATE_EXPLOSION_DROPS,
	ModCallbacks.MC_PRE_DEVIL_APPLY_STAGE_PENALTY,
	ModCallbacks.MC_CUSTOM_CHARACTER_UNLOCKED,
	ModCallbacks.MC_PRE_MEGA_SATAN_ENDING,
	ModCallbacks.MC_PRE_PLAYER_USE_BOMB,
	ModCallbacks.MC_PRE_PLAYER_TAKE_DMG,
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

rawset(Isaac, "SetCallbackMatchTest", function(callbackID, func)
	local callbacks = Isaac.GetCallbacks(callbackID, true)
	getmetatable(callbacks).__matchParams = func
end)

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

local function typeCheckCallback(callback, callbackID, ret, ...)
    local err
	local typeCheck = typecheckFunctions[callbackID]
	if typeCheck then
		local typ = type(ret)
		if typ == "userdata" then typ = GetMetatableType(ret) end
		if not err then
			if typeCheck[typ] == true then
			elseif typeCheck[typ] then
				err = typeCheck[typ](ret, typ, ...)
			else
				err = "bad return type (" ..
				typeCheck.expectedtypes .. " expected, got " .. tostring(typ) .. ")"
			end
		end

		if err then
			local info = debug_getinfo(callback.Function, "S")
			logError(callbackID, callback.Mod.Name,
				info.short_src .. ": " .. info.linedefined .. ": " .. err)
		end
	end

	local typeCheckWarn = typecheckWarnFunctions[callbackID]
	if typeCheckWarn then
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
	
	return err
end

function _RunCallback(callbackID, Param, ...)
	local callbacks = Isaac.GetCallbacks(callbackID)
	if callbacks then
		for _, callback in ipairs(callbacks) do
			local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams
			if matchFunc(Param, callback.Param) then
				local status, ret = xpcall(callback.Function, function(msg)
					return msg .. "\n" .. cleanTraceback(2)
				end, callback.Mod, ...)
				if status then
					if ret ~= nil then
						if not typeCheckCallback(callback, callbackID, ret, ...) then
							return ret
						end
					end
				else
					logError(callbackID, callback.Mod.Name, ret)
				end
			end
		end
	end
end

function _RunAdditiveCallback(callbackID, value, ...)
	local callbacks = Isaac.GetCallbacks(callbackID)
	if callbacks then
		for _, callback in ipairs(callbacks) do
			local status, ret = xpcall(callback.Function, function(msg)
				return msg .. "\n" .. cleanTraceback(2)
			end, callback.Mod, value, ...)
			if status then
				if ret ~= nil then
					if not typeCheckCallback(callback, callbackID, ret, ...) then
						value = ret
					end
				end
			else
				logError(callbackID, callback.Mod.Name, ret)
			end
		end
	end
	return value
end

function _RunPreRenderCallback(callbackID, param, mt, value, ...)
	local callbacks = Isaac.GetCallbacks(callbackID)
	
	if callbacks then
		local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams

		for _, callback in ipairs(callbacks) do
			if matchFunc(param, callback.Param) then
				local status, ret = xpcall(callback.Function, function(msg)
					return msg .. "\n" .. cleanTraceback(2)
				end, callback.Mod, mt, value, ...)
				if status then
					if ret ~= nil then
						if not typeCheckCallback(callback, callbackID, ret, ...) then
							if type(ret) == "boolean" and ret == false then
								return false
							elseif ret.X and ret.Y then -- We're a Vector, checking it directly will crash the game... While we should always be a Vector at this point it doesn't hurt to check
								value = value + ret
							end
						end
					end
				else
					logError(callbackID, callback.Mod.Name, ret)
				end
			end
		end
		return value
	end
end

-- Custom handling for the MC_ENTITY_TAKE_DMG rewrite, so if a mod changes the damage amount etc that doesn't terminate the callback
-- and the updated values are shown to later callbacks. The callback also now ONLY terminates early if FALSE is returned.
function _RunEntityTakeDmgCallback(callbackID, param, entity, damage, damageFlags, source, damageCountdown)
	local callbacks = Isaac.GetCallbacks(callbackID)

	if callbacks then
		local combinedRet

		for _, callback in ipairs(callbacks) do
			local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams
			if matchFunc(param, callback.Param) then
				local status, ret = xpcall(callback.Function, function(msg)
					return msg .. "\n" .. cleanTraceback(2)
				end, callback.Mod, entity, damage, damageFlags, source, damageCountdown)
				if status then
					if ret ~= nil then
						if not typeCheckCallback(callback, callbackID, ret, entity, damage, damageFlags, source, damageCountdown) then
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
				else
					logError(callbackID, callback.Mod.Name, ret)
				end
			end
		end

		return combinedRet
	end
end

rawset(Isaac, "RunPreRenderCallback", _RunPreRenderCallback)

rawset(Isaac, "RunAdditiveCallback", _RunAdditiveCallback)

rawset(Isaac, "RunEntityTakeDmgCallback", _RunEntityTakeDmgCallback)

Isaac.RunCallbackWithParam = _RunCallback

function Isaac.RunCallback(callbackID, ...)
	return Isaac.RunCallbackWithParam(callbackID, nil, ...)
end

--Menuman Hub
MenuManager.MainMenu = MainMenu
MenuManager.CharacterMenu = CharacterMenu
MenuManager.StatsMenu = StatsMenu

-- ImGui alias functions
--local ImGui = Isaac.GetImGui()
ImGui.ImGuiToWorld = function (_, position) return Isaac.ScreenToWorld(position) end
ImGui.WorldToImGui = function (_, position) return Isaac.WorldToScreen(position) * Isaac.GetScreenPointScale() end

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



--res load error stuff end

pcall(require("repentogon_extras/changelog"))
pcall(require("repentogon_extras/daily_stats"))
pcall(require("repentogon_extras/stats_menu"))
pcall(require("repentogon_extras/bestiary_menu"))
pcall(require("repentogon_extras/onlinestub"))
