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
        local result = ffi.new("struct Color", ffi.getprivate(self, "Color")) return result
    end,
    GetDuration = function(self)
        local result = ffi.getprivate(self, "Duration") return result
    end,
    GetFadeout = function(self)
        local result = ffi.getprivate(self, "Fadeout") return result
    end,
    GetLifespan = function(self)
        local result = ffi.getprivate(self, "Lifespan") return result
    end,
    GetPriority = function(self)
        local result = ffi.getprivate(self, "Priority") return result
    end,
    GetShared = function(self)
        local result = ffi.getprivate(self, "Shared") return result
    end,
    SetColor = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        ffi.setprivate(self, "Color", color)
    end,
    SetDuration = function(self, duration)
        duration = ffichecks.checkinteger(1, duration)
        ffi.setprivate(self, "Duration", duration)
    end,
    SetFadeout = function(self, fadeout)
        fadeout = ffichecks.checkboolean(1, fadeout)
        ffi.setprivate(self, "Fadeout", fadeout)
    end,
    SetLifespan = function(self, lifespan)
        lifespan = ffichecks.checkinteger(1, lifespan)
        ffi.setprivate(self, "Lifespan", lifespan)
    end,
    SetPriority = function(self, priority)
        priority = ffichecks.checkinteger(1, priority)
        ffi.setprivate(self, "Priority", priority)
    end,
    SetShared = function(self, shared)
        shared = ffichecks.checkboolean(1, shared)
        ffi.setprivate(self, "Shared", shared)
    end,

}

setmetatable(ColorParamsMT, { __index = function() end })
ColorParamsMT.__index = ColorParamsMT

local ColorParamsT = ffi.metatype("struct ColorParams", ColorParamsMT)

ColorParams = setmetatable({}, {
    __call = function(_, color, priority, duration, fadeout, shared) 
        ffichecks.checkcdata(1, color, "Color")
        priority = ffichecks.checkinteger(2, priority)
        duration = ffichecks.checkinteger(3, duration)
        fadeout = ffichecks.checkboolean(4, fadeout)
        shared = ffichecks.checkboolean(5, shared)
        local v = ColorParamsT(priority, color, duration, duration, fadeout, shared)
        return v 
    end,
    __class = ColorParamsMT,
})