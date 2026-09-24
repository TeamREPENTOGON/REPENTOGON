ffi.cdef [[
    struct Backdrop {
        private struct Image FloorImage : 0x0;
        private struct Image WallImage : 0x8;
        private struct Sprite FloorANM2 : 0x3c;
        private int Type : 0x150;
        private struct Sprite DetailANM2 : 0x4f3c;
        private struct Sprite ControlsANM2 : 0x5050;
        private struct Sprite ControlsButtonsANM2 : 0x5164;
    } : 0x5278;
]]
local ffi = ffi
    
local BackdropMT
BackdropMT = {
    __type = "Backdrop",
    GetControlsANM2 = function(self)
        return ffi.getprivate(self, "ControlsANM2")
    end,
    GetControlsButtonsANM2 = function(self)
        return ffi.getprivate(self, "ControlsButtonsANM2")
    end,
    GetDetailANM2 = function(self)
        return ffi.getprivate(self, "DetailANM2")
    end,
    GetFloorANM2 = function(self)
        return ffi.getprivate(self, "FloorANM2")
    end,
    GetFloorImage = function(self)
        return ffi.getprivate(self, "FloorImage")
    end,
    GetWallImage = function(self)
        return ffi.getprivate(self, "WallImage")
    end,
}

setmetatable(BackdropMT, {
    __index = function() end,
})
BackdropMT.__index = BackdropMT

local BackdropT = ffi.metatype("struct Backdrop", BackdropMT)

Backdrop = setmetatable({}, {
    __class = BackdropMT,
})