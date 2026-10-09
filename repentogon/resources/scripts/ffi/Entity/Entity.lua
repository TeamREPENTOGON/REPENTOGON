-- The fields every entity has. Subclasses repeat them at the start of their own struct.
local ENTITY_FIELDS = [[
    padding void* vtable;
    private float AccumulatedDamageValue : 0x14;
    private uint32_t IndexValue : 0x20;
    private uint32_t TypeValue : 0x28;
    private uint32_t VariantValue : 0x2c;
    uint32_t SubType : 0x30;
    uint32_t SpawnerType : 0x34;
    uint32_t SpawnerVariant : 0x38;
    private struct Vector SpriteOffsetValue : 0xdc;
    private struct Vector SpriteScaleValue : 0xe4;
    float SpriteRotation : 0xec;
    private bool FlipXValue : 0x148;
    private float ShadowSizeValue : 0x15c;
    private uint64_t EntityFlagsValue : 0x168;
    private bool VisibleValue : 0x171;
    private bool ExistsValue : 0x172;
    private bool DeadValue : 0x173;
    int GridCollisionClass : 0x184;
    int EntityCollisionClass : 0x188;
    private bool CollidesWithGridValue : 0x190;
    private struct Color ColorValue : 0x1b8;
    private struct Color SplatColorValue : 0x1e4;
    private int FireDamageCooldownValue : 0x210;
    private int DamageCountdownValue : 0x214;
    private int FreezeCountdownValue : 0x218;
    private int PoisonCountdownValue : 0x21c;
    private int SlowingCountdownValue : 0x220;
    private int CharmedCountdownValue : 0x224;
    private int ConfusionCountdownValue : 0x228;
    private int MidasFreezeCountdownValue : 0x22c;
    private int FearCountdownValue : 0x230;
    private int BurnCountdownValue : 0x234;
    private int BossStatusEffectCooldownValue : 0x238;
    private int ShrinkCountdownValue : 0x23c;
    private float PoisonDamageValue : 0x278;
    private float BurnDamageValue : 0x27c;
    private int PoisonDamageTimerValue : 0x2e0;
    private int BurnDamageTimerValue : 0x2e4;
    private int BleedingCountdownValue : 0x2e8;
    private int MagnetizedCountdownValue : 0x2f0;
    private int BaitedCountdownValue : 0x2f4;
    private int KnockbackCountdownValue : 0x2f8;
    private int WeaknessCountdownValue : 0x2fc;
    private int IceCountdownValue : 0x300;
    private int BrimstoneMarkCountdownValue : 0x304;
    private struct Vector KnockbackDirectionValue : 0x308;
    private int PauseTimeValue : 0x31c;
    private int SpawnGridIndexValue : 0x32c;
    private struct Vector TargetPositionValue : 0x334;
    private struct Vector PositionValue : 0x33c;
    private struct Vector PositionOffsetValue : 0x34c;
    int RenderZOffset : 0x354;
    float DepthOffset : 0x358;
    int SortingLayer : 0x35c;
    private struct Vector VelocityValue : 0x360;
    float Friction : 0x36c;
    private float SizeValue : 0x370;
    private struct Vector SizeMultiValue : 0x374;
    float Mass : 0x37c;
    float HitPoints : 0x380;
    float MaxHitPoints : 0x384;
    private float CollisionDamageValue : 0x388;
    private float SpeedMultiplierValue : 0x39c;
    private bool InvincibleValue : 0x3b0;
    private struct Entity* ParentValue : 0x3bc;
    private struct Entity* ChildValue : 0x3c0;
    private struct Entity* TargetValue : 0x3c4;
    private struct Entity* SpawnerEntityValue : 0x3c8;
    private uint32_t DropSeedValue : 0x3dc;
    private uint32_t InitSeedValue : 0x3ec;
]]

ffi.cdef("struct Entity { " .. ENTITY_FIELDS .. " } : 0x410;")

