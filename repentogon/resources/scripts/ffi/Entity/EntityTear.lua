local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityTear { " .. Entity.Fields .. [[
    private float Height : 0x410;
    float FallingSpeed : 0x414;
    float FallingAcceleration : 0x418;
    float HomingFriction : 0x41c;
    private float Scale : 0x420;
    private float BaseScale : 0x424;
    private struct BitSet128 TearFlags : 0x428;
    int WaitFrames : 0x438;
    private struct Vector ContinueVelocity : 0x43c;
    private int TearIndex : 0x450;
    private float BaseDamage : 0x454;
    float Rotation : 0x458;
    float KnockbackMultiplier : 0x460;
    private float DeadEyeIntensity : 0x464;
    private bool MultidimensionalTouched : 0x468;
    private bool PrismTouched : 0x469;
    private struct Entity* StickTarget : 0x478;
    private struct Vector StickDiff : 0x47c;
    int StickTimer : 0x484;
    private bool CanTriggerStreakEnd : 0x7e8;
    private struct Vector PosDisplacement : 0x7ec;
    private struct Vector ParentOffset : 0x7f4;
} : 0x848;
typedef struct EntityTear* EntityTearPtr;
]])

ffi.cdef [[
    void L_EntityTear_SetHeight(struct EntityTear*, float);
    void L_EntityTear_SetScale(struct EntityTear*, float);
    void L_EntityTear_ResetSpriteScale(struct EntityTear*, bool);
    void L_EntityTear_ChangeVariant(struct EntityTear*, int);
    void L_EntityTear_SetDeadEyeIntensity(struct EntityTear*, float);
    struct EntityTear* L_EntityTear_MakeMultidimensionalCopy(struct EntityTear*);
    struct Sprite* L_EntityTear_GetTearHaloSprite(struct EntityTear*);
    struct Sprite* L_EntityTear_GetTearEffectSprite(struct EntityTear*);
    struct Sprite* L_EntityTear_GetDeadEyeSprite(struct EntityTear*);
    unsigned int L_EntityTear_GetHitListSize(struct EntityTear*);
    unsigned int L_EntityTear_GetHitListEntry(struct EntityTear*, unsigned int);
    void L_EntityTear_ClearHitList(struct EntityTear*);
    void L_EntityTear_RemoveFromHitList(struct EntityTear*, void*);
    void L_EntityTear_AddToHitList(struct EntityTear*, void*);
    bool L_EntityTear_InHitList(struct EntityTear*, void*);
    bool L_EntityTear_SetInitSound(struct EntityTear*, unsigned int);
]]

local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_EntityTear_AddToHitList)
ffi.reentrant(repentogon.L_EntityTear_InHitList)
ffi.reentrant(repentogon.L_EntityTear_MakeMultidimensionalCopy)
ffi.reentrant(repentogon.L_EntityTear_RemoveFromHitList)

local helpers = Entity.Helpers
local Getter = helpers.Getter
local VectorGetter = helpers.VectorGetter
local VectorSetter = helpers.VectorSetter
local BooleanSetter = helpers.BooleanSetter
local EntityGetter = helpers.EntityGetter
local CopyStruct = helpers.CopyStruct
local EntityToPointer = ffichecks.entitytopointer

local TYPE_TEAR = 2
local VARIANT_GRIDENT = 40

local getters = {
    Height = Getter("Height"),
    Scale = Getter("Scale"),
    BaseScale = Getter("BaseScale"),
    TearFlags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlags"))
    end,
    ContinueVelocity = VectorGetter("ContinueVelocity"),
    TearIndex = Getter("TearIndex"),
    BaseDamage = Getter("BaseDamage"),
    StickTarget = EntityGetter("StickTarget"),
    StickDiff = VectorGetter("StickDiff"),
    CanTriggerStreakEnd = Getter("CanTriggerStreakEnd"),
    PosDisplacement = Getter("PosDisplacement"),
    ParentOffset = VectorGetter("ParentOffset"),
}

local setters = {
    Height = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_EntityTear_SetHeight(self, value)
    end,
    Scale = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_EntityTear_SetScale(self, value)
    end,
    TearFlags = function(self, value)
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "TearFlags", value)
    end,
    ContinueVelocity = VectorSetter("ContinueVelocity"),
    StickTarget = function(self, value)
        ffi.setprivate(self, "StickTarget", EntityToPointer(value))
    end,
    StickDiff = VectorSetter("StickDiff"),
    CanTriggerStreakEnd = function(self, value)
        ffi.setprivate(self, "CanTriggerStreakEnd", not not value)
    end,
    ParentOffset = VectorSetter("ParentOffset"),
}

