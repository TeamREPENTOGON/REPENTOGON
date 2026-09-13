ffi.cdef[[
    struct GridEntityWeb {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityWeb* GridEntityWebPtr;
    
    void L_GridEntityWeb_Update(struct GridEntityWeb*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityWebMT
GridEntityWebMT = {
    __type = "GridEntityWeb",
    Update = function(self)
        repentogon.L_GridEntityWeb_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityWebMT.__index = function(self, key)
    if GridEntityWebMT[key] ~= nil then
        return GridEntityWebMT[key]
    end
    return baseIndex(self, key)
end

GridEntityWebMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityWebT = ffi.metatype("struct GridEntityWeb", GridEntityWebMT)
GridEntityWeb = setmetatable({}, {__class = GridEntityWebMT})