ffi.cdef [[
    typedef struct Entity* EntityPtr;

    void L_Entity_Update(void*);
    void L_Entity_PostRender(void*);
    void L_Entity_Render(void*, struct Vector*);
    bool L_Entity_RenderShadowLayer(void*, struct Vector*);
    bool L_Entity_TakeDamage(void*, float, uint64_t, struct EntityRef*, int);
    void L_Entity_Kill(void*);
    void L_Entity_KillWithSource(void*, struct EntityRef*);
    void L_Entity_Remove(void*);
    void L_Entity_BloodExplode(void*);
    bool L_Entity_CanShutDoors(void*);
    bool L_Entity_IsBoss(void*);
    void L_Entity_SetCollisionDamage(void*, float);
    void L_Entity_SetColor(void*, struct Color*, int, int, bool, bool);
    void L_Entity_AddVelocity(void*, struct Vector*);
    void L_Entity_SetSize(void*, float, struct Vector*, int);
    void L_Entity_SetSizeKeepingMulti(void*, float);
    bool L_Entity_TryThrow(void*, struct EntityRef*, struct Vector*, float);
    void L_Entity_TeleportToRandomPosition(void*);
    bool L_Entity_ForceCollide(void*, void*, bool);
    void L_Entity_SetVariant(void*, unsigned int);
    void L_Entity_SetParent(void*, void*);
    void L_Entity_SetChild(void*, void*);
    void L_Entity_SetTarget(void*, void*);
    void L_Entity_SetSpawnerEntity(void*, void*);
    void* L_Entity_GetMinecart(void*);
    void* L_Entity_GiveMinecart(void*, struct Vector*, struct Vector*);
    bool L_Entity_HasCommonParentWithEntity(void*, void*);
    bool L_Entity_IsEnemy(void*);
    bool L_Entity_IsActiveEnemy(void*, bool);
    bool L_Entity_IsVulnerableEnemy(void*, void*);
    bool L_Entity_IsFlying(void*);
    bool L_Entity_IsFrame(void*, int, int);
    bool L_Entity_CanDevolve(void*);
    unsigned int L_Entity_GetHitListIndex(void*);
    int L_Entity_GetFrameCount(void*);
    int L_Entity_GetBossID(void*);
    void L_Entity_RemoveStatusEffects(void*);
    void L_Entity_AddBaited(void*, struct EntityRef*, int);
    void L_Entity_AddBleeding(void*, struct EntityRef*, int);
    void L_Entity_AddMagnetized(void*, struct EntityRef*, int);
    void L_Entity_AddWeakness(void*, struct EntityRef*, int);
    void L_Entity_AddBrimstoneMark(void*, struct EntityRef*, int);
    void L_Entity_AddIce(void*, struct EntityRef*, int);
    void L_Entity_AddKnockback(void*, struct EntityRef*, struct Vector*, int, bool);
    void L_Entity_AddBurn(void*, struct EntityRef*, int, float, bool);
    void L_Entity_AddCharmed(void*, struct EntityRef*, int, bool);
    void L_Entity_AddConfusion(void*, struct EntityRef*, int, bool);
    void L_Entity_AddFear(void*, struct EntityRef*, int, bool);
    void L_Entity_AddFreeze(void*, struct EntityRef*, int, bool);
    void L_Entity_AddMidasFreeze(void*, struct EntityRef*, int, bool);
    void L_Entity_AddPoison(void*, struct EntityRef*, int, float, bool);
    void L_Entity_AddShrink(void*, struct EntityRef*, int, bool);
    void L_Entity_AddSlowing(void*, struct EntityRef*, int, float, struct Color*, bool);
    unsigned int L_Entity_ComputeStatusEffectDuration(void*, int, struct EntityRef*);
    bool L_Entity_IgnoreEffectFromFriendly(void*, struct EntityRef*);
    void L_Entity_CopyStatusEffects(void*, void*, bool);
    unsigned int L_Entity_GetColorParamsCount(void*);
    struct ColorParams* L_Entity_GetColorParam(void*, unsigned int);
    void L_Entity_SetColorParams(void*, struct ColorParams*, unsigned int);
    void L_Entity_GetNullOffset(void*, const char*, struct Vector*);
    void L_Entity_GetNullCapsule(void*, const char*, struct Capsule*);
    void L_Entity_GetCollisionCapsule(void*, struct Vector*, struct Capsule*);
    void L_Entity_GetPredictedTargetPosition(void*, void*, float, struct Vector*);
    void L_Entity_SpawnWaterImpactEffects(struct Vector*, struct Vector*, float);
    struct Sprite* L_Entity_GetSprite(void*);
    struct RNG* L_Entity_GetDropRNG(void*);
    struct Shape* L_Entity_GetDebugShape(void*, bool);
    struct EntityConfigEntity* L_Entity_GetEntityConfigEntity(void*);
    void* L_Entity_SpawnBloodEffect(void*, int, struct Vector*, struct Vector*, struct Color*, struct Vector*);
    void* L_Entity_MakeBloodPoof(void*, struct Vector*, struct Color*, float);
    void* L_Entity_MakeGroundPoof(void*, struct Vector*, struct Color*, float);
    unsigned int L_Entity_GetWaterClipFlags(void*);
    void L_Entity_SetWaterClipFlags(void*, uint32_t);
    void L_Entity_ResetWaterClipFlags(void*);
    void* L_Entity_ToPlayer(void*);
    void* L_Entity_ToNPC(void*);
    void* L_Entity_ToFamiliar(void*);
    void* L_Entity_ToPickup(void*);
    void* L_Entity_ToProjectile(void*);
    void* L_Entity_FireSplitTear(void*, struct Vector*, struct Vector*, float, float, int, int, const char*);
]]

