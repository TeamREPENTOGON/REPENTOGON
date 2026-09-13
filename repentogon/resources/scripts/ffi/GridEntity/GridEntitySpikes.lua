ffi.cdef[[
    struct GridEntitySpikes {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        int Timeout;
    };
    typedef struct GridEntitySpikes* GridEntitySpikesPtr;
    
    void L_GridEntitySpikes_Update(struct GridEntitySpikes*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntitySpikesMT
GridEntitySpikesMT = {
    __type = "GridEntitySpikes",
    Update = function(self)
        repentogon.L_GridEntitySpikes_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntitySpikesMT.__index = function(self, key)
    if GridEntitySpikesMT[key] ~= nil then
        return GridEntitySpikesMT[key]
    end
    return baseIndex(self, key)
end

GridEntitySpikesMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntitySpikesT = ffi.metatype("struct GridEntitySpikes", GridEntitySpikesMT)
GridEntitySpikes = setmetatable({}, {__class = GridEntitySpikesMT})