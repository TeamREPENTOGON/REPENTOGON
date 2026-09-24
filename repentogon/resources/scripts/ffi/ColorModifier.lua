ffi.cdef[[
struct ColorModifier {
    float R;
    float G;
    float B;
    float A; 
    float Brightness;
    float Contrast;
};

typedef struct ColorModifier* ColorModifierPtr;
]]

local repentogon = ffidll
local ffi = ffi

local ColorModifierMT
ColorModifierMT = {
    __type = "ColorModifier",
    __add = function(self, second)
        ffichecks.checkcdata(1, second, "ColorModifier")
        return ColorModifier(self.R + second.R, self.G + second.G, self.B + second.B, self.A + second.A, self.Brightness + second.Brightness, self.Contrast + second.Contrast)
    end,
	__sub = function(self, second)
        ffichecks.checkcdata(1, second, "ColorModifier")
        return ColorModifier(self.R - second.R, self.G - second.G, self.B - second.B, self.A - second.A, self.Brightness - second.Brightness, self.Contrast - second.Contrast)
    end,	
	__mul = function(self, second)
        if ffichecks.isnumber(self) then
            ffichecks.argerror(1, self, "Cannot left multiply a ColorModifier with a number")
        elseif ffichecks.isnumber(second) then
            ffichecks.checkcdata(1, self, "ColorModifier")
            return ColorModifier (self.R * second, self.G * second, self.B * second, self.A * second, self.Brightness * second, self.Contrast * second)
        else 
            ffichecks.checkcdata(1, second, "ColorModifier")
            return ColorModifier(self.R * second.R, self.G * second.G, self.B * second.B, self.A * second.A, self.Brightness * second.Brightness, self.Contrast * second.Contrast)
        end
    end,
	__div = function(self, second)
		if ffichecks.isnumber(self) then
            ffichecks.argerror(1, self, "Cannot left divide a ColorModifier with a number")
        elseif ffichecks.isnumber(second) then
            ffichecks.checkcdata(1, self, "ColorModifier")
            return ColorModifier (self.R / second, self.G / second, self.B / second, self.A / second, self.Brightness / second, self.Contrast / second)
        else 
            ffichecks.checkcdata(1, second, "ColorModifier")
            return ColorModifier(self.R / second.R, self.G / second.G, self.B / second.B, self.A / second.A, self.Brightness / second.Brightness, self.Contrast / second.Contrast)
        end
	end,	
    __eq = function(self, second)
         ffichecks.checkcdata(1, second, "ColorModifier")
        return self.R  == second.R and  self.G == second.G and  self.B == second.B and self.A == second.A and self.Brightness == second.Brightness and self.Contrast == second.Contrast
    end,
}

setmetatable(ColorModifierMT, { __index = function() end })
ColorModifierMT.__index = ColorModifierMT

local ColorModifierT = ffi.metatype("struct ColorModifier", ColorModifierMT)

ColorModifier = setmetatable({}, {
    __class = ColorModifierMT,
    __call = function(_, R, G, B, A, Brightness, Contrast) local v = ColorModifierT(R or 1, G or 1, B or 1, A or 0, Brightness or 0, Contrast or 1) return v end,
})