ffi.cdef[[
typedef struct {
	char** strings;
	int length;
} FFI_StringTable;
	
void L_Console_Print(const char*, unsigned int);
void L_Console_PrintError(const char*);
FFI_StringTable L_Console_GetHistory();
void FreeStringTable(FFI_StringTable);
]]

local repentogon = ffidll
local lffi = ffi

Console = {}

function Console.PrintError(err)
	ffichecks.checkstring(1, err)
	repentogon.L_Console_PrintError(err)
end

function Console.PrintWarning(warn)
	ffichecks.checkstring(1, warn)
	repentogon.L_Console_Print(warn, 0xFFFCCA03)
end

function Console.GetHistory()
	local strings = repentogon.L_Console_GetHistory()
	
	local historyTable = {}
	for i = 0, strings.length - 1 do
		table.insert(historyTable, lffi.string(strings.strings[i]))
	end
	
	repentogon.FreeStringTable(strings)
	
	return historyTable
end