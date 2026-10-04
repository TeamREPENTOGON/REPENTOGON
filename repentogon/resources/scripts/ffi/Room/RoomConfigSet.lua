ffi.cdef [[
    struct RoomSet {
        private struct RoomConfigRoom* Configs : 0x18;
        private unsigned int Count : 0x1c;
        private bool Loaded : 0x28;
    } : 0x2c;

    struct RoomConfigSet {
        private struct RoomSet* VanillaSet;
        private unsigned int SetID;
    };

    struct RoomConfigLuaEntryDesc {
        int Type;
        uint32_t Variant;
        int Subtype;
        float Weight;
    };

    struct RoomConfigLuaSpawnDesc {
        int16_t X;
        int16_t Y;
        uint32_t EntryCount;
        struct RoomConfigLuaEntryDesc* Entries;
    };

    struct RoomConfigLuaRoomDesc {
        int Type;
        uint32_t Variant;
        int Subtype;
        int Difficulty;
        const char* Name;
        float Weight;
        int Shape;
        uint32_t Doors;
        uint32_t SpawnCount;
        struct RoomConfigLuaSpawnDesc* Spawns;
    };

    unsigned int L_RoomConfig_GetVanillaSetID(uint32_t, int);
    bool L_RoomConfig_HasShapeSlot(int, unsigned int);
    int L_RoomConfig_GetDoorFromPosition(int16_t, int16_t, int);
    void L_RoomConfig_Log(const char*);
    unsigned int L_RoomConfigSet_GetVirtualSize(unsigned int);
    struct RoomConfigRoom* L_RoomConfigSet_GetVirtualRoom(unsigned int, unsigned int);
    unsigned int L_RoomConfigSet_BeginAddRooms(unsigned int);
    struct RoomConfigRoom* L_RoomConfigSet_AddRoom(unsigned int, const struct RoomConfigLuaRoomDesc*);
    void L_RoomConfigSet_EndAddRooms(unsigned int, unsigned int);
    unsigned int L_RoomConfigSet_AddStbRooms(unsigned int, const char*);
]]

local repentogon = ffidll
local ffi = ffi

local ROOM_DEFAULT = 1
local NUM_ROOMTYPES = 31
local ROOMSHAPE_1x1 = 1
local NUM_ROOMSHAPES = 13
local NO_DOOR_SLOT = -1
local NUM_DOOR_SLOTS = 8
local FLT_MAX = 3.4028234663852886e38

local INT, UINT32, INT16, INT8 = 1, 2, 3, 4
local INTEGER_LIMITS = { [INT16] = { -32768, 32767 }, [INT8] = { -128, 127 } }

local function LogMessage(context, logType, message)
    repentogon.L_RoomConfig_Log(string.format("[%s] %s%s\n", logType, table.concat(context), message))
end

local function LogInvalidArg(context, err, fieldName)
    if fieldName then
        LogMessage(context, "ERROR", string.format('invalid argument for "%s" (%s)', fieldName, err))
    else
        LogMessage(context, "ERROR", string.format("invalid argument (%s)", err))
    end
end

local function TypeMessage(value, expected)
    return expected .. " expected, got " .. type(value)
end

local function ValidateTable(value, context)
    if type(value) ~= "table" then
        LogInvalidArg(context, TypeMessage(value, "table"))
        return false
    end
    return true
end

local function ReadInteger(tbl, fieldName, context, optional, kind)
    local value = tbl[fieldName]
    if math.type(value) ~= "integer" then
        if not optional or value ~= nil then
            LogInvalidArg(context, TypeMessage(value, "integer"), fieldName)
        end
        return nil
    end
    if kind == UINT32 and value < 0 then
        LogInvalidArg(context, TypeMessage(value, "unsigned integer"), fieldName)
        return 0
    end
    local limits = INTEGER_LIMITS[kind]
    if limits and (value < limits[1] or value > limits[2]) then
        LogInvalidArg(context, string.format('value "%d" is not within numeric limits (%d, %d)', value, limits[1], limits[2]), fieldName)
        return 0
    end
    return value
end

local function ReadNumber(tbl, fieldName, context, optional)
    local value = tbl[fieldName]
    local number = (type(value) == "number" or type(value) == "string") and tonumber(value)
    if not number then
        if not optional or value ~= nil then
            LogInvalidArg(context, TypeMessage(value, "number"), fieldName)
        end
        return nil
    end
    if number < -FLT_MAX or number > FLT_MAX then
        LogInvalidArg(context, string.format('value "%f" is not within numeric limits (%f, %f)', number, -FLT_MAX, FLT_MAX), fieldName)
        return 0
    end
    return number
end

