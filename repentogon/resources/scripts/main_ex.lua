REPENTOGON = {
	["Real"] = true,
	["Name"] = "REPENTOGON", --I avoid having to do RegisterMod this way, sorry ;P
	["RESOURCEPATH"] = "../../../resources-repentogon",
	["Extras"] = {}, -- Tables containing additional REPENTOGON data structures, example: ChangeLog or StatsMenu
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
		["number"] = checkInteger
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
	[ModCallbacks.MC_PRE_GRID_INIT] = {
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

	Game():GetConsole():PrintWarning('"' ..
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
			Isaac.GetImGui():Show()
			UnlinkCallbacks()
		
		end
	end

	if mousecontrol_off then
		mouse:Render(mouse_pos)
	end
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
	local imgui = Isaac.GetImGui()
	local windowId = "ErrorDisplayWindow"
	if not imgui:ElementExists("ErrorDisplayMenu") then
		imgui:CreateMenu("ErrorDisplayMenu", "REPENTOGON Error Display")
		imgui:AddElement("ErrorDisplayMenu", "ErrorDisplayMenuEntry", ImGuiElement.MenuItem, "Error Display")
	
		imgui:CreateWindow(windowId, "Error Display")
		imgui:LinkWindowToElement(windowId, "ErrorDisplayMenuEntry")
	
		imgui:AddText(windowId, "Below is the first error in the chain, which is likely (but not always) the one causing further problems.\nThis is **not** a replacement for a proper log file: it is simply an overview.\nAlways be sure to provide your full log file to mod developers.\n\n", true)
		imgui:AddText(windowId, errortext, true, "ErrorDisplayText")
		imgui:AddElement(windowId, "ErrorDisplayCopyToClipboard", ImGuiElement.Button, "Copy to Clipboard")
		imgui:AddCallback("ErrorDisplayCopyToClipboard", ImGuiCallback.Clicked, function() Isaac.SetClipboard(errortext) end)
	else
		imgui:UpdateText("ErrorDisplayText", errortext)
	end
	imgui:SetVisible(windowId, true)
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
	local console = Game():GetConsole()
	local ohistory = console:GetHistory()
	local history = {}

	--[[ First, inverse the table, as console history is returned last-to-first.
	     Next, concatenate the table with \n so we can compare with our tracebacks.
	  ]]
	for i = #ohistory, 1, -1 do
		history[#history + 1] = ohistory[i]
	end

	history = table.concat(history, "\n"):reverse()

	local cbName = callbackIDToName[callbackID] or callbackID
	if type(cbName) == "table" then
		cbName = "<Unknown Callback>"
	end
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
		console:PopHistory(select(2, string.gsub(parsedHistory, "\n", "")))
		consoleLog = consoleLog .. " (x" .. err_dupecount .. ")"
	else
		err_dupecount = 1
	end

	console:PrintError(consoleLog)
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
local ImGui = Isaac.GetImGui()
rawset(getmetatable(ImGui), "ImGuiToWorld", function (_, position) return Isaac.ScreenToWorld(position) end)
rawset(getmetatable(ImGui), "WorldToImGui", function (_, position) return Isaac.WorldToScreen(position) * Isaac.GetScreenPointScale() end)

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

-- Typecheck the return value of an invocation of a MC_PRE_OPENGL_RENDER callback.
--
-- Allowed return values are an integer (a shader ID) or a table containing two fields:
--  "shader": int (if native shader) or Shader userdata (if custom shader)
--  "buffer": VertexBuffer
--
-- Return true if ret is a valid return value, false otherwise.
local function TypecheckPreOpenGL(ret)
    if type(ret) == "number" then
        return ret >= 0 and ret <= 19
    end
    
    if type(ret) == "table" then
        if ret["shader"] == nil or ret["buffer"] == nil then
            return false
        end
        
        local shader = ret["shader"]
        local buffer = ret["buffer"]
        local tShader = type(shader)
        if tShader ~= "number" and tShader ~= "userdata" then
            return false
        end
        
        local tBuffer = type(buffer)
        if tBuffer ~= "userdata" then
            return false
        end
        
        if tShader == "number" then
            -- 0 : ColorOffset, 19 : Mirror (MAX)
            if shader < 0 or shader > 19 then
                return false
            end
        end
        
        if tShader == "userdata" and getmetatable(shader)["type"] ~= "Shader" then
            return false
        end 
        
        if getmetatable(buffer)["type"] ~= "VertexBuffer" then
            return false
        end
    end
    
    return true
end

-- Call the MC_PRE_OPENGL_RENDER callback.
-- 
-- The purpose of this function is to reduce the amount of jumps between Lua and C++.
-- For a given vertex buffer, we may have multiple render contexts. Trigerring the callback 
-- from C++ on every single one of these contexts could induce hundreds of jumps. The
-- solution here is to call this function as a precallback with lots of data as parameter,
-- and this data is then sent to different invocations of the callback.
--
-- Parameters:
--  - bufferAndContexts: a table consisting of pairs (vertexBuffer -> renderContext).
-- When compared to what exists on the C++ side, this is equivalent to slicing the 
-- input vertex buffer into the vertices associated with a single render operation.
--  - shaderId: identifier of the vertex program, as returned by the glCreateProgram
-- function, that is currently bound and should run during the incoming call to 
-- glDrawElements.
--
-- Return: a table containing the return value of every invocation of the MC_PRE_OPENGL_RENDER
-- callback. Check the documentation of the callback for more information.
--
-- This function is purely internal, despite being marked as global. Internally, as soon 
-- as this file is loaded we register a reference to this function in the Lua registry 
-- and free the symbol to prevent modders from overriding it.
function PreOpenGLRender(bufferAndContexts, shaderId)
    local callbacks = Isaac.GetCallbacks(ModCallbacks.MC_PRE_OPENGL_RENDER)
    if not callbacks then
        return {}
    end
    
    local changedBuffers = {}
    local returns = {}
    
    for _, callback in ipairs(callbacks) do
        for i, bufferAndContext in ipairs(bufferAndContexts) do
            if changedBuffers[i] then
                goto skip
            end
            
            returns[i] = { shader = shaderId, buffer = bufferAndContext.context }
        
            local status, ret = xpcall(callback.Function, function(msg) 
                return msg .. "\n" .. cleanTraceback(2)
            end, callback.Mod, bufferAndContext.buffer, shaderId, bufferAndContext.context)
            
            if status then
                if ret ~= nil then
                    if TypecheckPreOpenGL(ret) then
                        if type(ret) == "number" then
                            returns[i] = { shader = ret, buffer = bufferAndContext.buffer }
                        else
                            returns[i] = ret
                        end
                        
                        changedBuffers[i] = true
                        goto skip
                    end
                end 
                
                returns[i] = { shader = shaderId, buffer = bufferAndContext.buffer }
            else
                logError(ModCallbacks.MC_PRE_OPENGL_RENDER, callback.Mod.Name, ret)
            end
            
            ::skip::
        end
    end
    
    return returns
end

-- Reset Imgui Data after reload of all mods
Isaac.GetImGui():Reset()

pcall(require("repentogon_extras/changelog"))
pcall(require("repentogon_extras/daily_stats"))
pcall(require("repentogon_extras/stats_menu"))
pcall(require("repentogon_extras/onlinestub"))

-- TESTING !!
pcall(require("repentogon_tests/test_imgui"))
