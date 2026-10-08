ffi.cdef [[
    struct Game {
        private struct Room* CurrentRoom : 0x18300;
        private struct ItemPool ItemPoolValue : 0x1a740;
        private struct Seeds SeedsValue : 0x1bb84;
        private struct HUD HUDValue : 0x1d9ac;
        int FrameCount : 0x25a70;
        private int TimeCounterValue : 0x25a74;
        private int BossRushParTimeValue : 0x25a78;
        private int BlueWombParTimeValue : 0x25a7c;
        private int ScreenShakeCountdown : 0x25a80;
        private struct Vector ScreenShakeOffsetValue : 0x25a84;
        private int DarknessDuration : 0x25a8c;
        private float DarknessModifier : 0x25a90;
        private float TargetDarkness : 0x25a94;
        private int HallucinationFrames : 0x25ab0;
        private unsigned int DebugFlags : 0x25abc;
        private int LastDevilRoomStage : 0x25acc;
        private int TreasureRoomsVisited : 0x25ad0;
        private int PlanetariumsVisited : 0x25ad4;
        private int StagesWithoutHeartsPicked : 0x25ad8;
        private int StagesWithoutDamage : 0x25adc;
        private int DevilRoomDeals : 0x25ae0;
        private int DonationModGreed : 0x25ae4;
        private int DonationModAngel : 0x25ae8;
        private int ShopVisits : 0x25aec;
        private unsigned int ChallengeValue : 0x25afc;
        private bool Rerun : 0x25b00;
        private int VictoryLap : 0x25cec;
        private int LastLevelWithoutHalfHp : 0x25f38;
        private unsigned int DifficultyValue : 0x25f40;
        private int LastLevelWithDamage : 0x25f44;
        private uint8_t* EncounteredBossesBegin : 0x25f4c;
        private uint8_t* EncounteredBossesEnd : 0x25f50;
        private bool StartingFromState : 0x25f61;
        private struct ColorModifier CurrentColorModifier : 0x66920;
        private struct ColorModifier TargetColorModifier : 0x66938;
        private struct ColorModifier LerpColorModifier : 0x66950;
        private float LightningIntensity : 0x6699c;
        private float DizzyIntensity : 0x669a0;
        private float DizzyTargetIntensity : 0x669a4;
    } : 0x680e8;
    typedef struct Game* GamePtr;

    struct Game* L_Game_Get();
    void L_Game_Update(struct Game*);
    void L_Game_Render(struct Game*);
    bool L_Game_IsPaused(struct Game*);
    void L_Game_Fadein(struct Game*, float, bool, struct KColor*);
    void L_Game_Fadeout(struct Game*, float, int, struct KColor*);
    void L_Game_End(struct Game*, int);
    int L_Game_GetNumPlayers(struct Game*);
    void* L_Game_GetPlayer(struct Game*, int);
    void* L_Game_GetNearestPlayer(struct Game*, struct Vector*);
    void* L_Game_GetRandomPlayer(struct Game*, struct Vector*, float);
    struct Font* L_Game_GetFont();
    void L_Game_ShowFortune(struct Game*);
    void L_Game_ShowRule(struct Game*);
    void L_Game_ChangeRoom(struct Game*, int, int);
    void L_Game_StartRoomTransition(struct Game*, int, int, int, void*, int);
    void L_Game_StartStageTransition(struct Game*, bool, int, void*);
    void L_Game_MoveToRandomRoom(struct Game*, bool, int, void*);
    bool L_Game_GetStateFlag(struct Game*, int);
    void L_Game_SetStateFlag(struct Game*, int, bool);
    void L_Game_NextVictoryLap(struct Game*);
    void L_Game_RerollLevelCollectibles(struct Game*);
    void L_Game_RerollLevelPickups(struct Game*, int);
    void L_Game_FinishChallenge(struct Game*);
    void L_Game_AddPixelation(struct Game*, int);
    void L_Game_ShowHallucination(struct Game*, int, int);
    bool L_Game_RerollEnemy(struct Game*, void*, bool);
    void L_Game_AddEncounteredBoss(struct Game*, int, int);
    bool L_Game_HasEncounteredBoss(struct Game*, int, int);
    void L_Game_AddDevilRoomDeal(struct Game*);
    void L_Game_SetChallenge(struct Game*, int);
    void L_Game_ShakeScreen(struct Game*, int);
    void* L_Game_Spawn(struct Game*, int, int, struct Vector*, struct Vector*, void*, int, int);
    void L_Game_BombDamage(struct Game*, struct Vector*, float, float, bool, void*, struct BitSet128*, unsigned long long, bool);
    void L_Game_BombExplosionEffects(struct Game*, struct Vector*, float, struct BitSet128*, struct Color*, void*, float, bool, unsigned long long, bool);
    void L_Game_BombTearflagEffects(struct Game*, struct Vector*, float, struct BitSet128*, void*, float);
    void L_Game_ButterBeanFart(struct Game*, struct Vector*, float, void*, bool, bool);
    void L_Game_CharmFart(struct Game*, struct Vector*, float, void*);
    void L_Game_Fart(struct Game*, struct Vector*, float, void*, float, int, struct Color*);
    void L_Game_MakeShockwave(struct Game*, struct Vector*, float, float, int);
    void L_Game_SpawnParticles(struct Game*, struct Vector*, int, int, float, struct Color*, float, int);
    void L_Game_UpdateStrangeAttractor(struct Game*, struct Vector*, float, float);
    void* L_Game_SpawnBombCrater(struct Game*, struct Vector*, float);
    void L_Game_DevolveEnemy(struct Game*, void*);
    void* L_Game_ChainLightning(struct Game*, struct Vector*, float, struct BitSet128*, void*);
    void L_Game_AddErasedEnemy(struct Game*, void*);
    void L_Game_AddErasedEnemyByIds(struct Game*, int, int);
    void L_Game_RemoveErasedEnemy(struct Game*, int, int);
    bool L_Game_IsErased(struct Game*, int, int, int);
    struct ChallengeParam* L_Game_GetChallengeParams(struct Game*);
    bool L_Game_IsErasedEntity(struct Game*, void*);
    void L_Game_ClearErasedEnemies(struct Game*);
    bool L_Game_AchievementUnlocksDisallowed(struct Game*);
    bool L_Game_IsPauseMenuOpen(struct Game*);
    int L_Game_GetPauseMenuState(struct Game*);
    bool L_Game_IsHardMode(struct Game*);
    bool L_Game_IsGreedBoss(struct Game*);
    bool L_Game_IsGreedFinalBoss(struct Game*);
    bool L_Game_IsGreedMode(struct Game*);
    void L_Game_SetColorModifier(struct Game*, struct ColorModifier*, bool, float);
    void L_Game_SetBloom(struct Game*, int, float);
    void L_Game_ShowGenericLeaderboard(struct Game*);
    void L_Game_CopyGenericPrompt(struct Game*, struct GenericPrompt*);
    void L_Game_AddShopVisits(struct Game*, int);
    void L_Game_RecordPlayerCompletion(int);
]]

