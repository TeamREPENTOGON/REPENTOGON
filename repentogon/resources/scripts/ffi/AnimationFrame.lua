ffi.cdef [[
    struct AnimationFrame {
        private const struct Vector Crop;
        private const float Width;
        private const float Height;
        private const struct Vector Pos;
        private const struct Vector Scale;
        private const struct Vector Pivot;
        private const int Duration;
        private const bool Visible;
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
        local result = ffi.getprivate(self, "Color") return result
    end,
    GetCrop = function(self) 
        local result = ffi.getprivate(self, "Crop") return result
    end,
    GetEndFrame = function(self) 
        local result = ffi.getprivate(self, "EndFrame") return result
    end,
    GetHeight = function(self) 
        local result = ffi.getprivate(self, "Height") return result
    end,
    GetPivot = function(self) 
        local result = ffi.getprivate(self, "Pivot") return result
    end,
    GetPos = function(self) 
        local result = ffi.getprivate(self, "Pos") return result
    end,
    GetRotation = function(self) 
        local result = ffi.getprivate(self, "Rotation") return result
    end,
    GetScale = function(self) 
        local result = ffi.getprivate(self, "Scale") return result
    end,
    GetStartFrame = function(self) 
        local result = ffi.getprivate(self, "StartFrame") return result
    end,
    GetWidth = function(self) 
        local result = ffi.getprivate(self, "Width") return result
    end,
    IsInterpolated = function(self) 
        local result = ffi.getprivate(self, "Interpolated") return result
    end,
    IsVisible = function(self) 
        local result = ffi.getprivate(self, "Visible") return result
    end,
}

setmetatable(AnimationFrameMT, { __index = function() end })
AnimationFrameMT.__index = AnimationFrameMT

local AnimationFrameT = ffi.metatype("struct AnimationFrame", AnimationFrameMT)

AnimationFrame = setmetatable({}, {
    __class = AnimationFrameMT,
})