ffi.cdef[[
struct PosVel { private struct Vector Position; private struct Vector Velocity; };
typedef struct PosVel* PosVelPtr;
]]

local PosVelMT
PosVelMT = {
    __type = "PosVel",
}

local ffi = ffi

PosVelMT.__index = function(self, key)
    if key == "Position" then
        return ffichecks.copyvector(ffi.getprivate(self, "Position"))
    end
    if key == "Velocity" then
        return ffichecks.copyvector(ffi.getprivate(self, "Velocity"))
    end
    return PosVelMT[key]
end

PosVelMT.__newindex = function(self, key, value)
    if key == "Position" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "Position", value)
        return
    end
    if key == "Velocity" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "Velocity", value)
        return
    end
end

local PosVelT = ffi.metatype("struct PosVel", PosVelMT)

PosVel = setmetatable({}, {
    __class = PosVelMT,
})