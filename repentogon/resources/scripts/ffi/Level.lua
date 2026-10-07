ffi.cdef [[
    struct Level {
        private uint32_t Stage : 0x0;
        private uint32_t StageType : 0x4;
        private bool BossChallenge : 0x8;
        private bool DevilRoomDisabled : 0x9;
        private bool HeartPicked : 0x10;
        private bool CanSeeEverything : 0x11;
        private struct RoomDescriptor GridRooms[507] : 0x14;
        private uint32_t GreedWavesClearedWithoutRedHeartDamage : 0x18204;
        private uint32_t RoomCount : 0x182cc;
        private int StartingRoomIndex : 0x182d0;
        private struct RNG GenerationRNG : 0x182d4;
        private struct RNG DevilAngelRoomRNG : 0x182e4;
        private float AngelRoomChance : 0x182f4;
        private struct Room* CurrentRoom : 0x18300;
        private int CurrentRoomIndex : 0x18304;
        private int PreviousRoomIndex : 0x18308;
        private int CurrentDimension : 0x1830c;
        private int LastBossRoomListIndex : 0x18314;
        private int LeaveDoorValue : 0x18318;
        private int EnterDoorValue : 0x1831c;
        private struct Vector DungeonReturnPos : 0x18320;
        private int DungeonReturnRoomIndexValue : 0x18328;
        private uint32_t DungeonPlacementSeed : 0x1832c;
        private uint32_t GreedModeWaveValue : 0x18334;
        private struct EntitiesSaveStateVector MyosotisPickups : 0x183a4;
    } : 0x1890c;
    typedef struct Level* LevelPtr;

    void L_Level_Update(struct Level*);
    void L_Level_SetStage(struct Level*, int, int);
    void L_Level_SetNextStage(struct Level*);
    const char* L_Level_GetName(struct Level*);
    const char* L_Level_GetCurseName(struct Level*);
    bool L_Level_CanStageHaveCurseOfLabyrinth(struct Level*, int);
    void L_Level_ShowName(struct Level*, bool);
    bool L_Level_GetStateFlag(struct Level*, unsigned int);
    void L_Level_SetStateFlag(struct Level*, unsigned int, bool);
    int L_Level_GetRandomRoomIndex(struct Level*, bool, unsigned int);
    int L_Level_GetNonCompleteRoomIndex(struct Level*);
    struct RoomDescriptor* L_Level_GetRoomByIdx(struct Level*, int, int);
    struct RoomDescriptor* L_Level_GetCurrentRoomDesc(struct Level*);
    struct RoomDescriptor* L_Level_GetLastRoomDesc(struct Level*);
    int L_Level_QueryRoomTypeIndex(struct Level*, int, bool, struct RNG*, bool);
    bool L_Level_CanOpenChallengeRoom(struct Level*, int);
    void L_Level_GetEnterPosition(struct Level*, struct Vector*);
    void L_Level_ChangeRoom(struct Level*, int, int);
    bool L_Level_ForceHorsemanBoss(struct Level*, int);
    int L_Level_GetAbsoluteStage(struct Level*);
    int L_Level_GetCurses(struct Level*);
    void L_Level_UpdateVisibility(struct Level*);
    void L_Level_ApplyMapEffect(struct Level*);
    void L_Level_ApplyBlueMapEffect(struct Level*);
    void L_Level_ApplyCompassEffect(struct Level*, bool);
    void L_Level_RemoveCompassEffect(struct Level*);
    void L_Level_ShowMap(struct Level*);
    void L_Level_AddCurse(struct Level*, int, bool);
    void L_Level_RemoveCurses(struct Level*, int);
    bool L_Level_CanSpawnDevilRoom(struct Level*);
    void L_Level_InitializeDevilAngelRoom(struct Level*, bool, bool);
    void L_Level_UncoverHiddenDoor(struct Level*, int, int);
    bool L_Level_IsNextStageAvailable(struct Level*);
    float L_Level_GetPlanetariumChance(struct Level*);
    bool L_Level_MakeRedRoomDoor(struct Level*, int, int);
    bool L_Level_IsAscent(struct Level*);
    bool L_Level_IsPreAscent(struct Level*);
    void L_Level_SetRedHeartDamage(struct Level*);
    bool L_Level_CanSpawnDoorOutline(struct Level*, int, unsigned int);
    bool L_Level_HasAbandonedMineshaft(struct Level*);
    bool L_Level_HasMirrorDimension(struct Level*);
    bool L_Level_HasPhotoDoor(struct Level*);
    void L_Level_SetName(const char*);
    bool L_Level_IsStageAvailable(int, int);
    int L_Level_GetForceSpecialQuest();
    void L_Level_SetForceSpecialQuest(int);
    bool L_Level_PlaceRoom(struct Level*, struct LevelGeneratorEntry*, struct RoomConfigRoom*, unsigned int);
    bool L_Level_CanPlaceRoom(int, int, int, int, bool, bool, bool);
    struct RoomDescriptor* L_Level_TryPlaceRoom(struct RoomConfigRoom*, int, int, unsigned int, bool, bool, bool);
    bool L_Level_CanPlaceRoomAtDoor(int, int, struct RoomDescriptor*, int, bool, bool);
    struct RoomDescriptor* L_Level_TryPlaceRoomAtDoor(struct RoomConfigRoom*, struct RoomDescriptor*, int, unsigned int, bool, bool, bool*);
    int L_Level_FindValidRoomPlacementLocations(int, int, int, bool, bool, int*);
    int L_Level_GetNeighboringRooms(int, int, int, int*, struct RoomDescriptor**);
]]

