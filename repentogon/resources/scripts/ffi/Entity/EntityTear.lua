local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityTear { " .. Entity.Fields .. [[
    private float HeightValue : 0x410;
    float FallingSpeed : 0x414;
    float FallingAcceleration : 0x418;
    float HomingFriction : 0x41c;
    private float ScaleValue : 0x420;
    private float BaseScaleValue : 0x424;
    private struct BitSet128 TearFlagsValue : 0x428;
    int WaitFrames : 0x438;
    private struct Vector ContinueVelocityValue : 0x43c;
    private int TearIndexValue : 0x450;
    private float BaseDamageValue : 0x454;
    float Rotation : 0x458;
    float KnockbackMultiplier : 0x460;
    private float DeadEyeIntensityValue : 0x464;
    private bool MultidimensionalTouchedValue : 0x468;
    private bool PrismTouchedValue : 0x469;
    private struct Entity* StickTargetValue : 0x478;
    private struct Vector StickDiffValue : 0x47c;
    int StickTimer : 0x484;
    private bool CanTriggerStreakEndValue : 0x7e8;
    private struct Vector PosDisplacementValue : 0x7ec;
    private struct Vector ParentOffsetValue : 0x7f4;
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

local helpers = Entity.Helpers
local Getter = helpers.Getter
local VectorGetter = helpers.VectorGetter
local VectorSetter = helpers.VectorSetter
local BooleanSetter = helpers.BooleanSetter
local EntityGetter = helpers.EntityGetter
local CopyStruct = helpers.CopyStruct
local EntityToPointer = ffichecks.entitytopointer

local TYPE_TEAR = 2

local function CheckEntity(index, value)
    local pointer = EntityToPointer(value)
    if pointer == nil then
        ffichecks.argerror(index, "Entity expected, got " .. ffichecks.gettype(value), 3)
    end
    return pointer
end

local getters = {
    Height = Getter("HeightValue"),
    Scale = Getter("ScaleValue"),
    BaseScale = Getter("BaseScaleValue"),
    TearFlags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlagsValue"))
    end,
    ContinueVelocity = VectorGetter("ContinueVelocityValue"),
    TearIndex = Getter("TearIndexValue"),
    BaseDamage = Getter("BaseDamageValue"),
    StickTarget = EntityGetter("StickTargetValue"),
    StickDiff = VectorGetter("StickDiffValue"),
    CanTriggerStreakEnd = Getter("CanTriggerStreakEndValue"),
    PosDisplacement = VectorGetter("PosDisplacementValue"),
    ParentOffset = VectorGetter("ParentOffsetValue"),
}

local setters = {
    Height = function(self, value)
        ffichecks.checknumber(1, value)
        repentogon.L_EntityTear_SetHeight(self, value)
    end,
    Scale = function(self, value)
        ffichecks.checknumber(1, value)
        repentogon.L_EntityTear_SetScale(self, value)
    end,
    TearFlags = function(self, value)
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "TearFlagsValue", value)
    end,
    ContinueVelocity = VectorSetter("ContinueVelocityValue"),
    StickTarget = function(self, value)
        ffi.setprivate(self, "StickTargetValue", EntityToPointer(value))
    end,
    StickDiff = VectorSetter("StickDiffValue"),
    CanTriggerStreakEnd = function(self, value)
        ffi.setprivate(self, "CanTriggerStreakEndValue", not not value)
    end,
    ParentOffset = VectorSetter("ParentOffsetValue"),
}

local methods = {
    AddTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags | flags
    end,
    AddToHitList = function(self, entity)
        repentogon.L_EntityTear_AddToHitList(self, CheckEntity(1, entity))
    end,
    ChangeVariant = function(self, variant)
        ffichecks.checkinteger(1, variant)
        repentogon.L_EntityTear_ChangeVariant(self, variant)
    end,
    ClearHitList = function(self)
        repentogon.L_EntityTear_ClearHitList(self)
    end,
    ClearTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags & ~flags
    end,
    FireSplitTear = Entity.Helpers.FireSplitTear,
    GetDeadEyeIntensity = Getter("DeadEyeIntensityValue"),
    GetDeadEyeSprite = function(self)
        return repentogon.L_EntityTear_GetDeadEyeSprite(self)
    end,
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityTear_GetHitListSize(self) do
            result[i] = repentogon.L_EntityTear_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetTearEffectSprite = function(self)
        return repentogon.L_EntityTear_GetTearEffectSprite(self)
    end,
    GetTearHaloSprite = function(self)
        return repentogon.L_EntityTear_GetTearHaloSprite(self)
    end,
    HasTearFlags = function(self, flags)
        return (self.TearFlags & flags) ~= TearFlags.TEAR_NORMAL
    end,
    InHitList = function(self, entity)
        return repentogon.L_EntityTear_InHitList(self, CheckEntity(1, entity))
    end,
    IsMultidimensionalTouched = Getter("MultidimensionalTouchedValue"),
    IsPrismTouched = Getter("PrismTouchedValue"),
    MakeMultidimensionalCopy = function(self)
        return repentogon.L_EntityTear_MakeMultidimensionalCopy(self)
    end,
    RemoveFromHitList = function(self, entity)
        repentogon.L_EntityTear_RemoveFromHitList(self, CheckEntity(1, entity))
    end,
    ResetSpriteScale = function(self, force)
        repentogon.L_EntityTear_ResetSpriteScale(self, ffichecks.optboolean(force, false))
    end,
    SetDeadEyeIntensity = function(self, intensity)
        ffichecks.checknumber(1, intensity)
        repentogon.L_EntityTear_SetDeadEyeIntensity(self, intensity)
    end,
    SetInitSound = function(self, soundId)
        ffichecks.checkinteger(1, soundId)
        if not repentogon.L_EntityTear_SetInitSound(self, soundId) then
            ffichecks.argerror(1, "Invalid SoundEffect", 3)
        end
    end,
    SetKnockbackMultiplier = function(self, multiplier)
        ffichecks.checknumber(1, multiplier)
        self.KnockbackMultiplier = multiplier
    end,
    SetMultidimensionalTouched = BooleanSetter("MultidimensionalTouchedValue"),
    SetParentOffset = function(self, offset)
        setters.ParentOffset(self, offset)
    end,
    SetPrismTouched = BooleanSetter("PrismTouchedValue"),
    SetWaitFrames = function(self, frames)
        ffichecks.checkinteger(1, frames)
        self.WaitFrames = frames
    end,
}

local TearMT = Entity.Inherit("EntityTear", methods, getters, setters)
ffi.metatype("struct EntityTear", TearMT)
Entity.SetClassType(TYPE_TEAR, ffi.typeof("struct EntityTear*"))

EntityTear = setmetatable({}, { __class = TearMT })
