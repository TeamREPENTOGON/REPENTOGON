ffi.cdef[[
struct Input {};
typedef struct Input* InputPtr;

float L_Input_GetActionValue(int action, int controllerId);
float L_Input_GetButtonValue(int button, int controllerId);
char* L_Input_GetDeviceNameByIdx(int controllerId);
void L_Input_GetMousePosition(bool gameCoords, struct Vector* out);
void L_Input_GetMouseWheel(struct Vector* out);
bool L_Input_IsActionPressed(int action, int controllerId);
bool L_Input_IsActionTriggered(int action, int controllerId);
bool L_Input_IsButtonPressed(int button, int controllerId);
bool L_Input_IsButtonTriggered(int button, int controllerId);
bool L_Input_IsMouseBtnPressed(uint32_t button);
]]

local L_Input_GetActionValue = ffidll.L_Input_GetActionValue
local L_Input_GetButtonValue = ffidll.L_Input_GetButtonValue
local L_Input_GetDeviceNameByIdx = ffidll.L_Input_GetDeviceNameByIdx
local L_Input_GetMousePosition = ffidll.L_Input_GetMousePosition
local L_Input_GetMouseWheel = ffidll.L_Input_GetMouseWheel
local L_Input_IsActionPressed = ffidll.L_Input_IsActionPressed
local L_Input_IsActionTriggered = ffidll.L_Input_IsActionTriggered
local L_Input_IsButtonPressed = ffidll.L_Input_IsButtonPressed
local L_Input_IsButtonTriggered = ffidll.L_Input_IsButtonTriggered
local L_Input_IsMouseBtnPressed = ffidll.L_Input_IsMouseBtnPressed

local InputMT = {
    __type = "Input",
}

InputMT.__index = InputMT

local InputT = ffi.metatype("struct Input", InputMT)

local InputGlobalMT = {
    __class = InputMT,
}

InputGlobalMT.__index = InputGlobalMT

Input = setmetatable({
    GetActionValue = function(action, controllerId)
        return L_Input_GetActionValue(action, controllerId)
    end,

    GetButtonValue = function(button, controllerId)
        return L_Input_GetButtonValue(button, controllerId)
    end,

    GetDeviceNameByIdx = function(controllerId)
        local deviceName = L_Input_GetDeviceNameByIdx(controllerId)

        if not deviceName then
            return
        end
        
        return ffi.string(deviceName)
    end,

    GetMousePosition = function(gameCoords)
        local position = Vector()
        L_Input_GetMousePosition(gameCoords, position)
        return position
    end,

    GetMouseWheel = function()
        local wheel = Vector()
        L_Input_GetMouseWheel(wheel)
        return wheel
    end,

    IsActionPressed = function(action, controllerId)
        return L_Input_IsActionPressed(action, controllerId)
    end,

    IsActionTriggered = function(action, controllerId)
        return L_Input_IsActionTriggered(action, controllerId)
    end,

    IsButtonPressed = function(button, controllerId)
        return L_Input_IsButtonPressed(button, controllerId)
    end,

    IsButtonTriggered = function(button, controllerId)
        return L_Input_IsButtonTriggered(button, controllerId)
    end,

    IsMouseBtnPressed = function(button)
        return L_Input_IsMouseBtnPressed(button)
    end,
}, InputGlobalMT)
