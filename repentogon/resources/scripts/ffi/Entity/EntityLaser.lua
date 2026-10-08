local Entity = require("ffi.Entity.Entity")

ffi.cdef("struct EntityLaser { " .. Entity.Fields .. [[
    private struct Vector ParentOffsetValue : 0x410;
    float StartAngleDegrees : 0x418;
    float AngleDegrees : 0x41c;
    float LastAngleDegrees : 0x420;
    private struct BitSet128 TearFlagsValue : 0x428;
    int Timeout : 0x458;
    private bool FirstUpdateValue : 0x45c;
    private bool SampleLaserValue : 0x45d;
    private bool ShrinkValue : 0x45e;
    float LaserLength : 0x460;
    private struct VectorList SamplesValue : 0x480;
    private struct VectorList NonOptimizedSamplesValue : 0x48c;
    float CurveStrength : 0x4b8;
    float Radius : 0x4bc;
    private bool IsActiveRotatingValue : 0x4c0;
    int RotationDelay : 0x4c4;
    float RotationDegrees : 0x4c8;
    float RotationSpd : 0x4cc;
    float MaxDistance : 0x4d0;
    uint32_t HomingType : 0x4d4;
    private struct Vector EndPointValue : 0x4d8;
    private float ScaleValue : 0x4e0;
    private bool DisableFollowParentValue : 0x4e4;
    private struct Entity* BounceLaserValue : 0x4e8;
    float BlackHpDropChance : 0x4f4;
    private bool MultidimensionalTouchedValue : 0x500;
    private bool PrismTouchedValue : 0x514;
    private bool OneHitValue : 0x515;
    private bool GridHitValue : 0x516;
    private float DamageMultiplierValue : 0x51c;
    private int ChainedLasersValue : 0x560;
} : 0x568;
typedef struct EntityLaser* EntityLaserPtr;
]])

