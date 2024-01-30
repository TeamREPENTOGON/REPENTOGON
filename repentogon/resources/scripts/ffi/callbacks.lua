local debug_getinfo = debug.getinfo

local Callbacks = {}

local function checktype(index, val, typ, level)
	local t = type(val)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level).name, typ, t), level+1)
	end
end

local function checknumber(index, val, level) checktype(index, val, "number", (level or 2)+1) end
local function checkfunction(index, val, level) checktype(index, val, "function", (level or 2)+1) end
local function checkstring(index, val, level) checktype(index, val, "string", (level or 2)+1) end
local function checktable(index, val, level) checktype(index, val, "table", (level or 2)+1) end

local defaultCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

rawset(Isaac, "GetCallbacks", function(callbackId, createIfMissing)
	if createIfMissing and not Callbacks[callbackId] then
		Callbacks[callbackId] = setmetatable({}, defaultCallbackMeta)
	end
	
	return Callbacks[callbackId] or {}
end)

rawset(Isaac, "AddPriorityCallback", function(mod, callbackId, priority, fn, param)
	checknumber(3, priority)
	checkfunction(4, fn)
	
	local callbacks = Isaac.GetCallbacks(callbackId, true)
	local wasEmpty = #callbacks == 0
	
	local pos = #callbacks+1
	for i=#callbacks,1,-1 do
		if callbacks[i].Priority <= priority then
			break
		else
			pos = pos-1
		end
	end
	
	table.insert(callbacks, pos, {Mod = mod, Function = fn, Priority = priority, Param = param})
	
	if wasEmpty then
		if type(callbackId) == "number" then
			-- Enable this callback
			Isaac.SetBuiltInCallbackState(callbackId, true)
		end
	end
end)

rawset(Isaac, "AddCallback", function(mod, callbackId, fn, param)
	checkfunction(3, fn)
	--Isaac.AddPriorityCallback(mod, callbackId, CallbackPriority.DEFAULT, fn, param)
	Isaac.AddPriorityCallback(mod, callbackId, 0, fn, param)
end)

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
						--TODO reintroduce typechecking
						--if not typeCheckCallback(callback, callbackID, ret, ...) then
							return ret
						--end
					end
				--[[else
					logError(callbackID, callback.Mod.Name, ret)]]
				end
			end
		end
	end
end

rawset(Isaac, "RunCallbackWithParam", RunCallback)
rawset(Isaac, "RunCallback", function(callbackID, ...) return RunCallback(callbackID, nil, ...) end)