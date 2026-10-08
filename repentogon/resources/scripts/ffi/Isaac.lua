ffi.cdef [[
    int L_Isaac_FindByType(int, int, int, bool, bool);
    int L_Isaac_GetRoomEntities();
    int L_Isaac_FindInRadius(struct Vector*, float, unsigned int);
    int L_Isaac_FindInCapsule(struct Capsule*, unsigned int);
    void L_Isaac_Explode(struct Vector*, void*, float);
    void L_Isaac_GetFreeNearPosition(struct Vector*, struct Vector*, float);
    void L_Isaac_GetRandomPosition(struct Vector*);
    struct GridEntity* L_Isaac_GridSpawn(int, int, struct Vector*, bool);
    void L_Isaac_ScreenToWorld(struct Vector*, struct Vector*);
    void L_Isaac_ScreenToWorldDistance(struct Vector*, struct Vector*);
    void* L_Isaac_Spawn(int, int, int, struct Vector*, struct Vector*, void*);
    void L_Isaac_WorldToRenderPosition(struct Vector*, struct Vector*);
    void L_Isaac_WorldToScreen(struct Vector*, struct Vector*);
    void L_Isaac_WorldToScreenDistance(struct Vector*, struct Vector*);
    void L_Isaac_StartNewGame(int, int, unsigned int, struct Seeds*, unsigned int, bool);
    void L_Isaac_RenderToWorld(struct Vector*, struct Vector*);
    void L_Isaac_DrawLine(struct Vector*, struct Vector*, struct KColor*, struct KColor*, float);
    void L_Isaac_DrawQuad(struct Vector*, struct Vector*, struct Vector*, struct Vector*, struct KColor*, float);
    bool L_Isaac_SetClipboard(const char*);
    const char* L_Isaac_GetClipboard();
    int L_Isaac_GetSubTypeByName(const char*);
    void L_Isaac_PlayCutscene(unsigned int, bool);
    unsigned int L_Isaac_GetRoomSpawnSeed();
    void* L_Isaac_SpawnBoss(unsigned int, unsigned int, unsigned int, struct Vector*, struct Vector*, void*, unsigned int);
    int L_Isaac_GetCutsceneByName(const char*);
    int L_Isaac_GetGiantBookByName(const char*);
    bool L_Isaac_CanStartTrueCoop();
    int L_Isaac_GetNullItemIdByName(const char*);
    int L_Isaac_ShowErrorDialog(const char*, const char*, int, int);
    struct Sprite* L_Isaac_GetCursorSprite();
    void L_Isaac_GetRenderPosition(struct Vector*, struct Vector*, bool);
    void L_Isaac_GetCollectibleSpawnPosition(struct Vector*, struct Vector*);
    void L_Isaac_TriggerWindowResize();
    void L_Isaac_CenterCursor();
    int L_Isaac_SetDwmWindowAttribute(int, int);
    int L_Isaac_GetDwmWindowAttribute(int);
    void L_Isaac_SetWindowTitle(const char*);
    const char* L_Isaac_GetWindowTitle();
    bool L_Isaac_IsInGame();
    bool L_Isaac_IsChallengeDone(int);
    void L_Isaac_ClearChallenge(int);
    void L_Isaac_UndoChallenge(int);
    int L_Isaac_GetModChallengeCompletion(const char*, const char*);
    int L_Isaac_GetModChallengeClearCount(int);
    int L_Isaac_GetBossColorIdxByName(const char*);
    int L_Isaac_GetBackdropTypeByName(const char*);
    const char* L_Isaac_GetChangelog();
    void L_Isaac_SetIconID(int, bool);
    bool L_Isaac_SetIconPath(const char*, bool);
    int L_Isaac_FindTargetPit(struct Vector*, struct Vector*, int);
    void L_Isaac_GetAxisAlignedUnitVectorFromDir(struct Vector*, int);
    void L_Isaac_StartDailyGame(unsigned int);
    bool L_Isaac_IsShuttingDown();
    struct Sprite* L_Isaac_GetButtonsSprite();
    int64_t L_Isaac_GetNanoTime();
    bool L_Isaac_ReworkCollectible(int);
    bool L_Isaac_ReworkBirthright(int);
    bool L_Isaac_ReworkTrinket(int);
    bool L_Isaac_RenderCollectionItem(int, struct Vector*, struct Vector*, struct Color*);
    const char* L_Isaac_LoadModDataFromFolder(const char*);
    int L_Isaac_GetBabyIdByName(const char*);
    void L_Isaac_ClearBossHazards(bool);
    bool L_Isaac_CreateWeapon(int, void*, struct Weapon**);
    void L_Isaac_DestroyWeapon(struct Weapon*);

    const char* L_Isaac_GetString(const char*, const char*);
    unsigned int L_Isaac_GetLanguageId(const char*);
    const char* L_Isaac_GetLocalizedString(const char*, const char*, unsigned int);
    int L_Isaac_WorldToMenuPosition(int, struct Vector*, struct Vector*);
    int L_Isaac_GetAchievementIdByName(const char*);
    int L_Isaac_GetPoolIdByName(const char*);
    void L_Isaac_SetCurrentFloorMusic(int);
    void L_Isaac_SetCurrentFloorBackdrop(int);
    void L_Isaac_SetCurrentFloorName(const char*);
    int L_Isaac_GetCurrentStageConfigId();

    bool L_Isaac_CompletionMarksInitialized();
    int L_Isaac_GetCompletionMarkIndex(const char*);
    void L_Isaac_SetCompletionMarks(int, const int*);
    int L_Isaac_GetCompletionMark(int, int);
    void L_Isaac_SetCompletionMark(int, int, int);
    void L_Isaac_ClearCompletionMarks(int);
    void L_Isaac_FillCompletionMarks(int);
    int L_Isaac_AllTaintedCompletion(int, int);
    int L_Isaac_AllMarksFilled(int);
    void L_Isaac_GetCompletionMarks(int, int*);
    bool L_Isaac_GetCompletionMarkData(const char*, const char*, bool, int*);

    void L_Isaac_DebugString(const char*);
    void* L_Isaac_GetPlayer(int);
    unsigned int L_Isaac_GetFrameCount();
    unsigned int L_Isaac_GetChallenge();
    struct Font* L_Isaac_GetTextFont();
    void L_Isaac_AddPillEffectToPool(int);
    int L_Isaac_GetEntityTypeByName(const char*);
    int L_Isaac_GetEntityVariantByName(const char*);
    int L_Isaac_GetItemIdByName(const char*);
    int L_Isaac_GetPlayerTypeByName(const char*, bool);
    int L_Isaac_GetCardIdByName(const char*);
    int L_Isaac_GetPillEffectByName(const char*);
    int L_Isaac_GetTrinketIdByName(const char*);
    int L_Isaac_GetChallengeIdByName(const char*);
    int L_Isaac_GetCostumeIdByPath(const char*);
    int L_Isaac_GetCurseIdByName(const char*);
    int L_Isaac_GetSoundIdByName(const char*);
    int L_Isaac_GetMusicIdByName(const char*);
    unsigned int L_Isaac_GetTime();
    const char* L_Isaac_ExecuteCommand(const char*);
    void L_Isaac_ConsoleOutput(const char*);
    int L_Isaac_CountEntities(void*, int, int, int);
    float L_Isaac_GetScreenWidth();
    float L_Isaac_GetScreenHeight();
    float L_Isaac_GetScreenPointScale();
    void L_Isaac_RegisterMod(int, const char*, int);
    void L_Isaac_SaveModData(int, const char*);
    const char* L_Isaac_LoadModData(int);
    bool L_Isaac_HasModData(int);
    void L_Isaac_RemoveModData(int);
    void L_Isaac_SetBuiltInCallbackState(int, bool);
    bool L_Isaac_GetBuiltInCallbackState(int);
]]

