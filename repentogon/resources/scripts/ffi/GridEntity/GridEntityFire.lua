ffi.cdef[[
    struct GridEntityFire {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };
    typedef struct GridEntityFire* GridEntityFirePtr;
    
    bool L_GridEntityFire_Hurt(struct GridEntityFire*, int, struct EntityRef*);
    void L_GridEntityFire_Render(struct GridEntityFire*, struct Vector);
    void L_GridEntityFire_Update(struct GridEntityFire*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityFireMT
GridEntityFireMT = {
    __type = "GridEntityFire",
    Destroy = function(self)
        return repentogon.L_GridEntityFire_Hurt(self, 4, EntityRef())
    end,
    DestroyWithSource = function(self, _, source)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntityFire_Hurt(self, 4, source)
    end,
    Hurt = function(self, damage)
        ffichecks.checkinteger(1, damage)
        return repentogon.L_GridEntityFire_Hurt(self, damage, EntityRef())
    end,
    HurtWithSource = function(self, damage, source)
        ffichecks.checkinteger(1, damage)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntityFire_Hurt(self, damage, source)
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityFire_Render(self, offset)
    end,
    Update = function(self)
        repentogon.L_GridEntityFire_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityFireMT.__index = function(self, key)
    if GridEntityFireMT[key] ~= nil then
        return GridEntityFireMT[key]
    end
    return baseIndex(self, key)
end

GridEntityFireMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityFireT = ffi.metatype("struct GridEntityFire", GridEntityFireMT)
GridEntityFire = setmetatable({}, {__class = GridEntityFire})