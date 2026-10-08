ffi.cdef [[
    struct MinimapConfig {
        private struct Sprite Sprite : 0x0;
        private struct Vector Vec1 : 0x11c;
        private struct Vector BorderPadding : 0x124;
        private float PosOffsetX : 0x12c;
        private float BorderScale : 0x130;
        private int I1 : 0x134;
        private int I2 : 0x138;
        private int IconNum : 0x13c;
    } : 0x160;
    typedef struct MinimapConfig* MinimapConfigPtr;
]]

local ffi = ffi

local function GetVector(self, field)
    return ffichecks.copyvector(ffi.getprivate(self, field))
end

local MinimapConfigMT
MinimapConfigMT = {
    __type = "MinimapConfig",
    GetBorderPadding = function(self)
        return GetVector(self, "BorderPadding")
    end,
    GetBorderScale = function(self)
        local result = ffi.getprivate(self, "BorderScale") return result
    end,
    GetI1 = function(self)
        local result = ffi.getprivate(self, "I1") return result
    end,
    GetI2 = function(self)
        local result = ffi.getprivate(self, "I2") return result
    end,
    GetIconNum = function(self)
        local result = ffi.getprivate(self, "IconNum") return result
    end,
    GetPosOffsetX = function(self)
        local result = ffi.getprivate(self, "PosOffsetX") return result
    end,
    GetSprite = function(self)
        local result = ffi.getprivate(self, "Sprite") return result
    end,
    GetVec1 = function(self)
        return GetVector(self, "Vec1")
    end,
    SetBorderPadding = function(self, padding)
        ffichecks.checkcdata(1, padding, "Vector")
        ffi.setprivate(self, "BorderPadding", padding)
    end,
    SetBorderScale = function(self, scale)
        scale = ffichecks.checknumber(1, scale)
        ffi.setprivate(self, "BorderScale", scale)
    end,
    SetI1 = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "I1", value)
    end,
    SetI2 = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "I2", value)
    end,
    SetIconNum = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "IconNum", value)
    end,
    SetPosOffsetX = function(self, offset)
        offset = ffichecks.checknumber(1, offset)
        ffi.setprivate(self, "PosOffsetX", offset)
    end,
    SetVec1 = function(self, vec)
        ffichecks.checkcdata(1, vec, "Vector")
        ffi.setprivate(self, "Vec1", vec)
    end,
}

setmetatable(MinimapConfigMT, { __index = function() end })
MinimapConfigMT.__index = MinimapConfigMT

ffi.metatype("struct MinimapConfig", MinimapConfigMT)

MinimapConfig = setmetatable({}, {
    __class = MinimapConfigMT,
})
