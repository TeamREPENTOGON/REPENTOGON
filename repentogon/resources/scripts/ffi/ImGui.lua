ffi.cdef [[
    bool L_ImGui_AddElement(const char*, const char*, int, const char*);
    void L_ImGui_RemoveElement(const char*);
    int L_ImGui_LinkWindowToElement(const char*, const char*);
    bool L_ImGui_CreateMenu(const char*, const char*);
    bool L_ImGui_CreateWindow(const char*, const char*, const char*);
    bool L_ImGui_AddCallback(const char*, int, int);
    bool L_ImGui_RemoveCallback(const char*, int);
    bool L_ImGui_UpdateText(const char*, const char*);
    bool L_ImGui_ElementExists(const char*);

    int L_ImGui_GetValueKind(const char*);
    void L_ImGui_SetValueString(const char*, const char*);
    void L_ImGui_SetValueBoolean(const char*, bool);
    void L_ImGui_SetValueInteger(const char*, int);
    void L_ImGui_SetValueFloat(const char*, float);
    void L_ImGui_SetLabel(const char*, const char*);
    bool L_ImGui_SetHintText(const char*, const char*);
    bool L_ImGui_SetMinMax(const char*, bool, float);
    bool L_ImGui_IsPlot(const char*);
    void L_ImGui_SetListStrings(const char*, const char**, int);
    void L_ImGui_SetListNumbers(const char*, const float*, int);
    bool L_ImGui_SetColorValues(const char*, const float*, int);

    bool L_ImGui_AddButton(const char*, const char*, const char*, int, bool);
    bool L_ImGui_AddText(const char*, const char*, bool, const char*);
    bool L_ImGui_AddInputInteger(const char*, const char*, const char*, int, int, int, int);
    bool L_ImGui_AddInputFloat(const char*, const char*, const char*, int, float, float, float);
    bool L_ImGui_AddDragInteger(const char*, const char*, const char*, int, int, float, int, int, const char*);
    bool L_ImGui_AddDragFloat(const char*, const char*, const char*, int, float, float, float, float, const char*);
    bool L_ImGui_AddSliderInteger(const char*, const char*, const char*, int, int, int, int, const char*);
    bool L_ImGui_AddSliderFloat(const char*, const char*, const char*, int, float, float, float, const char*);
    bool L_ImGui_AddInputColor(const char*, const char*, const char*, int, float, float, float, bool, float);
    bool L_ImGui_AddCheckbox(const char*, const char*, const char*, int, bool);
    bool L_ImGui_AddRadioButtons(const char*, const char*, int, const char**, int, int, bool);
    bool L_ImGui_AddTabBar(const char*, const char*);
    int L_ImGui_AddTab(const char*, const char*, const char*);
    bool L_ImGui_AddCombobox(const char*, const char*, const char*, int, const char**, int, int, bool);
    bool L_ImGui_AddInputText(const char*, const char*, const char*, int, const char*, const char*);
    bool L_ImGui_AddInputTextMultiline(const char*, const char*, const char*, int, const char*, float);
    bool L_ImGui_AddInputController(const char*, const char*, const char*, int, int);
    bool L_ImGui_AddInputKeyboard(const char*, const char*, const char*, int, int);
    bool L_ImGui_AddPlotLines(const char*, const char*, const char*, const float*, int, const char*, float, float, float);
    bool L_ImGui_AddPlotHistogram(const char*, const char*, const char*, const float*, int, const char*, float, float, float);
    bool L_ImGui_AddProgressBar(const char*, const char*, const char*, float, const char*);

    bool L_ImGui_SetTooltip(const char*, const char*);
    bool L_ImGui_SetHelpmarker(const char*, const char*);
    bool L_ImGui_GetVisible(const char*);
    bool L_ImGui_SetVisible(const char*, bool);
    bool L_ImGui_SetColor(const char*, int, float, float, float, float);
    bool L_ImGui_RemoveColor(const char*, int);
    bool L_ImGui_SetTextColor(const char*, float, float, float, float);
    bool L_ImGui_SetSize(const char*, float, float);

    int L_ImGui_GetWindowPinned(const char*);
    bool L_ImGui_SetWindowPinned(const char*, bool);
    bool L_ImGui_GetWindowFlags(const char*, int*);
    bool L_ImGui_SetWindowFlags(const char*, int);
    bool L_ImGui_GetWindowChildFlags(const char*, int*);
    bool L_ImGui_SetWindowChildFlags(const char*, int);
    bool L_ImGui_SetWindowPosition(const char*, float, float);

    void L_ImGui_GetMousePosition(struct Vector*);
    void L_ImGui_GetGameWindowRect(struct Vector*, struct Vector*);
    void L_ImGui_PushNotification(const char*, int, int);
    void L_ImGui_Show();
    void L_ImGui_Hide();
    bool L_ImGui_IsVisible();
    void L_ImGui_Reset();
]]