local ffi = ffi
local repentogon = ffidll
local getregistry = debug.getregistry

-- Setting a mark fires MC_POST_COMPLETION_MARK from inside the call
ffi.reentrant(repentogon.L_Isaac_SetCompletionMark)
ffi.reentrant(repentogon.L_Isaac_SetCompletionMarks)
local Game = Game

local EntityToPointer = ffichecks.entitytopointer

local weaponOut = ffi.new("struct Weapon*[1]")
local marksOut = ffi.new("int[15]")

local MB_ICONERROR = 0x10
local MB_OK = 0

local function VectorResult(export, ...)
    local result = Vector(0, 0)
    export(result, ...)
    return result
end

local function StringOrNil(pointer)
    if pointer == nil then
        return nil
    end
    local result = ffi.string(pointer) return result
end

local function RegistryRef(value)
    if value == nil then
        return -1
    end

    local registry = getregistry()
    local ref = rawget(registry, 0)
    if type(ref) == "number" and ref ~= 0 then
        rawset(registry, 0, rawget(registry, ref))
    else
        ref = #registry + 1
    end
    rawset(registry, ref, value)
    return ref
end

local function CurrentRoom()
    local game = Game()
    if game == nil then
        return nil
    end
    return game:GetRoom()
end

local function CheckChallengeId(challengeId)
    if challengeId < 1 then
        error(string.format("Invalid Challenge ID (expected > 0, got %d)", challengeId), 3)
    end
