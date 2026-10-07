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
		SaveData = function(self, data)
			Isaac.SaveModData(self, data)
		end,
		LoadData = function(self)
			return Isaac.LoadModData(self)
		end,
		HasData = function(self)
			return Isaac.HasModData(self)
		end,
		RemoveData = function(self)
			Isaac.RemoveModData(self)
		end
	}
	Isaac.RegisterMod(mod, modname, apiversion)
	return mod
end

function StartDebug()
	local ok, m = pcall(require, 'mobdebug') 
	if ok and m then
		m.start()
	else
		Isaac.DebugString("Failed to start debugging.")
		-- m is now the error 
		-- Isaac.DebugString(m)
	end
end

local debug_getinfo = debug.getinfo

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

------------------------------------------------------------
-- Callbacks

local Callbacks = {}

local defaultCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

function _RunCallback(callbackId, param, ...)
	local callbacks = Callbacks[callbackId]
	if callbacks then
		local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams
		
		for _,v in ipairs(callbacks) do
			if matchFunc(param, v.Param) then
				local result = v.Function(v.Mod, ...)
				if result ~= nil  then
					return result
				end
			end
		end
	end
end

function _UnloadMod(mod)
	Isaac.RunCallback(ModCallbacks.MC_PRE_MOD_UNLOAD, mod)
	
	for callbackId,callbacks in pairs(Callbacks) do
		for i=#callbacks,1,-1 do
			if callbacks[i].Mod == mod then
				table.remove(callbacks, i)
			end
		end
		
		if #callbacks == 0 then
			if type(callbackId) == "number" then
				-- No more functions left, disable this callback
				Isaac.SetBuiltInCallbackState(callbackId, false)
			end
		end
	end
end

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
	Isaac.AddPriorityCallback(mod, callbackId, CallbackPriority.DEFAULT, fn, param)
end)

rawset(Isaac, "RemoveCallback", function(mod, callbackId, fn)
	checkfunction(3, fn)
	
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

rawset(Isaac, "GetCallbacks", function(callbackId, createIfMissing)
	if createIfMissing and not Callbacks[callbackId] then
		Callbacks[callbackId] = setmetatable({}, defaultCallbackMeta)
	end
	
	return Callbacks[callbackId] or {}
end)

local RunCallback = _RunCallback

rawset(Isaac, "RunCallbackWithParam", RunCallback)
rawset(Isaac, "RunCallback", function(callbackID, ...) return RunCallback(callbackID, nil, ...) end)

------------------------------------------------------------
-- Constants

REPENTANCE = true
REPENTANCE_PLUS = true

-- I allow Mod Profiler to live. For now.
_ENV = _G

------------------------------------------------------------
-- Compatibility wrappers begin here

local META, META0
local function BeginClass(T)
	META = {}
	if type(T) == "function" then
		META0 = getmetatable(T())
	else
		META0 = getmetatable(T).__class
	end
end

local function EndClass()
	local oldIndex = META0.__index
	local newMeta = META
	
	rawset(META0, "__index", function(self, k)
		return newMeta[k] or oldIndex(self, k)
	end)
end

local function tobitset128(n)
	if type(n) == "number" then
		return BitSet128(n, 0)
	else
		return n
	end
end

-- Isaac -----------------------------------------------

-- EntityPlayer Isaac.GetPlayer(int ID = 0)
-- table Isaac.QueryRadius(Vector Position, float Radius, int Partitions = 0xFFFFFFFF)
-- table Isaac.FindByType(EntityType Type, int Variant = -1, int SubType = -1, bool Cache = false, bool IgnoreFriendly = false)
-- int Isaac.CountEntities(Entity Spawner, EntityType Type = EntityType.ENTITY_NULL, int Variant = -1, int SubType = -1)

-- int Isaac.GetPlayerTypeByName(string Name, boolean IsBSkin = false)

---------------------------------------------------------

Game_0 = nil

if not _LUADEBUG then
	debug = nil
	arg = nil
	dofile = nil
	loadfile = nil
end
