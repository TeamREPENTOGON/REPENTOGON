ffi.cdef [[
    struct AnimationFrame {
        private const struct Vector Crop;
        private const float Width;
        private const float Height;
        private const struct Vector Pos;
        private const struct Vector Scale;
        private const struct Vector Pivot;
        private const bool Duration;
        padding char[0x3];
        private const struct Color Color;
        private const float Rotation;
        private const bool Interpolated;
        padding char[0x3];
        private const unsigned int StartFrame;
        private const unsigned int EndFrame;
    };
]]

local repentogon = ffidll
local ffi = ffi

local AnimationFrameMT
AnimationFrameMT = {
    __type = "AnimationFrame",
    GetColor = function(self) 
        return ffi.getprivate(self, "Color")
    end,
    GetCrop = function(self) 
        return ffi.getprivate(self, "Crop")
    end,
    GetEndFrame = function(self) 
        return ffi.getprivate(self, "EndFrame")
    end,
    GetHeight = function(self) 
        return ffi.getprivate(self, "Height")
    end,
    GetPivot = function(self) 
        return ffi.getprivate(self, "Pivot")
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
    GetStartFrame = function(self) 
        return ffi.getprivate(self, "StartFrame")
    end,
    GetWidth = function(self) 
        return ffi.getprivate(self, "Width")
    end,
    IsInterpolated = function(self) 
        return ffi.getprivate(self, "Interpolated")
    end,
    IsVisible = function(self) 
        return ffi.getprivate(self, "Visible")
    end,
}

setmetatable(AnimationFrameMT, { __index = function() end })
AnimationFrameMT.__index = AnimationFrameMT

local AnimationFrameT = ffi.metatype("struct AnimationFrame", AnimationFrameMT)

AnimationFrame = setmetatable({}, {
    __class = AnimationFrameMT,
})