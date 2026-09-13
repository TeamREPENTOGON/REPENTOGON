ffi.cdef[[
    struct GridEntityStairs {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityStairs* GridEntityStairsPtr;
    
    void L_GridEntityStairs_Update(struct GridEntityStairs*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityStairsMT
GridEntityStairsMT = {
    __type = "GridEntityStairs",
    Update = function(self)
        repentogon.L_GridEntityStairs_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityStairsMT.__index = function(self, key)
    if GridEntityStairsMT[key] ~= nil then
        return GridEntityStairsMT[key]
    end
    return baseIndex(self, key)
end

GridEntityStairsMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityStairsT = ffi.metatype("struct GridEntityStairs", GridEntityStairsMT)
GridEntityStairs = setmetatable({}, {__class = GridEntityStairsMT})