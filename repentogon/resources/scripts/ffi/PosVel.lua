ffi.cdef[[
struct PosVel { private struct Vector PositionValue; private struct Vector VelocityValue; };
typedef struct PosVel* PosVelPtr;
]]

local PosVelMT
PosVelMT = {
    __type = "PosVel",
}

local ffi = ffi

PosVelMT.__index = function(self, key)
    if key == "Position" then
        return ffichecks.copyvector(ffi.getprivate(self, "PositionValue"))
    end
    if key == "Velocity" then
        return ffichecks.copyvector(ffi.getprivate(self, "VelocityValue"))
    end
    return PosVelMT[key]
end

PosVelMT.__newindex = function(self, key, value)
    if key == "Position" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "PositionValue", value)
        return
    end
    if key == "Velocity" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "VelocityValue", value)
        return
    end
end

local PosVelT = ffi.metatype("struct PosVel", PosVelMT)

PosVel = setmetatable({}, {
    __class = PosVelMT,
})