local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_Entity_Update)
ffi.reentrant(repentogon.L_Entity_TakeDamage)
ffi.reentrant(repentogon.L_Entity_Kill)
ffi.reentrant(repentogon.L_Entity_KillWithSource)
ffi.reentrant(repentogon.L_Entity_Remove)
ffi.reentrant(repentogon.L_Entity_BloodExplode)
ffi.reentrant(repentogon.L_Entity_SpawnWaterImpactEffects)
ffi.reentrant(repentogon.L_Entity_AddBleeding)
ffi.reentrant(repentogon.L_Entity_AddBaited)
ffi.reentrant(repentogon.L_Entity_AddMagnetized)
ffi.reentrant(repentogon.L_Entity_AddWeakness)
ffi.reentrant(repentogon.L_Entity_AddBrimstoneMark)
ffi.reentrant(repentogon.L_Entity_AddIce)
ffi.reentrant(repentogon.L_Entity_AddKnockback)
ffi.reentrant(repentogon.L_Entity_AddBurn)
ffi.reentrant(repentogon.L_Entity_AddCharmed)
ffi.reentrant(repentogon.L_Entity_AddConfusion)
ffi.reentrant(repentogon.L_Entity_AddFear)
ffi.reentrant(repentogon.L_Entity_AddFreeze)
ffi.reentrant(repentogon.L_Entity_AddMidasFreeze)
ffi.reentrant(repentogon.L_Entity_AddPoison)
ffi.reentrant(repentogon.L_Entity_AddShrink)
ffi.reentrant(repentogon.L_Entity_AddSlowing)

local EntityToPointer = ffichecks.entitytopointer
local uintptrType = ffi.typeof("uintptr_t")
local entityType = ffi.typeof("struct Entity*")
local projectileType

-- 64 bit values are numbers while they are exact, and int64_t after that
local MAX_EXACT = 9007199254740992LL
local function Flags64(value)
    local signed = ffi.cast("int64_t", value)
    if signed >= -MAX_EXACT and signed <= MAX_EXACT then
        local result = tonumber(signed) return result
    end
    return signed
end

local function CopyStruct(ctype, reference)
    local copy = ffi.new(ctype)
    ffi.copy(copy, reference, ffi.sizeof(ctype))
    return copy
end

local function Getter(field)
    return function(self)
        local result = ffi.getprivate(self, field) return result
    end
end

local function VectorGetter(field)
    return function(self)
        local vector = ffi.getprivate(self, field)
        return Vector(vector.X, vector.Y)
    end
end

local function VectorReferenceGetter(field)
    return function(self)
        local result = ffi.getprivate(self, field) return result
    end
end

local function VectorSetter(field)
    return function(self, value)
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, field, value)
    end
end

local function IntegerSetter(field)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function NumberSetter(field)
    return function(self, value)
        value = ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function BooleanSetter(field)
    return function(self, value)
        value = ffichecks.checkboolean(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function EntityGetter(field)
    return function(self)
        local result = ffi.getprivate(self, field) return result
    end
end

local getters = {
    Type = Getter("TypeValue"),
    Variant = Getter("VariantValue"),
    Index = Getter("IndexValue"),
    SpawnGridIndex = Getter("SpawnGridIndexValue"),
    InitSeed = Getter("InitSeedValue"),
    DropSeed = Getter("DropSeedValue"),
    FrameCount = function(self)
        local result = repentogon.L_Entity_GetFrameCount(self) return result
    end,
    Position = VectorGetter("PositionValue"),
    Velocity = VectorGetter("VelocityValue"),
    SizeMulti = VectorGetter("SizeMultiValue"),
    PositionOffset = VectorReferenceGetter("PositionOffsetValue"),
    SpriteOffset = VectorReferenceGetter("SpriteOffsetValue"),
    SpriteScale = VectorReferenceGetter("SpriteScaleValue"),
    TargetPosition = VectorReferenceGetter("TargetPositionValue"),
    Color = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "ColorValue"))
    end,
    SplatColor = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "SplatColorValue"))
    end,
    Visible = Getter("VisibleValue"),
    FlipX = Getter("FlipXValue"),
    Size = Getter("SizeValue"),
    CollisionDamage = Getter("CollisionDamageValue"),
    Parent = EntityGetter("ParentValue"),
    Child = EntityGetter("ChildValue"),
    Target = EntityGetter("TargetValue"),
    SpawnerEntity = EntityGetter("SpawnerEntityValue"),
}

local setters = {
    Variant = function(self, value)
        value = ffichecks.checkinteger(1, value)
        repentogon.L_Entity_SetVariant(self, value)
    end,
    Position = VectorSetter("PositionValue"),
    Velocity = VectorSetter("VelocityValue"),
    SizeMulti = VectorSetter("SizeMultiValue"),
    PositionOffset = VectorSetter("PositionOffsetValue"),
    SpriteOffset = VectorSetter("SpriteOffsetValue"),
    SpriteScale = VectorSetter("SpriteScaleValue"),
    TargetPosition = VectorSetter("TargetPositionValue"),
    Color = function(self, value)
        ffichecks.checkcdata(1, value, "Color")
        repentogon.L_Entity_SetColor(self, value, -1, 255, false, true)
    end,
    SplatColor = function(self, value)
        ffichecks.checkcdata(1, value, "Color")
        ffi.setprivate(self, "SplatColorValue", value)
    end,
    Visible = function(self, value)
        ffi.setprivate(self, "VisibleValue", not not value)
    end,
    FlipX = function(self, value)
        ffi.setprivate(self, "FlipXValue", not not value)
    end,
    Size = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_Entity_SetSizeKeepingMulti(self, value)
    end,
    CollisionDamage = function(self, value)
        value = ffichecks.checknumber(1, value)
        repentogon.L_Entity_SetCollisionDamage(self, value)
    end,
    Parent = function(self, value)
        repentogon.L_Entity_SetParent(self, EntityToPointer(value))
    end,
    Child = function(self, value)
        repentogon.L_Entity_SetChild(self, EntityToPointer(value))
    end,
    Target = function(self, value)
        repentogon.L_Entity_SetTarget(self, EntityToPointer(value))
    end,
    SpawnerEntity = function(self, value)
        repentogon.L_Entity_SetSpawnerEntity(self, EntityToPointer(value))
    end,
}

