ffi.cdef[[
    struct GridEntityLock {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityLock* GridEntityLockPtr;
    
    void L_GridEntityLock_Render(struct GridEntityLock*, struct Vector);
    void L_GridEntityLock_Update(struct GridEntityLock*);
    void L_GridEntityLock_TryUnlock(struct GridEntityLock*, struct EntityPlayer*, bool);
]]
local ffi = ffi
local repentogon = ffidll


local GridEntityLockMT
GridEntityLockMT = {
    __type = "GridEntityLock",
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityLock_Render(self, offset)
    end,
    Update = function(self)
        repentogon.L_GridEntityLock_Update(self)
    end,
    TryUnlock = function(self, player, force)
        ffichecks.checkcdata(1, player, "EntityPlayer")
        repentogon.L_GridEntityLock_TryUnlock(self, player, ffichecks.checkboolean(2, force))
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityLockMT.__index = function(self, key)
    if GridEntityLockMT[key] ~= nil then
        return GridEntityLockMT[key]
    end
    return baseIndex(self, key)
end

GridEntityLockMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityLockT = ffi.metatype("struct GridEntityLock", GridEntityLockMT)
GridEntityLock = setmetatable({}, {__class = GridEntityLockMT})