local function ReadString(tbl, fieldName, context, optional)
    local value = tbl[fieldName]
    if type(value) == "number" then
        return tostring(value)
    elseif type(value) ~= "string" then
        if not optional or value ~= nil then
            LogInvalidArg(context, TypeMessage(value, "string"), fieldName)
        end
        return nil
    end
    return value
end

local function ReadBool(tbl, fieldName, context, optional)
    local value = tbl[fieldName]
    if type(value) ~= "boolean" then
        if not optional or value ~= nil then
            LogInvalidArg(context, TypeMessage(value, "boolean"), fieldName)
        end
        return nil
    end
    return value
end

local function ReadWeight(tbl, context)
    local weight = ReadNumber(tbl, "WEIGHT", context, true) or 0
    if weight < 0 then
        LogMessage(context, "ERROR", 'invalid argument for "WEIGHT" (weight cannot be negative)')
        weight = 0
    end
    return weight
end

local function BuildSpawnEntry(entryTable, context)
    if not ValidateTable(entryTable, context) then
        return nil
    end
    local entryType = ReadInteger(entryTable, "TYPE", context, false, INT)
    if not entryType then
        return nil
    end
    return {
        Type = entryType,
        Variant = ReadInteger(entryTable, "VARIANT", context, true, UINT32) or 0,
        Subtype = ReadInteger(entryTable, "SUBTYPE", context, true, INT) or 0,
        Weight = ReadWeight(entryTable, context),
    }
end

