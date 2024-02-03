ffi.cdef[[
void L_Level_AddAngelRoomChance(float);
void L_Level_AddCurse(int, bool);
void L_Level_ApplyBlueMapEffect();
void L_Level_ApplyCompassEffect(bool);
void L_Level_ApplyMapEffect();
bool L_Level_CanOpenChallengeRoom(int);
bool L_Level_CanSpawnChallengeRoom(int);
bool L_Level_CanSpawnDevilRoom();
bool L_Level_CanStageHaveCurseOfLabyrinth(int);
void L_Level_ChangeRoom(int, int);
void L_Level_DisableDevilRoom();
bool L_Level_ForceHorsemanBoss(int);
// LevelStage
int L_Level_GetAbsoluteStage();
float L_Level_GetAngelRoomChance();
bool L_Level_GetCanSeeEverything();
// Room
void* L_Level_GetCurrentRoom();
// RoomDescriptor
void* L_Level_GetCurrentRoomDesc();
int L_Level_GetCurrentRoomIndex();
const char* L_Level_GetCurseName();
int L_Level_GetCurses();
RNG* L_Level_GetDevilAngelRoomRNG();
int L_Level_GetDungeonPlacementSeed();
Vector* L_Level_GetEnterPosition();
bool L_Level_GetHeartPicked();
int L_Level_GetLastBossRoomListIndex();
// RoomDescriptor
void* L_Level_GetLastRoomDesc();
const char* L_Level_GetName();
int L_Level_GetNonCompleteRoomIndex();
float L_Level_GetPlanetariumChance();
int L_Level_GetPreviousRoomIndex();
int L_Level_GetRandomRoomIndex(bool, int);
// RoomDescriptor
void* L_Level_GetRoomByIdx(int, int);
int L_Level_GetRoomCount();
// RoomDescriptor[]
void* L_Level_GetRooms();
// LevelStage
int L_Level_GetStage();
// StageType
int L_Level_GetStageType();
int L_Level_GetStartingRoomIndex();
// LevelStateFlag
bool L_Level_GetStateFlag(int);
bool L_Level_HasBossChallenge();
void L_Level_InitializeDevilAngelRoom();
bool L_Level_IsAltStage();
bool L_Level_IsAscent();
bool L_Level_IsDevilRoomDisabled();
bool L_Level_IsNextStageAvailable();
bool L_Level_IsPreAscent();
// arg2: DoorSlot
bool L_Level_MakeRedRoomDoor(int, int);
// RoomType
int L_Level_QueryRoomTypeIndex(int, bool, RNG*, bool);
void L_Level_RemoveCompassEffect();
// LevelCurses
void L_Level_RemoveCurses(int);
void L_Level_SetCanSeeEverything(bool);
void L_Level_SetHeartPicked();
void L_Level_SetNextStage();
void L_Level_SetRedHeartDamage();
void L_Level_SetStage();
// LevelStateFlag
void L_Level_SetStateFlag(int, bool);
void L_Level_ShowMap();
void L_Level_ShowName(bool);
// arg2: DoorSlot
void L_Level_UncoverHiddenDoor(int, int);
void L_Level_Update();
void L_Level_UpdateVisibility();

Vector* L_Level_GetDungeonReturnPosition();
int L_Level_GetDungeonReturnRoomIndex();
int L_Level_GetEnterDoor();
int L_Level_GetGreedModeWave();
int L_Level_GetLeaveDoor();

void L_Level_SetDungeonReturnPosition(Vector*);
void L_Level_SetDungeonReturnRoomIndex(int);
void L_Level_SetEnterDoor(int);
void L_Level_SetGreedModeWave(int);
void L_Level_SetLeaveDoor(int);
]]

local repentogon = ffidll
local lffi = ffi

local LevelFuncs = {}

function LevelFuncs:AddAngelRoomChance(chance)
    ffichecks.checknumber(1, chance)
    repentogon.L_Level_AddAngelRoomChance(chance)
end

function LevelFuncs:AddCurse(curse, showName)
    ffichecks.checknumber(1, curse)
    ffichecks.checkboolean(2, showName)

    repentogon.L_Level_AddCurse(curse, showName)
end

function LevelFuncs:ApplyBlueMapEffect()
    repentogon.L_Level_ApplyBlueMapEffect()
end

function LevelFuncs:ApplyCompassEffect(persistent)
    ffichecks.checkboolean(1, persistent)
    repentogon.L_Level_ApplyCompassEffect(persistent)
end

function LevelFuncs:ApplyMapEffect()
    repentogon.L_Level_ApplyMapEffect()
