ffi.cdef[[
typedef struct {
	unsigned int _seed;
	unsigned int _shift[3];
} RNG;

void L_RNG_SetSeed(RNG*, unsigned int, unsigned int);
unsigned int L_RNG_RandomInt(RNG*, unsigned int);
float L_RNG_RandomFloat(RNG*);
unsigned int L_RNG_Next(RNG*);
]]

local repentogon = ffidll
local lffi = ffi

RNGFuncs = {}
local MAX_SHIFT_IDX = 80

function RNGFuncs:SetSeed(seed, shiftIdx)
	shiftIdx = shiftIdx or 35
	ffichecks.checknumber(2, seed)
	ffichecks.checknumber(3, shiftIdx)
	if seed <= 0 then
		error("RNG: Seed must be above 0", 2)
	end
	if shiftIdx < 0 or shiftIdx > MAX_SHIFT_IDX then
		error(string.format("RNG: Invalid ShiftIdx %d (must be between 0 and %d)", shiftIdx, MAX_SHIFT_IDX), 2)
	end
	
	repentogon.L_RNG_SetSeed(self, seed, shiftIdx)
end

function RNGFuncs:GetSeed()
	return self._seed
end

function RNGFuncs:RandomInt(max)
	if max then
		checknumber(2, max)
		return repentogon.L_RNG_RandomInt(self, max)
	else
		return repentogon.L_RNG_Next(self)
	end
end

function RNGFuncs:RandomFloat()
	return repentogon.L_RNG_RandomFloat(self)
end

function RNGFuncs:Next()
	return repentogon.L_RNG_Next(self)
end

local RNGMT
RNGMT = lffi.metatype("RNG", {
	__tostring = function(a)
		return "RNG"
	end;
	__call = function(a, max)
		if max then
			checknumber(2, max)
			return repentogon.L_RNG_RandomInt(a, max)
		else
			return repentogon.L_RNG_Next(a)
		end
	end;
	__index = RNGFuncs;
})

RNG = setmetatable({},
{
	-- Constructor
	__call = function(_, seed, shiftIdx)
		local rng = RNGMT()
		if seed then
			shiftIdx = shiftIdx or 35
			ffichecks.checknumber(1, seed)
			ffichecks.checknumber(2, shiftIdx)
			if seed <= 0 then
				error("RNG: Seed must be above 0", 2)
			end
			if shiftIdx < 0 or shiftIdx > MAX_SHIFT_IDX then
				error(string.format("RNG: Invalid ShiftIdx %d (must be between 0 and %d)", shiftIdx, MAX_SHIFT_IDX), 2)
			end
			repentogon.L_RNG_SetSeed(rng, seed, shiftIdx)
		else
			repentogon.L_RNG_SetSeed(rng, 2853650767, 35)
		end
		return rng
	end
})