local function BuildSpawn(spawnTable, context)
    if not ValidateTable(spawnTable, context) then
        return nil
    end
    local x = ReadInteger(spawnTable, "GRIDX", context, false, INT16)
    local y = ReadInteger(spawnTable, "GRIDY", context, false, INT16)
    if not x or not y then
        return nil
    end

    local entries = {}
    for i = 1, #spawnTable do
        context[#context + 1] = string.format("spawn entry #%d -> ", i)
        local entry = BuildSpawnEntry(rawget(spawnTable, i), context)
        if not entry then
            LogMessage(context, "ERROR", "unable to build spawn entry.")
        else
            entries[#entries + 1] = entry
        end
        context[#context] = nil
    end
    return { X = x, Y = y, Entries = entries }
end

local function GetDoorSlot(doorTable, shape, context)
    if not ValidateTable(doorTable, context) then
        return NO_DOOR_SLOT
    end
    if not ReadBool(doorTable, "EXISTS", context, true) then
        return NO_DOOR_SLOT
    end

    local slot = ReadInteger(doorTable, "SLOT", context, true, UINT32)
    if slot then
        if repentogon.L_RoomConfig_HasShapeSlot(shape, slot) then
            return slot
        end
        LogMessage(context, "WARN", string.format("provided door slot is invalid for shape %d: %d", shape, slot))
    end

    local x = ReadInteger(doorTable, "GRIDX", context, false, INT16)
    local y = ReadInteger(doorTable, "GRIDY", context, false, INT16)
    if not x or not y then
        return NO_DOOR_SLOT
    end

    local doorSlot = repentogon.L_RoomConfig_GetDoorFromPosition(x, y, shape)
    if doorSlot <= NO_DOOR_SLOT or doorSlot >= NUM_DOOR_SLOTS then
        LogMessage(context, "WARN", string.format("door slot position is invalid for shape %d: (%d, %d)", shape, x, y))
        doorSlot = NO_DOOR_SLOT
    end
    return doorSlot
end

local function BuildRoom(roomTable, context)
    if not ValidateTable(roomTable, context) then
        return nil
    end

    local roomType = ReadInteger(roomTable, "TYPE", context, false, INT); if not roomType then return nil end
    local variant = ReadInteger(roomTable, "VARIANT", context, false, UINT32); if not variant then return nil end
    local name = ReadString(roomTable, "NAME", context, false); if not name then return nil end
    local shape = ReadInteger(roomTable, "SHAPE", context, false, INT8); if not shape then return nil end

    if roomType < ROOM_DEFAULT or roomType >= NUM_ROOMTYPES then
        LogInvalidArg(context, string.format('invalid argument for "TYPE" (invalid room type %d)', roomType), "TYPE")
        roomType = ROOM_DEFAULT
    end
    if shape <= 0 or shape >= NUM_ROOMSHAPES then
        LogMessage(context, "ERROR", string.format('invalid argument for "SHAPE" (invalid room shape %d) ', shape))
        shape = ROOMSHAPE_1x1
    end

    local room = {
        Type = roomType,
        Variant = variant,
        Subtype = ReadInteger(roomTable, "SUBTYPE", context, true, INT) or 0,
        Difficulty = ReadInteger(roomTable, "DIFFICULTY", context, true, INT) or 1,
        Name = name,
        Weight = ReadWeight(roomTable, context),
        Shape = shape,
        Doors = 0,
        Spawns = {},
    }

    for i = 1, #roomTable do
        context[#context + 1] = string.format("spawn #%d -> ", i)
        local spawnTable = rawget(roomTable, i)
        if ValidateTable(spawnTable, context) then
            if ReadBool(spawnTable, "ISDOOR", context, true) then
                local doorSlot = GetDoorSlot(spawnTable, shape, context)
                if doorSlot ~= NO_DOOR_SLOT then
                    room.Doors = room.Doors | (1 << doorSlot)
                end
            else
                local spawn = BuildSpawn(spawnTable, context)
                if not spawn then
                    LogMessage(context, "ERROR", "unable to build spawn.")
                else
                    room.Spawns[#room.Spawns + 1] = spawn
                end
            end
        end
        context[#context] = nil
    end

    return room
end

local function AddRoom(setId, room)
    local spawns = ffi.new("struct RoomConfigLuaSpawnDesc[?]", #room.Spawns)
    local entryArrays = {}
    for i, spawn in ipairs(room.Spawns) do
        local entries = ffi.new("struct RoomConfigLuaEntryDesc[?]", #spawn.Entries)
        for j, entry in ipairs(spawn.Entries) do
            entries[j - 1] = entry
        end
        entryArrays[i] = entries
        spawns[i - 1].X, spawns[i - 1].Y = spawn.X, spawn.Y
        spawns[i - 1].EntryCount, spawns[i - 1].Entries = #spawn.Entries, entries
    end

    local desc = ffi.new("struct RoomConfigLuaRoomDesc", room.Type, room.Variant, room.Subtype, room.Difficulty,
        room.Name, room.Weight, room.Shape, room.Doors, #room.Spawns, spawns)
    return repentogon.L_RoomConfigSet_AddRoom(setId, desc), desc, spawns, entryArrays
end

local function AddLuaRooms(setId, rooms)
    local begin = repentogon.L_RoomConfigSet_BeginAddRooms(setId)
    local context = {}
    local added = {}
    for i = 1, #rooms do
        local room = BuildRoom(rawget(rooms, i), context)
        if room then
            added[i] = AddRoom(setId, room)
        end
    end
    repentogon.L_RoomConfigSet_EndAddRooms(setId, begin)
    return added
end

local function AddStbRooms(setId, filename)
    local added = {}
    for i = repentogon.L_RoomConfigSet_AddStbRooms(setId, filename), repentogon.L_RoomConfigSet_GetVirtualSize(setId) - 1 do
        added[#added + 1] = repentogon.L_RoomConfigSet_GetVirtualRoom(setId, i)
    end
    return added
end

local function ToInteger(value)
    local number = tonumber(value)
    if not number then
        return 0
    end
    return number < 0 and math.ceil(number) or math.floor(number)
end

local function GetSize(self)
    local vanillaSet = ffi.getprivate(self, "VanillaSet")
    local vanillaSize = vanillaSet and ffi.getprivate(vanillaSet, "Count") or 0
    return repentogon.L_RoomConfigSet_GetVirtualSize(ffi.getprivate(self, "SetID")) + vanillaSize
end

local RoomConfigSetMT
RoomConfigSetMT = {
    __type = "RoomConfigSet",
    __len = GetSize,
    AddRooms = function(self, rooms)
        if type(rooms) ~= "table" then
            ffichecks.argerror(1, TypeMessage(rooms, "table"))
        end
        return AddLuaRooms(ffi.getprivate(self, "SetID"), rooms)
    end,
    Get = function(self, index)
        index = ToInteger(index)
        if index < 0 then
            return nil
        end

        local vanillaSet = ffi.getprivate(self, "VanillaSet")
        if vanillaSet then
            local vanillaSize = ffi.getprivate(vanillaSet, "Count")
            if index < vanillaSize then
                return ffi.getprivate(vanillaSet, "Configs") + index
            end
            index = index - vanillaSize
        end

        local setId = ffi.getprivate(self, "SetID")
        if index < repentogon.L_RoomConfigSet_GetVirtualSize(setId) then
            return repentogon.L_RoomConfigSet_GetVirtualRoom(setId, index)
        end
        return nil
    end,
    LoadStb = function(self, filename)
        filename = ffichecks.checkstring(1, filename)
        return AddStbRooms(ffi.getprivate(self, "SetID"), filename)
    end,
}

setmetatable(RoomConfigSetMT, { __index = function() end })
RoomConfigSetMT.__index = function(self, key)
    if key == "Size" then
        return GetSize(self)
    end
    return RoomConfigSetMT[key]
end

ffi.metatype("struct RoomConfigSet", RoomConfigSetMT)

RoomConfigSet = setmetatable({}, {
    __class = RoomConfigSetMT,
})
