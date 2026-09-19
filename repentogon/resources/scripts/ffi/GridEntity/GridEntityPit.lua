ffi.cdef[[
    struct GridEntityPit {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        bool HasLadder;
    };
    typedef struct GridEntityPit* GridEntityPitPtr;
    
    void L_GridEntityPit_MakeBridge(struct GridEntityPit*, struct GridEntity*);
    void L_GridEntityPit_PostInit(struct GridEntityPit*);
    void L_GridEntityPit_Render(struct GridEntityPit*, struct Vector);
    void L_GridEntityPit_Update(struct GridEntityPit*);
    void L_GridEntityPit_UpdateCollision(struct GridEntityPit*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityPitMT
GridEntityPitMT = {
    __type = "GridEntityPit",
    MakeBridge = function(self, parent)
        parent = ffichecks.optcdata(1, parent, nil)
        repentogon.L_GridEntityPit_MakeBridge(self, parent)
    end,
    PostInit = function(self)
        repentogon.L_GridEntityPit_PostInit(self)
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityPit_Render(self, offset)
    end,
    SetLadder = function(self, value)
        ffichecks.checkboolean(1, value)
        self.HasLadder = value
        repentogon.L_GridEntityPit_UpdateCollision(self)
    end,
    Update = function(self)
        repentogon.L_GridEntityPit_Update(self)
    end,
    UpdateCollision = function(self, value)
        repentogon.L_GridEntityPit_UpdateCollision(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityPitMT.__index = function(self, key)
    if GridEntityPitMT[key] ~= nil then
        return GridEntityPitMT[key]
    end
    return baseIndex(self, key)
end

GridEntityPitMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityPitT = ffi.metatype("struct GridEntityPit", GridEntityPitMT)
GridEntityPit = setmetatable({}, {__class = GridEntityPitMT})