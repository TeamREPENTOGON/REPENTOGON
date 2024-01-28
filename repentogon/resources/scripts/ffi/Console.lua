ffi.cdef[[
void L_Console_Print(const char*, unsigned int);
void L_Console_PrintError(const char*);
]]

local repentogon = ffidll

Console = {}

function Console.PrintError(err)
	ffichecks.checkstring(1, err)
	repentogon.L_Console_PrintError(err)
end

function Console.PrintWarning(warn)
	ffichecks.checkstring(1, warn)
	repentogon.L_Console_Print(warn, 0xFFFCCA03)
end

