ffi.cdef[[
void L_KAGE_LogMessage(const char*);
void L_Console_Print(const char*, unsigned int);
unsigned int L_Isaac_GetTime();
void L_Isaac_SetBuiltInCallbackState(int, bool);
]]

local repentogon = ffidll

Isaac = {}

Isaac.DebugString = function(str) 
	ffichecks.checkstring(1, str)
	repentogon.L_KAGE_LogMessage("Lua Debug: " .. str .. "\n")
end

Isaac.ConsoleOutput = function(str) 
	ffichecks.checkstring(1, str)
	repentogon.L_Console_Print(str, 0xFFFFFFFF);
end

Isaac.GetTime = function() return repentogon.L_Isaac_GetTime() end

--[[ In the old API this gave C++ mod refs, presumably so it could run callbacks.
     If that's all it's used for, there is no reason for it to exist anymore.
     Technically, if that's the case, it's been obsolete since those started being handled Lua side- 
     and even more obsolete now.]]
Isaac.RegisterMod = function(mod, modname, apiversion) end

Isaac.SetBuiltInCallbackState = function(callback, state)
	ffichecks.checknumber(1, callback)
	ffichecks.checkboolean(2, state)
	repentogon.L_Isaac_SetBuiltInCallbackState(callback, state)
end