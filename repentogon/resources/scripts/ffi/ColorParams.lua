ffi.cdef[[
    struct ColorParams {    
        private int Priority : 0x0;
        private struct Color Color : 0x4;
        private int Duration : 0x30;
        private int Lifespan : 0x34;
        private bool Fadeout : 0x38;
        private bool Shared : 0x39;
    } : 0x3c;
    typedef struct ColorParams* ColorParamsPtr;
]]

local repentogon = ffidll
local ffi = ffi

local ColorParamsMT
ColorParamsMT = {
    __type = "ColorParams",
    GetColor = function(self)
        return ffi.new("struct Color", ffi.getprivate(self, "Color"))
    end,
    GetDuration = function(self)
        return ffi.getprivate(self, "Duration")
    end,
    GetFadeout = function(self)
        return ffi.getprivate(self, "Fadeout")
    end,
    GetLifespan = function(self)
        return ffi.getprivate(self, "Lifespan")
    end,
    GetPriority = function(self)
        return ffi.getprivate(self, "Priority")
    end,
    GetShared = function(self)
        return ffi.getprivate(self, "Shared")
    end,
    SetColor = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        ffi.setprivate(self, "Color", color)
    end,
    SetDuration = function(self, duration)
        ffichecks.checkinteger(1, duration)
        ffi.setprivate(self, "Duration", duration)
    end,
    SetFadeout = function(self, fadeout)
        ffichecks.checkboolean(1, fadeout)
        ffi.setprivate(self, "Fadeout", fadeout)
    end,
    SetLifespan = function(self, lifespan)
        ffichecks.checkinteger(1, lifespan)
        ffi.setprivate(self, "Lifespan", lifespan)
    end,
    SetPriority = function(self, priority)
        ffichecks.checkinteger(1, priority)
        ffi.setprivate(self, "Priority", priority)
    end,
    SetShared = function(self, shared)
        ffichecks.checkboolean(1, shared)
        ffi.setprivate(self, "Shared", shared)
    end,

}

setmetatable(ColorParamsMT, { __index = function() end })
ColorParamsMT.__index = ColorParamsMT

local ColorParamsT = ffi.metatype("struct ColorParams", ColorParamsMT)

ColorParams = setmetatable({}, {
    __call = function(_, color, priority, duration, fadeout, shared) 
        ffichecks.checkcdata(1, color, "Color")
        ffichecks.checkinteger(2, priority)
        ffichecks.checkinteger(3, duration)
        ffichecks.checkboolean(4, fadeout)
        ffichecks.checkboolean(5, shared)
        local v = ColorParamsT(priority, color, duration, duration, fadeout, shared)
        return v 
    end,
    __class = ColorParamsMT,
})