ffi.cdef[[
    struct RNG
    {
        unsigned int _seed;
        unsigned int _shift1;
        unsigned int _shift2;
        unsigned int _shift3;
    };
    typedef struct RNG* RNGPtr;

    const uint32_t* RNG_GetShiftsTable();
]]

local MAX_SHIFT_IDX = 80
local RANDOM_UNIT_VECTOR_ROW_IDX = 18
local NUM_SHIFT_INDEXES = MAX_SHIFT_IDX + 1
local RNG_ZERO_SEED_ERR_STR = "RNG Seed is zero!"
local RNG_INVALID_SHIFT_IDX_ERR_STR = "Invalid RNG ShiftIdx (must be between 0 and %d)"
RNG_INVALID_SHIFT_IDX_ERR_STR = RNG_INVALID_SHIFT_IDX_ERR_STR:format(MAX_SHIFT_IDX)

local ffi = ffi
local sin = math.sin
local cos = math.cos

-- Lua uses 64-bit floats to represent numbers. But internally, `unsigned int` is 32 bits.
-- We need this mask to keep shifting behaviro defined.
local UINT_32_MASK = 0xFFFFFFFF

-- From the decomp the constant is 0x2F7FFFFE before a MULSS instruction is performed.
-- The exact value is derived using the IEEE-754 formula.
local INT_TO_RAND_FLOAT = 2.3283061589829401e-10

-- For precision (this is what the decomp uses, same method as `INT_TO_RAND_FLOAT`).
-- math.pi gives widely different digits after `3.14`.
local PI = 3.140000104904175

local s_Shifts = ffi.cast("const uint32_t(*)[3]", ffidll.RNG_GetShiftsTable())

local function PerformSeedShift(self)
    local newSeed = self._seed
    newSeed = newSeed >> (self._shift1 & 0x1f) ~ newSeed
    newSeed = (newSeed << (self._shift2 & 0x1f) ~ newSeed) & UINT_32_MASK
    newSeed = newSeed >> (self._shift3 & 0x1f) ~ newSeed
    return newSeed
end


local function DoRandomInt(self, max)
    local newSeed = PerformSeedShift(self)
    self._seed = newSeed

    max = ffi.cast("uint32_t", max)

    if max == 0 then
        return 0
    end

    return tonumber(newSeed % max)
end

local function ReverseShrXor(result, shift)
    local op1 = 0
    local op2 = 0

    local loops = math.ceil(32 / shift)

    for i = 1, loops do
        op1 = op2 >> shift
        op2 = op1 ~ result
    end

    return op2
end

local function ReverseShlXor(result, shift)
    local op1 = 0
    local op2 = 0

    local loops = math.ceil(32 / shift)

    for i = 1, loops do
        op1 = (op2 << shift) & UINT_32_MASK
        op2 = op1 ~ result
    end

    return op2
end

