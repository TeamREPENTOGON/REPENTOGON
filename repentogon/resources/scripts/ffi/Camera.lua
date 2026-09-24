ffi.cdef[[
struct Camera {} : 0x94;

bool L_Camera_IsClampEnabled(struct Camera*);
bool L_Camera_IsPosVisible(struct Camera*, struct Vector*);
void L_Camera_SetClampEnabled(struct Camera*, bool);
void L_Camera_SetFocusPosition(struct Camera*, struct Vector*);
void L_Camera_SnapToPosition(struct Camera*, struct Vector);
void L_Camera_Update(struct Camera*, bool);
]]

local repentogon = ffidll
local ffi = ffi

local CameraMT
CameraMT = {
    __type = "Camera",
    IsClampEnabled = function(self)
        return repentogon.L_Camera_IsClampEnabled(self)
    end,
    IsPosVisible = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        return repentogon.L_Camera_IsPosVisible(self, pos)
    end,
    SetClampEnabled = function(self, clamped)
        ffichecks.checkboolean(1, clamped)
        repentogon.L_Camera_SetClampEnabled(self, clamped)
    end,
    SetFocusPosition = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        repentogon.L_Camera_SetFocusPosition(self, pos)
    end,
    SnapToPosition = function(self, pos)
        ffichecks.checkcdata(1, pos, "Vector")
        repentogon.L_Camera_SnapToPosition(self, pos)
    end,
    Update = function(self, flag)
        flag = ffichecks.optboolean(flag, false)
        repentogon.L_Camera_Update(self, flag)
    end
}

setmetatable(CameraMT, { __index = function() end })
CameraMT.__index = CameraMT

local CameraT = ffi.metatype("struct Camera", CameraMT)

Camera = setmetatable({}, {
    __class = CameraMT,
})