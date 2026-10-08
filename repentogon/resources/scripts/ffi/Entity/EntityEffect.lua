local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityEffect { " .. Entity.Fields .. [[
    float m_Height : 0x410;
    float FallingSpeed : 0x414;
    float FallingAcceleration : 0x418;
    private struct BitSet128 VarDataValue : 0x420;
    int State : 0x450;
    float MinRadius : 0x454;
    float MaxRadius : 0x458;
    int Timeout : 0x45c;
    int LifeSpan : 0x460;
    private bool IsFollowingValue : 0x464;
    struct Vector ParentOffset : 0x468;
    int DamageSource : 0x470;
    float Rotation : 0x474;
    float Scale : 0x478;
} : 0x490;
typedef struct EntityEffect* EntityEffectPtr;
]])

ffi.cdef [[
    void L_EntityEffect_FollowParent(struct EntityEffect*, void*);
    struct EntityEffect* L_EntityEffect_CreateLight(struct Vector*, float, int, int, struct Color*);
    struct EntityEffect* L_EntityEffect_CreateLootPreview(struct LootList*, struct Vector*, void*, void*);
    struct GridEntityDesc* L_EntityEffect_GetGridEntityDesc(struct EntityEffect*);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter
local CopyStruct = helpers.CopyStruct
local EntityToPointer = ffichecks.entitytopointer

local TYPE_EFFECT = 1000
local effectType = ffi.typeof("struct EntityEffect*")

local TEARFLAG_VARIANTS = {
    [54] = true,  -- PLAYER_CREEP_HOLYWATER_TRAIL (Aquarius)
    [113] = true, -- BRIMSTONE_BALL
    [126] = true, -- TECH_DOT
    [167] = true, -- CHAIN_LIGHTNING
}

local PLAYER_CREEP_VARIANTS = {
    [0x20] = true, [0x25] = true, [0x2c] = true, [0x2d] = true, [0x2e] = true, [0x35] = true,
    [0x36] = true, [0x4e] = true, [0x5a] = true, [0x5c] = true, [0x5d] = true, [0xcc] = true,
}

local function CanAccessTearFlags(self)
    return TEARFLAG_VARIANTS[self.Variant] == true
end

local getters = {
    IsFollowing = Getter("IsFollowingValue"),
}

local setters = {
    IsFollowing = function(self, value)
        ffi.setprivate(self, "IsFollowingValue", not not value)
    end,
}

local methods = {
    AddTearFlags = function(self, flags)
        ffichecks.checkcdata(1, flags, "BitSet128")
        if CanAccessTearFlags(self) then
            ffi.setprivate(self, "VarDataValue", ffi.getprivate(self, "VarDataValue") | flags)
        end
    end,
    ClearTearFlags = function(self, flags)
        ffichecks.checkcdata(1, flags, "BitSet128")
        if CanAccessTearFlags(self) then
            ffi.setprivate(self, "VarDataValue", ffi.getprivate(self, "VarDataValue") & ~flags)
        end
    end,
    FollowParent = function(self, parent)
        repentogon.L_EntityEffect_FollowParent(self, EntityToPointer(parent))
    end,
    GetGridEntityDesc = function(self)
        local result = repentogon.L_EntityEffect_GetGridEntityDesc(self) return result
    end,
    GetTearFlags = function(self)
        if CanAccessTearFlags(self) then
            return CopyStruct("struct BitSet128", ffi.getprivate(self, "VarDataValue"))
        end
        return nil
    end,
    HasTearFlags = function(self, flags)
        ffichecks.checkcdata(1, flags, "BitSet128")
        return CanAccessTearFlags(self) and (ffi.getprivate(self, "VarDataValue") & flags) ~= TearFlags.TEAR_NORMAL
    end,
    SetDamageSource = function(self, source)
        source = ffichecks.checkinteger(1, source)
        self.DamageSource = source
    end,
    SetRadii = function(self, minRadius, maxRadius)
        minRadius = ffichecks.checknumber(1, minRadius)
        maxRadius = ffichecks.checknumber(2, maxRadius)
        self.MinRadius = minRadius
        self.MaxRadius = maxRadius
    end,
    SetTearFlags = function(self, flags)
        ffichecks.checkcdata(1, flags, "BitSet128")
        if CanAccessTearFlags(self) then
            ffi.setprivate(self, "VarDataValue", flags)
        end
    end,
    SetTimeout = function(self, timeout)
        timeout = ffichecks.checkinteger(1, timeout)
        self.Timeout = timeout
        self.LifeSpan = timeout
    end,
}

methods.AddAquariusTearFlags = methods.AddTearFlags
methods.ClearAquariusTearFlags = methods.ClearTearFlags
methods.GetAquariusTearFlags = methods.GetTearFlags
methods.HasAquariusTearFlags = methods.HasTearFlags
methods.SetAquariusTearFlags = methods.SetTearFlags

local EffectMT = Entity.Inherit("EntityEffect", methods, getters, setters)
ffi.metatype("struct EntityEffect", EffectMT)
Entity.SetClassType(TYPE_EFFECT, effectType)

ffichecks.pointertoeffect = function(pointer)
    if pointer == nil then
        return nil
    end
    local result = ffi.cast(effectType, pointer) return result
end

EntityEffect = setmetatable({
    CreateLight = function(position, scale, lifespan, state, color)
        ffichecks.checkcdata(1, position, "Vector")
        scale = ffichecks.optnumber(scale, math.random())
        lifespan = ffichecks.optnumber(lifespan, -1)
        state = ffichecks.optnumber(state, 6)
        if color ~= nil and type(color) == "cdata" then
            ffichecks.checkcdata(5, color, "Color")
        else
            color = nil
        end
        local result = repentogon.L_EntityEffect_CreateLight(position, scale, lifespan, state, color) return result
    end,
    CreateLootPreview = function(lootList, position, owner, effect)
        ffichecks.checkcdata(1, lootList, "LootList")
        ffichecks.checkcdata(2, position, "Vector")
        local result = repentogon.L_EntityEffect_CreateLootPreview(lootList, position, ffichecks.checkentity(3, owner), ffichecks.checkentity(4, effect)) return result
    end,
    IsPlayerCreep = function(variant)
        variant = ffichecks.checkinteger(1, variant)
        return PLAYER_CREEP_VARIANTS[variant] == true
    end,
}, { __class = EffectMT })
