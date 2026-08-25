ffi.cdef[[
struct Color {
    float R;
    float G;
    float B;
    float A;
    float RC;
    float GC;
    float BC;
    float AC;
    float RO;
    float GO;
    float BO;
};
typedef struct Color* ColorPtr;
]]

local ColorMT = {
    __type = "Color",

    __tostring = function(self)
        return string.format(
            "[Color: Tint %g %g %g %g | Colorize %g %g %g %g | Offset %g %g %g]",
            self.R, self.G, self.B, self.A,
            self.RC, self.GC, self.BC, self.AC,
            self.RO, self.GO, self.BO
        )
    end,

    __mul = function(self, second)
        ffichecks.checkcdata(1, self, "Color")
        ffichecks.checkcdata(2, second, "Color")

        local RC, GC, BC, AC

        if second.RC + second.GC + second.BC ~= 0 then
            if self.RC + self.GC + self.BC ~= 0 then
                RC = (self.RC + second.RC) * 0.5
                GC = (self.GC + second.GC) * 0.5
                BC = (self.BC + second.BC) * 0.5
                AC = (self.AC + second.AC) * 0.5
            else
                RC = second.RC
                GC = second.GC
                BC = second.BC
                AC = second.AC
            end
        else
            RC = self.RC
            GC = self.GC
            BC = self.BC
            AC = self.AC
        end

        return Color(
            self.R * second.R,
            self.G * second.G,
            self.B * second.B,
            self.A * second.A,
            self.RO + second.RO,
            self.GO + second.GO,
            self.BO + second.BO,
            RC,
            GC,
            BC,
            AC
        )
    end,

    Reset = function(self)
        self.R = 1
        self.G = 1
        self.B = 1
        self.A = 1
        self.RC = 0
        self.GC = 0
        self.BC = 0
        self.AC = 0
        self.RO = 0
        self.GO = 0
        self.BO = 0
    end,

    SetColorize = function(self, r, g, b, a)
        ffichecks.checknumber(1, r)
        ffichecks.checknumber(2, g)
        ffichecks.checknumber(3, b)
        ffichecks.checknumber(4, a)

        self.RC = r
        self.GC = g
        self.BC = b
        self.AC = a
    end,

    SetOffset = function(self, r, g, b)
        ffichecks.checknumber(1, r)
        ffichecks.checknumber(2, g)
        ffichecks.checknumber(3, b)

        self.RO = r
        self.GO = g
        self.BO = b
    end,

    SetTint = function(self, r, g, b, a)
        ffichecks.checknumber(1, r)
        ffichecks.checknumber(2, g)
        ffichecks.checknumber(3, b)
        ffichecks.checknumber(4, a)

        self.R = r
        self.G = g
        self.B = b
        self.A = a
    end,

    GetColorize = function(self)
        return {self.RC, self.GC, self.BC, self.AC}
    end,

    GetOffset = function(self)
        return {self.RO, self.GO, self.BO}
    end,

    GetTint = function(self)
        return {self.R, self.G, self.B, self.A}
    end,

    Print = function(self)
        print(string.format(
            "Color: %g %g %g %g\nColorize: %g %g %g %g\nOffset: %g %g %g",
            self.R, self.G, self.B, self.A,
            self.RC, self.GC, self.BC, self.AC,
            self.RO, self.GO, self.BO
        ))
    end
}

local ColorT = ffi.metatype("struct Color", ColorMT)

ColorMT.__index = function(self, key)
	return ColorMT[key]
end

local ColorGlobalMT = {
    __call = function(_, R, G, B, A, RO, GO, BO, RC, GC, BC, AC)
        return ColorT(
            R or 1, G or 1, B or 1, A or 1,
            RC or 0, GC or 0, BC or 0, AC or 0,
            RO or 0, GO or 0, BO or 0
        )
    end,

    __class = ColorMT,
}

ColorGlobalMT.__index = ColorGlobalMT

Color = setmetatable({
    Lerp = function(col1, col2, t)
        ffichecks.checkcdata(1, col1, "Color")
        ffichecks.checkcdata(2, col2, "Color")
        ffichecks.checknumber(3, t)

        -- 1-1 recreation from Ghidra decomp
        local x = 1 - t

        return Color(
            x * col1.R + t * col2.R,
            x * col1.G + t * col2.G,
            x * col1.B + t * col2.B,
            x * col1.A + t * col2.A,
            x * col1.RO + t * col2.RO,
            x * col1.GO + t * col2.GO,
            x * col1.BO + t * col2.BO,
            x * col1.RC + t * col2.RC,
            x * col1.GC + t * col2.GC,
            x * col1.BC + t * col2.BC,
            x * col1.AC + t * col2.AC
        )
    end,
}, ColorGlobalMT)

ColorGlobalMT.Default = Color(1, 1, 1, 1)