end

local MARK_KEYS = {
    { "MomsHeart", 0 }, { "Isaac", 1 }, { "Satan", 2 }, { "BossRush", 3 }, { "BlueBaby", 4 }, { "Lamb", 5 }, { "MegaSatan", 6 },
    { "UltraGreed", 7 }, { "Hush", 9 }, { "UltraGreedier", 11 }, { "Delirium", 12 }, { "Mother", 13 }, { "Beast", 14 },
}

local function MarksToTable(marks, playerType)
    local result = {}
    if playerType ~= nil then
        result.PlayerType = playerType
    end
    for _, entry in ipairs(MARK_KEYS) do
        result[entry[1]] = marks[entry[2]]
    end
    return result
end

local function ToInteger(value)
    local number = tonumber(value)
    if number ~= nil and number == math.floor(number) then
        return number
    end
    return 0
end

local function DeprecatedUndoChallenge(challengeId)
    challengeId = ffichecks.checkinteger(1, challengeId)
    repentogon.L_Isaac_UndoChallenge(challengeId)
end

local isaac = {
    AddPillEffectToPool = function(effect)
        effect = ffichecks.checkinteger(1, effect)
        repentogon.L_Isaac_AddPillEffectToPool(effect)
    end,
    AllMarksFilled = function(playerType)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        playerType = ffichecks.checkinteger(1, playerType)
        local result = repentogon.L_Isaac_AllMarksFilled(playerType) return result
    end,
    AllTaintedCompletion = function(playerType, group)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        playerType = ffichecks.checkinteger(1, playerType)
        group = ffichecks.checkinteger(2, group)
        local result = repentogon.L_Isaac_AllTaintedCompletion(playerType, group) return result
    end,
    CanStartTrueCoop = function()
        local result = repentogon.L_Isaac_CanStartTrueCoop() return result
    end,
    CenterCursor = function()
        repentogon.L_Isaac_CenterCursor()
    end,
    ClearBossHazards = function(ignoreNPCs)
        if Game() == nil or Game():GetRoom() == nil then
            error("Must be in a room to use this!", 2)
        end
        repentogon.L_Isaac_ClearBossHazards(ffichecks.optboolean(ignoreNPCs, false))
    end,
    ClearChallenge = function(challengeId)
        challengeId = ffichecks.checkinteger(1, challengeId)
        CheckChallengeId(challengeId)
        repentogon.L_Isaac_ClearChallenge(challengeId)
    end,
    ClearCompletionMarks = function(playerType)
        playerType = ffichecks.checkinteger(1, playerType)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        repentogon.L_Isaac_ClearCompletionMarks(playerType)
    end,
    ConsoleOutput = function(text)
        text = ffichecks.checkstring(1, text)
        repentogon.L_Isaac_ConsoleOutput(text)
    end,
    CountBosses = function()
        local room = CurrentRoom()
        if room == nil then
            return 0
        end
        local result = math.max(0, room:GetAliveBossesCount()) return result
    end,
    CountEnemies = function()
        local room = CurrentRoom()
        if room == nil then
            return 0
        end
        local result = math.max(0, room:GetAliveEnemiesCount()) return result
    end,
    CountEntities = function(spawner, type, variant, subtype)
        type = ffichecks.optinteger(type, 0)
        variant = ffichecks.optinteger(variant, -1)
        subtype = ffichecks.optinteger(subtype, -1)
        local result = repentogon.L_Isaac_CountEntities(EntityToPointer(spawner), type, variant, subtype) return result
    end,
    CreateWeapon = function(weaponType, entity)
        weaponType = ffichecks.checkinteger(1, weaponType)
        if entity == nil then
            ffichecks.argerror(2, "Entity expected, got nil")
        end
        if not repentogon.L_Isaac_CreateWeapon(weaponType, EntityToPointer(entity), weaponOut) then
            ffichecks.argerror(1, "Invalid WeaponType")
        end
        return weaponOut[0]
    end,
    DebugString = function(text)
        text = ffichecks.checkstring(1, text)
        repentogon.L_Isaac_DebugString(text)
    end,
    DestroyWeapon = function(weapon)
        ffichecks.checkcdata(1, weapon, "Weapon")
        repentogon.L_Isaac_DestroyWeapon(weapon)
    end,
    DrawLine = function(position1, position2, color1, color2, thickness)
        ffichecks.checkcdata(1, position1, "Vector")
        ffichecks.checkcdata(2, position2, "Vector")
        ffichecks.checkcdata(3, color1, "KColor")
        ffichecks.checkcdata(4, color2, "KColor")
        repentogon.L_Isaac_DrawLine(position1, position2, color1, color2, ffichecks.optnumber(thickness, 1))
    end,
    DrawQuad = function(topLeft, topRight, bottomLeft, bottomRight, color, thickness)
        ffichecks.checkcdata(1, topLeft, "Vector")
        ffichecks.checkcdata(2, topRight, "Vector")
        ffichecks.checkcdata(3, bottomLeft, "Vector")
        ffichecks.checkcdata(4, bottomRight, "Vector")
        ffichecks.checkcdata(5, color, "KColor")
        repentogon.L_Isaac_DrawQuad(topLeft, topRight, bottomLeft, bottomRight, color, ffichecks.optnumber(thickness, 1))
    end,
    ExecuteCommand = function(command)
        command = ffichecks.checkstring(1, command)
        local result = ffi.string(repentogon.L_Isaac_ExecuteCommand(command)) return result
    end,
    Explode = function(position, source, damage)
        ffichecks.checkcdata(1, position, "Vector")
        damage = ffichecks.checknumber(3, damage)
        repentogon.L_Isaac_Explode(position, EntityToPointer(source), damage)
    end,
    FillCompletionMarks = function(playerType)
        playerType = ffichecks.checkinteger(1, playerType)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        repentogon.L_Isaac_FillCompletionMarks(playerType)
        return 2
    end,
    FindByType = function(type, variant, subtype, cache, ignoreFriendly)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.optinteger(variant, -1)
        subtype = ffichecks.optinteger(subtype, -1)
        repentogon.L_Isaac_FindByType(type, variant, subtype, ffichecks.optboolean(cache, false), ffichecks.optboolean(ignoreFriendly, false))
        return ffichecks.entityresults()
    end,
    FindInCapsule = function(capsule, partition)
        ffichecks.checkcdata(1, capsule, "Capsule")
        repentogon.L_Isaac_FindInCapsule(capsule, ffichecks.optinteger(partition, -1))
        return ffichecks.entityresults()
    end,
    FindInRadius = function(position, radius, partition)
        ffichecks.checkcdata(1, position, "Vector")
        radius = ffichecks.checknumber(2, radius)
        repentogon.L_Isaac_FindInRadius(position, radius, ffichecks.optinteger(partition, -1))
        return ffichecks.entityresults()
    end,
    FindTargetPit = function(position, targetPosition, pitIndex)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, targetPosition, "Vector")
        local result = repentogon.L_Isaac_FindTargetPit(position, targetPosition, ffichecks.optinteger(pitIndex, -1)) return result
    end,
    GetAchievementIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetAchievementIdByName(name) return result
    end,
    GetAxisAlignedUnitVectorFromDir = function(direction)
        return VectorResult(repentogon.L_Isaac_GetAxisAlignedUnitVectorFromDir, ffichecks.optinteger(direction, -1))
    end,
    GetBabyIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetBabyIdByName(name) return result
    end,
    GetBuiltInCallbackState = function(callbackId)
        callbackId = ffichecks.checkinteger(1, callbackId)
        local result = repentogon.L_Isaac_GetBuiltInCallbackState(callbackId) return result
    end,
    GetCardIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetCardIdByName(name) return result
    end,
    GetChallenge = function()
        local result = repentogon.L_Isaac_GetChallenge() return result
    end,
    GetChallengeIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetChallengeIdByName(name) return result
    end,
    GetCostumeIdByPath = function(path)
        path = ffichecks.checkstring(1, path)
        local result = repentogon.L_Isaac_GetCostumeIdByPath(path) return result
    end,
    GetCurseIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetCurseIdByName(name) return result
    end,
    GetEntityTypeByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetEntityTypeByName(name) return result
    end,
    GetEntityVariantByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetEntityVariantByName(name) return result
    end,
    GetFrameCount = function()
        local result = repentogon.L_Isaac_GetFrameCount() return result
    end,
    GetItemIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetItemIdByName(name) return result
    end,
    GetMusicIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetMusicIdByName(name) return result
    end,
    GetPillEffectByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetPillEffectByName(name) return result
    end,
    GetPlayer = function(index)
        index = ffichecks.optinteger(index, 0)
        return ffichecks.pointertoplayer(repentogon.L_Isaac_GetPlayer(index))
    end,
    GetPlayerTypeByName = function(name, isBSkin)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetPlayerTypeByName(name, ffichecks.optboolean(isBSkin, false)) return result
    end,
    GetScreenHeight = function()
        local result = repentogon.L_Isaac_GetScreenHeight() return result
    end,
    GetScreenPointScale = function()
        local result = repentogon.L_Isaac_GetScreenPointScale() return result
    end,
    GetScreenWidth = function()
        local result = repentogon.L_Isaac_GetScreenWidth() return result
    end,
    GetSoundIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetSoundIdByName(name) return result
    end,
    GetTextWidth = function(text)
        text = ffichecks.checkstring(1, text)
        return repentogon.L_Isaac_GetTextFont():GetStringWidth(text)
    end,
    GetTime = function()
        local result = repentogon.L_Isaac_GetTime() return result
    end,
    GetTrinketIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetTrinketIdByName(name) return result
    end,
    GetBackdropIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetBackdropTypeByName(name) return result
    end,
    GetBossColorIdxByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetBossColorIdxByName(name) return result
    end,
    GetButtonsSprite = function()
        local result = repentogon.L_Isaac_GetButtonsSprite() return result
    end,
    GetClipboard = function()
        return StringOrNil(repentogon.L_Isaac_GetClipboard())
    end,
    GetCollectibleSpawnPosition = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_GetCollectibleSpawnPosition, position)
    end,
    GetCompletionMark = function(playerType, completionType)
        playerType = ffichecks.checkinteger(1, playerType)
        completionType = ffichecks.checkinteger(2, completionType)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        local result = repentogon.L_Isaac_GetCompletionMark(playerType, completionType) return result
    end,
    GetCompletionMarkData = function(modId, playerName, tainted)
        modId = ffichecks.checkstring(1, modId)
        playerName = ffichecks.checkstring(2, playerName)
        tainted = ffichecks.checkboolean(3, tainted)
        if not repentogon.L_Isaac_GetCompletionMarkData(modId, playerName, tainted, marksOut) then
            return nil
        end
        return MarksToTable(marksOut)
    end,
    GetCompletionMarks = function(playerType)
        playerType = ffichecks.checkinteger(1, playerType)
        repentogon.L_Isaac_GetCompletionMarks(playerType, marksOut)
        return MarksToTable(marksOut, playerType)
    end,
    GetCurrentStageConfigId = function()
        local result = repentogon.L_Isaac_GetCurrentStageConfigId() return result
    end,
    GetCursorSprite = function()
        local result = repentogon.L_Isaac_GetCursorSprite() return result
    end,
    GetCutsceneIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetCutsceneByName(name) return result
    end,
    GetDwmWindowAttribute = function(attribute)
        local result = repentogon.L_Isaac_GetDwmWindowAttribute(ffichecks.optinteger(attribute, 0)) return result
    end,
    GetEntitySubTypeByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetSubTypeByName(name) return result
    end,
    GetFreeNearPosition = function(position, step)
        ffichecks.checkcdata(1, position, "Vector")
        step = ffichecks.checknumber(2, step)
        return VectorResult(repentogon.L_Isaac_GetFreeNearPosition, position, step)
    end,
    GetGiantBookIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetGiantBookByName(name) return result
    end,
    GetLoadedModules = function()
        return getregistry()._LOADED
    end,
    GetLocalizedString = function(category, key, language)
        category = ffichecks.checkstring(1, category)
        key = ffichecks.checkstring(2, key)
        if type(language) == "string" then
            language = repentogon.L_Isaac_GetLanguageId(language)
        else
            language = ffichecks.checkinteger(3, language)
        end
        return StringOrNil(repentogon.L_Isaac_GetLocalizedString(category, key, language))
    end,
    GetModChallengeClearCount = function(challengeId)
        challengeId = ffichecks.checkinteger(1, challengeId)
        local result = repentogon.L_Isaac_GetModChallengeClearCount(challengeId) return result
    end,
    GetModChallengeCompletionData = function(modId, challengeName)
        modId = ffichecks.checkstring(1, modId)
        challengeName = ffichecks.checkstring(2, challengeName)
        local completed = repentogon.L_Isaac_GetModChallengeCompletion(modId, challengeName)
        if completed < 0 then
            return nil
        end
        return completed ~= 0
    end,
    GetNanoTime = function()
        local result = tonumber(repentogon.L_Isaac_GetNanoTime()) return result
    end,
    GetNullItemIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetNullItemIdByName(name) return result
    end,
    GetPoolIdByName = function(name)
        name = ffichecks.checkstring(1, name)
        local result = repentogon.L_Isaac_GetPoolIdByName(name) return result
    end,
    GetRandomPosition = function()
        return VectorResult(repentogon.L_Isaac_GetRandomPosition)
    end,
    GetRenderPosition = function(position, scale)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_GetRenderPosition, position, ffichecks.optboolean(scale, true))
    end,
    GetRoomEntities = function()
        repentogon.L_Isaac_GetRoomEntities()
        return ffichecks.entityresults()
    end,
    GetString = function(category, key)
        category = ffichecks.checkstring(1, category)
        key = ffichecks.checkstring(2, key)
        return StringOrNil(repentogon.L_Isaac_GetString(category, key))
    end,
    GetWindowTitle = function()
        local result = ffi.string(repentogon.L_Isaac_GetWindowTitle()) return result
    end,
    GridSpawn = function(type, variant, position, forced)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        ffichecks.checkcdata(3, position, "Vector")
        local result = repentogon.L_Isaac_GridSpawn(type, variant, position, ffichecks.optboolean(forced, false)) return result
    end,
    HasModData = function(mod)
        local result = repentogon.L_Isaac_HasModData(RegistryRef(mod)) return result
    end,
    IsChallengeDone = function(challengeId)
        challengeId = ffichecks.checkinteger(1, challengeId)
        CheckChallengeId(challengeId)
        local result = repentogon.L_Isaac_IsChallengeDone(challengeId) return result
    end,
    IsInGame = function()
        local result = repentogon.L_Isaac_IsInGame() return result
    end,
    IsShuttingDown = function()
        local result = repentogon.L_Isaac_IsShuttingDown() return result
    end,
    LoadModData = function(mod)
        local result = ffi.string(repentogon.L_Isaac_LoadModData(RegistryRef(mod))) return result
    end,
    LoadModDataFromFolder = function(folderName)
        folderName = ffichecks.checkstring(1, folderName)
        return StringOrNil(repentogon.L_Isaac_LoadModDataFromFolder(folderName))
    end,
    PlayCutscene = function(cutscene, shouldClear)
        cutscene = ffichecks.checkinteger(1, cutscene)
        repentogon.L_Isaac_PlayCutscene(cutscene, ffichecks.optboolean(shouldClear, false))
    end,
    RGON_GetChangelog = function()
        local result = ffi.string(repentogon.L_Isaac_GetChangelog()) return result
    end,
    RegisterMod = function(mod, name, apiVersion)
        name = ffichecks.checkstring(2, name)
        apiVersion = ffichecks.checkinteger(3, apiVersion)
        repentogon.L_Isaac_RegisterMod(RegistryRef(mod), name, apiVersion)
    end,
    RemoveModData = function(mod)
        repentogon.L_Isaac_RemoveModData(RegistryRef(mod))
    end,
    RenderCollectionItem = function(itemId, position, scale, color)
        itemId = ffichecks.checkinteger(1, itemId)
        ffichecks.checkcdata(2, position, "Vector")
        if not repentogon.L_Isaac_RenderCollectionItem(itemId, position, ffichecks.optcdata(scale, "Vector", nil), ffichecks.optcdata(color, "Color", nil)) then
            ffichecks.argerror(1, "Invalid collectible ID")
        end
    end,
    RenderScaledText = function(text, x, y, scaleX, scaleY, red, green, blue, alpha)
        text = ffichecks.checkstring(1, text)
        x = ffichecks.checknumber(2, x)
        y = ffichecks.checknumber(3, y)
        scaleX = ffichecks.checknumber(4, scaleX)
        scaleY = ffichecks.checknumber(5, scaleY)
        red = ffichecks.checknumber(6, red)
        green = ffichecks.checknumber(7, green)
        blue = ffichecks.checknumber(8, blue)
        alpha = ffichecks.checknumber(9, alpha)
        repentogon.L_Isaac_GetTextFont():DrawStringScaled(text, x, y, scaleX, scaleY, KColor(red, green, blue, alpha))
    end,
    RenderText = function(text, x, y, red, green, blue, alpha)
        text = ffichecks.checkstring(1, text)
        x = ffichecks.checknumber(2, x)
        y = ffichecks.checknumber(3, y)
        red = ffichecks.checknumber(4, red)
        green = ffichecks.checknumber(5, green)
        blue = ffichecks.checknumber(6, blue)
        alpha = ffichecks.checknumber(7, alpha)
        repentogon.L_Isaac_GetTextFont():DrawString(text, x, y, KColor(red, green, blue, alpha))
    end,
    RenderToWorld = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_RenderToWorld, position)
    end,
    ReworkBirthright = function(playerType)
        playerType = ffichecks.checkinteger(1, playerType)
        if not repentogon.L_Isaac_ReworkBirthright(playerType) then
            ffichecks.argerror(1, "invalid PlayerType")
        end
    end,
    ReworkCollectible = function(collectible)
        collectible = ffichecks.checkinteger(1, collectible)
        if not repentogon.L_Isaac_ReworkCollectible(collectible) then
            ffichecks.argerror(1, "invalid CollectibleType")
        end
    end,
    ReworkTrinket = function(trinket)
        trinket = ffichecks.checkinteger(1, trinket)
        if not repentogon.L_Isaac_ReworkTrinket(trinket) then
            ffichecks.argerror(1, "invalid TrinketType")
        end
    end,
    ScreenToWorld = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_ScreenToWorld, position)
    end,
    ScreenToWorldDistance = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_ScreenToWorldDistance, position)
    end,
    SaveModData = function(mod, data)
        data = ffichecks.checkstring(2, data)
        repentogon.L_Isaac_SaveModData(RegistryRef(mod), data)
    end,
    SetChallengeCompletion = function(challengeId, completed)
        challengeId = ffichecks.checkinteger(1, challengeId)
        CheckChallengeId(challengeId)
        ffichecks.checktype(2, completed, "boolean")
        if completed then
            repentogon.L_Isaac_ClearChallenge(challengeId)
        else
            repentogon.L_Isaac_UndoChallenge(challengeId)
        end
    end,
    SetClipboard = function(text)
        text = ffichecks.checkstring(1, text)
        local result = repentogon.L_Isaac_SetClipboard(text) return result
    end,
    SetCompletionMark = function(playerType, completionType, value)
        playerType = ffichecks.checkinteger(1, playerType)
        completionType = ffichecks.checkinteger(2, completionType)
        value = ffichecks.checkinteger(3, value)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        if value < 0 or value > 2 then
            error(string.format("Invalid Completion Marks value!(%d)", value), 2)
        end
        repentogon.L_Isaac_SetCompletionMark(playerType, completionType, value)
    end,
    SetCompletionMarks = function(marks)
        if not repentogon.L_Isaac_CompletionMarksInitialized() then
            return
        end
        ffichecks.checktable(2, marks)

        local playerType = 0
        local values = ffi.new("int[15]")
        for key, value in pairs(marks) do
            local keyType = type(key)
            if keyType == "string" or keyType == "number" then
                key = tostring(key)
                value = ToInteger(value)
                local index = repentogon.L_Isaac_GetCompletionMarkIndex(key)
                if index >= 0 then
                    if value < 0 or value > 2 then
                        error(string.format("Invalid Completion Marks value for %s is invalid(%d)", key, value), 2)
                    end
                    values[index] = value
                elseif key == "PlayerType" then
                    playerType = value
                else
                    error(string.format("Invalid Completion Marks table index: %s", key), 2)
                end
            end
        end
        if playerType < 0 then
            error("Invalid Player Type", 2)
        end
        repentogon.L_Isaac_SetCompletionMarks(playerType, values)
    end,
    SetBuiltInCallbackState = function(callbackId, enabled)
        callbackId = ffichecks.checkinteger(1, callbackId)
        repentogon.L_Isaac_SetBuiltInCallbackState(callbackId, ffichecks.checkboolean(2, enabled))
    end,
    SetCurrentFloorBackdrop = function(backdropId)
        if tonumber(backdropId) == nil then
            error(string.format("Expected BackdropId as parameter #1, got %s", type(backdropId)), 2)
        end
        repentogon.L_Isaac_SetCurrentFloorBackdrop(tonumber(backdropId))
    end,
    SetCurrentFloorMusic = function(musicId)
        if tonumber(musicId) == nil then
            error(string.format("Expected MusicId as parameter #1, got %s", type(musicId)), 2)
        end
        repentogon.L_Isaac_SetCurrentFloorMusic(tonumber(musicId))
    end,
    SetCurrentFloorName = function(floorName)
        repentogon.L_Isaac_SetCurrentFloorName(ffichecks.checkstring(1, floorName))
    end,
    SetDwmWindowAttribute = function(attribute, value)
        attribute = ffichecks.optinteger(attribute, 0)
        value = ffichecks.optinteger(value, 0)
        local prohibited = repentogon.L_Isaac_SetDwmWindowAttribute(attribute, value)
        if prohibited == 1 then
            error("Usage of DWMWA_CLOAK attribute is prohibited!", 2)
        elseif prohibited == 2 then
            error("Usage of DWMWA_CLOAKED attribute is prohibited!", 2)
        end
    end,
    SetIcon = function(icon, ignoreCap)
        ignoreCap = ffichecks.optboolean(ignoreCap, false)
        if math.type(icon) == "integer" then
            repentogon.L_Isaac_SetIconID(icon, ignoreCap)
            return
        end
        if icon == nil then
            icon = ""
        else
            icon = ffichecks.checkstring(1, icon)
        end
        if not repentogon.L_Isaac_SetIconPath(icon, ignoreCap) then
            error("Icon has failed to load!", 2)
        end
    end,
    SetWindowTitle = function(title)
        if type(title) == "string" or type(title) == "number" then
            repentogon.L_Isaac_SetWindowTitle(tostring(title))
        else
            repentogon.L_Isaac_SetWindowTitle(nil)
        end
    end,
    ShowErrorDialog = function(title, text, icon, buttons)
        title = ffichecks.checkstring(1, title)
        text = ffichecks.checkstring(2, text)
        local result = repentogon.L_Isaac_ShowErrorDialog(title, text, ffichecks.optinteger(icon, MB_ICONERROR), ffichecks.optinteger(buttons, MB_OK)) return result
    end,
    Spawn = function(type, variant, subtype, position, velocity, spawner)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        subtype = ffichecks.checkinteger(3, subtype)
        ffichecks.checkcdata(4, position, "Vector")
        ffichecks.checkcdata(5, velocity, "Vector")
        return ffichecks.pointertoentity(repentogon.L_Isaac_Spawn(type, variant, subtype, position, velocity, EntityToPointer(spawner)))
    end,
    SpawnBoss = function(type, variant, subtype, position, velocity, spawner, seed)
        type = ffichecks.checkinteger(1, type)
        variant = ffichecks.checkinteger(2, variant)
        subtype = ffichecks.checkinteger(3, subtype)
        ffichecks.checkcdata(4, position, "Vector")
        ffichecks.checkcdata(5, velocity, "Vector")
        if seed == nil then
            seed = repentogon.L_Isaac_GetRoomSpawnSeed()
        else
            seed = ffichecks.checkinteger(7, seed)
        end
        if not (type > 9 and type < 990) then
            error("SpawnBoss only works with NPC-able entity types", 2)
        end
        return ffichecks.pointertonpc(repentogon.L_Isaac_SpawnBoss(type, variant, subtype, position, velocity, EntityToPointer(spawner), seed))
    end,
    StartDailyGame = function(date)
        date = ffichecks.checkinteger(1, date)
        repentogon.L_Isaac_StartDailyGame(date)
    end,
    StartNewGame = function(playerType, challenge, difficulty, seeds, isCustomRun)
        playerType = ffichecks.optinteger(playerType, 0)
        challenge = ffichecks.optinteger(challenge, 0)
        difficulty = ffichecks.optinteger(difficulty, 0)
        if ffichecks.iscdata(seeds, "Seeds") then
            repentogon.L_Isaac_StartNewGame(playerType, challenge, difficulty, seeds, 0, false)
        else
            repentogon.L_Isaac_StartNewGame(playerType, challenge, difficulty, nil, ffichecks.optinteger(seeds, 0), ffichecks.optboolean(isCustomRun, false))
        end
    end,
    TriggerWindowResize = function()
        repentogon.L_Isaac_TriggerWindowResize()
    end,
    UndoChallenge = DeprecatedUndoChallenge,
    WorldToMenuPosition = function(...)
        local count = select("#", ...)
        if count ~= 2 then
            error(string.format("Expected two parameters(MenuId,WorldPosition) got %d\n", count), 2)
        end
        local menuId, position = ...
        menuId = ffichecks.checkinteger(1, menuId)
        ffichecks.checkcdata(2, position, "Vector")
        local result = Vector(0, 0)
        local status = repentogon.L_Isaac_WorldToMenuPosition(menuId, position, result)
        if status == 1 then
            error(string.format("Invalid Menu Id %d\n", menuId), 2)
        elseif status == 2 then
            error("WorldToMenu can only be used in the main menu", 2)
        end
        return result
    end,
    WorldToRenderPosition = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_WorldToRenderPosition, position)
    end,
    WorldToScreen = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_WorldToScreen, position)
    end,
    WorldToScreenDistance = function(position)
        ffichecks.checkcdata(1, position, "Vector")
        return VectorResult(repentogon.L_Isaac_WorldToScreenDistance, position)
    end,
}

isaac.GetBossColorIdByName = isaac.GetBossColorIdxByName
isaac.MarkChallengeAsNotDone = isaac.UndoChallenge
isaac.UnClearChallenge = isaac.UndoChallenge

for name, fn in pairs(isaac) do
    rawset(Isaac, name, fn)
end
