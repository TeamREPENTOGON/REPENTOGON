ffi.cdef[[
void L_KAGE_LogMessage(const char*);
void L_Console_Print(const char*, unsigned int);
unsigned int L_Isaac_GetTime();
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