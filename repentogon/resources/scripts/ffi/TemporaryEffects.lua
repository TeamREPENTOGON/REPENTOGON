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

local EffectListMT; EffectListMT = { __type = "EffectList" }

local function el_size(self)
    return tonumber(ffi.getprivate(self, "_last") - ffi.getprivate(self, "_begin"))
end

EffectListMT.__len = function(self) return el_size(self) end

EffectListMT.__index = function(self, k)
    if k == "Size" then return el_size(self) end
    return EffectListMT[k]
end

function EffectListMT:Get(idx)
    ffichecks.checknumber(2, idx)
    local n = el_size(self)
    if idx < 0 or idx >= n then return nil end
    return ffi.cast("ConstTemporaryEffect*", ffi.getprivate(self, "_begin") + idx)
end

ffi.metatype("struct EffectList", EffectListMT)
EffectList = setmetatable({}, { __class = EffectListMT })

------------------

local TemporaryEffectsMT
TemporaryEffectsMT = {
    __type = "TemporaryEffects",
    AddCollectibleEffect = function(self, CollectibleType, AddCostume, Count)
        ffichecks.checkinteger(1, CollectibleType)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddCollectibleEffect(self, CollectibleType, AddCostume, Count)
    end,
    AddNullEffect = function(self, NullId, AddCostume, Count)
        ffichecks.checkinteger(1, NullId)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddNullEffect(self, NullId, AddCostume, Count)
    end,
    AddTrinketEffect = function(self, TrinketType, AddCostume, Count)
        ffichecks.checkinteger(1, TrinketType)
        AddCostume = ffichecks.optboolean(AddCostume, true)
        Count = ffichecks.optnumber(Count, 1)

        repentogon.L_TemporaryEffects_AddTrinketEffect(self, TrinketType, AddCostume, Count)
    end,
    ClearEffects = function(self)
        repentogon.L_TemporaryEffects_ClearEffects(self)
    end,
    GetCollectibleEffect = function(self, CollectibleType)
        ffichecks.checkinteger(1, CollectibleType)
        return ffichecks.fixreturn(repentogon.L_TemporaryEffects_GetCollectibleEffect(self, CollectibleType))
    end,
    GetCollectibleEffectNum = function(self, CollectibleType)
        ffichecks.checkinteger(1, CollectibleType)
        return repentogon.L_TemporaryEffects_GetCollectibleEffectNum(self, CollectibleType)
    end,
    GetEffectsList = function(self) return ffi.getprivate(self, "Effects") end,
    GetNullEffect = function(self, NullId)
        ffichecks.checkinteger(1, NullId)
        return ffichecks.fixreturn(repentogon.L_TemporaryEffects_GetNullEffect(self, NullId))
    end,
    GetNullEffectNum = function(self, NullId)
        ffichecks.checkinteger(1, NullId)
        return repentogon.L_TemporaryEffects_GetNullEffectNum(self, NullId)
    end,
    GetTrinketEffect = function(self, TrinketType)
        ffichecks.checkinteger(1, TrinketType)
        return ffichecks.fixreturn(repentogon.L_TemporaryEffects_GetTrinketEffect(self, TrinketType))
    end,
    GetTrinketEffectNum = function(self, TrinketType)
        ffichecks.checkinteger(1, TrinketType)
        return repentogon.L_TemporaryEffects_GetTrinketEffectNum(self, TrinketType)
    end,
    HasCollectibleEffect = function(self, CollectibleType)
        ffichecks.checkinteger(1, CollectibleType)
        return repentogon.L_TemporaryEffects_HasCollectibleEffect(self, CollectibleType)
    end,
    HasNullEffect = function(self, NullId)
        ffichecks.checkinteger(1, NullId)
        return repentogon.L_TemporaryEffects_HasNullEffect(self, NullId)
    end,
    HasTrinketEffect = function(self, TrinketType)
        ffichecks.checkinteger(1, TrinketType)
        return repentogon.L_TemporaryEffects_HasTrinketEffect(self, TrinketType)
    end,
    RemoveCollectibleEffect = function(self, CollectibleType, Count)
        ffichecks.checkinteger(1, CollectibleType)
        Count = ffichecks.optnumber(Count, 1)
        return repentogon.L_TemporaryEffects_RemoveCollectibleEffect(self, CollectibleType, Count)
    end,
    RemoveNullEffect = function(self, NullId, Count)
        ffichecks.checkinteger(1, NullId)
        Count = ffichecks.optnumber(Count, 1)
        return repentogon.L_TemporaryEffects_RemoveNullEffect(self, NullId, Count)
    end,
    RemoveTrinketEffect = function(self, TrinketType, Count)
        ffichecks.checkinteger(1, TrinketType)
        Count = ffichecks.optnumber(Count, 1)
        return repentogon.L_TemporaryEffects_RemoveTrinketEffect(self, TrinketType, Count)
    end,
}

setmetatable(TemporaryEffectsMT, {
    __index = function() end,
    __call = function(t, _, k) return t[k] end, -- Thank you APIOverride, very cool!
})
TemporaryEffectsMT.__index = TemporaryEffectsMT

local TemporaryEffectsT = ffi.metatype("struct TemporaryEffects", TemporaryEffectsMT)

TemporaryEffects = setmetatable({}, {
    __class = TemporaryEffectsMT,
})