local repentogon = ffidll
local ffi = ffi

local cfuncs = {
    Ref = __Lua_ImGui_Ref,
}

local FLT_MIN = 1.17549435e-38
local FLT_MAX = 3.40282347e+38
local INT_MAX_DEFAULT = -2147483648
local INT_FORMAT_DEFAULT = "%d%"
local FLOAT_FORMAT_DEFAULT = "%.3f"

local DATA_LABEL = 0
local DATA_VALUE = 1
local DATA_LIST_VALUES = 2
local DATA_MIN = 3
local DATA_MAX = 4
local DATA_HINT_TEXT = 5
local DATA_COLOR_VALUES = 6

local VALUE_STRING = 1
local VALUE_BOOLEAN = 2
local VALUE_INTEGER = 3
local VALUE_FLOAT = 4

local function CheckString(idx, value)
    local t = type(value)
    if t == "string" then
        return value
    elseif t == "number" then
        return tostring(value)
    end
    ffichecks.argerror(idx, "string expected, got " .. t, 3)
end

local function OptString(idx, value, default)
    if value == nil then
        return default
    end
    return CheckString(idx, value)
end

local function OptNumber(idx, value, default)
    if value == nil then
        return default
    end
    if type(value) ~= "number" then
        ffichecks.argerror(idx, "number expected, got " .. type(value), 3)
    end
    return value
end

local function OptCallback(idx, callback)
    if callback == nil then
        return 0
    end
    if type(callback) ~= "function" then
        ffichecks.argerror(idx, "function expected, got " .. type(callback), 3)
    end
    return cfuncs.Ref(callback)
end

local function CheckBoolean(idx, value)
    if value == nil then
        ffichecks.argerror(idx, "boolean expected, got nil", 3)
    end
    return not not value
end

local function CheckTable(idx, value)
    if type(value) ~= "table" then
        ffichecks.argerror(idx, "table expected, got " .. type(value), 3)
    end
end

