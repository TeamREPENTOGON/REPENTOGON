ffi.cdef[[
void L_KAGE_LogMessage(const char*);
void L_Console_Print(const char*, unsigned int);
]]

Isaac = {}

Isaac.DebugString = function(str) 
	libzhl.L_KAGE_LogMessage("Lua Debug: " .. str .. "\n")
end

Isaac.ConsoleOutput = function(str) 
	libzhl.L_Console_Print(str, 0xFFFFFFFF);
end