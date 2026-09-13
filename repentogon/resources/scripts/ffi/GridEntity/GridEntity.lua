ffi.cdef [[
    struct GridEntity {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
    };

    typedef struct GridEntity* GridEntityPtr;

    bool L_GridEntity_Destroy(struct GridEntity*, int, struct EntityRef*);
    void L_GridEntity_GetPosition(struct GridEntity*, struct Vector*);
    void L_GridEntity_GetRenderPosition(struct GridEntity*, struct Vector*);
    unsigned int L_GridEntity_GetWaterClipFlags(struct GridEntity*);
    bool L_GridEntity_Hurt(struct GridEntity*, int, struct EntityRef*);
    void L_GridEntity_HurtSurroundings(struct GridEntity*, float, float, float, int, uint64_t, bool);
    void L_GridEntity_Init(struct GridEntity*, unsigned int);
    void L_GridEntity_Render(struct GridEntity*, struct Vector);
    void L_GridEntity_ResetWaterClipFlags(struct GridEntity*);
    void L_GridEntity_SetWaterClipFlags(struct GridEntity*, unsigned int);
]]  

local ffi = ffi
local repentogon = ffidll

local cfuncs = {
    HurtDamage = __Lua_GridEntity_HurtDamage
}

local GridEntityType = {
	GRID_NULL = 0,
	GRID_DECORATION = 1,
	GRID_ROCK = 2,
	GRID_ROCKB = 3,	
	GRID_ROCKT = 4,	
	GRID_ROCK_BOMB = 5,	
	GRID_ROCK_ALT = 6,	
	GRID_PIT = 7,
	GRID_SPIKES = 8,
	GRID_SPIKES_ONOFF = 9,
	GRID_SPIDERWEB = 10,
	GRID_LOCK = 11,
	GRID_TNT = 12,
	GRID_FIREPLACE = 13, -- not used!
	GRID_POOP = 14,
	GRID_WALL = 15,
	GRID_DOOR = 16,
	GRID_TRAPDOOR = 17,
	GRID_STAIRS = 18,
	GRID_GRAVITY = 19,
	GRID_PRESSURE_PLATE = 20,
	GRID_STATUE = 21,
	GRID_ROCK_SS = 22,
	
	-- Repentance
	GRID_TELEPORTER = 23,
	GRID_PILLAR = 24,
	GRID_ROCK_SPIKED = 25,
	GRID_ROCK_ALT2 = 26, -- special skull in Depths 2
	GRID_ROCK_GOLD = 27,
}

local rocks = {
    GridEntityType.GRID_ROCK,
    GridEntityType.GRID_ROCKB,
    GridEntityType.GRID_ROCKT,
    GridEntityType.GRID_ROCK_BOMB,
    GridEntityType.GRID_ROCK_ALT,
    GridEntityType.GRID_ROCK_SS,
    GridEntityType.GRID_PILLAR,
    GridEntityType.GRID_ROCK_SPIKED,
    GridEntityType.GRID_ROCK_ALT2,
    GridEntityType.GRID_ROCK_GOLD,
}