local ffi = ffi
local repentogon = ffidll

local LevelPtr = ffi.typeof("struct Level*")
local GenericPromptT = ffi.typeof("struct GenericPrompt")
local EntityToPointer = ffichecks.entitytopointer
local PlayerToPointer = ffichecks.playertopointer

local function ToBitSet128(value)
    if ffichecks.isnumber(value) then
        return BitSet128(value, 0)
    end
    return value
end

local function Clamp(value, min, max)
    if value < min then
        return min
    end
    if value > max then
        return max
    end
    return value
end

local function Getter(field)
    return function(self)
        return ffi.getprivate(self, field)
    end
end

local function Incrementer(field)
    return function(self)
        ffi.setprivate(self, field, ffi.getprivate(self, field) + 1)
    end
end

local function Resetter(field)
    return function(self)
        ffi.setprivate(self, field, 0)
    end
end

local function Adder(field)
    return function(self, amount)
        amount = ffichecks.checkinteger(1, amount)
        ffi.setprivate(self, field, ffi.getprivate(self, field) + amount)
    end
end

local function ClampedSetter(field)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, Clamp(value, 0, 13))
    end
end

local function ColorModifierGetter(field)
    return function(self)
        local color = ffi.getprivate(self, field)
        return ColorModifier(color.R, color.G, color.B, color.A, color.Brightness, color.Contrast)
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

