ffi.cdef[[
struct Input {};
typedef struct Input* InputPtr;

void L_Input_GetActionButtons(int, int, int*, int*);
float L_Input_GetActionValue(int, int);
int L_Input_GetButtonFrame(int, int);
float L_Input_GetButtonValue(int, int);
char* L_Input_GetDeviceNameByIdx(int);
void L_Input_GetMousePosition(bool, struct Vector*);
void L_Input_GetMouseWheel(struct Vector*);
bool L_Input_IsActionPressed(int, int);
bool L_Input_IsActionTriggered(int, int);
bool L_Input_IsButtonPressed(int, int);
bool L_Input_IsButtonTriggered(int, int);
bool L_Input_IsMouseBtnPressed(uint32_t);
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
    GetActionButtons = function(action, controllerId)
        local buf = ffi.new("int[2]")
        repentogon.L_Input_GetActionButtons(action, controllerId, buf, buf + 1)
        return buf[0], buf[1]
    end,
    GetActionValue = function(action, controllerId)
        local result = repentogon.L_Input_GetActionValue(action, controllerId) return result
    end,
    GetButtonFrame = function(button, controllerId)
            local result = repentogon.L_Input_GetButtonFrame(button, controllerId) return result
    end,
    GetButtonValue = function(button, controllerId)
        local result = repentogon.L_Input_GetButtonValue(button, controllerId) return result
    end,
    GetDeviceNameByIdx = function(controllerId)
        local deviceName = repentogon.L_Input_GetDeviceNameByIdx(controllerId)

        if not deviceName then
            return
        end

        local result = ffi.string(deviceName) return result
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
        local result = repentogon.L_Input_IsActionPressed(action, controllerId) return result
    end,
    IsActionTriggered = function(action, controllerId)
        local result = repentogon.L_Input_IsActionTriggered(action, controllerId) return result
    end,
    IsButtonPressed = function(button, controllerId)
        local result = repentogon.L_Input_IsButtonPressed(button, controllerId) return result
    end,
    IsButtonTriggered = function(button, controllerId)
        local result = repentogon.L_Input_IsButtonTriggered(button, controllerId) return result
    end,
    IsMouseBtnPressed = function(button)
        local result = repentogon.L_Input_IsMouseBtnPressed(button) return result
    end,
}, InputGlobalMT)
