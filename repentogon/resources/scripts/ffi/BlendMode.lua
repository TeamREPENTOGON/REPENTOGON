ffi.cdef [[
    struct BlendMode {
        unsigned int Equation;
        unsigned int RGBSourceFactor;
        unsigned int RGBDestFactor;
        unsigned int AlphaSourceFactor;
        unsigned int AlphaDestFactor;
    };
]];

local ffi = ffi
local repentogon = ffidll
local debug_getinfo = debug.getinfo

-- We load before enums, forgive the magic numbers
local blendModes = {
    {1, 0, 1, 0, 0},
    {1, 7, 1, 7, 0},
    {1, 1, 1, 1, 0},
    {0, 2, 0, 2, 0},
    {4, 7, 4, 7, 0}
}

local function checkValidBlendFactor(factor)
    if factor < 0 and factor > 14 then
        error("Invalid blend factor")
    end
end

local function checkValidBlendEquation(factor)
    if factor < 0 and factor > 4 then
        error("Invalid blend equation")
    end
end


local BlendModeMT
BlendModeMT = {
    __type = "BlendMode",
    SetMode = function(self, blendType)
        local mode = {}
        if ffichecks.istable(1, blendType) then
            for i, v in pairs(blendType) do
                if i < 5 then
                    checkValidBlendFactor(v)
                else
                    checkValidBlendEquation(v)
                end
             table.insert(mode, v)
            end
        else
            ffichecks.checkinteger(1, blendType)
            if blendType >= 0 and blendType < 5 then
                mode = blendModes[blendType + 1]
            end
        end

        self.RGBSourceFactor = mode[1]
        self.RGBDestFactor = mode[2]
        self.AlphaSourceFactor = mode[3]
        self.AlphaDestFactor = mode[4]
        self.Equation = mode[5]
    end,
}

BlendModeMT.__index = function(self, key)
    if key == "Flag1" then
        return self.RGBSourceFactor
    end
    if key == "Flag2" then
        return self.RGBDestFactor
    end
    if key == "Flag3" then
        return self.AlphaSourceFactor
    end
    if key == "Flag4" then
        return self.AlphaDestFactor
    end
    return BlendModeMT[key]
end

BlendModeMT.__newindex = function(self, key, value)
    if key == "Flag1" then
        checkValidBlendFactor(value)
        self.RGBSourceFactor = value
    end
    if key == "Flag2" then
        checkValidBlendFactor(value)
        self.RGBDestFactor = value
    end
    if key == "Flag3" then
        checkValidBlendFactor(value)
        self.AlphaSourceFactor = value
    end
    if key == "Flag4" then
        checkValidBlendFactor(value)
        self.AlphaDestFactor = value
    end
end

local BlendModeT = ffi.metatype("struct BlendMode", BlendModeMT)
BlendMode = setmetatable({
    New = function(srcRGB, dstRGB, srcAlpha, dstAlpha, equation) 
        srcRGB = ffichecks.optnumber(srcRGB, 1)
        dstRGB = ffichecks.optnumber(dstRGB, 0)
        srcAlpha = ffichecks.optnumber(srcAlpha, 1)
        dstAlpha = ffichecks.optnumber(dstAlpha, 0)
        equation = ffichecks.optnumber(equation, 0)

        checkValidBlendFactor(srcRGB)
        checkValidBlendFactor(dstRGB)
        checkValidBlendFactor(srcAlpha)
        checkValidBlendFactor(dstAlpha)
        checkValidBlendEquation(equation)

        return BlendModeT(equation, srcRGB, dstRGB, srcAlpha, dstAlpha)
    end,
    NewFromType = function(blendType) 
        ffichecks.checknumber(1, blendType)
        
        if type < 0 or type > 4 then
            error(string.format("bad argument #1 to '%s' (Invalid blend type %d)", debug_getinfo(2).name, blendType), 3)
        end
    
        local mode = blendModes[blendType + 1]
        return BlendModeT(mode[5], mode[1], mode[2], mode[3], mode[4])
    end,
}, {
    __class = BlendModeMT,
})