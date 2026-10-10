ffi.cdef[[
    struct GridEntityPit {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        struct Sprite Sprite;
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

local GRID_ENTITY_TYPES = {
    "GridEntity", "GridEntityDecoration", "GridEntityDoor", "GridEntityFire", "GridEntityGravity", "GridEntityLock",
    "GridEntityPit", "GridEntityPoop", "GridEntityPressurePlate", "GridEntityRock", "GridEntitySpikes", "GridEntityStairs",
    "GridEntityStatue", "GridEntityTNT", "GridEntityTeleporter", "GridEntityTrapDoor", "GridEntityWall", "GridEntityWeb",
}

local GridEntityPitMT
GridEntityPitMT = {
    __type = "GridEntityPit",
    MakeBridge = function(self, parent)
        -- Like vanilla: nil, or any grid entity.
        if parent ~= nil then
            local valid = false
            for _, ctype in ipairs(GRID_ENTITY_TYPES) do
                if ffichecks.iscdata(parent, ctype) then valid = true; break end
            end
            if not valid then
                ffichecks.argerror(1, "GridEntity expected, got " .. ffichecks.gettype(parent))
            end
            parent = ffi.cast("struct GridEntity*", parent)
        end
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
        value = ffichecks.checkboolean(1, value)
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