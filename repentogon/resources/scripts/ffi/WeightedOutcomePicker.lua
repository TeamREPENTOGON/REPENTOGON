ffi.cdef[[
struct WeightedOutcomePicker_Outcome {
    uint32_t Value;
    uint32_t Weight;
};

struct WeightedOutcomePicker {
    private struct WeightedOutcomePicker_Outcome* _first;
    private struct WeightedOutcomePicker_Outcome* _last;
    private struct WeightedOutcomePicker_Outcome* _end;
};

typedef struct WeightedOutcomePicker* WeightedOutcomePickerPtr;

struct WeightedOutcomePicker* L_WeightedOutcomePicker_New();
void L_WeightedOutcomePicker_Delete(struct WeightedOutcomePicker*);
void L_WeightedOutcomePicker_AddOutcomeWeight(struct WeightedOutcomePicker*, struct WeightedOutcomePicker_Outcome*);
uint32_t L_WeightedOutcomePicker_PickOutcome(struct WeightedOutcomePicker*, struct RNG*);
void L_WeightedOutcomePicker_RemoveOutcome(struct WeightedOutcomePicker* self, uint32_t value);
]]

local ffi = ffi
local repentogon = ffidll

local WeightedOutcomePickerMT
WeightedOutcomePickerMT = {
    __type = "WeightedOutcomePicker",
    __gc = function(self)
        repentogon.L_WeightedOutcomePicker_Delete(self)
    end,
    __len = function(self)
        local first = ffi.cast("const char*", ffi.getprivate(self, "_first"))
        local last = ffi.cast("const char*", ffi.getprivate(self, "_last"))
        return tonumber(last - first) // ffi.sizeof("struct WeightedOutcomePicker_Outcome")
    end,
    AddOutcomeFloat = function(self, value, weight, scaleFactor)
        ffichecks.checkinteger(1, value)
        ffichecks.checknumber(2, weight)
        scaleFactor = ffichecks.optnumber(scaleFactor, 100)
        local outcome = ffi.new("struct WeightedOutcomePicker_Outcome", { value, math.floor(weight * scaleFactor) })
        repentogon.L_WeightedOutcomePicker_AddOutcomeWeight(self, outcome)
    end,
    AddOutcomeWeight = function(self, value, weight)
        ffichecks.checkinteger(1, value)
        ffichecks.checkinteger(2, weight)
        local outcome = ffi.new("struct WeightedOutcomePicker_Outcome", { value, weight })
        repentogon.L_WeightedOutcomePicker_AddOutcomeWeight(self, outcome)
    end,
    ClearOutcomes = function(self)
        ffi.setprivate(self, "_last", ffi.getprivate(self, "_first"))
    end,
    GetNumOutcomes = function(self) return #self end,
    GetOutcomes = function(self)
        local ret = {}
        for i = 0, #self - 1 do
            table.insert(ret, ffi.getprivate(self, "_first")[i])
        end
        return ret
    end,
    PickOutcome = function(self, rng)
        ffichecks.checkcdata(1, rng, "RNG")
        local result = repentogon.L_WeightedOutcomePicker_PickOutcome(self, rng)
        rng:Next()
        return result
    end,
    RemoveOutcome = function(self, value)
        ffichecks.checkinteger(1, value)
        repentogon.L_WeightedOutcomePicker_RemoveOutcome(self, value)
    end,
}

setmetatable(WeightedOutcomePickerMT, {
    __index = function() end,
})
WeightedOutcomePickerMT.__index = WeightedOutcomePickerMT

local WeightedOutcomePickerT = ffi.metatype("struct WeightedOutcomePicker", WeightedOutcomePickerMT)
    
WeightedOutcomePicker = setmetatable({}, {
        __class = WeightedOutcomePickerMT,
        __call = function()
            return ffi.gc(repentogon.L_WeightedOutcomePicker_New(), WeightedOutcomePickerMT.__gc)
        end,
})
