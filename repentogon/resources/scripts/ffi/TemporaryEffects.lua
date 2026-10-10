ffi.cdef[[
struct EffectList {
    private struct TemporaryEffect* _begin;
    private struct TemporaryEffect* _last;
    private struct TemporaryEffect* _end;
};

struct TemporaryEffects {
    private int CacheFlags;
    private const struct EffectList Effects;
    private bool ShouldEvaluateCache;
    private bool Disable;
    padding char[0x2];
    padding char[0x4]; // EntityPlayer -- we can probably reimplement a lot of TemporaryEffects in Lua once it's finally FFI'd, but C helpers for now
};

typedef struct TemporaryEffects* TemporaryEffectsPtr;

void L_TemporaryEffects_AddCollectibleEffect(struct TemporaryEffects*, int, bool, int);
void L_TemporaryEffects_AddNullEffect(struct TemporaryEffects*, int, bool, int);
void L_TemporaryEffects_AddTrinketEffect(struct TemporaryEffects*, int, bool, int);
void L_TemporaryEffects_ClearEffects(struct TemporaryEffects*);
const struct TemporaryEffect* L_TemporaryEffects_GetCollectibleEffect(struct TemporaryEffects*, int);
int  L_TemporaryEffects_GetCollectibleEffectNum(struct TemporaryEffects*, int);
const struct TemporaryEffect* L_TemporaryEffects_GetNullEffect(struct TemporaryEffects*, int);
int   L_TemporaryEffects_GetNullEffectNum(struct TemporaryEffects*, int);
const struct TemporaryEffect* L_TemporaryEffects_GetTrinketEffect(struct TemporaryEffects*, int);
int   L_TemporaryEffects_GetTrinketEffectNum(struct TemporaryEffects*, int);
bool  L_TemporaryEffects_HasCollectibleEffect(struct TemporaryEffects*, int);
bool  L_TemporaryEffects_HasNullEffect(struct TemporaryEffects*, int);
bool  L_TemporaryEffects_HasTrinketEffect(struct TemporaryEffects*, int);
void  L_TemporaryEffects_RemoveCollectibleEffect(struct TemporaryEffects*, int, int);
void  L_TemporaryEffects_RemoveNullEffect(struct TemporaryEffects*, int, int);
void  L_TemporaryEffects_RemoveTrinketEffect(struct TemporaryEffects*, int, int);
]]

local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_TemporaryEffects_AddCollectibleEffect)
ffi.reentrant(repentogon.L_TemporaryEffects_AddNullEffect)
ffi.reentrant(repentogon.L_TemporaryEffects_AddTrinketEffect)
ffi.reentrant(repentogon.L_TemporaryEffects_RemoveNullEffect)
ffi.reentrant(repentogon.L_TemporaryEffects_RemoveTrinketEffect)

local EffectListMT; EffectListMT = { __type = "EffectList" }

local function el_size(self)
    return ffichecks.vectorsize(ffi.getprivate(self, "_begin"), ffi.getprivate(self, "_last"), ffi.sizeof("struct TemporaryEffect"))
end

EffectListMT.__len = function(self) return el_size(self) end

EffectListMT.__index = function(self, k)
    if k == "Size" then return el_size(self) end
    return EffectListMT[k]
end

function EffectListMT:Get(idx)
    idx = ffichecks.checknumber(2, idx)
    local n = el_size(self)
    if idx < 0 or idx >= n then return nil end
    local result = ffi.cast("ConstTemporaryEffect*", ffi.getprivate(self, "_begin") + idx) return result
end

ffi.metatype("struct EffectList", EffectListMT)
EffectList = setmetatable({}, { __class = EffectListMT })

------------------

