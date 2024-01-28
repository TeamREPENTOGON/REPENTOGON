ffi.cdef[[
void L_Console_Print(const char*, unsigned int);
void L_Console_PrintError(const char*);
]]

Console = {}

--TODO: add typechecking. These are DIRECT C++ CALLS so we need to be careful.
function Console.PrintError(err)
	libzhl.L_Console_PrintError(err)
end

function Console.PrintWarning(warn)
	libzhl.L_Console_Print(warn, 0xFFFCCA03)
end