local function StringArray(idx, tbl)
    CheckTable(idx, tbl)
    local values = {}
    for i = 1, #tbl do
        local value = tbl[i]
        if value == nil then
            break
        end
        if type(value) ~= "string" and type(value) ~= "number" then
            ffichecks.argerror(idx, "table of strings expected", 3)
        end
        values[i] = tostring(value)
    end
    return ffi.new("const char*[?]", #values + 1, values), #values
end

local function NumberArray(idx, tbl)
    CheckTable(idx, tbl)
    local values = {}
    for i = 1, #tbl do
        local value = tbl[i]
        if value == nil then
            break
        end
        if type(value) ~= "number" then
            ffichecks.argerror(idx, "table of numbers expected", 3)
        end
        values[i] = value
    end
    return ffi.new("float[?]", #values + 1, values), #values
end

local function ParentNotFound(parentId)
    ffichecks.argerror(1, string.format("Parent Element with id '%s' doesn't exist.", parentId), 3)
end

local function ElementNotFound(id)
    ffichecks.argerror(1, string.format("Element with id '%s' not found", id), 3)
end

local function WindowNotFound(id)
    ffichecks.argerror(1, string.format("Window Element with id '%s' not found", id), 3)
end

local function NoElementWithId(id)
    ffichecks.argerror(1, string.format("No element with id '%s' found.", id), 3)
end

local function RemoveElement(id)
    repentogon.L_ImGui_RemoveElement(CheckString(1, id))
end

local function SetSize(id, x, y)
    id = CheckString(1, id)
    ffichecks.checknumber(2, x)
    ffichecks.checknumber(3, y)
    if not repentogon.L_ImGui_SetSize(id, x, y) then
        ElementNotFound(id)
    end
end

local function Show()
    repentogon.L_ImGui_Show()
end

ImGui = {
    AddButton = function(parentId, id, text, callback, isSmall)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddButton(parentId, id, text, ref, ffichecks.optboolean(isSmall, false)) then
            ParentNotFound(parentId)
        end
    end,
    AddCallback = function(parentId, type, callback)
        parentId = CheckString(1, parentId)
        ffichecks.checkinteger(2, type)
        ffichecks.checkfunction(3, callback)
        if not repentogon.L_ImGui_AddCallback(parentId, type, cfuncs.Ref(callback)) then
            ffichecks.argerror(1, string.format("No element '%s' found.", parentId))
        end
    end,
    AddCheckbox = function(parentId, id, text, callback, checked)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddCheckbox(parentId, id, text, ref, ffichecks.optboolean(checked, false)) then
            ParentNotFound(parentId)
        end
    end,
    AddCombobox = function(parentId, id, text, callback, values, index, isSlider)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        local array, count = StringArray(5, values)
        if not repentogon.L_ImGui_AddCombobox(parentId, id, text, ref, array, count, OptNumber(6, index, 0), ffichecks.optboolean(isSlider, false)) then
            ParentNotFound(parentId)
        end
    end,
    AddDragFloat = function(parentId, id, text, callback, defaultVal, speed, minVal, maxVal, formatting)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddDragFloat(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, speed, 1),
            OptNumber(7, minVal, FLT_MIN), OptNumber(8, maxVal, FLT_MAX), OptString(9, formatting, FLOAT_FORMAT_DEFAULT)) then
            ParentNotFound(parentId)
        end
    end,
    AddDragInteger = function(parentId, id, text, callback, defaultVal, speed, minVal, maxVal, formatting)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddDragInteger(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, speed, 1),
            OptNumber(7, minVal, 0), OptNumber(8, maxVal, INT_MAX_DEFAULT), OptString(9, formatting, INT_FORMAT_DEFAULT)) then
            ParentNotFound(parentId)
        end
    end,
    AddElement = function(parentId, id, type, text)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        ffichecks.checkinteger(3, type)
        if not repentogon.L_ImGui_AddElement(parentId, id, type, OptString(4, text, "")) then
            ParentNotFound(parentId)
        end
    end,
    AddInputColor = function(parentId, id, text, callback, r, g, b, a)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputColor(parentId, id, text, ref, OptNumber(5, r, 0), OptNumber(6, g, 0), OptNumber(7, b, 0), a ~= nil, OptNumber(8, a, 1)) then
            ParentNotFound(parentId)
        end
    end,
    AddInputController = function(parentId, id, text, callback, defaultVal)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputController(parentId, id, text, ref, OptNumber(5, defaultVal, 0)) then
            ParentNotFound(parentId)
        end
    end,
    AddInputFloat = function(parentId, id, text, callback, defaultVal, step, stepFast)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputFloat(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, step, 1), OptNumber(7, stepFast, 100)) then
            ParentNotFound(parentId)
        end
    end,
    AddInputInteger = function(parentId, id, text, callback, defaultVal, step, stepFast)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputInteger(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, step, 1), OptNumber(7, stepFast, 100)) then
            ParentNotFound(parentId)
        end
    end,
    AddInputKeyboard = function(parentId, id, text, callback, defaultVal)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputKeyboard(parentId, id, text, ref, OptNumber(5, defaultVal, 0)) then
            ParentNotFound(parentId)
        end
    end,
    AddInputText = function(parentId, id, text, callback, inputText, hintText)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputText(parentId, id, text, ref, OptString(5, inputText, ""), OptString(6, hintText, "")) then
            ParentNotFound(parentId)
        end
    end,
    AddInputTextMultiline = function(parentId, id, text, callback, inputText, lineCount)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddInputTextMultiline(parentId, id, text, ref, OptString(5, inputText, ""), OptNumber(6, lineCount, 6)) then
            ParentNotFound(parentId)
        end
    end,
    AddPlotHistogram = function(parentId, id, text, values, hintText, minVal, maxVal, height)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local array, count = NumberArray(4, values)
        if not repentogon.L_ImGui_AddPlotHistogram(parentId, id, text, array, count, OptString(5, hintText, ""),
            OptNumber(6, minVal, FLT_MIN), OptNumber(7, maxVal, FLT_MAX), OptNumber(8, height, 40)) then
            ParentNotFound(parentId)
        end
    end,
    AddPlotLines = function(parentId, id, text, values, hintText, minVal, maxVal, height)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local array, count = NumberArray(4, values)
        if not repentogon.L_ImGui_AddPlotLines(parentId, id, text, array, count, OptString(5, hintText, ""),
            OptNumber(6, minVal, FLT_MIN), OptNumber(7, maxVal, FLT_MAX), OptNumber(8, height, 40)) then
            ParentNotFound(parentId)
        end
    end,
    AddProgressBar = function(parentId, id, text, value, hintText)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        if not repentogon.L_ImGui_AddProgressBar(parentId, id, text, OptNumber(4, value, 0), OptString(5, hintText, "__DEFAULT__")) then
            ParentNotFound(parentId)
        end
    end,
    AddRadioButtons = function(parentId, id, callback, values, index, sameLine)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        local ref = OptCallback(3, callback)
        local array, count = StringArray(4, values)
        if not repentogon.L_ImGui_AddRadioButtons(parentId, id, ref, array, count, OptNumber(5, index, 0), ffichecks.optboolean(sameLine, true)) then
            ParentNotFound(parentId)
        end
    end,
    AddSliderFloat = function(parentId, id, text, callback, defaultVal, minVal, maxVal, formatting)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddSliderFloat(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, minVal, FLT_MIN),
            OptNumber(7, maxVal, FLT_MAX), OptString(8, formatting, FLOAT_FORMAT_DEFAULT)) then
            ParentNotFound(parentId)
        end
    end,
    AddSliderInteger = function(parentId, id, text, callback, defaultVal, minVal, maxVal, formatting)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = OptString(3, text, "")
        local ref = OptCallback(4, callback)
        if not repentogon.L_ImGui_AddSliderInteger(parentId, id, text, ref, OptNumber(5, defaultVal, 0), OptNumber(6, minVal, 0),
            OptNumber(7, maxVal, INT_MAX_DEFAULT), OptString(8, formatting, INT_FORMAT_DEFAULT)) then
            ParentNotFound(parentId)
        end
    end,
    AddTab = function(parentId, id, text)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        text = CheckString(3, text)
        local result = repentogon.L_ImGui_AddTab(parentId, id, text)
        if result == 1 then
            ParentNotFound(parentId)
        elseif result == 2 then
            ffichecks.argerror(1, "The given parent element is not of type 'TabBar'!")
        end
    end,
    AddTabBar = function(parentId, id)
        parentId = CheckString(1, parentId)
        id = CheckString(2, id)
        if not repentogon.L_ImGui_AddTabBar(parentId, id) then
            ParentNotFound(parentId)
        end
    end,
    AddText = function(parentId, text, isWrapped, id)
        parentId = CheckString(1, parentId)
        text = OptString(2, text, "")
        local wrapped = ffichecks.optboolean(isWrapped, false)
        if not repentogon.L_ImGui_AddText(parentId, text, wrapped, OptString(4, id, "")) then
            ParentNotFound(parentId)
        end
    end,
    CreateMenu = function(id, text)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_CreateMenu(id, CheckString(2, text)) then
            error(string.format("Error while adding new Menu '%s'", id), 2)
        end
    end,
    CreateWindow = function(id, title, parentId)
        id = CheckString(1, id)
        title = CheckString(2, title)
        if not repentogon.L_ImGui_CreateWindow(id, title, OptString(3, parentId, nil)) then
            error(string.format("Error while adding new Window '%s'", id), 2)
        end
    end,
    ElementExists = function(id)
        return repentogon.L_ImGui_ElementExists(CheckString(1, id))
    end,
    GetGameWindowRect = function()
        local position = Vector(0, 0)
        local size = Vector(0, 0)
        repentogon.L_ImGui_GetGameWindowRect(position, size)
        return position, size
    end,
    GetMousePosition = function()
        local position = Vector(0, 0)
        repentogon.L_ImGui_GetMousePosition(position)
        return position
    end,
    GetVisible = function(id)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_ElementExists(id) then
            ElementNotFound(id)
        end
        return repentogon.L_ImGui_GetVisible(id)
    end,
    GetWindowChildFlags = function(id)
        id = CheckString(1, id)
        local flags = ffi.new("int[1]")
        if not repentogon.L_ImGui_GetWindowChildFlags(id, flags) then
            WindowNotFound(id)
        end
        return flags[0]
    end,
    GetWindowFlags = function(id)
        id = CheckString(1, id)
        local flags = ffi.new("int[1]")
        if not repentogon.L_ImGui_GetWindowFlags(id, flags) then
            WindowNotFound(id)
        end
        return flags[0]
    end,
    GetWindowPinned = function(id)
        id = CheckString(1, id)
        local pinned = repentogon.L_ImGui_GetWindowPinned(id)
        if pinned < 0 then
            WindowNotFound(id)
        end
        return pinned == 1
    end,
    Hide = function()
        repentogon.L_ImGui_Hide()
    end,
    IsVisible = function()
        return repentogon.L_ImGui_IsVisible()
    end,
    LinkWindowToElement = function(windowId, elementId)
        windowId = CheckString(1, windowId)
        elementId = CheckString(2, elementId)
        local result = repentogon.L_ImGui_LinkWindowToElement(windowId, elementId)
        if result == 1 then
            ffichecks.argerror(1, string.format("No window with id '%s' exists", windowId))
        elseif result == 2 then
            ffichecks.argerror(2, string.format("Element with id '%s' not found", elementId))
        end
    end,
    PushNotification = function(text, severity, lifetime)
        text = CheckString(1, text)
        severity = ffichecks.optnumber(severity, 0)
        if severity < 0 or severity > 3 then
            ffichecks.argerror(2, "Severity needs to be a value between 0 and 3")
        end
        repentogon.L_ImGui_PushNotification(text, severity, ffichecks.optnumber(lifetime, 5000))
    end,
    RemoveCallback = function(parentId, type)
        parentId = CheckString(1, parentId)
        ffichecks.checkinteger(2, type)
        if not repentogon.L_ImGui_RemoveCallback(parentId, type) then
            ffichecks.argerror(1, string.format("No element '%s' found.", parentId))
        end
    end,
    RemoveColor = function(id, type)
        id = CheckString(1, id)
        ffichecks.checkinteger(2, type)
        if not repentogon.L_ImGui_RemoveColor(id, type) then
            ElementNotFound(id)
        end
    end,
    RemoveElement = RemoveElement,
    RemoveMenu = RemoveElement, -- deprecated
    RemoveWindow = RemoveElement, -- deprecated
    Reset = function()
        repentogon.L_ImGui_Reset()
    end,
    SetColor = function(id, type, r, g, b, a)
        id = CheckString(1, id)
        ffichecks.checkinteger(2, type)
        ffichecks.checknumber(3, r)
        ffichecks.checknumber(4, g)
        ffichecks.checknumber(5, b)
        if not repentogon.L_ImGui_SetColor(id, type, r, g, b, ffichecks.optnumber(a, 1)) then
            ElementNotFound(id)
        end
    end,
    SetHelpmarker = function(id, text)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_SetHelpmarker(id, CheckString(2, text)) then
            NoElementWithId(id)
        end
    end,
    SetSize = SetSize,
    SetTextColor = function(id, r, g, b, a)
        id = CheckString(1, id)
        ffichecks.checknumber(2, r)
        ffichecks.checknumber(3, g)
        ffichecks.checknumber(4, b)
        if not repentogon.L_ImGui_SetTextColor(id, r, g, b, ffichecks.optnumber(a, 1)) then
            ElementNotFound(id)
        end
    end,
    SetTooltip = function(id, text)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_SetTooltip(id, CheckString(2, text)) then
            NoElementWithId(id)
        end
    end,
    SetVisible = function(id, visible)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_SetVisible(id, CheckBoolean(2, visible)) then
            ElementNotFound(id)
        end
    end,
    SetWindowChildFlags = function(id, flags)
        id = CheckString(1, id)
        ffichecks.checkinteger(2, flags)
        if not repentogon.L_ImGui_SetWindowChildFlags(id, flags) then
            WindowNotFound(id)
        end
    end,
    SetWindowFlags = function(id, flags)
        id = CheckString(1, id)
        ffichecks.checkinteger(2, flags)
        if not repentogon.L_ImGui_SetWindowFlags(id, flags) then
            WindowNotFound(id)
        end
    end,
    SetWindowPinned = function(id, pinned)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_SetWindowPinned(id, CheckBoolean(2, pinned)) then
            WindowNotFound(id)
        end
    end,
    SetWindowPosition = function(id, x, y)
        id = CheckString(1, id)
        ffichecks.checknumber(2, x)
        ffichecks.checknumber(3, y)
        if not repentogon.L_ImGui_SetWindowPosition(id, x, y) then
            WindowNotFound(id)
        end
    end,
    SetWindowSize = SetSize, -- deprecated
    Show = Show,
    UpdateData = function(id, dataType, value)
        id = CheckString(1, id)
        ffichecks.checkinteger(2, dataType)
        if not repentogon.L_ImGui_ElementExists(id) then
            NoElementWithId(id)
        end

        local supported = true
        if dataType == DATA_LABEL then
            repentogon.L_ImGui_SetLabel(id, CheckString(3, value))
        elseif dataType == DATA_VALUE then
            local kind = repentogon.L_ImGui_GetValueKind(id)
            if kind == VALUE_STRING then
                repentogon.L_ImGui_SetValueString(id, CheckString(3, value))
            elseif kind == VALUE_BOOLEAN then
                repentogon.L_ImGui_SetValueBoolean(id, CheckBoolean(3, value))
            elseif kind == VALUE_INTEGER then
                ffichecks.checkinteger(3, value)
                repentogon.L_ImGui_SetValueInteger(id, value)
            elseif kind == VALUE_FLOAT then
                ffichecks.checknumber(3, value)
                repentogon.L_ImGui_SetValueFloat(id, value)
            else
                supported = false
            end
        elseif dataType == DATA_LIST_VALUES then
            if repentogon.L_ImGui_IsPlot(id) then
                local array, count = NumberArray(3, value)
                repentogon.L_ImGui_SetListNumbers(id, array, count)
            else
                local array, count = StringArray(3, value)
                repentogon.L_ImGui_SetListStrings(id, array, count)
            end
        elseif dataType == DATA_MIN or dataType == DATA_MAX then
            ffichecks.checknumber(3, value)
            supported = repentogon.L_ImGui_SetMinMax(id, dataType == DATA_MAX, value)
        elseif dataType == DATA_HINT_TEXT then
            supported = repentogon.L_ImGui_SetHintText(id, CheckString(3, value))
        elseif dataType == DATA_COLOR_VALUES then
            local array, count = NumberArray(3, value)
            supported = repentogon.L_ImGui_SetColorValues(id, array, count)
        else
            supported = false
        end

        if not supported then
            error("The given element does not use the provided data type.", 2)
        end
    end,
    UpdateText = function(id, text)
        id = CheckString(1, id)
        if not repentogon.L_ImGui_UpdateText(id, CheckString(2, text)) then
            NoElementWithId(id)
        end
    end,
}

rawset(Isaac, "OpenConsole", Show)

__Lua_ImGui_Ref = nil
