REPENTOGON = true

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

local typecheckFunctions = {}

typecheckFunctions[ModCallbacks.MC_PRE_ADD_COLLECTIBLE] = function(ret) --TODO might be better as a table of paths to take depending on ret type? might help performance a bit, less if/else statements
	if type(ret) == "table" then
		local tablesize = #ret
		if tablesize ~= 5 then
			return "bad return table length (5 expected, got " .. tostring(tablesize) .. ")"
		end
	elseif type(ret) == "number" and math.type(ret) == "integer" then -- valid
	else
		return "bad return type (table or integer expected, got " .. tostring(type(ret)) .. ")"
	end
end

rawset(Isaac, "SetCallbackTypeCheck", function(callbackID, func)
	typecheckFunctions[callbackID] = func
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
				msg = msg .. "  " .. tostring(info.short_src) .. ":" .. tostring(info.currentline) .. ": in function '" .. tostring(info.name) .. "'\n"
			else
				msg = msg .. "  " .. tostring(info.short_src) .. ":" .. tostring(info.currentline) .. ": in function at line " .. tostring(info.linedefined) .. "\n"
			end
		end
		
		level = level + 1
	end
	
	return msg
end

local function logError(callbackID, modName, err)
	local cbName = callbackIDToName[callbackID] or callbackID
	Game():GetConsole():PrintError('"' .. cbName .. '" from "' .. modName .. '" failed: ' ..  err)
	Isaac.DebugString('Error in "' ..cbName .. '" call from "' .. modName .. '": ' .. err) -- this should be replaced with a proper log function so it can have the [INFO] header
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
						local err
						if typecheckFunctions[callbackID] then
							err = typecheckFunctions[callbackID](ret)
							if err then
								local info = debug_getinfo(callback.Function, "S")
								logError(callbackID, callback.Mod.Name, info.short_src .. ": " .. info.linedefined .. ": " .. err)
							end
						end
						
						if not err then
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

Isaac.RunCallbackWithParam = _RunCallback

function Isaac.RunCallback(callbackID, ...)
	return Isaac.RunCallbackWithParam(callbackID, nil, ...)
end

if not _LUADEBUG then
	debug = nil
	os = {
		date = os.date,
		time = os.time,
		clock = os.clock,
		difftime = os.difftime
	}
end
