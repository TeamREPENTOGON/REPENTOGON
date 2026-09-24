ffi.cdef [[
    struct RoomDescriptor {
        int GridIndex : 0x0;
        int SafeGridIndex : 0x4;
        int ListIndex : 0x8;
        private int Dimension : 0xc;
        struct RoomConfigRoom* Data : 0x10;
        struct RoomConfigRoom* OverrideData : 0x14;
        int AllowedDoors : 0x18;
        private int _Doors[8] : 0x1c;
        int DisplayFlags : 0x3c;
        int VisitedCount : 0x40;
        int Flags : 0x44;
        int16_t ClearCount : 0x4a;
        int16_t PoopCount : 0x50;
        private int16_t TaintedKeeperCoinSpawns : 0x52;
        int PitsCount : 0x54;
        int DecorationSeed : 0x58;
        int SpawnSeed : 0x5c;
        int AwardSeed : 0x60;
        const unsigned int BossDeathSeed: 0x64;
        int16_t ShopItemIdx: 0xac;
        int16_t ShopItemDiscountIdx : 0xae;
        int DeliriumDistance : 0xb0;
        private int Group : 0xb4;
    } : 0xb8;

    typedef struct RoomDescriptor* RoomDescriptorPtr;

    struct RoomDescriptorList {
        private uint32_t _size : 0x0;
        private struct RoomDescriptor* _data : 0x4;
    } : 0x8;

    typedef struct RoomDescriptorList* RoomDescriptorListPtr;

    void L_RoomDescriptor_AddRestrictedGridIndex(struct RoomDescriptor*, int);
    int L_RoomDescriptor_GetErrorTrinketEffect(struct RoomDescriptor*);
    int L_RoomDescriptor_GetNeighboringRooms(struct RoomDescriptor*, int*, struct RoomDescriptor**);
    int L_RoomDescriptor_GetRestrictedGridIndexesCount(struct RoomDescriptor*);
    int L_RoomDescriptor_GetRestrictedGridIndexes(struct RoomDescriptor*, int*, int);
    int L_RoomDescriptor_GetValidNeighborPlacementLocationsCount(struct RoomDescriptor*, int, int, bool, bool);
    int L_RoomDescriptor_GetValidNeighborPlacementLocations(struct RoomDescriptor*, int, int, bool, bool, int*, int);
]]

local ffi = ffi
local repentogon = ffidll

local cfuncs = {
    GetDecoSaveState = __Lua_RoomDescriptor_GetDecoSaveState,
    GetEntitiesSaveState = __Lua_RoomDescriptor_GetEntitiesSaveState,
    GetGridEntitiesSaveState = __Lua_RoomDescriptor_GetGridEntitiesSaveState,
    InitSeeds = __Lua_RoomDescriptor_InitSeeds,
}

local RoomDescriptorListMT; RoomDescriptorListMT = { __type = "RoomDescriptorList" }

local function rdl_size(self)
    return ffi.getprivate(self, "_size")
end

RoomDescriptorListMT.__len = function(self) return rdl_size(self) end

RoomDescriptorListMT.__index = function(self, k)
    if k == "Size" then return rdl_size(self) end
    return RoomDescriptorListMT[k]
end

function RoomDescriptorListMT:Get(idx)
    ffichecks.checkinteger(1, idx)
    if idx < 0 or idx >= rdl_size(self) then return nil end
    return ffi.getprivate(self, "_data") + idx
end

ffi.metatype("struct RoomDescriptorList", RoomDescriptorListMT)
RoomDescriptorList = setmetatable({}, { __class = RoomDescriptorListMT })

------------------