local methods = {}

local COUNTERS = {
    { "FireDamageCooldown", "FireDamageCooldownValue" },
    { "FreezeCountdown", "FreezeCountdownValue" },
    { "PoisonCountdown", "PoisonCountdownValue" },
    { "SlowingCountdown", "SlowingCountdownValue" },
    { "CharmedCountdown", "CharmedCountdownValue" },
    { "ConfusionCountdown", "ConfusionCountdownValue" },
    { "MidasFreezeCountdown", "MidasFreezeCountdownValue" },
    { "FearCountdown", "FearCountdownValue" },
    { "BurnCountdown", "BurnCountdownValue" },
    { "ShrinkCountdown", "ShrinkCountdownValue" },
    { "BleedingCountdown", "BleedingCountdownValue" },
    { "MagnetizedCountdown", "MagnetizedCountdownValue" },
    { "BaitedCountdown", "BaitedCountdownValue" },
    { "KnockbackCountdown", "KnockbackCountdownValue" },
    { "WeaknessCountdown", "WeaknessCountdownValue" },
    { "IceCountdown", "IceCountdownValue" },
    { "BrimstoneMarkCountdown", "BrimstoneMarkCountdownValue" },
    { "PoisonDamageTimer", "PoisonDamageTimerValue" },
    { "BurnDamageTimer", "BurnDamageTimerValue" },
    { "BossStatusEffectCooldown", "BossStatusEffectCooldownValue" },
    { "PauseTime", "PauseTimeValue" },
}
for _, counter in ipairs(COUNTERS) do
    local field = counter[2]
    methods["Get" .. counter[1]] = Getter(field)
    methods["Set" .. counter[1]] = IntegerSetter(field)
end

local NUMBERS = {
    { "PoisonDamage", "PoisonDamageValue" },
    { "BurnDamage", "BurnDamageValue" },
    { "SpeedMultiplier", "SpeedMultiplierValue" },
    { "ShadowSize", "ShadowSizeValue" },
}
for _, number in ipairs(NUMBERS) do
    local field = number[2]
    methods["Get" .. number[1]] = Getter(field)
    methods["Set" .. number[1]] = NumberSetter(field)
end

local function DurationEffect(export)
    return function(self, source, duration)
        ffichecks.checkcdata(1, source, "EntityRef")
        duration = ffichecks.checkinteger(2, duration)
        export(self, source, duration)
    end
end

local function BossEffect(export, bossDefault)
    return function(self, source, duration, ignoreBosses)
        ffichecks.checkcdata(1, source, "EntityRef")
        duration = ffichecks.checkinteger(2, duration)
        export(self, source, duration, ffichecks.optboolean(ignoreBosses, bossDefault))
    end
end

local function DamageEffect(export, bossDefault)
    return function(self, source, duration, damage, ignoreBosses)
        ffichecks.checkcdata(1, source, "EntityRef")
        duration = ffichecks.checkinteger(2, duration)
        damage = ffichecks.checknumber(3, damage)
        export(self, source, duration, damage, ffichecks.optboolean(ignoreBosses, bossDefault))
    end
end

local function VoidMethod(export)
    return function(self)
        export(self)
    end
end

local function BoolMethod(export)
    return function(self)
        return export(self)
    end
end

local function EntityResult(pointer)
    if pointer == nil then
        return nil
    end
    return ffichecks.pointertoentity(pointer)
end

local TYPE_PLAYER = 1
local TYPE_TEAR = 2
local TYPE_FAMILIAR = 3
local TYPE_BOMB = 4
local TYPE_PICKUP = 5
local TYPE_SLOT = 6
local TYPE_LASER = 7
local TYPE_KNIFE = 8
local TYPE_EFFECT = 1000

local classTypes = {}

local function ToClass(self, wantedType)
    if ffi.getprivate(self, "TypeValue") ~= wantedType then
        return nil
    end
    local result = ffi.cast(classTypes[wantedType], self) return result
end

local function EffectResult(pointer)
    if pointer == nil then
        return nil
    end
    local result = ffi.cast(classTypes[TYPE_EFFECT], pointer) return result
end

local function Methods(table)
    for name, fn in pairs(table) do
        methods[name] = fn
    end
end

