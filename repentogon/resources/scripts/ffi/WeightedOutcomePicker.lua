ffi.cdef[[
typedef struct {
    unsigned int value;
    unsigned int weight;
} Outcome;

typedef struct {
    Outcome** outcomes;
    int length;
} FFI_OutcomeList;

typedef struct {
    unsigned int* _begin;
    unsigned int* _last;
    unsigned int* _end;
} WeightedOutcomePicker;

void L_WeightedOutcomePicker_AddOutcomeWeight(WeightedOutcomePicker*, unsigned int, unsigned int);
void L_WeightedOutcomePicker_AddOutcomeFloat(WeightedOutcomePicker*, unsigned int, float, int);
void L_WeightedOutcomePicker_ClearOutcomes(WeightedOutcomePicker*);
WeightedOutcomePicker* L_WeightedOutcomePicker_Constructor();
void L_WeightedOutcomePicker_Free(WeightedOutcomePicker*);
unsigned int L_WeightedOutcomePicker_GetNumOutcomes(WeightedOutcomePicker*);
FFI_OutcomeList L_WeightedOutcomePicker_GetOutcomes(WeightedOutcomePicker*);
unsigned int L_WeightedOutcomePicker_PickOutcome(WeightedOutcomePicker*, RNG*);
void L_WeightedOutcomePicker_RemoveOutcome(WeightedOutcomePicker*, unsigned int);

void FreeOutcomeList(FFI_OutcomeList);
]]

local repentogon = ffidll
local lffi = ffi

local WeightedOutcomePickerFuncs = {}

function WeightedOutcomePickerFuncs:AddOutcomeWeight(value, weight)
    ffichecks.checknumber(2, value)
    ffichecks.checknumber(3, weight)

    repentogon.L_WeightedOutcomePicker_AddOutcomeWeight(self, value, weight)
end

function WeightedOutcomePickerFuncs:AddOutcomeFloat(value, weight, scaleFactor)
    scaleFactor = scaleFactor or 100

    ffichecks.checknumber(2, value)
    ffichecks.checknumber(3, weight)
    ffichecks.checknumber(4, scaleFactor)

    repentogon.L_WeightedOutcomePicker_AddOutcomeFloat(self, value, weight, scaleFactor)
end

function WeightedOutcomePickerFuncs:ClearOutcomes()
    repentogon.L_WeightedOutcomePicker_ClearOutcomes(self)
end

function WeightedOutcomePickerFuncs:GetNumOutcomes()
    return repentogon.L_WeightedOutcomePicker_GetNumOutcomes(self)
end

function WeightedOutcomePickerFuncs:GetOutcomes()
    local outcomeList = repentogon.L_WeightedOutcomePicker_GetOutcomes(self)

    local outcomesTable = {}
    for i = 0, outcomeList.length - 1 do 
        local outcome = outcomeList.outcomes[i][0]
        local outcomeTable = {}

        outcomeTable.Value = outcome.value
        outcomeTable.Weight = outcome.weight

        table.insert(outcomesTable, outcomeTable)
    end

    repentogon.FreeOutcomeList(outcomeList)

    return outcomesTable
end

function WeightedOutcomePickerFuncs:PickOutcome(rng)
    ffichecks.checkcdata(2, rng, "RNG")
    return repentogon.L_WeightedOutcomePicker_PickOutcome(self, rng)
end

function WeightedOutcomePickerFuncs:RemoveOutcome(value)
    ffichecks.checknumber(2, value)
    repentogon.L_WeightedOutcomePicker_RemoveOutcome(self, value)
end

local WeightedOutcomePickerMT = lffi.metatype("WeightedOutcomePicker", {
    __tostring = function(self) 
        return "WeightedOutcomePicker[" .. repentogon.L_WeightedOutcomePicker_GetNumOutcomes(self) .. "]" 
    end,
    __call = function() 
        return lffi.gc(repentogon.L_WeightedOutcomePicker_Constructor(), repentogon.L_WeightedOutcomePicker_Free) 
    end,
    __index = WeightedOutcomePickerFuncs
})

WeightedOutcomePicker = setmetatable({}, {
    __call = function()
        return WeightedOutcomePickerMT()
    end
})