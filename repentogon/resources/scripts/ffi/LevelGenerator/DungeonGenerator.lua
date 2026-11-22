ffi.cdef [[
    struct DungeonGeneratorRoom {};

    struct DungeonGeneratorRoom* L_DungeonGenerator_PlaceRoom(struct DungeonGenerator*, struct RoomConfigRoom*, int, int, int);
    struct DungeonGeneratorRoom* L_DungeonGenerator_TryPlaceDefaultStartingRoom(struct DungeonGenerator*, int);
    void L_DungeonGenerator_SetFinalBossRoom(struct DungeonGenerator*, struct DungeonGeneratorRoom*);
    void L_DungeonGenerator_SetGreedGoldRoom(struct DungeonGenerator*, struct DungeonGeneratorRoom*);
    void L_DungeonGenerator_BlockIndex(struct DungeonGenerator*, int);
    bool L_DungeonGenerator_Validate(struct DungeonGenerator*);
    void L_DungeonGenerator_Reset(struct DungeonGenerator*);
    void L_DungeonGenerator_InitializeDefaultOffGridRooms(struct DungeonGenerator*);
]]

local repentogon = ffidll
local ffi = ffi

local DungeonGeneratorMT
DungeonGeneratorMT = {
    __type = "DungeonGenerator",
    PlaceRoom = function(self, room, col, row, allowedDoors)
        ffichecks.checkcdata(1, room, "RoomConfigRoom")
        col = ffichecks.checkinteger(2, col)
        row = ffichecks.checkinteger(3, row)
        allowedDoors = ffichecks.optinteger(allowedDoors, -1)
        if column < 0 or column > 12 then
            ffichecks.argerror(2, string.format("invalid column %d, value must be between 0 and 12 (inclusive)", column))
        end
        if row < 0 or row > 12 then
            ffichecks.argerror(3, string.format("invalid row %d, value must be between 0 and 12 (inclusive)", row))
        end
        return L_DungeonGenerator_PlaceRoom(self, room, col, row, allowedDoors)
    end,
    PlaceRoomAtIdx = function(self, room, idx, allowedDoors)
        idx = ffichecks.checkinteger(2, idx)
        local col = idx % 13;
        local row = idx // 13;
        return self:PlaceRoom(room, col, row, allowedDoors)
    end,
    TryPlaceDefaultStartingRoom = function(self, doors)
        doors = ffichecks.optinteger(doors, -1)
        return L_DungeonGenerator_TryPlaceDefaultStartingRoom(self, doors)
    end,
    SetFinalBossRoom = function(self, generatorRoom)
        ffichecks.checkcdata(1, generatorRoom, "DungeonGeneratorRoom")
        return L_DungeonGenerator_SetFinalBossRoom(self, generatorRoom)
    end,
    SetGreedGoldRoom = function(self, generatorRoom)
        ffichecks.checkcdata(1, generatorRoom, "DungeonGeneratorRoom")
        return L_DungeonGenerator_SetGreedGoldRoom(self, generatorRoom)
    end,
    BlockIndex = function(self, idx)
        idx = ffichecks.checkinteger(2, idx)
        return L_DungeonGenerator_BlockIndex(self, idx)
    end,
    Validate = function(self)
        return L_DungeonGenerator_Validate(self)
    end,
    Reset = function(self)
        return L_DungeonGenerator_Reset(self)
    end,
    InitializeDefaultOffGridRooms = function(self)
        return L_DungeonGenerator_InitializeDefaultOffGridRooms(self)
    end,
}

setmetatable(DungeonGeneratorMT, { __index = function() end })
DungeonGeneratorMT.__index = DungeonGeneratorMT

ffi.metatype("struct DungeonGenerator", DungeonGeneratorMT)

DungeonGenerator = setmetatable({}, {
    __class = DungeonGeneratorMT,
})
