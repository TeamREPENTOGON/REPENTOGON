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
        return ffi.getprivate(self, "HighTopLeft")
    end,
    GetHighBottomRight = function(self)
        return ffi.getprivate(self, "HighBottomRight")
    end,
    GetLowTopLeft = function(self)
        return ffi.getprivate(self, "LowTopLeft")
    end,
    GetLowBottomRight = function(self)
        return ffi.getprivate(self, "LowBottomRight")
    end,
}

setmetatable(LRoomAreaDescMT, { __index = function() end })
LRoomAreaDescMT.__index = LRoomAreaDescMT

local LRoomAreaDescT = ffi.metatype("struct LRoomAreaDesc", LRoomAreaDescMT)

LRoomAreaDesc = setmetatable({
}, {
    __class = LRoomAreaDescMT,
})
