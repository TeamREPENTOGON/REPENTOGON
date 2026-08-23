ffi.cdef[[
struct Vector { float X; float Y; };
typedef struct Vector* VectorPtr;
]]

local repentogon = ffidll
local lffi = ffi

local function DistanceSquared(self, second)
	ffichecks.checkcdata(1, second, "Vector")
	
	local fVar1 = self.Y - second.Y
	local fVar2 = self.X - second.X
	
	return fVar1 * fVar1 + fVar2 * fVar2
end

local VectorMT
VectorMT = {
    __type = "Vector",

    __tostring = function(a) 
        return string.format("%g %g", a.X, a.Y) 
    end,
    __add = function(self, second)
        if ffichecks.isnumber(second) then
            return Vector(self.X + second, self.Y + second)
        elseif ffichecks.isnumber(self) then
            ffichecks.checkcdata(2, second, "Vector")
            return Vector(second.X + self, second.Y + self)
        else
            ffichecks.checkcdata(1, second, "Vector")
            return Vector(self.X + second.X, self.Y + second.Y)
        end
    end,
	__sub = function(self, second)
        if ffichecks.isnumber(second) then
            return Vector(self.X - second, self.Y - second)
        elseif ffichecks.isnumber(self) then
            ffichecks.checkcdata(2, second, "Vector")
            return Vector(self - second.X, self - second.Y)
        else
            ffichecks.checkcdata(1, second, "Vector")
            return Vector(self.X - second.X, self.Y - second.Y)
        end
    end,	
	__mul = function(self, second)
        if ffichecks.isnumber(second) then
            return Vector(self.X * second, self.Y * second)
        elseif ffichecks.isnumber(self) then
            ffichecks.checkcdata(2, second, "Vector")
            return Vector(self * second.X, self * second.Y)
        else
            ffichecks.checkcdata(1, second, "Vector")
            return Vector(self.X * second.X, self.Y * second.Y)
        end
    end,
	__div = function(self, second)
		if ffichecks.isnumber(second) then
			if second == 0 then
				error("Divide by zero")
			end
			return Vector(self.X / second, self.Y / second)
		elseif ffichecks.isnumber(self) then
			ffichecks.checkcdata(2, second, "Vector")
			if second.X == 0 or second.Y == 0 then
				error("Divide by zero")
			end
			return Vector(self / second.X, self / second.Y)
		else
			ffichecks.checkcdata(1, second, "Vector")
			if second.X == 0 or second.Y == 0 then
				error("Divide by zero")
			end
			return Vector(self.X / second.X, self.Y / second.Y)
		end
	end,	
	__unm = function(self)
		return Vector(self.X * -1, self.Y * -1)
	end,

    Clamp = function(self, MinX, MinY, MaxX, MaxY) 
        ffichecks.checknumber(1, MinX)
        ffichecks.checknumber(2, MinY)
        ffichecks.checknumber(3, MaxX)
        ffichecks.checknumber(4, MaxY)
        
        self.X = math.min(math.max(self.X, MinX), MaxX)
        self.Y = math.min(math.max(self.Y, MinY), MaxY)
    end,
    Clamped = function(self, MinX, MinY, MaxX, MaxY)
        ffichecks.checknumber(1, MinX)
        ffichecks.checknumber(2, MinY)
        ffichecks.checknumber(3, MaxX)
        ffichecks.checknumber(4, MaxY)
        
        return Vector(
            math.min(math.max(self.X, MinX), MaxX),
            math.min(math.max(self.Y, MinY), MaxY)
        )
    end,
    Cross = function(self, second)
        ffichecks.checkcdata(1, second, "Vector")
        
        return second.Y * self.X - self.Y * second.X;
    end,
    Distance = function(self, second)
        local distanceSquared = DistanceSquared(self, second)
        
        return math.sqrt(distanceSquared)
    end,
    DistanceSquared = function(self, second)
        return DistanceSquared(self, second)
    end,
    Dot = function(self, second)
        ffichecks.checkcdata(1, second, "Vector")
        
        return second.X * self.X + second.Y * self.Y;
    end,
    GetAngleDegrees = function(self)
        local atan2 = math.atan2(self.Y, self.X)
        return atan2 * 180/math.pi
    end,
    Length = function(self)
        local lengthSquared = self.X * self.X + self.Y * self.Y
	    return math.sqrt(lengthSquared)
    end,
    LengthSquared = function(self)
        return self.X * self.X + self.Y * self.Y
    end,
    Lerp = function(self, m2, t)
        ffichecks.checkcdata(1, m2, "Vector")
        ffichecks.checknumber(2, t)
        
        local it = 1-t
        
        self.X = self.X * it + m2.X * t
        self.Y = self.Y * it + m2.Y * t
    end,
    Normalize = function(self)
        local length = math.sqrt(self.X * self.X + self.Y * self.Y)
        if length > 0 then
            local normalize = 1/length
            self.X = self.X * normalize
            self.Y = self.Y * normalize
        end
    end,
    Normalized = function(self)
        local res = Vector(self.X, self.Y)
        local length = math.sqrt(res.X * res.X + res.Y * res.Y)
        if length > 0 then
            local normalize = 1/length
            res.X = res.X * normalize
            res.Y = res.Y * normalize
        end
        return res
    end,
    Resize = function(self, NewLength)
        ffichecks.checknumber(1, NewLength)

        local length = math.sqrt(self.X * self.X + self.Y * self.Y)
        if length > 0 then
            self.X = self.X * (NewLength / length)
            self.Y = self.Y * (NewLength / length)
        end
    end,
    Resized = function(self, NewLength)
        ffichecks.checknumber(1, NewLength)
        
        local res = Vector(self.X, self.Y)
        local length = math.sqrt(res.X * res.X + res.Y * res.Y)
        if length > 0 then
            res.X = res.X * (NewLength / length)
            res.Y = res.Y * (NewLength / length)
        end
        return res
    end,
    Rotated = function(self, Angle)
        ffichecks.checknumber(1, Angle)
        
        local rads = math.rad(Angle)
        local cos = math.cos(rads)
        local sin = math.sin(rads)
        
        return Vector(self.X * cos - self.Y * sin, self.Y * cos + self.X * sin)
    end
}

VectorMT.__index = VectorMT 

local VectorT = ffi.metatype("struct Vector", VectorMT)

Vector = setmetatable({
    FromAngle = function(angle)
		ffichecks.checknumber(1, angle)
		
		local radians = math.rad(angle)
		return VectorT(math.cos(radians), math.sin(radians))
	end,
    Zero = VectorT(0, 0),
    One = VectorT(1, 1)
}, {
    __call = function(_, X, Y) return VectorT(X or 0, Y or 0) end,
    __class = VectorMT,
})

RandomVector = function()
	local angle = math.random() * (2 * math.pi)
	return Vector(math.cos(angle), math.sin(angle))
end