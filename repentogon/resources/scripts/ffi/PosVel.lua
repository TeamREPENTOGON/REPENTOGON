ffi.cdef[[
struct PosVel { struct Vector Position; struct Vector Velocity; };
typedef struct PosVel* PosVelPtr;
]]

local PosVelMT
PosVelMT = {
    __type = "PosVel",
}

setmetatable(PosVelMT, { __index = function() end })
PosVelMT.__index = PosVelMT

local PosVelT = ffi.metatype("struct PosVel", PosVelMT)

PosVel = setmetatable({}, {
    __class = PosVelMT,
})