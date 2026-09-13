ffi.cdef[[
    struct GridEntityStatue {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityStatue* GridEntityStatuePtr;
    
    void L_GridEntityStatue_Update(struct GridEntityStatue*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityStatueMT
GridEntityStatueMT = {
    __type = "GridEntityStatue",
    Update = function(self)
        repentogon.L_GridEntityStatue_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityStatueMT.__index = function(self, key)
    if GridEntityStatueMT[key] ~= nil then
        return GridEntityStatueMT[key]
    end
    return baseIndex(self, key)
end

GridEntityStatueMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityStatueT = ffi.metatype("struct GridEntityStatue", GridEntityStatueMT)
GridEntityStatue = setmetatable({}, {__class = GridEntityStatueMT})