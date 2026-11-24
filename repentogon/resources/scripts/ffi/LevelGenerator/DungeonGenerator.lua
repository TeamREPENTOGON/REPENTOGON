ffi.cdef [[
    struct DungeonGenerator {};
    typedef struct DungeonGenerator* DungeonGeneratorPtr;

    struct DungeonGeneratorRoom {};
    typedef struct DungeonGeneratorRoom* DungeonGeneratorRoomPtr;

    struct DungeonGeneratorRoom* L_DungeonGenerator_PlaceRoom(struct DungeonGenerator*, int, struct RoomConfigRoom*, int);
    struct DungeonGeneratorRoom* L_DungeonGenerator_PlaceRandomRoom(struct DungeonGenerator*, int, int, int, int, int, int, int, int, int, int, int);
    struct DungeonGeneratorRoom* L_DungeonGenerator_PlaceOffGridRoom(struct DungeonGenerator*, int, struct RoomConfigRoom*);
    struct DungeonGeneratorRoom* L_DungeonGenerator_PlaceRandomOffGridRoom(struct DungeonGenerator*, int, int, int, int, int, int, int, int, int, int);
    struct DungeonGeneratorRoom* L_DungeonGenerator_TryPlaceDefaultStartingRoom(struct DungeonGenerator*, int);
    void L_DungeonGenerator_SetFinalBossRoom(struct DungeonGenerator*, struct DungeonGeneratorRoom*);
    void L_DungeonGenerator_SetGreedGoldRoom(struct DungeonGenerator*, struct DungeonGeneratorRoom*);
    void L_DungeonGenerator_BlockIndex(struct DungeonGenerator*, int);
    bool L_DungeonGenerator_Validate(struct DungeonGenerator*);
    void L_DungeonGenerator_Reset(struct DungeonGenerator*);
    void L_DungeonGenerator_InitializeDefaultOffGridRooms(struct DungeonGenerator*);
    struct RNG* L_DungeonGenerator_GetRNG(struct DungeonGenerator*);
]]

local repentogon = ffidll
local ffi = ffi

local STB_UNUSED1 = 18
local STB_ULTRA_GREED = 25
local STB_THE_VOID = 26