local getters = {
    BlueWombParTime = Getter("BlueWombParTimeValue"),
    BossRushParTime = Getter("BossRushParTimeValue"),
    Challenge = Getter("ChallengeValue"),
    Difficulty = Getter("DifficultyValue"),
    ScreenShakeOffset = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "ScreenShakeOffsetValue"))
    end,
    TimeCounter = Getter("TimeCounterValue"),
}

local setters = {
    BlueWombParTime = function(self, value)
        value = ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "BlueWombParTimeValue", value)
    end,
    BossRushParTime = function(self, value)
        value = ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "BossRushParTimeValue", value)
    end,
    Challenge = function(self, value)
        value = ffichecks.checkinteger(3, value)
        repentogon.L_Game_SetChallenge(self, value)
    end,
    Difficulty = function(self, value)
        value = ffichecks.checkinteger(3, value)
        if value >= 0 and value <= 3 then
            ffi.setprivate(self, "DifficultyValue", value)
        end
    end,
    ScreenShakeOffset = function(self, value)
        ffichecks.checkcdata(3, value, "Vector")
        local offset = ffi.getprivate(self, "ScreenShakeOffsetValue")
        offset.X = value.X
        offset.Y = value.Y
    end,
    TimeCounter = function(self, value)
        value = ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "TimeCounterValue", value)
    end,
}

