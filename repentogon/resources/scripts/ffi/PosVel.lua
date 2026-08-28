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

local PosVelT = ffi.metatype("struct PosVel", PosVelMT)

PosVel = setmetatable({}, {
    __class = PosVelMT,
})