ffi.cdef[[
    struct GridEntityDecoration {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityDecoration* GridEntityDecorationPtr;
    
    void L_GridEntityDecoration_Render(struct GridEntityDecoration*, struct Vector);
    void L_GridEntityDecoration_Update(struct GridEntityDecoration*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityDecorationMT
GridEntityDecorationMT = {
    __type = "GridEntityDecoration",
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityDecoration_Render(self, offset)
    end,
    Update = function(self)
        repentogon.L_GridEntityDecoration_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityDecorationMT.__index = function(self, key)
    if GridEntityDecorationMT[key] ~= nil then
        return GridEntityDecorationMT[key]
    end
    return baseIndex(self, key)
end

GridEntityDecorationMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityDecorationT = ffi.metatype("struct GridEntityDecoration", GridEntityDecorationMT)
GridEntityDecoration = setmetatable({}, {__class = GridEntityDecorationMT})