local RoomDescriptorMT
RoomDescriptorMT = {
    __type = "RoomDescriptor",
    AddRestrictedGridIndex = function(self, gridIndex) 
        ffichecks.checkinteger(1, gridIndex)
        repentogon.L_RoomDescriptor_AddRestrictedGridIndex(self, gridIndex)
    end,
    GetDecoSaveState = function(self)
        return cfuncs.GetDecoSaveState(self)
    end,
    GetDimension = function(self)
        return ffi.getprivate(self, "Dimension")
    end,
    GetEntitiesSaveState = function(self)
        return cfuncs.GetEntitiesSaveState(self)
    end,
    GetErrorTrinketEffect = function(self)
        return repentogon.L_RoomDescriptor_GetErrorTrinketEffect(self)
    end,
    GetGridEntitiesSaveState = function(self)
        return cfuncs.GetGridEntitiesSaveState(self)
    end,
    GetGroup = function(self)
        return ffi.getprivate(self, "Group")
    end,
    GetNeighboringRooms = function(self)
        local slots = ffi.new("int[?]", 8)
        local rooms = ffi.new("RoomDescriptor*[?]", 8)

        local count = repentogon.L_RoomDescriptor_GetNeighboringRooms(self, slots, rooms)

        local result = {}
        for i = 0, count - 1 do
            result[slots[i]] = rooms[i]
        end
        return result
    end,
    GetRestrictedGridIndexes = function(self)
        local count = repentogon.L_RoomDescriptor_GetRestrictedGridIndexesCount(self)
        if count == 0 then return {} end

        local indexes = ffi.new("int[?]", count)
        repentogon.L_RoomDescriptor_GetRestrictedGridIndexes(self, indexes, count)

        local result = {}
        for i = 0, count - 1 do
            result[i + 1] = indexes[i]
        end
        return result
    end,
    GetTaintedKeeperCoinSpawns = function(self)
        return ffi.getprivate(self, "TaintedKeeperCoinSpawns")
    end,
    GetValidNeighborPlacementLocations = function(self, param1, param2, param3, param4)
        local roomShape, doorMask, allowMultipleDoors, allowSpecialNeighbors
        if ffichecks.iscdata(param1, "RoomConfigRoom") then
            roomShape = param1.Shape
            doorMask = param1.Doors
            allowMultipleDoors = ffichecks.optboolean(param2, true)
            allowSpecialNeighbors = ffichecks.optboolean(param3, false)
        else
            roomShape = ffichecks.optnumber(param1, 1)
            doorMask = ffichecks.optnumber(param2, -1)
            allowMultipleDoors = ffichecks.optboolean(param3, true)
            allowSpecialNeighbors = ffichecks.optboolean(param4, true)
        end

        local count = repentogon.L_RoomDescriptor_GetValidNeighborPlacementLocationsCount(
            self, roomShape, doorMask, allowMultipleDoors, allowSpecialNeighbors)

        if count == 0 then return {} end
        local locations = ffi.new("int[?]", count)

        repentogon.L_RoomDescriptor_GetValidNeighborPlacementLocations(
            self, roomShape, doorMask, allowMultipleDoors, allowSpecialNeighbors, locations, count)

        local result = {}
        for i = 0, count - 1 do
            result[i + 1] = locations[i]
        end
        return result
    end,
    InitSeeds = function(self, rng)
        cfuncs.InitSeeds(self, rng)
    end,
    SetGroup = function(self, group)
        ffichecks.checkinteger(1, group)
        ffi.setprivate(self, "Group", group)
    end,
    SetTaintedKeeperCoinSpawns = function(self, num)
        ffichecks.checkinteger(1, num)
        ffi.setprivate(self, "TaintedKeeprCoinSpawns", num)
    end,
}

setmetatable(RoomDescriptorMT, { __index = function() end })

local FLAG_BITS = {
    Clear                   = 0x01,
    PressurePlatesTriggered = 0x02,
    SacrificeDone           = 0x04,
    ChallengeDone           = 0x08,
    SurpriseMiniboss        = 0x10,
    HasWater                = 0x20,
    NoReward                = 0x80,
}

local DOOR_COUNT = 8

RoomDescriptorMT.__index = function(self, key)
    local flag = FLAG_BITS[key]
    if flag then
        return (self.Flags & flag) ~= 0
    end
    if key == "Doors" then
        local result = {}
        for i = 0, DOOR_COUNT - 1 do
            result[i + 1] = ffi.getprivate(self, "_Doors")[i]
        end
        return result
    end
    return RoomDescriptorMT[key]
end

RoomDescriptorMT.__newindex = function(self, key, value)
    local flag = FLAG_BITS[key]
    if flag then
        if type(value) ~= "boolean" then
            error(string.format("bad value for RoomDescriptor.%s (boolean expected, got %s)", key, type(value)), 2)
        end
        if value then
            self.Flags = self.Flags | flag
        else
            self.Flags = self.Flags & ~flag
        end
        return
    end
    error(string.format("cannot assign to RoomDescriptor.%s", tostring(key)), 2)
end


local RoomDescriptorT = ffi.metatype("struct RoomDescriptor", RoomDescriptorMT)

RoomDescriptor = setmetatable({
    CreateGroup = function(groupName)
        ffichecks.checkstring(1, groupName)
    end,
}, {
    __class = RoomDescriptorMT,
})

__Lua_RoomDescriptor_GetDecoSaveState = nil
__Lua_RoomDescriptor_GetEntitiesSaveState = nil
__Lua_RoomDescriptor_GetGridEntitiesSaveState = nil
__Lua_RoomDescriptor_InitSeeds = nil