local GameMT
GameMT = {
    __type = "Game",

    __index = function(self, key)
        local getter = getters[key]
        if getter then
            return getter(self)
        end
        return GameMT[key]
    end,

    __newindex = function(self, key, value)
        local setter = setters[key]
        if setter then
            return setter(self, value)
        end
        error(string.format("cannot set '%s'", tostring(key)))
    end,

    AchievementUnlocksDisallowed = BoolMethod(repentogon.L_Game_AchievementUnlocksDisallowed),
    AddDebugFlags = function(self, flags)
        flags = ffichecks.checkinteger(1, flags)
        ffi.setprivate(self, "DebugFlags", ffi.getprivate(self, "DebugFlags") | flags)
    end,
    AddDevilRoomDeal = VoidMethod(repentogon.L_Game_AddDevilRoomDeal),
    AddEncounteredBoss = function(self, type, variant)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        repentogon.L_Game_AddEncounteredBoss(self, type, variant)
    end,
    AddErasedEnemy = function(self, entityOrType, variant)
        if ffichecks.isnumber(entityOrType) then
            variant = ffichecks.checkinteger(2, variant)
            repentogon.L_Game_AddErasedEnemyByIds(self, entityOrType, variant)
        else
            repentogon.L_Game_AddErasedEnemy(self, EntityToPointer(entityOrType))
        end
    end,
    AddPixelation = function(self, duration)
        duration = ffichecks.checkinteger(1, duration)
        repentogon.L_Game_AddPixelation(self, duration)
    end,
    AddShopVisits = function(self, visitCount)
        visitCount = ffichecks.checkinteger(1, visitCount)
        repentogon.L_Game_AddShopVisits(self, visitCount)
    end,
    AddStageWithoutDamage = Incrementer("StagesWithoutDamage"),
    AddStageWithoutHeartsPicked = Incrementer("StagesWithoutHeartsPicked"),
    AddTreasureRoomsVisited = Incrementer("TreasureRoomsVisited"),
    BombDamage = function(self, position, damage, radius, lineCheck, source, tearFlags, damageFlags, damageSource)
        ffichecks.checkcdata(1, position, "Vector")
        damage = ffichecks.checknumber(2, damage)
        radius = ffichecks.checknumber(3, radius)
        if damageFlags == nil then
            damageFlags = DamageFlag.DAMAGE_EXPLOSION
        else
            damageFlags = ffichecks.checkinteger(7, damageFlags)
        end
        repentogon.L_Game_BombDamage(self, position, damage, radius, lineCheck ~= false, EntityToPointer(source),
            ToBitSet128(tearFlags), damageFlags, ffichecks.optboolean(damageSource, false))
    end,
    BombExplosionEffects = function(self, position, damage, tearFlags, color, source, radiusMult, lineCheck, damageSource, damageFlags)
        ffichecks.checkcdata(1, position, "Vector")
        damage = ffichecks.checknumber(2, damage)
        radiusMult = ffichecks.optnumber(radiusMult, 1)
        if damageFlags == nil then
            damageFlags = DamageFlag.DAMAGE_EXPLOSION
        else
            damageFlags = ffichecks.checkinteger(9, damageFlags)
        end
        repentogon.L_Game_BombExplosionEffects(self, position, damage, ToBitSet128(tearFlags), ffichecks.optcdata(color, "Color", nil),
            EntityToPointer(source), radiusMult, lineCheck ~= false, damageFlags, ffichecks.optboolean(damageSource, false))
    end,
    BombTearflagEffects = function(self, position, radius, tearFlags, source, radiusMult)
        ffichecks.checkcdata(1, position, "Vector")
        radius = ffichecks.checknumber(2, radius)
        ffichecks.checkcdata(3, tearFlags, "BitSet128")
        repentogon.L_Game_BombTearflagEffects(self, position, radius, tearFlags, EntityToPointer(source), ffichecks.optnumber(radiusMult, 1))
    end,
    ButterBeanFart = function(self, position, radius, source, showEffect, doSuperKnockback)
        ffichecks.checkcdata(1, position, "Vector")
        radius = ffichecks.checknumber(2, radius)
        showEffect = ffichecks.checkboolean(4, showEffect)
        doSuperKnockback = ffichecks.checkboolean(5, doSuperKnockback)
        repentogon.L_Game_ButterBeanFart(self, position, radius, EntityToPointer(source), showEffect, doSuperKnockback)
    end,
    ChainLightning = function(self, position, baseDamage, tearFlags, spawner)
        ffichecks.checkcdata(1, position, "Vector")
        baseDamage = ffichecks.optnumber(baseDamage, 3.5)
        local effect = repentogon.L_Game_ChainLightning(self, position, baseDamage, ToBitSet128(tearFlags), EntityToPointer(spawner))
        return ffichecks.pointertoeffect(effect)
    end,
    ChangeRoom = function(self, roomIndex, dimension)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        dimension = ffichecks.optnumber(dimension, -1)
        dimension = ffichecks.checkinteger(2, dimension)
        repentogon.L_Game_ChangeRoom(self, roomIndex, dimension)
    end,
    CharmFart = function(self, position, radius, source)
        ffichecks.checkcdata(1, position, "Vector")
        radius = ffichecks.checknumber(2, radius)
        repentogon.L_Game_CharmFart(self, position, radius, EntityToPointer(source))
    end,
    ClearDonationModAngel = Resetter("DonationModAngel"),
    ClearDonationModGreed = Resetter("DonationModGreed"),
    ClearErasedEnemies = VoidMethod(repentogon.L_Game_ClearErasedEnemies),
    ClearStagesWithoutDamage = Resetter("StagesWithoutDamage"),
    ClearStagesWithoutHeartsPicked = Resetter("StagesWithoutHeartsPicked"),
    Darken = function(self, amount, duration)
        amount = ffichecks.checknumber(1, amount)
        duration = ffichecks.checkinteger(2, duration)
        ffi.setprivate(self, "TargetDarkness", amount)
        ffi.setprivate(self, "DarknessDuration", duration)
    end,
    DevolveEnemy = function(self, entity)
        repentogon.L_Game_DevolveEnemy(self, EntityToPointer(entity))
    end,
    DonateAngel = Adder("DonationModAngel"),
    DonateGreed = Adder("DonationModGreed"),
    End = function(self, endingID)
        endingID = ffichecks.checkinteger(1, endingID)
        repentogon.L_Game_End(self, endingID)
    end,
    Fadein = function(self, speed, showIcon, color)
        speed = ffichecks.checknumber(1, speed)
        if showIcon == nil then
            showIcon = true
        end
        repentogon.L_Game_Fadein(self, speed, not not showIcon, ffichecks.optcdata(color, "KColor", KColor(0, 0, 0, 1)))
    end,
    Fadeout = function(self, speed, target, color)
        speed = ffichecks.checknumber(1, speed)
        target = ffichecks.checkinteger(2, target)
        repentogon.L_Game_Fadeout(self, speed, target, ffichecks.optcdata(color, "KColor", KColor(0, 0, 0, 1)))
    end,
    Fart = function(self, position, radius, source, fartScale, fartSubType, color)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_Game_Fart(self, position, ffichecks.optnumber(radius, 85), EntityToPointer(source), ffichecks.optnumber(fartScale, 1),
            ffichecks.optnumber(fartSubType, 0), ffichecks.optcdata(color, "Color", nil))
    end,
    FinishChallenge = VoidMethod(repentogon.L_Game_FinishChallenge),
    GetAmbush = function(self)
        return Ambush
    end,
    GetChallengeParams = function(self)
        return repentogon.L_Game_GetChallengeParams(self)
    end,
    GetCurrentColorModifier = ColorModifierGetter("CurrentColorModifier"),
    GetDarknessModifier = Getter("DarknessModifier"),
    GetDebugFlags = Getter("DebugFlags"),
    GetDevilRoomDeals = Getter("DevilRoomDeals"),
    GetDizzyAmount = Getter("DizzyIntensity"),
    GetDonationModAngel = Getter("DonationModAngel"),
    GetDonationModGreed = Getter("DonationModGreed"),
    GetFont = function(self)
        return repentogon.L_Game_GetFont()
    end,
    GetFrameCount = function(self)
        return self.FrameCount
    end,
    GetGenericPrompt = function(self)
        local prompt = GenericPromptT()
        repentogon.L_Game_CopyGenericPrompt(self, prompt)
        return prompt
    end,
    GetGreedBossWaveNum = function(self)
        return ffi.getprivate(self, "DifficultyValue") == 3 and 10 or 9
    end,
    GetGreedWavesNum = function(self)
        return ffi.getprivate(self, "DifficultyValue") == 3 and 12 or 11
    end,
    GetHUD = Getter("HUDValue"),
    GetItemOverlay = function(self)
        return ItemOverlay
    end,
    GetItemPool = Getter("ItemPoolValue"),
    GetLastDevilRoomStage = Getter("LastDevilRoomStage"),
    GetLastLevelWithDamage = Getter("LastLevelWithDamage"),
    GetLastLevelWithoutHalfHp = Getter("LastLevelWithoutHalfHp"),
    GetLerpColorModifier = ColorModifierGetter("LerpColorModifier"),
    GetLevel = function(self)
        return ffi.cast(LevelPtr, self)
    end,
    GetNearestPlayer = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        return ffichecks.pointertoplayer(repentogon.L_Game_GetNearestPlayer(self, position))
    end,
    GetNumEncounteredBosses = function(self)
        return ffichecks.vectorsize(ffi.getprivate(self, "EncounteredBossesBegin"), ffi.getprivate(self, "EncounteredBossesEnd"), 8)
    end,
    GetNumPlayers = function(self)
        return repentogon.L_Game_GetNumPlayers(self)
    end,
    GetPauseMenuState = function(self)
        return repentogon.L_Game_GetPauseMenuState(self)
    end,
    GetPlanetariumsVisited = Getter("PlanetariumsVisited"),
    GetPlayer = function(self, index)
        index = ffichecks.optnumber(index, 0)
        index = ffichecks.checkinteger(1, index)
        return ffichecks.pointertoplayer(repentogon.L_Game_GetPlayer(self, index))
    end,
    GetRandomPlayer = function(self, position, radius)
        ffichecks.checkcdata(1, position, "Vector")
        radius = ffichecks.checknumber(2, radius)
        return ffichecks.pointertoplayer(repentogon.L_Game_GetRandomPlayer(self, position, radius))
    end,
    GetRoom = Getter("CurrentRoom"),
    GetScreenShakeCountdown = Getter("ScreenShakeCountdown"),
    GetSeeds = Getter("SeedsValue"),
    GetShopVisits = Getter("ShopVisits"),
    GetStagesWithoutDamage = Getter("StagesWithoutDamage"),
    GetStagesWithoutHeartsPicked = Getter("StagesWithoutHeartsPicked"),
    GetStateFlag = function(self, flag)
        flag = ffichecks.checkinteger(1, flag)
        return repentogon.L_Game_GetStateFlag(self, flag)
    end,
    GetTargetColorModifier = ColorModifierGetter("TargetColorModifier"),
    GetTargetDarkness = Getter("TargetDarkness"),
    GetTreasureRoomVisitCount = Getter("TreasureRoomsVisited"),
    GetVictoryLap = Getter("VictoryLap"),
    HasEncounteredBoss = function(self, type, variant)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        return repentogon.L_Game_HasEncounteredBoss(self, type, variant)
    end,
    HasHallucination = function(self)
        return ffi.getprivate(self, "HallucinationFrames") > 0
    end,
    IsErased = function(self, entityOrType, variant, subtype)
        if ffichecks.isnumber(entityOrType) then
            variant = ffichecks.optnumber(variant, -1)
            variant = ffichecks.checkinteger(2, variant)
            subtype = ffichecks.optnumber(subtype, -1)
            subtype = ffichecks.checkinteger(3, subtype)
            return repentogon.L_Game_IsErased(self, entityOrType, variant, subtype)
        end
        return repentogon.L_Game_IsErasedEntity(self, EntityToPointer(entityOrType))
    end,
    IsGreedBoss = BoolMethod(repentogon.L_Game_IsGreedBoss),
    IsGreedFinalBoss = BoolMethod(repentogon.L_Game_IsGreedFinalBoss),
    IsGreedMode = BoolMethod(repentogon.L_Game_IsGreedMode),
    IsHardMode = BoolMethod(repentogon.L_Game_IsHardMode),
    IsPauseMenuOpen = BoolMethod(repentogon.L_Game_IsPauseMenuOpen),
    IsPaused = BoolMethod(repentogon.L_Game_IsPaused),
    IsRerun = Getter("Rerun"),
    IsStartingFromState = Getter("StartingFromState"),
    MakeShockwave = function(self, position, amplitude, speed, duration)
        ffichecks.checkcdata(1, position, "Vector")
        amplitude = ffichecks.checknumber(2, amplitude)
        speed = ffichecks.checknumber(3, speed)
        duration = ffichecks.checkinteger(4, duration)
        repentogon.L_Game_MakeShockwave(self, position, amplitude, speed, duration)
    end,
    MoveToRandomRoom = function(self, iAmErrorRoom, seed, player)
        seed = ffichecks.checkinteger(2, seed)
        if seed == 0 then
            error("The given seed is not valid", 2)
        end
        repentogon.L_Game_MoveToRandomRoom(self, not not iAmErrorRoom, seed, PlayerToPointer(player))
    end,
    NextVictoryLap = VoidMethod(repentogon.L_Game_NextVictoryLap),
    RecordPlayerCompletion = function(self, event)
        event = ffichecks.checkinteger(1, event)
        if event < 0 or event > 17 then
            error(string.format("Bad CompletionType %d (valid range is 0-17)", event), 2)
        end
        repentogon.L_Game_RecordPlayerCompletion(event)
    end,
    Render = VoidMethod(repentogon.L_Game_Render),
    RemoveErasedEnemy = function(self, type, variant)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.optnumber(variant, -1)
        variant = ffichecks.checkinteger(2, variant)
        repentogon.L_Game_RemoveErasedEnemy(self, type, variant)
    end,
    RerollEnemy = function(self, entity, unk)
        return repentogon.L_Game_RerollEnemy(self, EntityToPointer(entity), ffichecks.optboolean(unk, false))
    end,
    RerollLevelCollectibles = VoidMethod(repentogon.L_Game_RerollLevelCollectibles),
    RerollLevelPickups = function(self, seed)
        seed = ffichecks.checkinteger(1, seed)
        repentogon.L_Game_RerollLevelPickups(self, seed)
    end,
    SetBloom = function(self, time, strength)
        time = ffichecks.checkinteger(1, time)
        strength = ffichecks.checknumber(2, strength)
        repentogon.L_Game_SetBloom(self, time, strength)
    end,
    SetColorModifier = function(self, color, lerp, rate)
        ffichecks.checkcdata(1, color, "ColorModifier")
        if lerp == nil then
            lerp = true
        end
        repentogon.L_Game_SetColorModifier(self, color, not not lerp, ffichecks.optnumber(rate, 0.015))
    end,
    SetDizzyAmount = function(self, targetIntensity, currentIntensity)
        targetIntensity = ffichecks.checknumber(1, targetIntensity)
        if currentIntensity == nil then
            currentIntensity = ffi.getprivate(self, "DizzyIntensity")
        else
            currentIntensity = ffichecks.checknumber(2, currentIntensity)
        end
        ffi.setprivate(self, "DizzyTargetIntensity", targetIntensity)
        ffi.setprivate(self, "DizzyIntensity", currentIntensity)
    end,
    SetDonationModAngel = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "DonationModAngel", value)
    end,
    SetDonationModGreed = function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, "DonationModGreed", value)
    end,
    SetLastDevilRoomStage = ClampedSetter("LastDevilRoomStage"),
    SetLastLevelWithDamage = ClampedSetter("LastLevelWithDamage"),
    SetLastLevelWithoutHalfHp = ClampedSetter("LastLevelWithoutHalfHp"),
    SetStateFlag = function(self, flag, value)
        flag = ffichecks.checkinteger(1, flag)
        repentogon.L_Game_SetStateFlag(self, flag, ffichecks.optboolean(value, false))
    end,
    ShakeScreen = function(self, timeout)
        timeout = ffichecks.checkinteger(1, timeout)
        repentogon.L_Game_ShakeScreen(self, timeout)
    end,
    ShowFortune = VoidMethod(repentogon.L_Game_ShowFortune),
    ShowGenericLeaderboard = VoidMethod(repentogon.L_Game_ShowGenericLeaderboard),
    ShowHallucination = function(self, frameCount, backdrop)
        frameCount = ffichecks.checkinteger(1, frameCount)
        if backdrop == nil then
            backdrop = BackdropType.NUM_BACKDROPS
        else
            backdrop = ffichecks.checkinteger(2, backdrop)
        end
        repentogon.L_Game_ShowHallucination(self, frameCount, backdrop)
    end,
    ShowRule = VoidMethod(repentogon.L_Game_ShowRule),
    Spawn = function(self, type, variant, position, velocity, spawner, subtype, seed)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        ffichecks.checkcdata(3, position, "Vector")
        ffichecks.checkcdata(4, velocity, "Vector")
        subtype = ffichecks.checkinteger(6, subtype)
        seed = ffichecks.checkinteger(7, seed)
        return ffichecks.pointertoentity(repentogon.L_Game_Spawn(self, type, variant, position, velocity, EntityToPointer(spawner), subtype, seed))
    end,
    SpawnBombCrater = function(self, position, radius)
        ffichecks.checkcdata(1, position, "Vector")
        return ffichecks.pointertoentity(repentogon.L_Game_SpawnBombCrater(self, position, ffichecks.optnumber(radius, 1)))
    end,
    SpawnParticles = function(self, position, variant, num, speed, color, height, subtype)
        ffichecks.checkcdata(1, position, "Vector")
        variant = ffichecks.checkinteger(2, variant)
        num = ffichecks.checkinteger(3, num)
        speed = ffichecks.checknumber(4, speed)
        repentogon.L_Game_SpawnParticles(self, position, variant, num, speed, ffichecks.optcdata(color, "Color", nil),
            ffichecks.optnumber(height, 100000), ffichecks.optnumber(subtype, 0))
    end,
    StartRoomTransition = function(self, roomIndex, direction, animation, player, dimension)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        direction = ffichecks.checkinteger(2, direction)
        animation = ffichecks.optnumber(animation, RoomTransitionAnim.WALK)
        animation = ffichecks.checkinteger(3, animation)
        dimension = ffichecks.optnumber(dimension, -1)
        dimension = ffichecks.checkinteger(5, dimension)
        repentogon.L_Game_StartRoomTransition(self, roomIndex, direction, animation, PlayerToPointer(player), dimension)
    end,
    StartStageTransition = function(self, sameStage, transition, player)
        sameStage = ffichecks.checkboolean(1, sameStage)
        transition = ffichecks.checkinteger(2, transition)
        repentogon.L_Game_StartStageTransition(self, sameStage, transition, PlayerToPointer(player))
    end,
    Update = VoidMethod(repentogon.L_Game_Update),
    UpdateStrangeAttractor = function(self, position, force, radius)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_Game_UpdateStrangeAttractor(self, position, ffichecks.optnumber(force, 10), ffichecks.optnumber(radius, 250))
    end,
}

GameMT.__propget = getters
GameMT.__propset = setters

ffi.metatype("struct Game", GameMT)

Game = setmetatable({}, {
    __class = GameMT,
    __call = function()
        return repentogon.L_Game_Get()
    end,
})