local GridEntityMT
GridEntityMT = {
    __type = "GridEntity",
    Destroy = function(self, immediate)
        immediate = ffichecks.optboolean(immediate, false)
        return repentogon.L_GridEntity_Destroy(ffi.cast("struct GridEntity*", self), immediate, EntityRef())
    end,
    DestroyWithSource = function(self, immediate, source)
        immediate = ffichecks.optboolean(immediate, false)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntity_Destroy(ffi.cast("struct GridEntity*", self), immediate, source)
    end,
    GetGridIndex = function(self)
        return ffi.getprivate(self, "GridIndex")
    end,
    GetRenderPosition = function(self)
        local ret = Vector()
        repentogon.L_GridEntity_GetRenderPosition(ffi.cast("struct GridEntity*", self), ret)
        return ret
    end,
    GetRNG = function(self)
        return ffi.getprivate(self, "RNG")
    end,
    GetSaveState = function(self)
        return self.Desc
    end,
    GetSprite = function(self)
        return ffi.getprivate(self, "Sprite")
    end,
    GetType = function(self)
        return self.Desc.Type
    end,
    GetVariant = function(self)
        return self.Desc.Variant
    end,
    GetWaterClipFlags = function(self)
        return repentogon.L_GridEntity_GetWaterClipFlags(ffi.cast("struct GridEntity*", self))
    end,
    Hurt = function(self, damage)
        ffichecks.checkinteger(1, damage)
        return repentogon.L_GridEntity_Hurt(ffi.cast("struct GridEntity*", self), damage, EntityRef())
    end,
    HurtDamage = function(self, ent, playerDamage, damageFlags, damage, ignoreGridCollision)
        cfuncs.HurtDamage(ffi.cast("struct GridEntity*", self), ent, playerDamage, damageFlags, damage, ignoreGridCollision)
    end,
    HurtSurroundings = function(self, enemyDistance, playerDistance, enemyDamage, playerDamage, damageFlags, ignoreGridCol)
        ffichecks.checknumber(1, enemyDistance)
        ffichecks.checknumber(2, playerDistance)
        ffichecks.checknumber(3, enemyDamage)
        ffichecks.checkinteger(4, playerDamage)
        ffichecks.checkinteger(5, damageFlags)
        ffichecks.checkboolean(6, ignoreGridCol)
        repentogon.L_GridEntity_HurtSurroundings(ffi.cast("struct GridEntity*", self), enemyDistance, playerDistance, enemyDamage, playerDamage, damageFlags, ignoreGridCol)
    end,
    HurtWithSource = function(self, damage, source)
        ffichecks.checkinteger(1, damage)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntity_Hurt(ffi.cast("struct GridEntity*", self), damage, source)
    end,
    Init = function(self, seed)
        ffichecks.checkinteger(1, seed)
        repentogon.L_GridEntity_Init(ffi.cast("struct GridEntity*", self), seed)
    end,
    IsBreakableRock = function(self)
        for i, rock in ipairs(rocks) do
            if self.Desc.Type == rock then 
                return true
            end
        end
        return false
    end,
    PostInit = function(self)
        return
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntity_Render(ffi.cast("struct GridEntity*", self), offset)
    end,
    ResetWaterClipFlags = function(self)
        repentogon.L_GridEntity_ResetWaterClipFlags(ffi.cast("struct GridEntity*", self))
    end,
    SetType = function(self, newType)
        ffichecks.checkinteger(1, newType)
        self.Desc.Type = newType
    end,
    SetVariant = function(self, variant)
        ffichecks.checkinteger(1, variant)
        self.Desc.Variant = variant
    end,
    SetWaterClipFlags = function(self, flags)
        ffichecks.checkinteger(1, flags)
        repentogon.L_GridEntity_SetWaterClipFlags(ffi.cast("struct GridEntity*", self), flags)
    end,
    ToDecoration = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_DECORATION then return nil end
        return ffi.cast("struct GridEntityDecoration*", self)
    end,
    ToDoor = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_DOOR then return nil end
        return ffi.cast("struct GridEntityDoor*", self)
    end,
    ToFire = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_FIRE then return nil end
        return ffi.cast("struct GridEntityFire*", self)
    end,
    ToGravity = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_GRAVITY then return nil end
        return ffi.cast("struct GridEntityGravity*", self)
    end,    
    ToLock = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_LOCK then return nil end
        return ffi.cast("struct GridEntityLock*", self)
    end,
    ToPit = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_PIT then return nil end
        return ffi.cast("struct GridEntityPit*", self)
    end,
    ToPoop = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_POOP then return nil end
        return ffi.cast("struct GridEntityPoop*", self)
    end,
    ToPressurePlate = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_PRESSURE_PLATE then return nil end
        return ffi.cast("struct GridEntityPressurePlate*", self)
    end,
    ToRock = function(self)
        for i, rock in ipairs(rocks) do
            if self.Desc.Type == rock then 
                return ffi.cast("struct GridEntityRock*", self)
            end
        end   
    end,
    ToSpikes = function(self)
        if self.Desc.Type == GridEntityType.GRID_SPIKES or self.Desc.Type == GridEntityType.GRID_SPIKES_ONOFF then 
            return ffi.cast("struct GridEntitySpikes*", self)
        end
    end,
    ToStairs = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_STAIRS then return nil end
        return ffi.cast("struct GridEntityStairs*", self)
    end,
    ToStatue = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_STATUE then return nil end
        return ffi.cast("struct GridEntityStatue*", self)
    end,
    ToTeleporter = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TELEPORTER then return nil end
        return ffi.cast("struct GridEntityTeleporter*", self)
    end,
    ToTNT = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TNT then return nil end
        return ffi.cast("struct GridEntityTNT*", self)
    end,
    ToTrapDoor = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TRAPDOOR then return nil end
        return ffi.cast("struct GridEntityTrapDoor*", self)
    end,
    ToWall = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_WALL then return nil end
        return ffi.cast("struct GridEntityWall*", self)
    end,
    ToWeb = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_SPIDERWEB then return nil end
        return ffi.cast("struct GridEntityWeb*", self)
    end,
    Update = function(self)
        return
    end,
}

GridEntityMT.__index = function(self, key)
    if key == "Position" then
        local ret = Vector()
        repentogon.L_GridEntity_GetPosition(ffi.cast("struct GridEntity*", self), ret)
        return ret
    end
    if key == "State" then
        return self.Desc.State
    end 
    if key == "VarData" then
        return self.Desc.VarData
    end
    return GridEntityMT[key]
end

GridEntityMT.__newindex = function(self, key, value)
    if key == "State" then
        ffichecks.checkinteger(1, value)
        self.Desc.State = value
    end
    if key == "VarData" then
        ffichecks.checkinteger(1, value)
        self.Desc.VarData = value
    end
end

local GridEntityT = ffi.metatype("struct GridEntity", GridEntityMT)
GridEntity = setmetatable({}, {__class = GridEntityMT})

__Lua_GridEntity_HurtDamage = nil