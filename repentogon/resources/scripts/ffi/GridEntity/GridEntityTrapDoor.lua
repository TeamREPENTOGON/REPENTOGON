ffi.cdef[[
    struct GridEntityTrapDoor {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityTrapDoor* GridEntityTrapDoorPtr;
    
    void L_GridEntityTrapDoor_Update(struct GridEntityTrapDoor*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityTrapDoorMT
GridEntityTrapDoorMT = {
    __type = "GridEntityTrapDoor",
    Update = function(self)
        repentogon.L_GridEntityTrapDoor_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityTrapDoorMT.__index = function(self, key)
    if GridEntityTrapDoorMT[key] ~= nil then
        return GridEntityTrapDoorMT[key]
    end
    return baseIndex(self, key)
end

GridEntityTrapDoorMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityTrapDoorT = ffi.metatype("struct GridEntityTrapDoor", GridEntityTrapDoorMT)
GridEntityTrapDoor = setmetatable({}, {__class = GridEntityTrapDoorMT})