local TemporaryEffectsMT
TemporaryEffectsMT = {
    __type = "TemporaryEffects",
    AddCollectibleEffect = function(self, CollectibleType, AddCostume, Count)
        CollectibleType = ffichecks.checkinteger(1, CollectibleType)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddCollectibleEffect(self, CollectibleType, AddCostume, Count)
    end,
    AddNullEffect = function(self, NullId, AddCostume, Count)
        NullId = ffichecks.checkinteger(1, NullId)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddNullEffect(self, NullId, AddCostume, Count)
    end,
    AddTrinketEffect = function(self, TrinketType, AddCostume, Count)
        TrinketType = ffichecks.checkinteger(1, TrinketType)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddTrinketEffect(self, TrinketType, AddCostume, Count)
    end,
    ClearEffects = function(self)
        repentogon.L_TemporaryEffects_ClearEffects(self)
    end,
    GetCollectibleEffect = function(self, CollectibleType)
        CollectibleType = ffichecks.checkinteger(1, CollectibleType)
        local effect = repentogon.L_TemporaryEffects_GetCollectibleEffect(self, CollectibleType)
        if effect == nil then return nil end
        return effect
    end,
    GetCollectibleEffectNum = function(self, CollectibleType)
        CollectibleType = ffichecks.checkinteger(1, CollectibleType)
        local result = repentogon.L_TemporaryEffects_GetCollectibleEffectNum(self, CollectibleType) return result
    end,
    GetEffectsList = function(self) local result = ffi.getprivate(self, "Effects") return result end,
    GetNullEffect = function(self, NullId)
        NullId = ffichecks.checkinteger(1, NullId)
        local effect = repentogon.L_TemporaryEffects_GetNullEffect(self, NullId)
        if effect == nil then return nil end
        return effect
    end,
    GetNullEffectNum = function(self, NullId)
        NullId = ffichecks.checkinteger(1, NullId)
        local result = repentogon.L_TemporaryEffects_GetNullEffectNum(self, NullId) return result
    end,
    GetTrinketEffect = function(self, TrinketType)
        TrinketType = ffichecks.checkinteger(1, TrinketType)
        local effect = repentogon.L_TemporaryEffects_GetTrinketEffect(self, TrinketType)
        if effect == nil then return nil end
        return effect
    end,
    GetTrinketEffectNum = function(self, TrinketType)
        TrinketType = ffichecks.checkinteger(1, TrinketType)
        local result = repentogon.L_TemporaryEffects_GetTrinketEffectNum(self, TrinketType) return result
    end,
    HasCollectibleEffect = function(self, CollectibleType)
        CollectibleType = ffichecks.checkinteger(1, CollectibleType)
        local result = repentogon.L_TemporaryEffects_HasCollectibleEffect(self, CollectibleType) return result
    end,
    HasNullEffect = function(self, NullId)
        NullId = ffichecks.checkinteger(1, NullId)
        local result = repentogon.L_TemporaryEffects_HasNullEffect(self, NullId) return result
    end,
    HasTrinketEffect = function(self, TrinketType)
        TrinketType = ffichecks.checkinteger(1, TrinketType)
        local result = repentogon.L_TemporaryEffects_HasTrinketEffect(self, TrinketType) return result
    end,
    RemoveCollectibleEffect = function(self, CollectibleType, Count)
        CollectibleType = ffichecks.checkinteger(1, CollectibleType)
        Count = ffichecks.optnumber(Count, 1)
        repentogon.L_TemporaryEffects_RemoveCollectibleEffect(self, CollectibleType, Count) return
    end,
    RemoveNullEffect = function(self, NullId, Count)
        NullId = ffichecks.checkinteger(1, NullId)
        Count = ffichecks.optnumber(Count, 1)
        repentogon.L_TemporaryEffects_RemoveNullEffect(self, NullId, Count) return
    end,
    RemoveTrinketEffect = function(self, TrinketType, Count)
        TrinketType = ffichecks.checkinteger(1, TrinketType)
        Count = ffichecks.optnumber(Count, 1)
        repentogon.L_TemporaryEffects_RemoveTrinketEffect(self, TrinketType, Count) return
    end,
}

setmetatable(TemporaryEffectsMT, {
    __index = function() end
})
TemporaryEffectsMT.__index = TemporaryEffectsMT

local TemporaryEffectsT = ffi.metatype("struct TemporaryEffects", TemporaryEffectsMT)

TemporaryEffects = setmetatable({}, {
    __class = TemporaryEffectsMT,
})