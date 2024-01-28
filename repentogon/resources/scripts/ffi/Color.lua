ffi.cdef[[
typedef struct {   
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
}  ColorMod;

]]

local repentogon = ffidll
local lffi = ffi

local ColorFuncs = {}

function ColorFuncs:Reset() 
	self.R, self.G, self.B, self.A = 1, 1, 1, 1
	self.RC, self.GC, self.BC, self.AC = 0, 0, 0, 0
	self.RO, self.GO, self.BO = 0, 0, 0
end

function ColorFuncs:SetTint(r, g, b, a) 
	ffichecks.checknumber(1, r)
	ffichecks.checknumber(2, g)
	ffichecks.checknumber(3, b)
	ffichecks.checknumber(4, a)

	self.R = r
	self.G = g
	self.B = b
	self.A = a
end

function ColorFuncs:SetColorize(r, g, b, a) 
	ffichecks.checknumber(1, r)
	ffichecks.checknumber(2, g)
	ffichecks.checknumber(3, b)
	ffichecks.checknumber(4, a)

	self.RC = r
	self.GC = g
	self.BC = b
	self.AC = a
end

function ColorFuncs:SetOffset(r, g, b) 
	ffichecks.checknumber(1, r)
	ffichecks.checknumber(2, g)
	ffichecks.checknumber(3, b)

	self.RO = r
	self.GO = g
	self.BO = b
end

function ColorFuncs:GetTint()
	return {R = self.R, G = self.G, B = self.B, A = self.A}
end

function ColorFuncs:GetColorize()
	return {R = self.RC, G = self.GC, B = self.BC, A = self.AC}
end

function ColorFuncs:GetOffset()
	return {R = self.RO, G = self.GO, B = self.BO}
end


local function ParseColorString(str, mt)
	return string.format(str, mt.R, mt.G, mt.B, mt.A, mt.RC, mt.GC, mt.BC, mt.AC, mt.RO, mt.GO, mt.BO)
end

function ColorFuncs:__tostring()
	return ParseColorString("[Color %g %g %g %g | Colorize %g %g %g %g | Offset %g %g %g]", self)
end

function ColorFuncs:Print()
	print(ParseColorString("Color: %g %g %g %g\nColorize: %g %g %g %g\nOffset: %g %g %g", self))
end

local ColorMT
ColorMT = lffi.metatype("ColorMod", {
	__tostring = function(a)
		return ParseColorString("[Color %g %g %g %g | Colorize %g %g %g %g | Offset %g %g %g]", a)
	end;

	__index = ColorFuncs;
})

Color = setmetatable({

	Lerp = function(m1, m2, t)
		
		local it = 1-t
		
		return ColorMT(
			m1.R * it + m2.R * t,
			m1.G * it + m2.G * t,
			m1.B * it + m2.B * t,
			m1.A * it + m2.A * t,
			m1.RC * it + m2.RC * t,
			m1.GC * it + m2.GC * t,
			m1.BC * it + m2.BC * t,
			m1.AC * it + m2.AC * t,
			m1.RO * it + m2.RO * t,
			m1.GO * it + m2.GO * t,
			m1.BO * it + m2.BO * t
		)

	end;

	Default = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0);
},
{
	__call = function(_, R, G, B, A, RO, GO, BO)
		return ColorMT(
			R or 1, G or 1, B or 1, A or 1, 
			0, 0, 0, 0, 
			RO or 0, GO or 0, BO or 0
		)
	end
})