ffi.cdef [[
    struct RoomConfigRoom* L_RoomConfig_GetRoomByStageTypeAndVariant(uint32_t, uint32_t, uint32_t, int);
    struct RoomConfigRoom* L_RoomConfig_GetRandomRoom(unsigned int, bool, int, int, int, unsigned int, int, int, int, unsigned int, int, int);
    struct RoomConfigStage* L_RoomConfig_GetStage(int);
]]

local repentogon = ffidll
local ffi = ffi

local RoomConfigSetT = ffi.typeof("struct RoomConfigSet")

local NUM_STB = 37
local STB_UNUSED1 = 18
local STB_ULTRA_GREED = 25
local STB_THE_VOID = 26

local function CheckStageAndMode(stage, mode)
    stage = ffichecks.checkinteger(1, stage, 3)
    mode = ffichecks.checkinteger(2, mode, 3)
    if stage < 0 or stage >= NUM_STB then
        ffichecks.argerror(1, string.format("invalid stage %d", stage), 3)
    end
    if mode < -1 or mode > 1 then
        ffichecks.argerror(2, string.format("invalid mode %d", mode), 3)
    end
    return stage, mode
end

RoomConfig = {
    AddRooms = function(stage, mode, rooms)
        stage, mode = CheckStageAndMode(stage, mode)
        ffichecks.checktable(3, rooms)
        return RoomConfigSetT(nil, repentogon.L_RoomConfig_GetVanillaSetID(stage, mode)):AddRooms(rooms)
    end,
    GetRandomRoom = function(seed, reduceWeight, stage, roomType, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode)
        seed = ffichecks.checkinteger(1, seed)
        reduceWeight = ffichecks.checkboolean(2, reduceWeight)
        stage = ffichecks.checkinteger(3, stage)
        if stage < 0 or (stage >= STB_UNUSED1 and stage <= STB_ULTRA_GREED) or stage == STB_THE_VOID or stage >= NUM_STB then
            ffichecks.argerror(3, string.format("invalid stage %d", stage))
        end
        roomType = ffichecks.checkinteger(4, roomType)
        if roomType < 1 or roomType > 29 then
            ffichecks.argerror(4, string.format("invalid type %d", roomType))
        end
        shape = ffichecks.optnumber(shape, 13)
        if shape < 1 or shape > 13 then
            ffichecks.argerror(5, string.format("invalid shape %d", shape))
        end
        minVariant = math.max(ffichecks.optnumber(minVariant, 0), 0)
        maxVariant = ffichecks.optnumber(maxVariant, -1)
        if maxVariant < minVariant and maxVariant >= 0 then
            ffichecks.argerror(7, string.format("maxVariant is lower than minVariant (min = %d, max = %d)", minVariant, maxVariant))
        elseif maxVariant < 0 then
            maxVariant = -1
        end
        minDifficulty = math.max(ffichecks.optnumber(minDifficulty, 0), 0)
        maxDifficulty = ffichecks.optnumber(maxDifficulty, 10)
        if maxDifficulty < minDifficulty then
            ffichecks.argerror(9, string.format("maxDifficulty is lower than minDifficulty (min = %d, max = %d)", minDifficulty, maxDifficulty))
        end
        doors = ffichecks.optnumber(doors, 0)
        if doors < 0 then
            ffichecks.argerror(10, string.format("invalid door mask %d", doors))
        end
        subtype = ffichecks.optnumber(subtype, -1)
        if subtype < -1 then
            ffichecks.argerror(11, string.format("invalid subtype %d", subtype))
        end
        mode = ffichecks.optnumber(mode, -1)
        if mode < -1 or mode > 1 then
            ffichecks.argerror(12, string.format("invalid mode %d", mode))
        end
        return repentogon.L_RoomConfig_GetRandomRoom(seed, reduceWeight, stage, roomType, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode)
    end,
    GetRoomByStageTypeAndVariant = function(...)
        local n = select("#", ...)
        if n < 3 then
            error(string.format("Expected three parameters, got %d", n), 2)
        end
        local stage, roomType, variant, mode = ...
        stage = ffichecks.checkinteger(1, stage)
        if stage < 0 or stage >= NUM_STB then
            ffichecks.argerror(1, string.format("StageID must be between 0 and 36 (both inclusive), got %d", stage))
        end
        roomType = ffichecks.checkinteger(2, roomType)
        if roomType < 1 or roomType > 29 then
            ffichecks.argerror(2, string.format("Type must be between 1 and 29 (both inclusive), got %d", roomType))
        end
        variant = ffichecks.checkinteger(3, variant)
        mode = ffichecks.optnumber(mode, -1)
        if mode < -2 or mode > 1 then
            mode = -1
        end
        return repentogon.L_RoomConfig_GetRoomByStageTypeAndVariant(stage, roomType, variant, mode)
    end,
    GetStage = function(stage)
        stage = ffichecks.checkinteger(1, stage)
        if stage < 0 or stage > 36 then
            ffichecks.argerror(1, string.format("StageID must be between 0 and 36 (both inclusive), got %d", stage))
        end
        return repentogon.L_RoomConfig_GetStage(stage)
    end,
    LoadStb = function(stage, mode, filename)
        stage, mode = CheckStageAndMode(stage, mode)
        filename = ffichecks.checkstring(3, filename)
        return RoomConfigSetT(nil, repentogon.L_RoomConfig_GetVanillaSetID(stage, mode)):LoadStb(filename)
    end,
}
