ffi.cdef [[
    struct LevelGenerator {
        private struct LevelGeneratorRoom* RoomsFirst : 0x364;
        private struct LevelGeneratorRoom* RoomsLast : 0x368;
        private int* DeadEndsFirst : 0x370;
        private int* DeadEndsLast : 0x374;
        private int* NonDeadEndsFirst : 0x37c;
        private int* NonDeadEndsLast : 0x380;
    } : 0x394;
    typedef struct LevelGenerator* LevelGeneratorPtr;

    const char* L_LevelGenerator_PlaceRoom(struct LevelGenerator*, int, int, int, struct LevelGeneratorRoom*, int*);
]]

local repentogon = ffidll
local ffi = ffi

local MAX_ROOMSHAPES = 13

local function GetRooms(self, indicesFirst, indicesLast)
    local rooms = ffi.getprivate(self, "RoomsFirst")
    local first = ffi.getprivate(self, indicesFirst)
    local result = {}
    for i = 0, ffichecks.vectorsize(first, ffi.getprivate(self, indicesLast), ffi.sizeof("int")) - 1 do
        result[i + 1] = rooms + first[i]
    end
    return result
end

local LevelGeneratorMT
LevelGeneratorMT = {
    __type = "LevelGenerator",
    GetAllRooms = function(self)
        local first = ffi.getprivate(self, "RoomsFirst")
        local result = {}
        for i = 0, ffichecks.vectorsize(first, ffi.getprivate(self, "RoomsLast"), ffi.sizeof("struct LevelGeneratorRoom")) - 1 do
            result[i + 1] = first + i
        end
        return result
    end,
    GetDeadEnds = function(self)
        return GetRooms(self, "DeadEndsFirst", "DeadEndsLast")
    end,
    GetNonDeadEnds = function(self)
        return GetRooms(self, "NonDeadEndsFirst", "NonDeadEndsLast")
    end,
    PlaceRoom = function(self, column, line, shape, neighbor)
        column = ffichecks.checkinteger(1, column)
        if column < 0 or column > 12 then
            ffichecks.argerror(1, string.format("invalid column %d, value must be between 0 and 12 (inclusive)", column))
        end
        line = ffichecks.checkinteger(2, line)
        if line < 0 or line > 12 then
            ffichecks.argerror(2, string.format("invalid line %d, value must be between 0 and 12 (inclusive)", line))
        end
        shape = ffichecks.checkinteger(3, shape)
        if shape < 0 or shape >= MAX_ROOMSHAPES then
            ffichecks.argerror(3, string.format("invalid room shape %d, value must be between 0 and %d (inclusive)", shape, MAX_ROOMSHAPES - 1))
        end
        ffichecks.checkcdata(4, neighbor, "LevelGeneratorRoom")

        local index = ffi.new("int[1]")
        local err = repentogon.L_LevelGenerator_PlaceRoom(self, column, line, shape, neighbor, index)
        if err ~= nil then
            error(ffi.string(err), 2)
        end
        if index[0] < 0 then
            return nil
        end
        return index[0]
    end,
}

setmetatable(LevelGeneratorMT, { __index = function() end })
LevelGeneratorMT.__index = LevelGeneratorMT

ffi.metatype("struct LevelGenerator", LevelGeneratorMT)

LevelGenerator = setmetatable({}, {
    __class = LevelGeneratorMT,
})