local DungeonGeneratorMT
DungeonGeneratorMT = {
    __type = "DungeonGenerator",
    PlaceRoom = function(self, idx, room, allowedDoors)
        idx = ffichecks.checkinteger(1, idx)
        if idx < 0 or idx > 168 then
            ffichecks.argerror(1, string.format("invalid grid index %d", idx))
        end
        ffichecks.checkcdata(2, room, "RoomConfigRoom")
        allowedDoors = ffichecks.optinteger(allowedDoors, -1)
        return repentogon.L_DungeonGenerator_PlaceRoom(self, idx, room, allowedDoors)
    end,
    PlaceRandomRoom = function(self, idx, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType, mode)
        idx = ffichecks.checkinteger(1, idx)
        if idx < 0 or idx > 168 then
            ffichecks.argerror(1, string.format("invalid grid index %d", idx))
        end
        stage = ffichecks.optinteger(stage, -1)
        if stage < -1 or (stage >= STB_UNUSED1 and stage <= STB_ULTRA_GREED) or stage == STB_THE_VOID or stage >= RoomConfig.GetNumStages() then
            ffichecks.argerror(2, string.format("invalid StbType %d", stage))
        end
        roomType = ffichecks.optinteger(roomType, RoomType.ROOM_DEFAULT)
        if roomType < 1 or roomType > 29 then
            ffichecks.argerror(3, string.format("invalid RoomType %d", roomType))
        end
        roomShape = ffichecks.optinteger(roomShape, RoomShape.ROOMSHAPE_1x1)
        if roomShape < 1 or roomShape > 13 then
            ffichecks.argerror(4, string.format("invalid RoomShape %d", roomShape))
        end
        minVariant = math.max(ffichecks.optnumber(minVariant, 0), 0)
        maxVariant = ffichecks.optinteger(maxVariant, -1)
        if maxVariant < minVariant and maxVariant >= 0 then
            ffichecks.argerror(6, string.format("maxVariant is lower than minVariant (min = %d, max = %d)", minVariant, maxVariant))
        elseif maxVariant < 0 then
            maxVariant = -1
        end
        minDifficulty = math.max(ffichecks.optnumber(minDifficulty, 0), 0)
        maxDifficulty = ffichecks.optinteger(maxDifficulty, 10)
        if maxDifficulty < minDifficulty then
            ffichecks.argerror(8, string.format("maxDifficulty is lower than minDifficulty (min = %d, max = %d)", minDifficulty, maxDifficulty))
        end
        doors = ffichecks.optnumber(doors, -1)
        if doors < -1 then
            ffichecks.argerror(9, string.format("invalid door mask %d", doors))
        end
        subType = ffichecks.optinteger(subType, -1)
        if subType < -1 then
            ffichecks.argerror(10, string.format("invalid subtype %d", subType))
        end
        mode = ffichecks.optinteger(mode, -1)
        if mode < -1 or mode > 1 then
            ffichecks.argerror(11, string.format("invalid mode %d", mode))
        end

        return repentogon.L_DungeonGenerator_PlaceRandomRoom(self, idx, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType, mode)
    end,
    PlaceOffGridRoom = function(self, idx, room)
        idx = ffichecks.checkinteger(1, idx)
        if idx < -GridRooms.NUM_OFF_GRID_ROOMS or idx >=0 then
            ffichecks.argerror(1, string.format("invalid off-grid index %d", idx))
        end
        ffichecks.checkcdata(2, room, "RoomConfigRoom")
        return repentogon.L_DungeonGenerator_PlaceOffGridRoom(self, idx, room)
    end,
    PlaceRandomOffGridRoom = function(self, idx, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode)
        idx = ffichecks.checkinteger(1, idx)
        if idx < -GridRooms.NUM_OFF_GRID_ROOMS or idx >=0 then
            ffichecks.argerror(1, string.format("invalid off-grid index %d", idx))
        end
        stage = ffichecks.checkinteger(2, stage)
        if stage < 0 or (stage >= STB_UNUSED1 and stage <= STB_ULTRA_GREED) or stage == STB_THE_VOID or stage >= RoomConfig.GetNumStages() then
            ffichecks.argerror(2, string.format("invalid StbType %d", stage))
        end
        roomType = ffichecks.checkinteger(3, roomType)
        if roomType < 1 or roomType > 29 then
            ffichecks.argerror(3, string.format("invalid RoomType %d", roomType))
        end
        roomShape = ffichecks.optinteger(roomShape, RoomShape.ROOMSHAPE_1x1)
        if roomShape < 1 or roomShape > 13 then
            ffichecks.argerror(4, string.format("invalid RoomShape %d", roomShape))
        end
        minVariant = math.max(ffichecks.optnumber(minVariant, 0), 0)
        maxVariant = ffichecks.optinteger(maxVariant, -1)
        if maxVariant < minVariant and maxVariant >= 0 then
            ffichecks.argerror(6, string.format("maxVariant is lower than minVariant (min = %d, max = %d)", minVariant, maxVariant))
        elseif maxVariant < 0 then
            maxVariant = -1
        end
        minDifficulty = math.max(ffichecks.optnumber(minDifficulty, 0), 0)
        maxDifficulty = ffichecks.optinteger(maxDifficulty, 10)
        if maxDifficulty < minDifficulty then
            ffichecks.argerror(8, string.format("maxDifficulty is lower than minDifficulty (min = %d, max = %d)", minDifficulty, maxDifficulty))
        end
        subType = ffichecks.optinteger(subType, -1)
        if subType < -1 then
            ffichecks.argerror(10, string.format("invalid subtype %d", subType))
        end
        mode = ffichecks.optinteger(mode, -1)
        if mode < -1 or mode > 1 then
            ffichecks.argerror(11, string.format("invalid mode %d", mode))
        end

        return repentogon.L_DungeonGenerator_PlaceRandomOffGridRoom(self, idx, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, subType, mode)
    end,
    TryPlaceDefaultStartingRoom = function(self, doors)
        doors = ffichecks.optinteger(doors, -1)
        return repentogon.L_DungeonGenerator_TryPlaceDefaultStartingRoom(self, doors)
    end,
    SetFinalBossRoom = function(self, generatorRoom)
        ffichecks.checkcdata(1, generatorRoom, "DungeonGeneratorRoom")
        return repentogon.L_DungeonGenerator_SetFinalBossRoom(self, generatorRoom)
    end,
    SetGreedGoldRoom = function(self, generatorRoom)
        ffichecks.checkcdata(1, generatorRoom, "DungeonGeneratorRoom")
        return repentogon.L_DungeonGenerator_SetGreedGoldRoom(self, generatorRoom)
    end,
    BlockIndex = function(self, idx)
        idx = ffichecks.checkinteger(2, idx)
        if idx < 0 or idx > 168 then
            ffichecks.argerror(1, string.format("invalid grid index %d", idx))
        end
        return repentogon.L_DungeonGenerator_BlockIndex(self, idx)
    end,
    Validate = function(self)
        return repentogon.L_DungeonGenerator_Validate(self)
    end,
    Reset = function(self)
        return repentogon.L_DungeonGenerator_Reset(self)
    end,
    InitializeDefaultOffGridRooms = function(self)
        return repentogon.L_DungeonGenerator_InitializeDefaultOffGridRooms(self)
    end,
    GetRNG = function(self)
        return repentogon.L_DungeonGenerator_GetRNG(self);
    end,
}

setmetatable(DungeonGeneratorMT, { __index = function() end })
DungeonGeneratorMT.__index = DungeonGeneratorMT

ffi.metatype("struct DungeonGenerator", DungeonGeneratorMT)

DungeonGenerator = setmetatable({}, {
    __class = DungeonGeneratorMT,
})
