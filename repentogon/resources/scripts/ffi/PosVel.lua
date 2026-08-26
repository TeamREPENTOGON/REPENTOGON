ffi.cdef[[
struct PosVel { struct Vector Position; struct Vector Velocity; };
typedef struct PosVel* PosVelPtr;
]]

local PosVelMT
PosVelMT = {
    __type = "PosVel",
}

PosVelMT.__index = function(self, key)
	return PosVelMT[key]
end 


PosVel = setmetatable({}, {
    __class = PosVelMT,
})