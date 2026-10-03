ffi.cdef[[ 
    int L_Console_GetCommandHistorySize();
    const char* L_Console_GetCommandHistoryEntry(int);
    int L_Console_GetHistorySize();
    const char* L_Console_GetHistoryEntry(int);
    void L_Console_PopHistory(int);
    void L_Console_PrintError(const char*);
    void L_Console_PrintWarning(const char*);
    void L_Console_RegisterCommand(const char*, const char*, const char*, bool, int);
    void L_Console_RegisterMacro(const char*, const char**, int);
]]

local ffi = ffi
local repentogon = ffidll


Console = {
    GetCommandHistory = function()
        local result = {}
        local size = repentogon.L_Console_GetCommandHistorySize()
        for i = 1, size do
            result[i] = ffi.string(repentogon.L_Console_GetCommandHistoryEntry(size - i))
        end
        return result
    end,
    GetHistory = function()
        local result = {}
        for i = 0, repentogon.L_Console_GetHistorySize() - 1 do
            result[i + 1] = ffi.string(repentogon.L_Console_GetHistoryEntry(i))
        end
        return result
    end,
    PopHistory = function(amount)
        if amount == nil then
            amount = 1
        else
            ffichecks.checkinteger(1, amount)
        end
        repentogon.L_Console_PopHistory(amount)
    end,
    PrintError = function(err)
        ffichecks.checkstring(1, err)
        repentogon.L_Console_PrintError(err)
    end,
    PrintWarning = function(text)
        ffichecks.checkstring(1, text)
        repentogon.L_Console_PrintWarning(text)
    end,
    RegisterCommand = function(name, desc, helpText, showOnMenu, autocompleteType)
        ffichecks.checkstring(1, name)
        ffichecks.checkstring(2, desc)
        ffichecks.checkstring(3, helpText)
        showOnMenu = ffichecks.optboolean(showOnMenu, false)
        autocompleteType = ffichecks.optnumber(autocompleteType, 0)
        repentogon.L_Console_RegisterCommand(name, desc, helpText, showOnMenu, autocompleteType)
    end,
    RegisterMacro = function(name, commands)
        ffichecks.checkstring(1, name)
        if type(commands) ~= "table" then
            ffichecks.argerror(2, "Expected a table of strings, got " .. type(commands))
        end

        local strings = {}
        for i = 1, #commands do
            local cmd = commands[i]
            if cmd == nil then break end
            ffichecks.checkstring(2, cmd)
            strings[i] = cmd
        end

        local count = #strings
        local array = ffi.new("const char*[?]", count)
        for i = 1, count do
            array[i - 1] = strings[i]
        end
        repentogon.L_Console_RegisterMacro(name, array, count)
    end,
}
