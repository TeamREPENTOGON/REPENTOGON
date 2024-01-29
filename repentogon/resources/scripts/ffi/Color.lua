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

	EmberFade = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 1, 131/255, 1/255);

	TearIpecac = ColorMT(0.5, 0.9, 0.4, 1, 0, 0, 0, 0, 0, 0, 0);
	TearHoming = ColorMT(0.4, 0.15, 0.38, 1, 0, 0, 0, 0, 71/255, 0, 116/255);
	TearTar = ColorMT(0.95, 0.8, 0.6, 1, 0, 0, 0, 0, -150/255, -150/255, -150/255);
	TearSoy = ColorMT(1.5, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0);
	TearChocolate = ColorMT(0.33, 0.18, 0.18, 1, 0, 0, 0, 0, 66/255, 40/255, 40/255);
	TearAlmond = ColorMT(1.8, 1.7, 1, 1, 0, 0, 0, 0, 0, 0, 0);
	TearScorpio = ColorMT(50/255, 1, 50/255, 1, 0, 0, 0, 0, 0, 0, 0);
	TearSerpentsKiss = ColorMT(0.5, 0.97, 0.5, 1, 0, 0, 0, 0, 0, 0, 0);
	TearCommonCold = ColorMT(0.4, 0.97, 0.5, 1, 0, 0, 0, 0, 0, 0, 0);
	TearCoal = ColorMT(0.2, 0.09, 0.065, 1, 0, 0, 0, 0, 0, 0, 0);
	TearNumberOne = ColorMT(1, 1, 0, 1, 0, 0, 0, 0, 45/255, 15/255, 0);

	ProjectileIpecac = ColorMT(1, 1, 1, 1, 0.4, 2, 0.5, 1, 0, 0, 0);
	ProjectileHoming = ColorMT(1, 1, 1, 1, 0.8, 0.15, 1, 1, 0.26, 0.05, 0.4);
	ProjectileTar = ColorMT(1, 1, 1, 1, 0.5, 0.5, 0.5, 1, 0, 0, 0);
	ProjectileSoy = ColorMT(1, 1, 1, 1, 1, 1, 1, 1, 0.8, 0.7, 0.5);
	ProjectileFireWave = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 1, 0.3, 0);

	ProjectileCageBlue = ColorMT(1, 1, 1, 1, 0.8, 1, 0.85, 1, 0, 0, 0);
	ProjectileMegaSatanBlack = ColorMT(1, 1, 1, 1, 0.6, 0.6, 0.6, 1, 0, 0, 0);
	ProjectileMegaSatanWhite = ColorMT(1, 1, 1, 1, 2, 2, 2, 1, 0, 0, 0);
	ProjectileHushBlue = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 0, 0.2, 0.4);
	ProjectileHushYellow = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 0.4, 0.2, 0);
	ProjectileHushGreen = ColorMT(1, 1, 1, 1, 0, 0, 0, 0, 0.2, 0.2, 0);

	LaserIpecac = ColorMT(1, 1, 1, 1, 1.8, 3, 1, 1, 0, 0, 0);
	LaserHoming = ColorMT(1, 1, 1, 1, 3, 1, 3.5, 1, 0, 0, 0);
	LaserSoy = ColorMT(1, 1, 1, 1, 5.6, 5, 4.2, 1, 0, 0, 0);
	LaserChocolate = ColorMT(1, 1, 1, 1, 3, 1.7, 1.7, 1, 0, 0, 0);
	LaserAlmond = ColorMT(1, 1, 1, 1, 5.6, 5.2, 3.8, 1, 0, 0, 0);
	LaserPoison = ColorMT(1, 1, 1, 1, 1.8, 4, 1, 1, 0, 0, 0);
	LaserCoal = ColorMT(3, 3, 3, 1, 1.3, 1.2, 1.2, 1, -0.5, -0.5, -0.5);
	LaserFireMind = ColorMT(1, 1, 1, 1, 5, 3, 1, 1, 0, 0, 0);
	LaserNumberOne = ColorMT(1, 1, 1, 1, 5, 4.9, 1, 1, 0, 0, 0);
	LaserMother = ColorMT(1, 1, 1, 1, 2, 2.2, 1, 1, 0, 0, 0);

	ProjectileCorpsePink = ColorMT(1, 1, 1, 1, 4, 3.5, 3.2, 1, 0, 0, 0);
	ProjectileCorpseWhite = ColorMT(1, 1, 1, 1, 2.7, 3, 2, 1, 0, 0, 0);
	ProjectileCorpseGreen = ColorMT(1, 1, 1, 1, 1.5, 2, 1, 1, 0, 0, 0);
	ProjectileCorpseYellow = ColorMT(1, 1, 1, 1, 3.5, 2.5, 1, 1, 0, 0, 0);
	ProjectileCorpseClusterLight = ColorMT(4, 4, 4, 1, 0.63, 0.85, 0.32, 1, 0, 0, 0);
	ProjectileCorpseClusterDark = ColorMT(1, 1, 1, 1, 0.63, 0.85, 0.32, 1, 0, 0, 0);	
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