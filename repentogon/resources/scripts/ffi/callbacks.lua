local debug_getinfo = debug.getinfo
local repentogon = ffidll
local lffi = ffi

local Callbacks = {}

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
	ffichecks.checknumber(3, priority)
	ffichecks.checkfunction(4, fn)
	
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
	ffichecks.checkfunction(3, fn)
	--TODO enum
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
					--return msg .. "\n" .. cleanTraceback(2)
					return msg
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

rawset(Isaac, "RemoveCallback", function(mod, callbackId, fn)
	ffichecks.checkfunction(3, fn)
	
	local callbacks = Callbacks[callbackId]
	if callbacks then
		for i=#callbacks,1,-1 do
			if callbacks[i].Function == fn then
				table.remove(callbacks, i)
			end
		end
		
		-- No more functions left, disable this callback
		if not next(callbacks) then
			if type(callbackId) == "number" then
				Isaac.SetBuiltInCallbackState(callbackId, false)
			end
		end
	end
end)

local ReturnType = {
	string = 1,
	number = 2,
	boolean = 3,
}

local ffiReturns = {
	string = function(ret) 
		local str = lffi.new("char[?]", #ret + 1)
        lffi.copy(str, ret)
		return str
	end,
	number = function(ret)
		return lffi.new("double[1]", ret)
	end, 
	boolean = function(ret)
		local bool = lffi.new("bool[1]", ret)
		return bool
	end
}
function CallbackWrapper(id, param, args, numArgs, retType, ret)
	local argtable = {}
	for i = 1, numArgs do
		table.insert(argtable, args[i])
        local argType = type(args[i])
    end

	local retValue = _RunCallback(id, param, unpack(argtable))
	local typ = type(retValue)
	if retValue ~= nil and ReturnType[typ] ~= nil then
		retType[0] = ReturnType[typ]
		ret[0] = lffi.cast("void*", ffiReturns[typ](retValue)) 
		return
	end
	retType[0] = 0
end

repentogon.RegisterCallback(lffi.cast("LuaCallback", function(id, param, args, numArgs, retType, ret)
	--- The less time spent here the better- callback aren't compiled.
	CallbackWrapper(id, param, args, numArgs, retType, ret)
end))