ffi.cdef[[
typedef struct {   
    float X;
	float Y;
}  Vector;

]]

local repentogon = ffidll
local lffi = ffi

local VectorFuncs = {}

local function ParseVectorString(str, mt)
	return string.format(str, mt.X, mt.Y)
end

function VectorFuncs:__tostring()
	return ParseVectorString("%g %g", self)
end

local VectorMT
VectorMT = lffi.metatype("Vector", {
	__tostring = function(a)
		return ParseVectorString("%g %g", a)
	end;

	__index = VectorFuncs;
})

function VectorFuncs:Clamp(MinX, MinY, MaxX, MaxY) 
	ffichecks.checknumber(1, MinX)
	ffichecks.checknumber(2, MinY)
	ffichecks.checknumber(3, MaxX)
	ffichecks.checknumber(4, MaxY)
	
	self.X = math.min(math.max(self.X, MinX), MaxX)
	self.Y = math.min(math.max(self.Y, MinY), MaxY)
end

function VectorFuncs:Clamped(MinX, MinY, MaxX, MaxY)
	ffichecks.checknumber(1, MinX)
	ffichecks.checknumber(2, MinY)
	ffichecks.checknumber(3, MaxX)
	ffichecks.checknumber(4, MaxY)
	
	return VectorMT(
		math.min(math.max(self.X, MinX), MaxX),
		math.min(math.max(self.Y, MinY), MaxY)
	)
end

function VectorFuncs:Cross(second)
	ffichecks.checktype(1, second, "Vector")
	
	return second.Y * self.X - self.Y * second.X;
end

local function DistanceSquared(second)
	ffichecks.checktype(1, second, "Vector")
	
	local fVar1 = self.Y - second.Y
	local fVar2 = self.X - second.X
	
	return fVar1 * fVar1 + fVar2 * fVar2
end

function VectorFuncs:Distance(second)
	local distanceSquared = DistanceSquared(second)
	
	return math.sqrt(distanceSquared)
end

function VectorFuncs:DistanceSquared(second)
	return DistanceSquared(second)
end

function VectorFuncs:Dot(second)
	ffichecks.checktype(1, second, "Vector")
	
	return second.X * self.X + second.Y * self.Y;
end

function VectorFuncs:GetAngleDegrees()
	local atan2 = math.atan2(self.Y, self.X)
	return atan2 * 180/math.pi
end

function VectorFuncs:Length()
	local lengthSquared = self.X * self.X + self.Y + self.Y
	
	return math.sqrt(lengthSquared)
end

function VectorFuncs:LengthSquared()
	return self.X * self.X + self.Y + self.Y
end

function VectorFuncs:Lerp(m2, t)
	ffichecks.checktype(1, m2, "Vector")
	ffichecks.checknumber(2, t)
	
	local it = 1-t
	
	self.X = self.X * it + m2.X * t
	self.Y = self.Y * it + m2.Y * t
end

function VectorFuncs:Normalize()
	local length = math.sqrt(self.X * self.X + self.Y + self.Y)
	if length > 0 then
		local normalize = 1/Length
		self.X = self.X * normalize
		self.Y = self.Y * normalize
	end
end

function VectorFuncs:Normalized()
	local res = VectorMT(self.X, self.Y)
	local length = math.sqrt(res.X * res.X + res.Y + res.Y)
	if length > 0 then
		local normalize = 1/Length
		res.X = res.X * normalize
		res.Y = res.Y * normalize
	end
	return res
end

function VectorFuncs:Resize(NewLength)
	ffichecks.checknumber(1, NewLength)

	local length = math.sqrt(self.X * self.X + self.Y + self.Y)
	if length > 0 then
		self.X = self.X * (NewLength / length)
		self.Y = self.Y * (NewLength / length)
	end
end

function VectorFuncs:Resized(NewLength)
	ffichecks.checknumber(1, NewLength)
	
	local res = VectorMT(self.X, self.Y)
	local length = math.sqrt(res.X * res.X + res.Y + res.Y)
	if length > 0 then
		res.X = res.X * (NewLength / length)
		res.Y = res.Y * (NewLength / length)
	end
	return res
end

function VectorFuncs:Rotated(Angle)
	ffichecks.checknumber(1, Angle)
	
	local rads = math.rad(Angle)
	local cos = math.cos(rads)
	local sin = math.sin(rads)
	
	return VectorMT(self.X * cos - self.Y * sin, self.Y * cos - self.X * sin)
end

Vector = setmetatable({

	FromAngle = function(angle)
		ffichecks.checknumber(1, angle)
		
		local radians = math.rad(angle)
		return VectorMT(math.cos(radians), math.sin(radians))
	end;

	Zero = VectorMT(0, 0);
	One = VectorMT(1, 1);
},
{
	__call = function(_, X, Y)
		return VectorMT(
			X or 0,
			Y or 0
		)
	end,
	
	__add = function(second)
		ffichecks.checktype(1, second, "Vector")

		return VectorMT(self.X + second.X, self.Y + second.Y)
	end,
	
	__sub = function(second)
		ffichecks.checktype(1, second, "Vector")

		return VectorMT(self.X - second.X, self.Y - second.Y)
	end,
	
	__mul = function(second)
		if ffichecks.isnumber(1, second) then
			return VectorMT(self.X * second, self.Y * second)
		else
			ffichecks.checktype(1, second, "Vector")
			return VectorMT(self.X * second.X, self.Y * second.Y)
		end
	end,

	__div = function(second)
		if ffichecks.isnumber(1, second) then
			if second == 0 then
				error("Divide by zero")
			end
			return VectorMT(self.X / second, self.Y / second)
		else
			ffichecks.checktype(1, second, "Vector")
			if second.X == 0 or second.Y == 0 then
				error("Divide by zero")
			end
			return VectorMT(self.X / second.X, self.Y / second.Y)
		end
	end,
	
	__unm = function()
		return VectorMT(self.X * -1, self.Y * -1)
	end,

})