local methods = {
    AddTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags | flags
    end,
    AddToHitList = function(self, entity)
        repentogon.L_EntityTear_AddToHitList(self, ffichecks.checkentity(1, entity))
    end,
    ChangeVariant = function(self, variant)
        variant = ffichecks.checkinteger(1, variant)
        repentogon.L_EntityTear_ChangeVariant(self, variant)
    end,
    ClearHitList = function(self)
        repentogon.L_EntityTear_ClearHitList(self)
    end,
    ClearTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags & ~flags
    end,
    FireSplitTear = Entity.Helpers.FireSplitTear,
    GetBackdropType = function(self)
        -- only grid entity tears store a backdrop type in the tear index
        if ffi.getprivate(self, "Variant") == VARIANT_GRIDENT then
            return ffi.getprivate(self, "TearIndex")
        end
        return 0 -- BACKDROP_NULL
    end,
    GetDeadEyeIntensity = Getter("DeadEyeIntensity"),
    GetDeadEyeSprite = function(self)
        local result = repentogon.L_EntityTear_GetDeadEyeSprite(self) return result
    end,
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityTear_GetHitListSize(self) do
            result[i] = repentogon.L_EntityTear_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetTearEffectSprite = function(self)
        local result = repentogon.L_EntityTear_GetTearEffectSprite(self) return result
    end,
    GetTearHaloSprite = function(self)
        local result = repentogon.L_EntityTear_GetTearHaloSprite(self) return result
    end,
    HasTearFlags = function(self, flags)
        return (self.TearFlags & flags) ~= TearFlags.TEAR_NORMAL
    end,
    InHitList = function(self, entity)
        local result = repentogon.L_EntityTear_InHitList(self, ffichecks.checkentity(1, entity)) return result
    end,
    IsMultidimensionalTouched = Getter("MultidimensionalTouched"),
    IsPrismTouched = Getter("PrismTouched"),
    MakeMultidimensionalCopy = function(self)
        local result = repentogon.L_EntityTear_MakeMultidimensionalCopy(self) return result
    end,
    RemoveFromHitList = function(self, entity)
        repentogon.L_EntityTear_RemoveFromHitList(self, ffichecks.checkentity(1, entity))
    end,
    ResetSpriteScale = function(self, force)
        repentogon.L_EntityTear_ResetSpriteScale(self, ffichecks.optboolean(force, false))
    end,
    SetBackdropType = function(self, backdrop)
        backdrop = ffichecks.checkinteger(1, backdrop)
        if backdrop >= 0 and ffi.getprivate(self, "Variant") == VARIANT_GRIDENT then
            ffi.setprivate(self, "TearIndex", backdrop)
        end
    end,
    SetDeadEyeIntensity = function(self, intensity)
        intensity = ffichecks.checknumber(1, intensity)
        repentogon.L_EntityTear_SetDeadEyeIntensity(self, intensity)
    end,
    SetInitSound = function(self, soundId)
        soundId = ffichecks.checkinteger(1, soundId)
        if not repentogon.L_EntityTear_SetInitSound(self, soundId) then
            ffichecks.argerror(1, "Invalid SoundEffect", 3)
        end
    end,
    SetKnockbackMultiplier = function(self, multiplier)
        multiplier = ffichecks.checknumber(1, multiplier)
        self.KnockbackMultiplier = multiplier
    end,
    SetMultidimensionalTouched = BooleanSetter("MultidimensionalTouched"),
    SetParentOffset = function(self, offset)
        setters.ParentOffset(self, offset)
    end,
    SetPrismTouched = BooleanSetter("PrismTouched"),
    SetWaitFrames = function(self, frames)
        frames = ffichecks.checkinteger(1, frames)
        self.WaitFrames = frames
    end,
}

local TearMT = Entity.Inherit("EntityTear", methods, getters, setters)
ffi.metatype("struct EntityTear", TearMT)
Entity.SetClassType(TYPE_TEAR, ffi.typeof("struct EntityTear*"))

EntityTear = setmetatable({}, { __class = TearMT })
