ffi.cdef[[
    struct GridEntityWall {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityWall* GridEntityWallPtr;
    
    void L_GridEntityWall_Render(struct GridEntityWall*, struct Vector);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityWallMT
GridEntityWallMT = {
    __type = "GridEntityWall",
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityWall_Render(self, offset)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityWallMT.__index = function(self, key)
    if GridEntityWallMT[key] ~= nil then
        return GridEntityWallMT[key]
    end
    return baseIndex(self, key)
end

GridEntityWallMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityWallT = ffi.metatype("struct GridEntityWall", GridEntityWallMT)
GridEntityWall = setmetatable({}, {__class = GridEntityWallMT})