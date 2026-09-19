ffi.cdef[[
    struct GridEntityGravity {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityGravity* GridEntityGravityPtr;
    
    void L_GridEntityGravity_PostInit(struct GridEntityGravity*);
    void L_GridEntityGravity_Update(struct GridEntityGravity*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityGravityMT
GridEntityGravityMT = {
    __type = "GridEntityGravity",
    PostInit = function(self)
        repentogon.L_GridEntityGravity_PostInit(self)
    end,
    Update = function(self)
        repentogon.L_GridEntityGravity_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityGravityMT.__index = function(self, key)
    if GridEntityGravityMT[key] ~= nil then
        return GridEntityGravityMT[key]
    end
    return baseIndex(self, key)
end

GridEntityGravityMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityGravityT = ffi.metatype("struct GridEntityGravity", GridEntityGravityMT)
GridEntityGravity = setmetatable({}, {__class = GridEntityGravityMT})