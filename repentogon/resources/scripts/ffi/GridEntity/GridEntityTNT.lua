ffi.cdef[[
    struct GridEntityTNT {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        struct Sprite Sprite;
        int FrameCnt;
    };
    typedef struct GridEntityTNT* GridEntityTNTPtr;
    
    bool L_GridEntityTNT_Destroy(struct GridEntityTNT*, int, struct EntityRef*);
    bool L_GridEntityTNT_Hurt(struct GridEntityTNT*, int, struct EntityRef*);
    void L_GridEntityTNT_Update(struct GridEntityTNT*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityTNTMT
GridEntityTNTMT = {
    __type = "GridEntityTNT",
    Destroy = function(self, immediate)
        immediate = ffichecks.checkboolean(1, immediate)
        local result = repentogon.L_GridEntityTNT_Destroy(self, immediate, EntityRef()) return result
    end,
    DestroyWithSource = function(self, immediate, source)
        immediate = ffichecks.checkboolean(1, immediate)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_GridEntityTNT_Destroy(self, immediate, source) return result
    end,
    Hurt = function(self, damage)
        damage = ffichecks.checkinteger(1, damage)
        local result = repentogon.L_GridEntityTNT_Hurt(self, damage, EntityRef()) return result
    end,
    HurtWithSource = function(self, damage, source)
        damage = ffichecks.checkinteger(1, damage)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_GridEntityTNT_Hurt(self, damage, source) return result
    end,
    Update = function(self)
        repentogon.L_GridEntityTNT_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityTNTMT.__index = function(self, key)
    if GridEntityTNTMT[key] ~= nil then
        return GridEntityTNTMT[key]
    end
    return baseIndex(self, key)
end

GridEntityTNTMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityTNTT = ffi.metatype("struct GridEntityTNT", GridEntityTNTMT)
GridEntityTNT = setmetatable({}, {__class = GridEntityTNT})