local ffi = ffi
local repentogon = ffidll

local ROOMSHAPE_1x1 = 1
local RoomDescriptorListT = ffi.typeof("struct RoomDescriptorList")
local attemptedOut = ffi.new("bool[1]")

local function CheckDimension(index, value)
    value = ffichecks.optinteger(value, -1)
    if value < -1 or value > 2 then
        ffichecks.argerror(index, "Invalid Dimension", 3)
    end
    return value
end

local function CheckPlacementSeed(index, value)
    value = ffichecks.optinteger(value, 0)
    if value < 0 then
        ffichecks.argerror(index, "Invalid Seed", 3)
    end
    return value
end

local function ClampInt(value, min, max)
    if value < min then
        return min
    end
    if value > max then
        return max
    end
    return value
end

local function ParseRoomShape(index, arg1, arg2, defaultShape, defaultMask, shapeRequired)
    if ffichecks.iscdata(arg1, "RoomConfigRoom") then
        return arg1.Shape, arg1.Doors, index + 1
    end

    local roomShape = defaultShape
    if arg1 ~= nil or shapeRequired then
        ffichecks.checkinteger(index, arg1)
        roomShape = arg1
    end
    local doorMask = defaultMask
    if arg2 ~= nil then
        ffichecks.checkinteger(index + 1, arg2)
        doorMask = arg2
    end
    return roomShape, doorMask, index + 2
end

local getters = {
    DungeonReturnPosition = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "DungeonReturnPos"))
    end,
    DungeonReturnRoomIndex = function(self)
        return ffi.getprivate(self, "DungeonReturnRoomIndexValue")
    end,
    EnterDoor = function(self)
        return ffi.getprivate(self, "EnterDoorValue")
    end,
    GreedModeWave = function(self)
        return ffi.getprivate(self, "GreedModeWaveValue")
    end,
    LeaveDoor = function(self)
        return ffi.getprivate(self, "LeaveDoorValue")
    end,
}

local setters = {
    DungeonReturnPosition = function(self, value)
        ffichecks.checkcdata(3, value, "Vector")
        local position = ffi.getprivate(self, "DungeonReturnPos")
        position.X = value.X
        position.Y = value.Y
    end,
    DungeonReturnRoomIndex = function(self, value)
        ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "DungeonReturnRoomIndexValue", ClampInt(value, -20, 168))
    end,
    EnterDoor = function(self, value)
        ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "EnterDoorValue", ClampInt(value, -1, 7))
    end,
    GreedModeWave = function(self, value)
        ffichecks.checkinteger(3, value)
        if value < 0 or value > 12 then
            value = 12
        end
        ffi.setprivate(self, "GreedModeWaveValue", value)
    end,
    LeaveDoor = function(self, value)
        ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "LeaveDoorValue", ClampInt(value, -1, 7))
    end,
}