Methods {
    AddBaited = DurationEffect(repentogon.L_Entity_AddBaited),
    AddBleeding = DurationEffect(repentogon.L_Entity_AddBleeding),
    AddBrimstoneMark = DurationEffect(repentogon.L_Entity_AddBrimstoneMark),
    AddBurn = DamageEffect(repentogon.L_Entity_AddBurn, false),
    AddCharmed = BossEffect(repentogon.L_Entity_AddCharmed, false),
    AddConfusion = BossEffect(repentogon.L_Entity_AddConfusion, false),
    AddEntityFlags = function(self, flags)
        flags = ffichecks.checkinteger64(1, flags)
        ffi.setprivate(self, "EntityFlagsValue", ffi.getprivate(self, "EntityFlagsValue") | flags)
    end,
    AddFear = BossEffect(repentogon.L_Entity_AddFear, false),
    AddFreeze = BossEffect(repentogon.L_Entity_AddFreeze, false),
    AddHealth = function(self, amount)
        amount = ffichecks.checknumber(1, amount)
        local sum = self.HitPoints + amount
        local max = self.MaxHitPoints
        if max <= sum then
            self.HitPoints = max
        else
            self.HitPoints = sum
        end
    end,
    AddIce = DurationEffect(repentogon.L_Entity_AddIce),
    AddKnockback = function(self, source, pushDirection, duration, takeImpactDamage)
        ffichecks.checkcdata(1, source, "EntityRef")
        ffichecks.checkcdata(2, pushDirection, "Vector")
        duration = ffichecks.checkinteger(3, duration)
        takeImpactDamage = ffichecks.checkboolean(4, takeImpactDamage)
        repentogon.L_Entity_AddKnockback(self, source, pushDirection, duration, takeImpactDamage)
    end,
    AddMagnetized = DurationEffect(repentogon.L_Entity_AddMagnetized),
    AddMidasFreeze = BossEffect(repentogon.L_Entity_AddMidasFreeze, false),
    AddPoison = DamageEffect(repentogon.L_Entity_AddPoison, true),
    AddShrink = BossEffect(repentogon.L_Entity_AddShrink, true),
    AddSlowing = function(self, source, duration, amount, color, ignoreBosses)
        ffichecks.checkcdata(1, source, "EntityRef")
        duration = ffichecks.checkinteger(2, duration)
        amount = ffichecks.checknumber(3, amount)
        ffichecks.checkcdata(4, color, "Color")
        repentogon.L_Entity_AddSlowing(self, source, duration, amount, color, ffichecks.optboolean(ignoreBosses, false))
    end,
    AddVelocity = function(self, velocity)
        ffichecks.checkcdata(1, velocity, "Vector")
        repentogon.L_Entity_AddVelocity(self, velocity)
    end,
    AddWeakness = DurationEffect(repentogon.L_Entity_AddWeakness),
    BloodExplode = VoidMethod(repentogon.L_Entity_BloodExplode),
    CanDevolve = BoolMethod(repentogon.L_Entity_CanDevolve),
    CanShutDoors = BoolMethod(repentogon.L_Entity_CanShutDoors),
    ClearEntityFlags = function(self, flags)
        flags = ffichecks.checkinteger64(1, flags)
        ffi.setprivate(self, "EntityFlagsValue", ffi.getprivate(self, "EntityFlagsValue") & ~ffi.cast("uint64_t", flags))
    end,
    CollidesWithGrid = Getter("CollidesWithGridValue"),
    ComputeStatusEffectDuration = function(self, initial, source)
        initial = ffichecks.checkinteger(1, initial)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_Entity_ComputeStatusEffectDuration(self, initial, source) return result
    end,
    CopyStatusEffects = function(self, other, overwrite)
        repentogon.L_Entity_CopyStatusEffects(self, EntityToPointer(other), ffichecks.optboolean(overwrite, false))
    end,
    Die = function(self)
        ffi.setprivate(self, "DeadValue", true)
    end,
    Exists = Getter("ExistsValue"),
    ForceCollide = function(self, collider, low)
        low = ffichecks.checkboolean(2, low)
        local result = repentogon.L_Entity_ForceCollide(self, EntityToPointer(collider), low) return result
    end,
    GetBossID = function(self)
        local result = repentogon.L_Entity_GetBossID(self) return result
    end,
    GetCollisionCapsule = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector", true)
        local capsule = Capsule()
        repentogon.L_Entity_GetCollisionCapsule(self, offset or Vector(0, 0), capsule)
        return capsule
    end,
    GetColor = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "ColorValue"))
    end,
    GetColorParams = function(self)
        local result = {}
        for i = 0, repentogon.L_Entity_GetColorParamsCount(self) - 1 do
            result[i + 1] = CopyStruct("struct ColorParams", repentogon.L_Entity_GetColorParam(self, i))
        end
        return result
    end,
    GetDamageCountdown = Getter("DamageCountdownValue"),
    GetData = function(self)
        return _GetEntityData(self)
    end,
    GetDebugShape = function(self, unk)
        unk = ffichecks.checkboolean(1, unk)
        local result = repentogon.L_Entity_GetDebugShape(self, unk) return result
    end,
    GetDropRNG = function(self)
        local result = repentogon.L_Entity_GetDropRNG(self) return result
    end,
    GetEntityConfigEntity = function(self)
        local result = repentogon.L_Entity_GetEntityConfigEntity(self) return result
    end,
    GetEntityFlags = function(self)
        return Flags64(ffi.getprivate(self, "EntityFlagsValue"))
    end,
    GetHitListIndex = function(self)
        local result = repentogon.L_Entity_GetHitListIndex(self) return result
    end,
    GetLastChild = function(self)
        local entity = self
        while entity.Child ~= nil do
            entity = entity.Child
        end
        return entity
    end,
    GetLastParent = function(self)
        local entity = self
        while entity.Parent ~= nil do
            entity = entity.Parent
        end
        return entity
    end,
    GetMinecart = function(self)
        local minecart = repentogon.L_Entity_GetMinecart(self)
        if minecart == nil then
            return nil
        end
        if repentogon.L_Entity_ToNPC(minecart) ~= nil then
            return ffichecks.pointertonpc(minecart)
        end
        return ffichecks.pointertoentity(minecart)
    end,
    GetNullCapsule = function(self, nullLayerName)
        nullLayerName = ffichecks.checkstring(1, nullLayerName)
        local capsule = Capsule()
        repentogon.L_Entity_GetNullCapsule(self, nullLayerName, capsule)
        return capsule
    end,
    GetNullOffset = function(self, nullLayerName)
        nullLayerName = ffichecks.checkstring(1, nullLayerName)
        local offset = Vector(0, 0)
        repentogon.L_Entity_GetNullOffset(self, nullLayerName, offset)
        return offset
    end,
    GetPosVel = function(self)
        return PosVel(getters.Position(self), getters.Velocity(self))
    end,
    GetPredictedTargetPosition = function(self, target, delay)
        delay = ffichecks.checknumber(2, delay)
        local result = Vector(0, 0)
        repentogon.L_Entity_GetPredictedTargetPosition(self, EntityToPointer(target), delay, result)
        return result
    end,
    GetKnockbackDirection = function(self)
        local direction = ffi.getprivate(self, "KnockbackDirectionValue")
        return Vector(direction.X, direction.Y)
    end,
    GetSprite = function(self)
        local result = repentogon.L_Entity_GetSprite(self) return result
    end,
    GetType = Getter("TypeValue"),
    GetWaterClipFlags = function(self)
        local result = repentogon.L_Entity_GetWaterClipFlags(self) return result
    end,
    GiveMinecart = function(self, ...)
        local count = select("#", ...)
        if count ~= 2 then
            error(string.format("Expected two parameters, got %d\n", count), 2)
        end
        local position, velocity = ...
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        return ffichecks.pointertonpc(repentogon.L_Entity_GiveMinecart(self, position, velocity))
    end,
    HasCommonParentWithEntity = function(self, other)
        local result = repentogon.L_Entity_HasCommonParentWithEntity(self, EntityToPointer(other)) return result
    end,
    HasEntityFlags = function(self, flags)
        flags = ffichecks.checkinteger64(1, flags)
        return (ffi.getprivate(self, "EntityFlagsValue") & ffi.cast("uint64_t", flags)) ~= 0
    end,
    HasFullHealth = function(self)
        return self.HitPoints >= self.MaxHitPoints
    end,
    HasMortalDamage = function(self)
        return ffi.getprivate(self, "AccumulatedDamageValue") >= self.HitPoints
    end,
    IgnoreEffectFromFriendly = function(self, source)
        ffichecks.checkcdata(1, source, "EntityRef")
        local result = repentogon.L_Entity_IgnoreEffectFromFriendly(self, source) return result
    end,
    IsActiveEnemy = function(self, includeDead)
        local result = repentogon.L_Entity_IsActiveEnemy(self, ffichecks.optboolean(includeDead, false)) return result
    end,
    IsBoss = BoolMethod(repentogon.L_Entity_IsBoss),
    IsDead = Getter("DeadValue"),
    IsEnemy = BoolMethod(repentogon.L_Entity_IsEnemy),
    IsFlying = BoolMethod(repentogon.L_Entity_IsFlying),
    IsFrame = function(self, frame, offset)
        frame = ffichecks.checkinteger(1, frame)
        offset = ffichecks.checkinteger(2, offset)
        local result = repentogon.L_Entity_IsFrame(self, frame, offset) return result
    end,
    IsInvincible = Getter("InvincibleValue"),
    IsVisible = Getter("VisibleValue"),
    IsVulnerableEnemy = function(self, source)
        local result = repentogon.L_Entity_IsVulnerableEnemy(self, EntityToPointer(source)) return result
    end,
    Kill = VoidMethod(repentogon.L_Entity_Kill),
    KillWithSource = function(self, source)
        ffichecks.checkcdata(1, source, "EntityRef")
        repentogon.L_Entity_KillWithSource(self, source)
    end,
    MakeBloodPoof = function(self, position, color, scale)
        ffichecks.checkcdata(1, position, "Vector", true)
        ffichecks.checkcdata(2, color, "Color", true)
        return EffectResult(repentogon.L_Entity_MakeBloodPoof(self, position, color, ffichecks.optnumber(scale, 1.0)))
    end,
    MakeGroundPoof = function(self, position, color, scale)
        ffichecks.checkcdata(1, position, "Vector", true)
        ffichecks.checkcdata(2, color, "Color", true)
        return EffectResult(repentogon.L_Entity_MakeGroundPoof(self, position, color, ffichecks.optnumber(scale, 1.0)))
    end,
    MultiplyFriction = function(self, value)
        value = ffichecks.checknumber(1, value)
        self.Friction = self.Friction * value
    end,
    PostRender = VoidMethod(repentogon.L_Entity_PostRender),
    Remove = VoidMethod(repentogon.L_Entity_Remove),
    RemoveStatusEffects = VoidMethod(repentogon.L_Entity_RemoveStatusEffects),
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_Entity_Render(self, offset)
    end,
    RenderShadowLayer = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        local result = repentogon.L_Entity_RenderShadowLayer(self, offset) return result
    end,
    ResetWaterClipFlags = VoidMethod(repentogon.L_Entity_ResetWaterClipFlags),
    SetColor = function(self, color, duration, priority, fadeout, share)
        ffichecks.checkcdata(1, color, "Color")
        duration = ffichecks.checkinteger(2, duration)
        priority = ffichecks.checkinteger(3, priority)
        fadeout = ffichecks.checkboolean(4, fadeout)
        share = ffichecks.checkboolean(5, share)
        repentogon.L_Entity_SetColor(self, color, duration, priority, fadeout, share)
    end,
    SetColorParams = function(self, params)
        ffichecks.checktable(1, params)
        local count = #params
        local list = ffi.new("struct ColorParams[?]", math.max(count, 1))
        for i = 1, count do
            ffichecks.checkcdata(1, params[i], "ColorParams")
            list[i - 1] = params[i]
        end
        repentogon.L_Entity_SetColorParams(self, list, count)
    end,
    SetDamageCountdown = function(self, countdown)
        countdown = ffichecks.checkinteger(1, countdown)
        if countdown < 0 then
            countdown = 0
        end
        ffi.setprivate(self, "DamageCountdownValue", countdown)
    end,
    SetDead = function(self, dead)
        ffi.setprivate(self, "DeadValue", ffichecks.checkboolean(1, dead))
    end,
    SetInvincible = function(self, invincible)
        ffi.setprivate(self, "InvincibleValue", ffichecks.checkboolean(1, invincible))
    end,
    SetKnockbackDirection = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        ffi.setprivate(self, "KnockbackDirectionValue", direction)
    end,
    SetSize = function(self, size, sizeMulti, numGridCollisionPoints)
        size = ffichecks.checknumber(1, size)
        ffichecks.checkcdata(2, sizeMulti, "Vector")
        numGridCollisionPoints = ffichecks.checkinteger(3, numGridCollisionPoints)
        repentogon.L_Entity_SetSize(self, size, sizeMulti, numGridCollisionPoints)
    end,
    SetSpriteFrame = function(self, animation, frame)
        animation = ffichecks.checkstring(1, animation)
        frame = ffichecks.checkinteger(2, frame)
        repentogon.L_Entity_GetSprite(self):SetFrame(animation, frame)
    end,
    SetSpriteOverlayFrame = function(self, animation, frame)
        animation = ffichecks.checkstring(1, animation)
        frame = ffichecks.checkinteger(2, frame)
        repentogon.L_Entity_GetSprite(self):SetOverlayFrame(animation, frame)
    end,
    SetWaterClipFlags = function(self, flags)
        flags = ffichecks.checkinteger(1, flags)
        repentogon.L_Entity_SetWaterClipFlags(self, flags)
    end,
    SpawnBloodEffect = function(self, subtype, position, offset, color, velocity)
        subtype = ffichecks.optnumber(subtype, 0)
        ffichecks.checkcdata(2, position, "Vector", true)
        ffichecks.checkcdata(3, offset, "Vector", true)
        ffichecks.checkcdata(4, color, "Color", true)
        ffichecks.checkcdata(5, velocity, "Vector", true)
        return EffectResult(repentogon.L_Entity_SpawnBloodEffect(self, subtype, position, offset, color, velocity))
    end,
    SpawnWaterImpactEffects = function(self, position, velocity, scale)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector", true)
        scale = ffichecks.checknumber(3, scale)
        repentogon.L_Entity_SpawnWaterImpactEffects(position, velocity or Vector(0, 0), scale)
    end,
    TakeDamage = function(self, damage, flags, source, damageCountdown)
        damage = ffichecks.checknumber(1, damage)
        flags = ffichecks.checkinteger64(2, flags)
        ffichecks.checkcdata(3, source, "EntityRef", true)
        damageCountdown = ffichecks.checkinteger(4, damageCountdown)
        local result = repentogon.L_Entity_TakeDamage(self, damage, flags, source, damageCountdown) return result
    end,
    TeleportToRandomPosition = VoidMethod(repentogon.L_Entity_TeleportToRandomPosition),
    ToBomb = function(self)
        return ToClass(self, TYPE_BOMB)
    end,
    ToEffect = function(self)
        return ToClass(self, TYPE_EFFECT)
    end,
    ToFamiliar = function(self)
        return ToClass(self, TYPE_FAMILIAR)
    end,
    ToKnife = function(self)
        return ToClass(self, TYPE_KNIFE)
    end,
    ToLaser = function(self)
        return ToClass(self, TYPE_LASER)
    end,
    ToNPC = function(self)
        local entityType = ffi.getprivate(self, "TypeValue")
        if entityType < 10 or entityType == TYPE_EFFECT then
            return nil
        end
        local npc = repentogon.L_Entity_ToNPC(self)
        if npc == nil then
            return nil
        end
        return ffichecks.pointertonpc(npc)
    end,
    ToPickup = function(self)
        return ToClass(self, TYPE_PICKUP)
    end,
    ToPlayer = function(self)
        local player = repentogon.L_Entity_ToPlayer(self)
        if player == nil then
            return nil
        end
        return ffichecks.pointertoplayer(player)
    end,
    ToProjectile = function(self)
        local projectile = repentogon.L_Entity_ToProjectile(self)
        if projectile == nil then
            return nil
        end
        local result = ffi.cast(projectileType, projectile) return result
    end,
    ToSlot = function(self)
        return ToClass(self, TYPE_SLOT)
    end,
    ToTear = function(self)
        return ToClass(self, TYPE_TEAR)
    end,
    TryThrow = function(self, source, direction, force)
        ffichecks.checkcdata(1, source, "EntityRef")
        ffichecks.checkcdata(2, direction, "Vector")
        force = ffichecks.checknumber(3, force)
        local result = repentogon.L_Entity_TryThrow(self, source, direction, force) return result
    end,
    Update = VoidMethod(repentogon.L_Entity_Update),
}

