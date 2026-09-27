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

local repentogon = ffidll
local ffi = ffi

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
        return repentogon.L_Input_GetActionValue(action, controllerId)
    end,

    GetButtonValue = function(button, controllerId)
        return repentogon.L_Input_GetButtonValue(button, controllerId)
    end,

    GetDeviceNameByIdx = function(controllerId)
        local deviceName = repentogon.L_Input_GetDeviceNameByIdx(controllerId)

        if not deviceName then
            return
        end
        
        return ffi.string(deviceName)
    end,

    GetMousePosition = function(gameCoords)
        local position = Vector()
        repentogon.L_Input_GetMousePosition(gameCoords, position)
        return position
    end,

    GetMouseWheel = function()
        local wheel = Vector()
        repentogon.L_Input_GetMouseWheel(wheel)
        return wheel
    end,

    IsActionPressed = function(action, controllerId)
        return repentogon.L_Input_IsActionPressed(action, controllerId)
    end,

    IsActionTriggered = function(action, controllerId)
        return repentogon.L_Input_IsActionTriggered(action, controllerId)
    end,

    IsButtonPressed = function(button, controllerId)
        return repentogon.L_Input_IsButtonPressed(button, controllerId)
    end,

    IsButtonTriggered = function(button, controllerId)
        return repentogon.L_Input_IsButtonTriggered(button, controllerId)
    end,

    IsMouseBtnPressed = function(button)
        return repentogon.L_Input_IsMouseBtnPressed(button)
    end,
}, InputGlobalMT)
