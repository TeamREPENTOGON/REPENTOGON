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

-- We load before enums, forgive the magic numbers
local blendModes = {
    {1, 0, 1, 0, 0},
    {1, 7, 1, 7, 0},
    {1, 1, 1, 1, 0},
    {0, 2, 0, 2, 0},
    {4, 7, 4, 7, 0}
}

local function checkValidBlendFactor(idx, factor)
    if factor < 0 or factor >= 10 then
        ffichecks.argerror(idx, "Invalid blend factor", 3)
    end
end

local function checkValidBlendEquation(idx, equation)
    if equation < 0 or equation >= 5 then
        ffichecks.argerror(idx, "Invalid equation", 3)
    end
end


local BlendModeMT
BlendModeMT = {
    __type = "BlendMode",
    SetMode = function(self, ...)
        local first = ...
        local parts
        if select("#", ...) > 1 then
            parts = { ... }
        elseif type(first) == "table" then
            parts = first
        else
            first = ffichecks.checknumber(1, first)
            local preset = blendModes[math.floor(first) + 1]
            if preset then
                self.RGBSourceFactor, self.RGBDestFactor = preset[1], preset[2]
                self.AlphaSourceFactor, self.AlphaDestFactor = preset[3], preset[4]
                self.Equation = preset[5]
            end
            return
        end

        local rgbSrc = ffichecks.optnumber(parts[1], self.RGBSourceFactor)
        local rgbDst = ffichecks.optnumber(parts[2], self.RGBDestFactor)
        local alphaSrc = ffichecks.optnumber(parts[3], self.AlphaSourceFactor)
        local alphaDst = ffichecks.optnumber(parts[4], self.AlphaDestFactor)
        local equation = ffichecks.optnumber(parts[5], self.Equation)
        checkValidBlendFactor(1, rgbSrc)
        checkValidBlendFactor(2, rgbDst)
        checkValidBlendFactor(3, alphaSrc)
        checkValidBlendFactor(4, alphaDst)
        checkValidBlendEquation(5, equation)

        self.RGBSourceFactor, self.RGBDestFactor = rgbSrc, rgbDst
        self.AlphaSourceFactor, self.AlphaDestFactor = alphaSrc, alphaDst
        self.Equation = equation
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
        checkValidBlendFactor(1, value)
        self.RGBSourceFactor = value
    end
    if key == "Flag2" then
        checkValidBlendFactor(1, value)
        self.RGBDestFactor = value
    end
    if key == "Flag3" then
        checkValidBlendFactor(1, value)
        self.AlphaSourceFactor = value
    end
    if key == "Flag4" then
        checkValidBlendFactor(1, value)
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

        checkValidBlendFactor(1, srcRGB)
        checkValidBlendFactor(2, dstRGB)
        checkValidBlendFactor(3, srcAlpha)
        checkValidBlendFactor(4, dstAlpha)
        checkValidBlendEquation(5, equation)

        return BlendModeT(equation, srcRGB, dstRGB, srcAlpha, dstAlpha)
    end,
    NewFromType = function(blendType) 
        blendType = ffichecks.checknumber(1, blendType)
        
        blendType = math.floor(blendType)
        if blendType < 0 or blendType > 4 then
            ffichecks.argerror(1, string.format("invalid blend type %d", blendType))
        end
    
        local mode = blendModes[blendType + 1]
        return BlendModeT(mode[5], mode[1], mode[2], mode[3], mode[4])
    end,
}, {
    __class = BlendModeMT,
})