ffi.cdef [[
    struct EntityLaser* L_EntityLaser_ShootAngle(int, struct Vector*, float, int, struct Vector*, void*);
    void L_EntityLaser_CalculateEndPoint(struct Vector*, struct Vector*, struct Vector*, void*, float, struct Vector*);
    void L_EntityLaser_SetAngle(struct EntityLaser*, float);
    void L_EntityLaser_SetActiveRotation(struct EntityLaser*, int, float, float, bool);
    int L_EntityLaser_GetRenderZ(struct EntityLaser*);
    void L_EntityLaser_ResetSpriteScale(struct EntityLaser*);
    void L_EntityLaser_RotateToAngle(struct EntityLaser*, float, float);
    void L_EntityLaser_RecalculateSamplesNextUpdate(struct EntityLaser*);
    bool L_EntityLaser_SetInitSound(struct EntityLaser*, unsigned int);
    void L_EntityLaser_SetBounceLaser(struct EntityLaser*, void*);
    unsigned int L_EntityLaser_GetHitListSize(struct EntityLaser*);
    unsigned int L_EntityLaser_GetHitListEntry(struct EntityLaser*, unsigned int);
    void L_EntityLaser_RemoveFromHitList(struct EntityLaser*, void*);
    void L_EntityLaser_AddToHitList(struct EntityLaser*, void*);
    bool L_EntityLaser_InHitList(struct EntityLaser*, void*);
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

local TYPE_LASER = 7

local function NumberSetter(field)
    return function(self, value)
        value = ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function AnyBooleanSetter(field)
    return function(self, value)
        ffi.setprivate(self, field, not not value)
    end
end

local getters = {
    Angle = function(self)
        return self.AngleDegrees
    end,
    TearFlags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlagsValue"))
    end,
    ParentOffset = Getter("ParentOffsetValue"),
    EndPoint = VectorGetter("EndPointValue"),
    FirstUpdate = Getter("FirstUpdateValue"),
    SampleLaser = Getter("SampleLaserValue"),
    Shrink = Getter("ShrinkValue"),
    IsActiveRotating = Getter("IsActiveRotatingValue"),
    DisableFollowParent = Getter("DisableFollowParentValue"),
    BounceLaser = EntityGetter("BounceLaserValue"),
    OneHit = Getter("OneHitValue"),
    GridHit = Getter("GridHitValue"),
}

local setters = {
    Angle = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_EntityLaser_SetAngle(self, value)
    end,
    TearFlags = function(self, value)
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "TearFlagsValue", value)
    end,
    ParentOffset = VectorSetter("ParentOffsetValue"),
    EndPoint = VectorSetter("EndPointValue"),
    FirstUpdate = AnyBooleanSetter("FirstUpdateValue"),
    SampleLaser = AnyBooleanSetter("SampleLaserValue"),
    Shrink = AnyBooleanSetter("ShrinkValue"),
    IsActiveRotating = AnyBooleanSetter("IsActiveRotatingValue"),
    DisableFollowParent = AnyBooleanSetter("DisableFollowParentValue"),
    BounceLaser = function(self, value)
        repentogon.L_EntityLaser_SetBounceLaser(self, EntityToPointer(value))
    end,
    OneHit = AnyBooleanSetter("OneHitValue"),
    GridHit = AnyBooleanSetter("GridHitValue"),
}

local methods = {
    AddTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags | flags
    end,
    AddToHitList = function(self, entity)
        repentogon.L_EntityLaser_AddToHitList(self, ffichecks.checkentity(1, entity))
    end,
    ClearTearFlags = function(self, flags)
        self.TearFlags = self.TearFlags & ~flags
    end,
    FireSplitTear = Entity.Helpers.FireSplitTear,
    GetDamageMultiplier = Getter("DamageMultiplierValue"),
    GetDisableFollowParent = Getter("DisableFollowParentValue"),
    GetEndPoint = VectorGetter("EndPointValue"),
    GetHitList = function(self)
        local result = {}
        for i = 1, repentogon.L_EntityLaser_GetHitListSize(self) do
            result[i] = repentogon.L_EntityLaser_GetHitListEntry(self, i - 1)
        end
        return result
    end,
    GetNonOptimizedSamples = Getter("NonOptimizedSamplesValue"),
    GetNumChainedLasers = Getter("ChainedLasersValue"),
    GetOneHit = Getter("OneHitValue"),
    GetRenderZ = function(self)
        return repentogon.L_EntityLaser_GetRenderZ(self)
    end,
    GetSamples = Getter("SamplesValue"),
    GetScale = Getter("ScaleValue"),
    GetShrink = Getter("ShrinkValue"),
    GetTimeout = function(self)
        return self.Timeout
    end,
    HasTearFlags = function(self, flags)
        return (self.TearFlags & flags) ~= TearFlags.TEAR_NORMAL
    end,
    InHitList = function(self, entity)
        return repentogon.L_EntityLaser_InHitList(self, ffichecks.checkentity(1, entity))
    end,
    IsCircleLaser = function(self)
        local subType = self.SubType
        return subType == 1 or subType == 2 or subType == 3
    end,
    IsMultidimensionalTouched = Getter("MultidimensionalTouchedValue"),
    IsPrismTouched = Getter("PrismTouchedValue"),
    IsSampleLaser = Getter("SampleLaserValue"),
    RecalculateSamplesNextUpdate = function(self)
        repentogon.L_EntityLaser_RecalculateSamplesNextUpdate(self)
    end,
    RemoveFromHitList = function(self, entity)
        repentogon.L_EntityLaser_RemoveFromHitList(self, ffichecks.checkentity(1, entity))
    end,
    ResetSpriteScale = function(self)
        repentogon.L_EntityLaser_ResetSpriteScale(self)
    end,
    RotateToAngle = function(self, angle, speed)
        angle = ffichecks.checknumber(1, angle)
        repentogon.L_EntityLaser_RotateToAngle(self, angle, ffichecks.optnumber(speed, 8.0))
    end,
    SetActiveRotation = function(self, delay, degrees, speed, setTimeout)
        delay = ffichecks.checkinteger(1, delay)
        degrees = ffichecks.checknumber(2, degrees)
        speed = ffichecks.checknumber(3, speed)
        repentogon.L_EntityLaser_SetActiveRotation(self, delay, degrees, speed, not not setTimeout)
    end,
    SetBlackHpDropChance = function(self, chance)
        chance = ffichecks.checknumber(1, chance)
        self.BlackHpDropChance = chance
    end,
    SetDamageMultiplier = NumberSetter("DamageMultiplierValue"),
    SetDisableFollowParent = BooleanSetter("DisableFollowParentValue"),
    SetHomingType = function(self, homingType)
        homingType = ffichecks.checkinteger(1, homingType)
        self.HomingType = homingType
    end,
    SetInitSound = function(self, soundId)
        soundId = ffichecks.checkinteger(1, soundId)
        if not repentogon.L_EntityLaser_SetInitSound(self, soundId) then
            ffichecks.argerror(1, "Invalid SoundEffect", 3)
        end
    end,
    SetMaxDistance = function(self, distance)
        distance = ffichecks.checknumber(1, distance)
        self.MaxDistance = distance
    end,
    SetMultidimensionalTouched = AnyBooleanSetter("MultidimensionalTouchedValue"),
    SetNumChainedLasers = function(self, count)
        count = ffichecks.checkinteger(1, count)
        ffi.setprivate(self, "ChainedLasersValue", count)
    end,
    SetOneHit = AnyBooleanSetter("OneHitValue"),
    SetPrismTouched = BooleanSetter("PrismTouchedValue"),
    SetScale = function(self, scale)
        scale = ffichecks.checknumber(1, scale)
        ffi.setprivate(self, "ScaleValue", scale)
        repentogon.L_EntityLaser_ResetSpriteScale(self)
    end,
    SetShrink = BooleanSetter("ShrinkValue"),
    SetTimeout = function(self, timeout)
        timeout = ffichecks.checkinteger(1, timeout)
        self.Timeout = timeout
    end,
}

local LaserMT = Entity.Inherit("EntityLaser", methods, getters, setters)
ffi.metatype("struct EntityLaser", LaserMT)
Entity.SetClassType(TYPE_LASER, ffi.typeof("struct EntityLaser*"))

EntityLaser = setmetatable({
    ShootAngle = function(variant, sourcePosition, angleDegrees, timeout, positionOffset, source)
        variant = ffichecks.checkinteger(1, variant)
        ffichecks.checkcdata(2, sourcePosition, "Vector")
        angleDegrees = ffichecks.checknumber(3, angleDegrees)
        timeout = ffichecks.checkinteger(4, timeout)
        ffichecks.checkcdata(5, positionOffset, "Vector")
        return repentogon.L_EntityLaser_ShootAngle(variant, sourcePosition, angleDegrees, timeout, positionOffset, EntityToPointer(source))
    end,
    CalculateEndPoint = function(start, direction, positionOffset, parent, margin)
        ffichecks.checkcdata(1, start, "Vector")
        ffichecks.checkcdata(2, direction, "Vector")
        ffichecks.checkcdata(3, positionOffset, "Vector")
        local parentPointer = ffichecks.checkentity(4, parent)
        margin = ffichecks.checknumber(5, margin)
        local result = Vector(0, 0)
        repentogon.L_EntityLaser_CalculateEndPoint(start, direction, positionOffset, parentPointer, margin, result)
        return result
    end,
}, { __class = LaserMT })
