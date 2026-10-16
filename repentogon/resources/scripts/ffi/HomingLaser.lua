ffi.cdef[[
    struct HomingLaser {
        private char Storage[0x54];
    } : 0x54;
]]

local HomingLaserMT
HomingLaserMT = {
    __type = "HomingLaser",
}

setmetatable(HomingLaserMT, { __index = function() end })
HomingLaserMT.__index = HomingLaserMT

local HomingLaserT = ffi.metatype("struct HomingLaser", HomingLaserMT)

HomingLaser = setmetatable({}, {
    __class = HomingLaserMT,
})