local Entity = {}

function Entity.SetProjectileType(ctype)
    projectileType = ctype
end

function Entity.SetClassType(entityType, ctype)
    classTypes[entityType] = ctype
end

local function FireSplitTear(self, position, velocity, damageMultiplier, sizeMultiplier, variant, splitType)
    ffichecks.checkcdata(1, position, "Vector")
    ffichecks.checkcdata(2, velocity, "Vector")
    damageMultiplier = ffichecks.optnumber(damageMultiplier, 0.5)
    sizeMultiplier = ffichecks.optnumber(sizeMultiplier, 0.6)
    if variant == nil then
        variant = 0
    else
        variant = ffichecks.checkinteger(5, variant)
    end

    local splitTypeId, splitTypeName = 0, nil
    if splitType ~= nil then
        if type(splitType) == "number" then
            splitType = ffichecks.checkinteger(6, splitType)
            splitTypeId = splitType
        else
            splitTypeName = ffichecks.checkstring(6, splitType)
        end
    end

    local tear = repentogon.L_Entity_FireSplitTear(self, position, velocity, damageMultiplier, sizeMultiplier, variant, splitTypeId, splitTypeName)
    if tear == nil then
        return nil
    end
    local result = ffi.cast(classTypes[TYPE_TEAR], tear) return result
