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
        arg1 = ffichecks.checkinteger(index, arg1)
        roomShape = arg1
    end
    local doorMask = defaultMask
    if arg2 ~= nil then
        arg2 = ffichecks.checkinteger(index + 1, arg2)
        doorMask = arg2
    end
    return roomShape, doorMask, index + 2
end

local getters = {
    DungeonReturnPosition = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "DungeonReturnPos"))
    end,
    DungeonReturnRoomIndex = function(self)
        local result = ffi.getprivate(self, "DungeonReturnRoomIndexValue") return result
    end,
    EnterDoor = function(self)
        local result = ffi.getprivate(self, "EnterDoorValue") return result
    end,
    GreedModeWave = function(self)
        local result = ffi.getprivate(self, "GreedModeWaveValue") return result
    end,
    LeaveDoor = function(self)
        local result = ffi.getprivate(self, "LeaveDoorValue") return result
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
        value = ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "DungeonReturnRoomIndexValue", ClampInt(value, -20, 168))
    end,
    EnterDoor = function(self, value)
        value = ffichecks.checkinteger(3, value)
        ffi.setprivate(self, "EnterDoorValue", ClampInt(value, -1, 7))
    end,
    GreedModeWave = function(self, value)
        value = ffichecks.checkinteger(3, value)
        if value < 0 or value > 12 then
            value = 12
        end
        ffi.setprivate(self, "GreedModeWaveValue", value)
    end,
    LeaveDoor = function(self, value)
        value = ffichecks.checkinteger(3, value)
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
        chance = ffichecks.checknumber(1, chance)
        ffi.setprivate(self, "AngelRoomChance", ffi.getprivate(self, "AngelRoomChance") + chance)
    end,
    AddCurse = function(self, curse, showName)
        curse = ffichecks.checkinteger(1, curse)
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
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        local result = repentogon.L_Level_CanOpenChallengeRoom(self, roomIndex) return result
    end,
    CanPlaceRoom = function(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
        local args = { arg1, arg2, arg3, arg4, arg5, arg6, arg7 }
        local roomShape, doorMask, index = ParseRoomShape(1, arg1, arg2, 0, -1, true)

        local gridIndex = args[index]
        gridIndex = ffichecks.checkinteger(index, gridIndex)
        if gridIndex < 0 or gridIndex > 168 then
            return false
        end
        local dimension = CheckDimension(index + 1, args[index + 1])
        local result = repentogon.L_Level_CanPlaceRoom(roomShape, doorMask, gridIndex, dimension,
            ffichecks.optboolean(args[index + 2], true), ffichecks.optboolean(args[index + 3], false), ffichecks.optboolean(args[index + 4], false))
        return result
    end,
    CanPlaceRoomAtDoor = function(self, arg1, arg2, arg3, arg4, arg5, arg6)
        local args = { arg1, arg2, arg3, arg4, arg5, arg6 }
        local roomShape, doorMask, index = ParseRoomShape(1, arg1, arg2, 0, -1, true)
        local descriptor = args[index]
        ffichecks.checkcdata(index, descriptor, "RoomDescriptor")
        local doorSlot = args[index + 1]
        doorSlot = ffichecks.checkinteger(index + 1, doorSlot)
        local result = repentogon.L_Level_CanPlaceRoomAtDoor(roomShape, doorMask, descriptor, doorSlot,
            ffichecks.optboolean(args[index + 2], true), ffichecks.optboolean(args[index + 3], false))
        return result
    end,
    CanSpawnDevilRoom = function(self)
        local result = repentogon.L_Level_CanSpawnDevilRoom(self) return result
    end,
    CanSpawnDoorOutline = function(self, roomIndex, doorSlot)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        doorSlot = ffichecks.checkinteger(2, doorSlot)
        local result = repentogon.L_Level_CanSpawnDoorOutline(self, roomIndex, doorSlot) return result
    end,
    CanStageHaveCurseOfLabyrinth = function(self, stage)
        stage = ffichecks.checkinteger(1, stage)
        local result = repentogon.L_Level_CanStageHaveCurseOfLabyrinth(self, stage) return result
    end,
    ChangeRoom = function(self, roomIndex, dimension)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        dimension = ffichecks.optnumber(dimension, -1)
        dimension = ffichecks.checkinteger(2, dimension)
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
        seed = ffichecks.checkinteger(1, seed)
        local result = repentogon.L_Level_ForceHorsemanBoss(self, seed) return result
    end,
    GetAbsoluteStage = function(self)
        local result = repentogon.L_Level_GetAbsoluteStage(self) return result
    end,
    GetAngelRoomChance = function(self)
        local result = ffi.getprivate(self, "AngelRoomChance") return result
    end,
    GetCanSeeEverything = function(self)
        local result = ffi.getprivate(self, "CanSeeEverything") return result
    end,
    GetCurrentRoom = function(self)
        local result = ffi.getprivate(self, "CurrentRoom") return result
    end,
    GetCurrentRoomDesc = function(self)
        local result = repentogon.L_Level_GetCurrentRoomDesc(self) return result
    end,
    GetCurrentRoomIndex = function(self)
        local result = ffi.getprivate(self, "CurrentRoomIndex") return result
    end,
    GetCurseName = function(self)
        local result = ffi.string(repentogon.L_Level_GetCurseName(self)) return result
    end,
    GetCurses = function(self)
        local result = repentogon.L_Level_GetCurses(self) return result
    end,
    GetDevilAngelRoomRNG = function(self)
        local result = ffi.getprivate(self, "DevilAngelRoomRNG") return result
    end,
    GetDimension = function(self)
        local result = ffi.getprivate(self, "CurrentDimension") return result
    end,
    GetDungeonPlacementSeed = function(self)
        local result = ffi.getprivate(self, "DungeonPlacementSeed") return result
    end,
    GetEnterPosition = function(self)
        local position = Vector(0, 0)
        repentogon.L_Level_GetEnterPosition(self, position)
        return position
    end,
    GetForceSpecialQuest = function(self)
        local result = repentogon.L_Level_GetForceSpecialQuest() return result
    end,
    GetGenerationRNG = function(self)
        local result = ffi.getprivate(self, "GenerationRNG") return result
    end,
    GetGreedWavesClearedWithoutRedHeartDamage = function(self)
        local result = ffi.getprivate(self, "GreedWavesClearedWithoutRedHeartDamage") return result
    end,
    GetHeartPicked = function(self)
        local result = ffi.getprivate(self, "HeartPicked") return result
    end,
    GetLastBossRoomListIndex = function(self)
        local result = ffi.getprivate(self, "LastBossRoomListIndex") return result
    end,
    GetLastRoomDesc = function(self)
        local result = repentogon.L_Level_GetLastRoomDesc(self) return result
    end,
    GetMyosotisPickups = function(self)
        local result = ffi.getprivate(self, "MyosotisPickups") return result
    end,
    GetName = function(self)
        local result = ffi.string(repentogon.L_Level_GetName(self)) return result
    end,
    GetNeighboringRooms = function(self, gridIndex, roomShape, dimension)
        gridIndex = ffichecks.checkinteger(1, gridIndex)
        roomShape = ffichecks.checkinteger(2, roomShape)
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
        local result = repentogon.L_Level_GetNonCompleteRoomIndex(self) return result
    end,
    GetPlanetariumChance = function(self)
        local result = repentogon.L_Level_GetPlanetariumChance(self) return result
    end,
    GetPreviousRoomIndex = function(self)
        local result = ffi.getprivate(self, "PreviousRoomIndex") return result
    end,
    GetRandomRoomIndex = function(self, iAmErrorRoom, seed)
        seed = ffichecks.checkinteger(2, seed)
        local result = repentogon.L_Level_GetRandomRoomIndex(self, ffichecks.optboolean(iAmErrorRoom, false), seed) return result
    end,
    GetRoomByIdx = function(self, index, dimension)
        index = ffichecks.checkinteger(1, index)
        dimension = ffichecks.optnumber(dimension, -1)
        dimension = ffichecks.checkinteger(2, dimension)
        local result = repentogon.L_Level_GetRoomByIdx(self, index, dimension) return result
    end,
    GetRoomCount = function(self)
        local result = ffi.getprivate(self, "RoomCount") return result
    end,
    GetRooms = function(self)
        local list = RoomDescriptorListT()
        ffi.setprivate(list, "_size", ffi.getprivate(self, "RoomCount"))
        ffi.setprivate(list, "_data", ffi.getprivate(self, "GridRooms"))
        return list
    end,
    GetStage = function(self)
        local result = ffi.getprivate(self, "Stage") return result
    end,
    GetStageType = function(self)
        local result = ffi.getprivate(self, "StageType") return result
    end,
    GetStartingRoomIndex = function(self)
        local result = ffi.getprivate(self, "StartingRoomIndex") return result
    end,
    GetStateFlag = function(self, flag)
        flag = ffichecks.checkinteger(1, flag)
        local result = repentogon.L_Level_GetStateFlag(self, flag) return result
    end,
    HasAbandonedMineshaft = function(self)
        local result = repentogon.L_Level_HasAbandonedMineshaft(self) return result
    end,
    HasBossChallenge = function(self)
        local result = ffi.getprivate(self, "BossChallenge") return result
    end,
    HasMirrorDimension = function(self)
        local result = repentogon.L_Level_HasMirrorDimension(self) return result
    end,
    HasPhotoDoor = function(self)
        local result = repentogon.L_Level_HasPhotoDoor(self) return result
    end,
    InitializeDevilAngelRoom = function(self, forceAngel, forceDevil)
        repentogon.L_Level_InitializeDevilAngelRoom(self, ffichecks.optboolean(forceAngel, false), ffichecks.optboolean(forceDevil, false))
    end,
    IsAltStage = function(self)
        return ffi.getprivate(self, "StageType") ~= 0
    end,
    IsAscent = function(self)
        local result = repentogon.L_Level_IsAscent(self) return result
    end,
    IsDevilRoomDisabled = function(self)
        local result = ffi.getprivate(self, "DevilRoomDisabled") return result
    end,
    IsNextStageAvailable = function(self)
        local result = repentogon.L_Level_IsNextStageAvailable(self) return result
    end,
    IsPreAscent = function(self)
        local result = repentogon.L_Level_IsPreAscent(self) return result
    end,
    IsStageAvailable = function(self, stage, stageType)
        stage = ffichecks.checkinteger(1, stage)
        stageType = ffichecks.checkinteger(2, stageType)
        local result = repentogon.L_Level_IsStageAvailable(stage, stageType) return result
    end,
    MakeRedRoomDoor = function(self, roomIndex, doorSlot)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        doorSlot = ffichecks.checkinteger(2, doorSlot)
        local result = repentogon.L_Level_MakeRedRoomDoor(self, roomIndex, doorSlot) return result
    end,
    PlaceRoom = function(self, entry, roomConfig, seed)
        ffichecks.checkcdata(1, entry, "LevelGeneratorEntry")
        ffichecks.checkcdata(2, roomConfig, "RoomConfigRoom")
        seed = ffichecks.checkinteger(3, seed)
        local result = repentogon.L_Level_PlaceRoom(self, entry, roomConfig, seed) return result
    end,
    QueryRoomTypeIndex = function(self, roomType, visited, rng, ignoreGroup)
        roomType = ffichecks.checkinteger(1, roomType)
        ffichecks.checkcdata(3, rng, "RNG")
        local result = repentogon.L_Level_QueryRoomTypeIndex(self, roomType, ffichecks.optboolean(visited, false), rng, ffichecks.optboolean(ignoreGroup, false)) return result
    end,
    RemoveCompassEffect = function(self)
        repentogon.L_Level_RemoveCompassEffect(self)
    end,
    RemoveCurses = function(self, curses)
        curses = ffichecks.checkinteger(1, curses)
        repentogon.L_Level_RemoveCurses(self, curses)
    end,
    SetCanSeeEverything = function(self, value)
        ffi.setprivate(self, "CanSeeEverything", ffichecks.optboolean(value, false))
    end,
    SetForceSpecialQuest = function(self, quest)
        quest = ffichecks.checkinteger(1, quest)
        repentogon.L_Level_SetForceSpecialQuest(quest)
    end,
    SetGreedWavesClearedWithoutRedHeartDamage = function(self, waves)
        waves = ffichecks.checkinteger(1, waves)
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
        stage = ffichecks.checkinteger(1, stage)
        stageType = ffichecks.checkinteger(2, stageType)
        repentogon.L_Level_SetStage(self, stage, stageType)
    end,
    SetStateFlag = function(self, flag, value)
        flag = ffichecks.checkinteger(1, flag)
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
        gridIndex = ffichecks.checkinteger(2, gridIndex)
        if gridIndex < 0 or gridIndex > 168 then
            return false
        end
        dimension = CheckDimension(3, dimension)
        seed = CheckPlacementSeed(4, seed)
        local result = repentogon.L_Level_TryPlaceRoom(roomConfig, gridIndex, dimension, seed,
            ffichecks.optboolean(allowMultipleDoors, true), ffichecks.optboolean(allowSpecialNeighbors, false), ffichecks.optboolean(allowNoNeighbors, false))
        return result
    end,
    TryPlaceRoomAtDoor = function(self, roomConfig, descriptor, doorSlot, seed, allowMultipleDoors, allowSpecialNeighbors)
        ffichecks.checkcdata(1, roomConfig, "RoomConfigRoom")
        ffichecks.checkcdata(2, descriptor, "RoomDescriptor")
        doorSlot = ffichecks.checkinteger(3, doorSlot)
        seed = CheckPlacementSeed(4, seed)
        local room = repentogon.L_Level_TryPlaceRoomAtDoor(roomConfig, descriptor, doorSlot, seed,
            ffichecks.optboolean(allowMultipleDoors, true), ffichecks.optboolean(allowSpecialNeighbors, false), attemptedOut)
        if not attemptedOut[0] then
            return false
        end
        return room
    end,
    UncoverHiddenDoor = function(self, roomIndex, doorSlot)
        roomIndex = ffichecks.checkinteger(1, roomIndex)
        doorSlot = ffichecks.checkinteger(2, doorSlot)
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