local RngMT = {
    __type = "RNG",

    GetSeed = function(self)
        return self._seed
    end,

    Next = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        self._seed = PerformSeedShift(self)
        return self._seed
    end,

    RandomFloat = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        local newSeed = PerformSeedShift(self)
        self._seed = newSeed
        
        -- Needed cuz in the ASM there's a CVTPD2PS call, which rounds the seed to a 32-bit float
        -- before performing a MULSS instruction. Probably overkill but RNG parity between vanilla
        -- mods and REPENTOGON is a must.
        local seed32 = tonumber(ffi.cast("float", newSeed))
        return tonumber(ffi.cast("float", seed32 * INT_TO_RAND_FLOAT))
    end,

    PhantomFloat = function(self)
        local oldSeed = self._seed
        local res = self:RandomFloat()
        self._seed = oldSeed
        return res
    end,

    RandomInt = function(self, min, max)
		ffichecks.checkinteger(1, min)

        if max ~= nil then
		    ffichecks.checkinteger(2, max)
        end

        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        if max == nil then
            return DoRandomInt(self, min)
        else
            if min > max then
                error("Interval is empty")
            else
                local interval = max - min
                return min + DoRandomInt(self, interval + 1)
            end
        end
    end,

    PhantomInt = function(self, min, max)
		ffichecks.checkinteger(1, min)

        if max ~= nil then
		    ffichecks.checkinteger(2, max)
        end

        local oldSeed = self._seed
        local res = self:RandomInt(min, max)
        self._seed = oldSeed
        return res
    end,

    SetSeed = function(self, seed, shiftIdx)
        if shiftIdx == nil then
            shiftIdx = 35
        end

		ffichecks.checkinteger(1, seed)
		ffichecks.checkinteger(2, shiftIdx)

        -- The vanilla API accepts negative values but because it's unsigned, so a wraparound is performed.
        -- This preserves the behavior in case mods use a negative seed for some weird cursed reason
        seed = seed & UINT_32_MASK
        shiftIdx = shiftIdx & UINT_32_MASK

        if seed == 0 then
            error("RNG seed must be an integer above 0")
        end

        if shiftIdx > MAX_SHIFT_IDX then
            error("RNG shift index must be between 0 and " .. MAX_SHIFT_IDX .. " (inclusive)")
        end

        self._seed = seed
        self._shift1 = s_Shifts[shiftIdx][0]
        self._shift2 = s_Shifts[shiftIdx][1]
        self._shift3 = s_Shifts[shiftIdx][2]
    end,

    GetShiftIdx = function(self)
        for i = 0, NUM_SHIFT_INDEXES - 1 do
            local shifts = s_Shifts[i]

            if self._shift1 == shifts[0] and self._shift2 == shifts[1] and self._shift3 == shifts[2] then
                return i
            end
        end

        return nil
    end,

    Previous = function(self)
	    local newSeed = ReverseShrXor(self._seed, self._shift3)
	    newSeed = ReverseShlXor(newSeed, self._shift2)
        newSeed = ReverseShrXor(newSeed, self._shift1)
        self._seed = newSeed
        return newSeed
    end,

    PhantomPrevious = function(self)
        local oldSeed = self._seed
        local res = self:Previous()
        self._seed = oldSeed
        return res
    end,

    RandomVector = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        local newSeed = self._seed
        newSeed = newSeed >> (s_Shifts[RANDOM_UNIT_VECTOR_ROW_IDX][0] & 0x1f) ~ newSeed
        newSeed = (newSeed << (s_Shifts[RANDOM_UNIT_VECTOR_ROW_IDX][1] & 0x1f) ~ newSeed) & UINT_32_MASK
        newSeed = newSeed >> (s_Shifts[RANDOM_UNIT_VECTOR_ROW_IDX][2] & 0x1f) ~ newSeed
           
        -- Ditto (same ASM)
        local seedF32 = tonumber(ffi.cast("float", newSeed))
        local angle = tonumber(ffi.cast("float", seedF32 * INT_TO_RAND_FLOAT))
        angle = tonumber(ffi.cast("float", angle * PI)) * 2

        self:Next()

        return Vector(cos(angle), sin(angle))
    end,

    PhantomVector = function(self)
        local oldSeed = self._seed
        local res = self:RandomVector()
        self._seed = oldSeed
        return res
    end,

    PhantomNext = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        local oldSeed = self._seed
        local res = self:Next()
        self._seed = oldSeed
        return res
    end
}

RngMT.__index = function(self, key)
	return RngMT[key]
end

local RngT = ffi.metatype("struct RNG", RngMT)
RNGF = setmetatable({}, {
    __index = RngMT,
    __call = function(_, seed, shiftIdx)
        if seed == nil then
            seed = 2853650767
        end

        if shiftIdx == nil then
            shiftIdx = 35
        end

		ffichecks.checkinteger(1, seed)
		ffichecks.checkinteger(2, shiftIdx)
        
        if seed == 0 then
            error("Invalid seed 0 for RNG object")
        end

        local newRNG = RngT()
        newRNG._seed = seed

        if shiftIdx < 0 or shiftIdx > MAX_SHIFT_IDX then
            error("Invalid shift index " .. shiftIdx .. " for RNG object")
        end

        newRNG:SetSeed(seed, shiftIdx)

        return newRNG
    end
})