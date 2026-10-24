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
        local result = ffi.new("struct Color", ffi.getprivate(self, "Color")) return result
    end,
    GetPos = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "Pos"))
    end,
    GetRotation = function(self)
        local result = ffi.getprivate(self, "Rotation") return result
    end,
    GetScale = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "Scale"))
    end,
    IsVisible = function(self)
        local result = ffi.getprivate(self, "Visible") return result
    end
}

setmetatable(NullFrameMT, { __index = function() end })
NullFrameMT.__index = NullFrameMT

local NullFrameT = ffi.metatype("struct NullFrame", NullFrameMT)

NullFrame = setmetatable({}, {
    __class = NullFrameMT
})