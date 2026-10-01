ffi.cdef [[
    struct LRoomAreaDesc {
        private struct Vector HighTopLeft;
        private struct Vector HighBottomRight;
        private struct Vector LowTopLeft;
        private struct Vector LowBottomRight;
    }
]]

local repentogon = ffidll
local ffi = ffi

local LRoomAreaDescMT
LRoomAreaDescMT = {
    __type = "LRoomAreaDesc",
    GetHighTopLeft = function(self)
        local v = ffi.getprivate(self, "HighTopLeft")
        return Vector(v.X, v.Y)
    end,
    GetHighBottomRight = function(self)
        local v = ffi.getprivate(self, "HighBottomRight")
        return Vector(v.X, v.Y)
    end,
    GetLowTopLeft = function(self)
        local v = ffi.getprivate(self, "LowTopLeft")
        return Vector(v.X, v.Y)
    end,
    GetLowBottomRight = function(self)
        local v = ffi.getprivate(self, "LowBottomRight")
        return Vector(v.X, v.Y)
    end,
}

setmetatable(LRoomAreaDescMT, { __index = function() end })
LRoomAreaDescMT.__index = LRoomAreaDescMT

local LRoomAreaDescT = ffi.metatype("struct LRoomAreaDesc", LRoomAreaDescMT)

LRoomAreaDesc = setmetatable({
}, {
    __class = LRoomAreaDescMT,
})