local LevelMT
LevelMT = {
    __type = "Level",

    __index = function(self, key)
        local getter = getters[key]
        if getter then
            return getter(self)
        end
        return LevelMT[key]
    end,

    __newindex = function(self, key, value)
        local setter = setters[key]
        if setter then
            return setter(self, value)
        end
        error(string.format("cannot set '%s'", tostring(key)))
    end,

    AddAngelRoomChance = function(self, chance)
        ffichecks.checknumber(1, chance)
        ffi.setprivate(self, "AngelRoomChance", ffi.getprivate(self, "AngelRoomChance") + chance)
    end,
    AddCurse = function(self, curse, showName)
        ffichecks.checkinteger(1, curse)
        repentogon.L_Level_AddCurse(self, curse, ffichecks.optboolean(showName, false))
    end,
    ApplyBlueMapEffect = function(self)
        repentogon.L_Level_ApplyBlueMapEffect(self)
    end,
    ApplyCompassEffect = function(self, persistent)
        repentogon.L_Level_ApplyCompassEffect(self, ffichecks.optboolean(persistent, false))
    end,
    ApplyMapEffect = function(self)
        repentogon.L_Level_ApplyMapEffect(self)
    end,
    CanOpenChallengeRoom = function(self, roomIndex)
        ffichecks.checkinteger(1, roomIndex)
        return repentogon.L_Level_CanOpenChallengeRoom(self, roomIndex)
    end,
    CanPlaceRoom = function(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
        local args = { arg1, arg2, arg3, arg4, arg5, arg6, arg7 }
        local roomShape, doorMask, index = ParseRoomShape(1, arg1, arg2, 0, -1, true)

        local gridIndex = args[index]
        ffichecks.checkinteger(index, gridIndex)
        if gridIndex < 0 or gridIndex > 168 then
            return false
        end
        local dimension = CheckDimension(index + 1, args[index + 1])
        return repentogon.L_Level_CanPlaceRoom(roomShape, doorMask, gridIndex, dimension,
            ffichecks.optboolean(args[index + 2], true), ffichecks.optboolean(args[index + 3], false), ffichecks.optboolean(args[index + 4], false))
    end,
    CanPlaceRoomAtDoor = function(self, arg1, arg2, arg3, arg4, arg5, arg6)
        local args = { arg1, arg2, arg3, arg4, arg5, arg6 }
        local roomShape, doorMask, index = ParseRoomShape(1, arg1, arg2, 0, -1, true)
        local descriptor = args[index]
        ffichecks.checkcdata(index, descriptor, "RoomDescriptor")
        local doorSlot = args[index + 1]
        ffichecks.checkinteger(index + 1, doorSlot)
        return repentogon.L_Level_CanPlaceRoomAtDoor(roomShape, doorMask, descriptor, doorSlot,
            ffichecks.optboolean(args[index + 2], true), ffichecks.optboolean(args[index + 3], false))
    end,
    CanSpawnDevilRoom = function(self)
        return repentogon.L_Level_CanSpawnDevilRoom(self)
    end,
    CanSpawnDoorOutline = function(self, roomIndex, doorSlot)
        ffichecks.checkinteger(1, roomIndex)
        ffichecks.checkinteger(2, doorSlot)
        return repentogon.L_Level_CanSpawnDoorOutline(self, roomIndex, doorSlot)
    end,
    CanStageHaveCurseOfLabyrinth = function(self, stage)
        ffichecks.checkinteger(1, stage)
        return repentogon.L_Level_CanStageHaveCurseOfLabyrinth(self, stage)
    end,
    ChangeRoom = function(self, roomIndex, dimension)
        ffichecks.checkinteger(1, roomIndex)
        dimension = ffichecks.optnumber(dimension, -1)
        ffichecks.checkinteger(2, dimension)
        repentogon.L_Level_ChangeRoom(self, roomIndex, dimension)
    end,
    DisableDevilRoom = function(self)
        ffi.setprivate(self, "DevilRoomDisabled", true)
    end,
    FindValidRoomPlacementLocations = function(self, arg1, arg2, arg3, arg4, arg5)
        local args = { arg1, arg2, arg3, arg4, arg5 }
        local roomShape, doorMask, index = ParseRoomShape(1, arg1, arg2, ROOMSHAPE_1x1, -1, false)
        local dimension = CheckDimension(index, args[index])
        local allowMultipleDoors = ffichecks.optboolean(args[index + 1], true)
        local allowSpecialNeighbors = ffichecks.optboolean(args[index + 2], false)

        local result = {}
        local count = repentogon.L_Level_FindValidRoomPlacementLocations(roomShape, doorMask, dimension, allowMultipleDoors, allowSpecialNeighbors, nil)
        if count > 0 then
            local locations = ffi.new("int[?]", count)
            repentogon.L_Level_FindValidRoomPlacementLocations(roomShape, doorMask, dimension, allowMultipleDoors, allowSpecialNeighbors, locations)
            for i = 1, count do
                result[i] = locations[i - 1]
            end
        end
        return result
    end,
    ForceHorsemanBoss = function(self, seed)
        ffichecks.checkinteger(1, seed)
        return repentogon.L_Level_ForceHorsemanBoss(self, seed)
    end,
    GetAbsoluteStage = function(self)
        return repentogon.L_Level_GetAbsoluteStage(self)
    end,
    GetAngelRoomChance = function(self)
        return ffi.getprivate(self, "AngelRoomChance")
    end,
    GetCanSeeEverything = function(self)
        return ffi.getprivate(self, "CanSeeEverything")
    end,
    GetCurrentRoom = function(self)
        return ffi.getprivate(self, "CurrentRoom")
    end,
    GetCurrentRoomDesc = function(self)
        return repentogon.L_Level_GetCurrentRoomDesc(self)
    end,
    GetCurrentRoomIndex = function(self)
        return ffi.getprivate(self, "CurrentRoomIndex")
    end,
    GetCurseName = function(self)
        return ffi.string(repentogon.L_Level_GetCurseName(self))
    end,
    GetCurses = function(self)
        return repentogon.L_Level_GetCurses(self)
    end,
    GetDevilAngelRoomRNG = function(self)
        return ffi.getprivate(self, "DevilAngelRoomRNG")
    end,
    GetDimension = function(self)
        return ffi.getprivate(self, "CurrentDimension")
    end,
    GetDungeonPlacementSeed = function(self)
        return ffi.getprivate(self, "DungeonPlacementSeed")
    end,
    GetEnterPosition = function(self)
        local position = Vector(0, 0)
        repentogon.L_Level_GetEnterPosition(self, position)
        return position
    end,
    GetForceSpecialQuest = function(self)
        return repentogon.L_Level_GetForceSpecialQuest()
    end,
    GetGenerationRNG = function(self)
        return ffi.getprivate(self, "GenerationRNG")
    end,
    GetGreedWavesClearedWithoutRedHeartDamage = function(self)
        return ffi.getprivate(self, "GreedWavesClearedWithoutRedHeartDamage")
    end,
    GetHeartPicked = function(self)
        return ffi.getprivate(self, "HeartPicked")
    end,
    GetLastBossRoomListIndex = function(self)
        return ffi.getprivate(self, "LastBossRoomListIndex")
    end,
    GetLastRoomDesc = function(self)
        return repentogon.L_Level_GetLastRoomDesc(self)
    end,
    GetMyosotisPickups = function(self)
        return ffi.getprivate(self, "MyosotisPickups")
    end,
    GetName = function(self)
        return ffi.string(repentogon.L_Level_GetName(self))
    end,
    GetNeighboringRooms = function(self, gridIndex, roomShape, dimension)
        ffichecks.checkinteger(1, gridIndex)
        ffichecks.checkinteger(2, roomShape)
        dimension = CheckDimension(3, dimension)

        local result = {}
        local count = repentogon.L_Level_GetNeighboringRooms(gridIndex, roomShape, dimension, nil, nil)
        if count > 0 then
            local doorSlots = ffi.new("int[?]", count)
            local rooms = ffi.new("struct RoomDescriptor*[?]", count)
            repentogon.L_Level_GetNeighboringRooms(gridIndex, roomShape, dimension, doorSlots, rooms)
            for i = 0, count - 1 do
                result[doorSlots[i]] = rooms[i]
            end
        end
        return result
    end,
    GetNonCompleteRoomIndex = function(self)
        return repentogon.L_Level_GetNonCompleteRoomIndex(self)
    end,
    GetPlanetariumChance = function(self)
        return repentogon.L_Level_GetPlanetariumChance(self)
    end,
    GetPreviousRoomIndex = function(self)
        return ffi.getprivate(self, "PreviousRoomIndex")
    end,
    GetRandomRoomIndex = function(self, iAmErrorRoom, seed)
        ffichecks.checkinteger(2, seed)
        return repentogon.L_Level_GetRandomRoomIndex(self, ffichecks.optboolean(iAmErrorRoom, false), seed)
    end,
    GetRoomByIdx = function(self, index, dimension)
        ffichecks.checkinteger(1, index)
        dimension = ffichecks.optnumber(dimension, -1)
        ffichecks.checkinteger(2, dimension)
        return repentogon.L_Level_GetRoomByIdx(self, index, dimension)
    end,
    GetRoomCount = function(self)
        return ffi.getprivate(self, "RoomCount")
    end,
    GetRooms = function(self)
        local list = RoomDescriptorListT()
        ffi.setprivate(list, "_size", ffi.getprivate(self, "RoomCount"))
        ffi.setprivate(list, "_data", ffi.getprivate(self, "GridRooms"))
        return list
    end,
    GetStage = function(self)
        return ffi.getprivate(self, "Stage")
    end,
    GetStageType = function(self)
        return ffi.getprivate(self, "StageType")
    end,
    GetStartingRoomIndex = function(self)
        return ffi.getprivate(self, "StartingRoomIndex")
    end,
    GetStateFlag = function(self, flag)
        ffichecks.checkinteger(1, flag)
        return repentogon.L_Level_GetStateFlag(self, flag)
    end,
    HasAbandonedMineshaft = function(self)
        return repentogon.L_Level_HasAbandonedMineshaft(self)
    end,
    HasBossChallenge = function(self)
        return ffi.getprivate(self, "BossChallenge")
    end,
    HasMirrorDimension = function(self)
        return repentogon.L_Level_HasMirrorDimension(self)
    end,
    HasPhotoDoor = function(self)
        return repentogon.L_Level_HasPhotoDoor(self)
    end,
    InitializeDevilAngelRoom = function(self, forceAngel, forceDevil)
        repentogon.L_Level_InitializeDevilAngelRoom(self, ffichecks.optboolean(forceAngel, false), ffichecks.optboolean(forceDevil, false))
    end,
    IsAltStage = function(self)
        return ffi.getprivate(self, "StageType") ~= 0
    end,
    IsAscent = function(self)
        return repentogon.L_Level_IsAscent(self)
    end,
    IsDevilRoomDisabled = function(self)
        return ffi.getprivate(self, "DevilRoomDisabled")
    end,
    IsNextStageAvailable = function(self)
        return repentogon.L_Level_IsNextStageAvailable(self)
    end,
    IsPreAscent = function(self)
        return repentogon.L_Level_IsPreAscent(self)
    end,
    IsStageAvailable = function(self, stage, stageType)
        ffichecks.checkinteger(1, stage)
        ffichecks.checkinteger(2, stageType)
        return repentogon.L_Level_IsStageAvailable(stage, stageType)
    end,
    MakeRedRoomDoor = function(self, roomIndex, doorSlot)
        ffichecks.checkinteger(1, roomIndex)
        ffichecks.checkinteger(2, doorSlot)
        return repentogon.L_Level_MakeRedRoomDoor(self, roomIndex, doorSlot)
    end,
    PlaceRoom = function(self, entry, roomConfig, seed)
        ffichecks.checkcdata(1, entry, "LevelGeneratorEntry")
        ffichecks.checkcdata(2, roomConfig, "RoomConfigRoom")
        ffichecks.checkinteger(3, seed)
        return repentogon.L_Level_PlaceRoom(self, entry, roomConfig, seed)
    end,
    QueryRoomTypeIndex = function(self, roomType, visited, rng, ignoreGroup)
        ffichecks.checkinteger(1, roomType)
        ffichecks.checkcdata(3, rng, "RNG")
        return repentogon.L_Level_QueryRoomTypeIndex(self, roomType, ffichecks.optboolean(visited, false), rng, ffichecks.optboolean(ignoreGroup, false))
    end,
    RemoveCompassEffect = function(self)
        repentogon.L_Level_RemoveCompassEffect(self)
    end,
    RemoveCurses = function(self, curses)
        ffichecks.checkinteger(1, curses)
        repentogon.L_Level_RemoveCurses(self, curses)
    end,
    SetCanSeeEverything = function(self, value)
        ffi.setprivate(self, "CanSeeEverything", ffichecks.optboolean(value, false))
    end,
    SetForceSpecialQuest = function(self, quest)
        ffichecks.checkinteger(1, quest)
        repentogon.L_Level_SetForceSpecialQuest(quest)
    end,
    SetGreedWavesClearedWithoutRedHeartDamage = function(self, waves)
        ffichecks.checkinteger(1, waves)
        ffi.setprivate(self, "GreedWavesClearedWithoutRedHeartDamage", waves)
    end,
    SetHeartPicked = function(self)
        ffi.setprivate(self, "HeartPicked", true)
    end,
    SetName = function(self, name)
        name = ffichecks.checkstring(1, name)
        repentogon.L_Level_SetName(name)
    end,
    SetNextStage = function(self)
        repentogon.L_Level_SetNextStage(self)
    end,
    SetRedHeartDamage = function(self)
        repentogon.L_Level_SetRedHeartDamage(self)
    end,
    SetStage = function(self, stage, stageType)
        ffichecks.checkinteger(1, stage)
        ffichecks.checkinteger(2, stageType)
        repentogon.L_Level_SetStage(self, stage, stageType)
    end,
    SetStateFlag = function(self, flag, value)
        ffichecks.checkinteger(1, flag)
        repentogon.L_Level_SetStateFlag(self, flag, ffichecks.optboolean(value, false))
    end,
    ShowMap = function(self)
        repentogon.L_Level_ShowMap(self)
    end,
    ShowName = function(self, sticky)
        repentogon.L_Level_ShowName(self, ffichecks.optboolean(sticky, false))
    end,
    TryPlaceRoom = function(self, roomConfig, gridIndex, dimension, seed, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors)
        ffichecks.checkcdata(1, roomConfig, "RoomConfigRoom")
        ffichecks.checkinteger(2, gridIndex)
        if gridIndex < 0 or gridIndex > 168 then
            return false
        end
        dimension = CheckDimension(3, dimension)
        seed = CheckPlacementSeed(4, seed)
        return repentogon.L_Level_TryPlaceRoom(roomConfig, gridIndex, dimension, seed,
            ffichecks.optboolean(allowMultipleDoors, true), ffichecks.optboolean(allowSpecialNeighbors, false), ffichecks.optboolean(allowNoNeighbors, false))
    end,
    TryPlaceRoomAtDoor = function(self, roomConfig, descriptor, doorSlot, seed, allowMultipleDoors, allowSpecialNeighbors)
        ffichecks.checkcdata(1, roomConfig, "RoomConfigRoom")
        ffichecks.checkcdata(2, descriptor, "RoomDescriptor")
        ffichecks.checkinteger(3, doorSlot)
        seed = CheckPlacementSeed(4, seed)
        local room = repentogon.L_Level_TryPlaceRoomAtDoor(roomConfig, descriptor, doorSlot, seed,
            ffichecks.optboolean(allowMultipleDoors, true), ffichecks.optboolean(allowSpecialNeighbors, false), attemptedOut)
        if not attemptedOut[0] then
            return false
        end
        return room
    end,
    UncoverHiddenDoor = function(self, roomIndex, doorSlot)
        ffichecks.checkinteger(1, roomIndex)
        ffichecks.checkinteger(2, doorSlot)
        repentogon.L_Level_UncoverHiddenDoor(self, roomIndex, doorSlot)
    end,
    Update = function(self)
        repentogon.L_Level_Update(self)
    end,
    UpdateVisibility = function(self)
        repentogon.L_Level_UpdateVisibility(self)
    end,
}

ffi.metatype("struct Level", LevelMT)

Level = setmetatable({}, {__class = LevelMT})
