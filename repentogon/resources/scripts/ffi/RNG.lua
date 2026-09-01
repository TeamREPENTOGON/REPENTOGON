ffi.cdef[[
    struct RNG
    {
        unsigned int _seed;
        padding char [0xc];
    };
    typedef struct RNG* RNGPtr;

    const uint32_t L_RNG_Next(struct RNG*);
    const float L_RNG_RandomFloat(struct RNG*);
    const float L_RNG_RandomInt(struct RNG*, uint32_t max);
    const float L_RNG_SetSeed(struct RNG*, uint32_t seed, uint32_t shiftIdx);
    const int L_RNG_GetShiftIdx(struct RNG*);
    const uint32_t L_RNG_Previous(struct RNG*);
    void L_RNG_RandomVector(struct RNG*, bool phantom, struct Vector* out);
]]

local MAX_SHIFT_IDX = 80
local RANDOM_UNIT_VECTOR_ROW_IDX = 18
local RNG_ZERO_SEED_ERR_STR = "RNG Seed is zero!"
local RNG_INVALID_SHIFT_IDX_ERR_STR = "Invalid RNG ShiftIdx (must be between 0 and %d)"
RNG_INVALID_SHIFT_IDX_ERR_STR = RNG_INVALID_SHIFT_IDX_ERR_STR:format(MAX_SHIFT_IDX)

local L_RNG_Next = ffidll.L_RNG_Next
local L_RNG_RandomFloat = ffidll.L_RNG_RandomFloat
local L_RNG_RandomInt = ffidll.L_RNG_RandomInt
local L_RNG_SetSeed = ffidll.L_RNG_SetSeed
local L_RNG_GetShiftIdx = ffidll.L_RNG_GetShiftIdx
local L_RNG_Previous = ffidll.L_RNG_Previous
local L_RNG_RandomVector = ffidll.L_RNG_RandomVector

local ffi = ffi

local RngMT = {
    __type = "RNG",

    GetSeed = function(self)
        return self._seed
    end,

    Next = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end
        
        return L_RNG_Next(self)
    end,

    RandomFloat = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        return L_RNG_RandomFloat(self)
    end,

    PhantomFloat = function(self)
        local oldSeed = self._seed
        local res = L_RNG_RandomFloat(self)
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
            return L_RNG_RandomInt(self, min)
        else
            if min > max then
                error("Interval is empty")
            else
                local interval = max - min
                return min + L_RNG_RandomInt(self, interval + 1)
            end
        end
    end,

    PhantomInt = function(self, min, max)
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
        L_RNG_SetSeed(self, seed, shiftIdx)
    end,

    GetShiftIdx = function(self)
        local res = L_RNG_GetShiftIdx(self)

        if res >= 0 then
            return res
        end
    end,

    Previous = function(self)
        return L_RNG_Previous(self)
    end,

    PhantomPrevious = function(self)
        local oldSeed = self._seed
        local res = L_RNG_Previous(self)
        self._seed = oldSeed
        return res
    end,

    RandomVector = function(self)
        local result = ffi.new("struct Vector")
        L_RNG_RandomVector(self, false, result)
        return result
    end,

    PhantomVector = function(self)
        local result = ffi.new("struct Vector")
        L_RNG_RandomVector(self, true, result)
        return result
    end,

    PhantomNext = function(self)
        if self._seed == 0 then
            error(RNG_ZERO_SEED_ERR_STR)
        end

        local oldSeed = self._seed
        local res = L_RNG_Next(self)
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
        L_RNG_SetSeed(newRNG, seed, shiftIdx)
        return newRNG
    end
})