ffi.cdef [[
    struct NullFrame {
        private struct Vector Pos;
        private unsigned int Duration;
        private bool Visible;
        padding char[0x3];
        private struct Vector Scale;
        private struct Color Color;
        private float Rotation;
        private bool Interpolated;
        padding char[0x3];
        private int StartFrame;
        private int EndFrame;
    };
]]
local ffi = ffi
local NullFrameMT
NullFrameMT = {
    __type = "NullFrame",
    GetColor = function(self)
        return ffi.getprivate(self, "Color")
    end,
    GetPos = function(self)
        return ffi.getprivate(self, "Pos")
    end,
    GetRotation = function(self)
        return ffi.getprivate(self, "Rotation")
    end,
    GetScale = function(self)
        return ffi.getprivate(self, "Scale")
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Visible")
    end
}

setmetatable(NullFrameMT, { __index = function() end })
NullFrameMT.__index = NullFrameMT

local NullFrameT = ffi.metatype("struct NullFrame", NullFrameMT)

NullFrame = setmetatable({}, {
    __class = NullFrameMT
})