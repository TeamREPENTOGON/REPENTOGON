ffi.cdef[[
typedef struct {
	char** strings;
	int length;
} FFI_StringTable;
	
typedef struct {
	const char* name;
	const char* desc;
	const char* helpText;
	bool showOnMenu;
	int autocompleteType;
} L_RegisterCommandHolder;

void L_Console_Print(const char*, unsigned int);
void L_Console_PrintError(const char*);
FFI_StringTable L_Console_GetHistory();
FFI_StringTable L_Console_GetCommandHistory();
void L_Console_PopHistory(int);
void L_Console_RegisterCommand(L_RegisterCommandHolder*);
void L_Console_RegisterMacro(const char*, FFI_StringTable);

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

function Console.GetCommandHistory()
	local strings = repentogon.L_Console_GetCommandHistory()
	
	local historyTable = {}
	for i = 0, strings.length - 1 do
		table.insert(historyTable, lffi.string(strings.strings[i]))
	end
	
	repentogon.FreeStringTable(strings)
	
	return historyTable
end

function Console.PopHistory(amount)
	if amount ~= nil then 
		ffichecks.checknumber(1, amount) 
		if amount < 1 then amount = 1 end
	end

	repentogon.L_Console_PopHistory(amount or 1)
end

function Console.RegisterCommand(name, desc, helpText, showOnMenu, autocompleteType)
	ffichecks.checkstring(1, name)
	ffichecks.checkstring(2, desc)
	ffichecks.checkstring(3, helpText)
	if showOnMenu ~= nil then ffichecks.checkboolean(4, showOnMenu) end
	if autocompleteType ~= nil then ffichecks.checknumber(5, autocompleteType) end

	-- Per FFI docs, these cdata will be automatically garbage collected.
	-- Any cdata created on the C side will need to be freed by hand, however.
	local registerCommandHolder = lffi.new("L_RegisterCommandHolder", {name, desc, helpText, showOnMenu or false, autocompleteType or 0})

	-- Next, create the pointer, then pass it to the function.
	local registerCommandHolderPtr = lffi.new("L_RegisterCommandHolder*", registerCommandHolder)
	repentogon.L_Console_RegisterCommand(registerCommandHolderPtr);
end

function Console.RegisterMacro(name, macro)
	-- TODO typechecking for table
	ffichecks.checkstring(1, name)

	for i, entry in ipairs(macro) do
		local t = type(entry)
		if t ~= "string" then
			error(string.format("bad table value #%s in argument #2 to 'Console.RegisterMacro' (string expected expected, got %s)", i, t))
		end
	end

	-- Initialize array with length of macro + 1 to account for null termination
	local cstr = lffi.new("char*[?]", #macro)
	
	for i, entry in ipairs(macro) do
		cstr[i - 1] = lffi.new("char[?]", #entry + 1)
        lffi.copy(cstr[i - 1], entry)
	end

	local sta = lffi.new("FFI_StringTable", {cstr, #macro})
	
	repentogon.L_Console_RegisterMacro(name, sta)
end