end

Entity.Helpers = {
    FireSplitTear = FireSplitTear,
    Flags64 = Flags64,
    CopyStruct = CopyStruct,
    Getter = Getter,
    VectorGetter = VectorGetter,
    VectorSetter = VectorSetter,
    BooleanSetter = BooleanSetter,
    EntityGetter = EntityGetter,
}

Entity.Fields = ENTITY_FIELDS
Entity.Getters = getters
Entity.Setters = setters
Entity.Methods = methods

-- parent is the metatable of the class being extended, which is Entity's by default
function Entity.Inherit(typeName, ownMethods, ownGetters, ownSetters, parent)
    local parentGetters = parent and parent.__propget or getters
    local parentSetters = parent and parent.__propset or setters
    local classGetters = ownGetters == getters and getters or setmetatable(ownGetters or {}, { __index = parentGetters })
    local classSetters = ownSetters == setters and setters or setmetatable(ownSetters or {}, { __index = parentSetters })
    local class = setmetatable(ownMethods or {}, { __index = parent or methods })

    class.__type = typeName
    class.__index = function(self, key)
        local getter = classGetters[key]
        if getter then
            return getter(self)
        end
        return class[key]
    end
    class.__newindex = function(self, key, value)
        local setter = classSetters[key]
        if setter then
            return setter(self, value)
        end
        error(string.format("cannot set '%s'", tostring(key)))
    end
    class.__propget = classGetters
    class.__propset = classSetters
    return class
end

local EntityMT = Entity.Inherit("Entity", nil, getters, setters)
local EntityT = ffi.metatype("struct Entity", EntityMT)

ffichecks.pointertoentity = function(pointer)
    if pointer == nil then
        return nil
    end
    local result = ffi.cast(entityType, pointer) return result
end

Entity.Class = setmetatable({}, { __class = EntityMT })

_G.Entity = Entity.Class

return Entity
