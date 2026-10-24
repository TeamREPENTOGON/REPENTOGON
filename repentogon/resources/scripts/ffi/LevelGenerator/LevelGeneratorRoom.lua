ffi.cdef [[
    struct LevelGeneratorRoom {
        private uint32_t GenIndex : 0x4;
        private uint32_t ColIdx : 0x8;
        private uint32_t LineIdx : 0xc;
        private uint32_t RoomShape : 0x18;
        private uint32_t Doors : 0x1c;
        private bool DeadEnd : 0x3c;
    } : 0x48;
    typedef struct LevelGeneratorRoom* LevelGeneratorRoomPtr;

    unsigned int L_LevelGeneratorRoom_GetNeighborCount(struct LevelGeneratorRoom*);
    void L_LevelGeneratorRoom_GetNeighbors(struct LevelGeneratorRoom*, int*);
]]

local repentogon = ffidll
local ffi = ffi

local LevelGeneratorRoomMT
LevelGeneratorRoomMT = {
    __type = "LevelGeneratorRoom",
    Column = function(self)
        local result = ffi.getprivate(self, "ColIdx") return result
    end,
    DoorMask = function(self)
        local result = ffi.getprivate(self, "Doors") return result
    end,
    GenerationIndex = function(self)
        local result = ffi.getprivate(self, "GenIndex") return result
    end,
    IsDeadEnd = function(self)
        local result = ffi.getprivate(self, "DeadEnd") return result
    end,
    Neighbors = function(self)
        local count = repentogon.L_LevelGeneratorRoom_GetNeighborCount(self)
        local neighbors = {}
        if count > 0 then
            local buffer = ffi.new("int[?]", count)
            repentogon.L_LevelGeneratorRoom_GetNeighbors(self, buffer)
            for i = 0, count - 1 do
                neighbors[i + 1] = buffer[i]
            end
        end
        return neighbors
    end,
    Row = function(self)
        local result = ffi.getprivate(self, "LineIdx") return result
    end,
    Shape = function(self)
        local result = ffi.getprivate(self, "RoomShape") return result
    end,
}

setmetatable(LevelGeneratorRoomMT, { __index = function() end })
LevelGeneratorRoomMT.__index = LevelGeneratorRoomMT

ffi.metatype("struct LevelGeneratorRoom", LevelGeneratorRoomMT)

LevelGeneratorRoom = setmetatable({}, {
    __class = LevelGeneratorRoomMT,
})