end

function LevelFuncs:CanOpenChallengeRoom(roomIndex)
    ffichecks.checknumber(1, roomIndex)
    return repentogon.L_Level_CanOpenChallengeRoom(roomIndex)
end

function LevelFuncs:CanSpawnDevilRoom()
    return repentogon.L_Level_CanSpawnDevilRoom()
end

function LevelFuncs:CanStageHaveCurseOfLabyrinth(stage)
    ffichecks.checknumber(1, stage)
    return repentogon.L_Level_CanStageHaveCurseOfLabyrinth(stage)
end

function LevelFuncs:ChangeRoom(roomIndex, dimension)
    ffichecks.checknumber(1, roomIndex)
    dimension = ffichecks.optnumber(dimension, -1)

    return repentogon.L_Level_ChangeRoom(roomIndex, dimension)
end

function LevelFuncs:DisableDevilRoom()
    repentogon.L_Level_DisableDevilRoom()
end

function LevelFuncs:ForceHorsemanBoss(seed)
    ffichecks.checknumber(1, seed)
    repentogon.L_Level_ForceHorsemanBoss(seed)
end

function LevelFuncs:GetAbsoluteStage()
    return repentogon.L_Level_GetAbsoluteStage()
end

function LevelFuncs:GetAngelRoomChance()
    return repentogon.L_Level_GetAngelRoomChance()
end

function LevelFuncs:GetCanSeeEverything()
    return repentogon.L_Level_GetCanSeeEverything()
end

function LevelFuncs:GetCurrentRoom()
    return Room
end

function LevelFuncs:GetCurrentRoomDesc()
    return repentogon.L_Level_GetCurrentRoomDesc()
end

function LevelFuncs:GetCurrentRoomIndex()
    return repentogon.L_Level_GetCurrentRoomIndex()
end

function LevelFuncs:GetCurseName()
    return lffi.string(lffi.gc(repentogon.L_Level_GetCurseName(), repentogon.L_Free))
end

function LevelFuncs:GetCurses()
    return repentogon.L_Level_GetCurses()
end

function LevelFuncs:GetDevilAngelRoomRNG()
    return repentogon.L_Level_GetDevilAngelRoomRNG()
end

function LevelFuncs:GetEnterPosition()
    return repentogon.L_Level_GetEnterPosition()
end

function LevelFuncs:GetHeartPicked()
    return repentogon.L_Level_GetHeartPicked()
end

function LevelFuncs:GetLastBossRoomListIndex()
    return repentogon.L_Level_GetLastBossRoomListIndex()
end

function LevelFuncs:GetLastRoomDesc()
    return repentogon.L_Level_GetLastRoomDesc()
end

function LevelFuncs:GetName()
    return lffi.string(lffi.gc(repentogon.L_Level_GetName(), repentogon.L_Free))
end

function LevelFuncs:GetNonCompleteRoomIndex()
    return repentogon.L_Level_GetNonCompleteRoomIndex()
end

function LevelFuncs:GetPlanetariumChance()
    return repentogon.L_Level_GetPlanetariumChance()
end

function LevelFuncs:GetPreviousRoomIndex()
    return repentogon.L_Level_GetPreviousRoomIndex()
end

function LevelFuncs:GetRandomRoomIndex(iAmErrorRoom, seed)
    ffichecks.checkboolean(1, iAmErrorRoom)
    ffichecks.checknumber(2, seed)

    return repentogon.L_Level_GetRandomRoomIndex(iAmErrorRoom, seed)
end

function LevelFuncs:GetRoomByIdx(idx, dimension)
    ffichecks.checknumber(1, idx)
    dimension = ffichecks.optnumber(dimension, -1)

    return repentogon.L_Level_GetRoomByIdx(idx, dimension)
end

function LevelFuncs:GetRoomCount()
    return repentogon.L_Level_GetRoomCount()
end

function LevelFuncs:GetRooms()
    return repentogon.L_Level_GetRooms()
end

function LevelFuncs:GetStage()
    return repentogon.L_Level_GetStage()
end

function LevelFuncs:GetStageType()
    return repentogon.L_Level_GetStageType()
end

function LevelFuncs:GetStartingRoomIndex()
    return repentogon.L_Level_GetStartingRoomIndex()
end

function LevelFuncs:GetStateFlag(levelStateFlag)
    ffichecks.checknumber(1, levelStateFlag)
    return repentogon.L_Level_GetStateFlag(levelStateFlag)
end

function LevelFuncs:HasBossChallenge()
    return repentogon.L_Level_HasBossChallenge() 
end

