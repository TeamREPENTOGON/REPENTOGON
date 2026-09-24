ffi.cdef [[
    struct RailManager {
        private struct Sprite RailsSprite : 0x1c4;
    } : 0x2d8;
]]
    
local repentogon = ffidll
local ffi = ffi

local RailManagerMT
RailManagerMT = {
    __type = "RailManager",
    GetRailsSprite = function(self)
        return ffi.getprivate(self, "RailsSprite")
    end,
}

setmetatable(RailManagerMT, { __index = function() end })
RailManagerMT.__index = function(self, key)
    return RailManagerMT[key]
end

local RailManagerT = ffi.metatype("struct RailManager", RailManagerMT)

RailManager = setmetatable({}, {
    __class = RailManagerMT,
})