function LevelFuncs:InitializeDevilAngelRoom(forceAngel, forceDevil)
    ffichecks.checkboolean(1, forceAngel)
    ffichecks.checkboolean(2, forceDevil)

    return repentogon.L_Level_InitializeDevilAngelRoom(forceAngel, forceDevil)
end

function LevelFuncs:IsAltStage()
    return repentogon.L_Level_IsAltStage()
end

function LevelFuncs:IsAscent()
    return repentogon.L_Level_IsAscent()
end

function LevelFuncs:IsDevilRoomDisabled()
    return repentogon.L_Level_IsDevilRoomDisabled()
end

function LevelFuncs:IsNextStageAvailable()
    return repentogon.L_Level_IsNextStageAvailable()
end

function LevelFuncs:IsPreAscent()
    return repentogon.L_Level_IsPreAscent()
end

function LevelFuncs:MakeRedRoomDoor(roomIdx, doorSlot)
    ffichecks.checknumber(1, roomIdx)
    ffichecks.checknumber(2, doorSlot)

    return repentogon.L_Level_MakeRedRoomDoor(roomIdx, doorSlot)
end

function LevelFuncs:QueryRoomTypeIndex(roomType, visited, rng, ignoreGroup)
    ffichecks.checknumber(1, roomType)
    ffichecks.checkboolean(2, visited)
    ffichecks.checkcdata(3, rng, "RNG")
    ffichecks.checkboolean(4, ignoreGroup)

    return repentogon.L_Level_QueryRoomTypeIndex(roomType, visited, rng, ignoreGroup)
end

function LevelFuncs:RemoveCompassEffect()
    repentogon.L_Level_RemoveCompassEffect()
end

function LevelFuncs:RemoveCurses()
    repentogon.L_Level_RemoveCurses()
end

function LevelFuncs:SetCanSeeEverything(canSeeEverything)
    ffichecks.checkboolean(1, canSeeEverything)

    repentogon.L_Level_SetCanSeeEverything(canSeeEverything)
end

function LevelFuncs:SetHeartPicked()
    repentogon.L_Level_SetHeartPicked()
end

function LevelFuncs:SetNextStage()
    repentogon.L_Level_SetNextStage()
end

function LevelFuncs:SetRedHeartDamage()
    repentogon.L_Level_SetRedHeartDamage()
end

function LevelFuncs:SetStage(stageOffset, stageTypeOffset)
    ffichecks.checknumber(1, stageOffset)
    ffichecks.checknumber(2, stageTypeOffset)

    repentogon.L_Level_SetStage(stageOffset, stageTypeOffset)
end

function LevelFuncs:SetStateFlag(levelStateFlag, value)
    ffichecks.checknumber(1, levelStateFlag)
    ffichecks.checkboolean(2, value)

    repentogon.L_Level_SetStateFlag(levelStateFlag, value)
end

function LevelFuncs:ShowMap()
    repentogon.L_Level_ShowMap()
end

function LevelFuncs:ShowName(sticky)
    ffichecks.checkboolean(1, sticky)
    
    repentogon.L_Level_ShowName(sticky)
end

function LevelFuncs:UncoverHiddenDoor(roomIdx, doorSlot)
    ffichecks.checknumber(1, roomIdx)
    ffichecks.checknumber(2, doorSlot)

    repentogon.L_Level_UncoverHiddenDoor(roomIdx, doorSlot)
end

function LevelFuncs:Update()
    repentogon.L_Level_Update()
end

function LevelFuncs:UpdateVisibility()
    repentogon.L_Level_UpdateVisibility()
end

local getkeys = {
    DungeonReturnPosition = repentogon.L_Level_GetDungeonReturnPosition,
    DungeonReturnRoomIndex = repentogon.L_Level_GetDungeonReturnRoomIndex,
    EnterDoor = repentogon.L_Level_GetEnterDoor,
    GreedModeWave = repentogon.L_Level_GetGreedModeWave,
    LeaveDoor = repentogon.L_Level_GetLeaveDoor
}

local setkeys = {
    DungeonReturnPosition = repentogon.L_Level_SetDungeonReturnPosition,
    DungeonReturnRoomIndex = repentogon.L_Level_SetDungeonReturnRoomIndex,
    EnterDoor = repentogon.L_Level_SetEnterDoor,
    GreedModeWave = repentogon.L_Level_SetGreedModeWave,
    LeaveDoor = repentogon.L_Level_SetLeaveDoor
}

local LevelMT = {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key]()
        end
        
        return LevelFuncs[key]
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
}

Level = setmetatable({}, LevelMT)