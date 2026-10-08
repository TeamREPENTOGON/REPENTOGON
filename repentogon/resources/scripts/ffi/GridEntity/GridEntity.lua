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
    void L_GridEntity_HurtDamage(struct GridEntity*, void*, int, uint64_t, float, bool);
    void L_GridEntity_Init(struct GridEntity*, unsigned int);
    void L_GridEntity_Render(struct GridEntity*, struct Vector);
    void L_GridEntity_ResetWaterClipFlags(struct GridEntity*);
    void L_GridEntity_SetWaterClipFlags(struct GridEntity*, unsigned int);
]]  

local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_GridEntity_Destroy)

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
        local result = repentogon.L_GridEntity_Destroy(ffi.cast("struct GridEntity*", self), immediate, EntityRef()) return result
    end,
    DestroyWithSource = function(self, immediate, source)
        immediate = ffichecks.optboolean(immediate, false)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_GridEntity_Destroy(ffi.cast("struct GridEntity*", self), immediate, source) return result
    end,
    GetGridIndex = function(self)
        local result = ffi.getprivate(self, "GridIndex") return result
    end,
    GetRenderPosition = function(self)
        local ret = Vector()
        repentogon.L_GridEntity_GetRenderPosition(ffi.cast("struct GridEntity*", self), ret)
        return ret
    end,
    GetRNG = function(self)
        local result = ffi.getprivate(self, "RNG") return result
    end,
    GetSaveState = function(self)
        return self.Desc
    end,
    GetSprite = function(self)
        local result = ffi.getprivate(self, "Sprite") return result
    end,
    GetType = function(self)
        return self.Desc.Type
    end,
    GetVariant = function(self)
        return self.Desc.Variant
    end,
    GetWaterClipFlags = function(self)
        local result = repentogon.L_GridEntity_GetWaterClipFlags(ffi.cast("struct GridEntity*", self)) return result
    end,
    Hurt = function(self, damage)
        damage = ffichecks.checkinteger(1, damage)
        local result = repentogon.L_GridEntity_Hurt(ffi.cast("struct GridEntity*", self), damage, EntityRef()) return result
    end,
    HurtDamage = function(self, ent, playerDamage, damageFlags, damage, ignoreGridCollision)
        playerDamage = ffichecks.checkinteger(2, playerDamage)
        damageFlags = ffichecks.checkinteger(3, damageFlags)
        damage = ffichecks.checknumber(4, damage)
        repentogon.L_GridEntity_HurtDamage(ffi.cast("struct GridEntity*", self), ffichecks.checkentity(1, ent), playerDamage, damageFlags, damage, ffichecks.checkboolean(5, ignoreGridCollision))
    end,
    HurtSurroundings = function(self, enemyDistance, playerDistance, enemyDamage, playerDamage, damageFlags, ignoreGridCol)
        enemyDistance = ffichecks.checknumber(1, enemyDistance)
        playerDistance = ffichecks.checknumber(2, playerDistance)
        enemyDamage = ffichecks.checknumber(3, enemyDamage)
        playerDamage = ffichecks.checkinteger(4, playerDamage)
        damageFlags = ffichecks.checkinteger(5, damageFlags)
        ignoreGridCol = ffichecks.checkboolean(6, ignoreGridCol)
        repentogon.L_GridEntity_HurtSurroundings(ffi.cast("struct GridEntity*", self), enemyDistance, playerDistance, enemyDamage, playerDamage, damageFlags, ignoreGridCol)
    end,
    HurtWithSource = function(self, damage, source)
        damage = ffichecks.checkinteger(1, damage)
        ffichecks.checkcdata(2, source, "EntityRef")
        local result = repentogon.L_GridEntity_Hurt(ffi.cast("struct GridEntity*", self), damage, source) return result
    end,
    Init = function(self, seed)
        seed = ffichecks.checkinteger(1, seed)
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
        if self:ToGravity() then
            self:ToGravity():PostInit()
        elseif self:ToPit() then
            self:ToPit():PostInit()
        elseif self:ToPoop() then
            self:ToPoop():PostInit()
        elseif self:ToRock() then
            self:ToRock():PostInit()
        end
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        if self:ToDecoration() then
            self:ToDecoration():Render(offset)
        elseif self:ToDoor() then
            self:ToDoor():Render(offset)
        elseif self:ToFire() then
            self:ToFire():Render(offset)
        elseif self:ToLock() then
            self:ToLock():Render(offset)
        elseif self:ToPit() then
            self:ToPit():Render(offset)
        elseif self:ToPoop() then
            self:ToPoop():Render(offset)
        elseif self:ToPressurePlate() then
            self:ToPressurePlate():Render(offset)
        elseif self:ToRock() then
            self:ToRock():Render(offset)
        elseif self:ToTeleporter() then
            self:ToTeleporter():Render(offset)
        elseif self:ToWall() then
            self:ToWall():Render(offset)
        else
            repentogon.L_GridEntity_Render(ffi.cast("struct GridEntity*", self), offset)
        end
    end,
    ResetWaterClipFlags = function(self)
        repentogon.L_GridEntity_ResetWaterClipFlags(ffi.cast("struct GridEntity*", self))
    end,
    SetType = function(self, newType)
        newType = ffichecks.checkinteger(1, newType)
        self.Desc.Type = newType
    end,
    SetVariant = function(self, variant)
        variant = ffichecks.checkinteger(1, variant)
        self.Desc.Variant = variant
    end,
    SetWaterClipFlags = function(self, flags)
        flags = ffichecks.checkinteger(1, flags)
        repentogon.L_GridEntity_SetWaterClipFlags(ffi.cast("struct GridEntity*", self), flags)
    end,
    ToDecoration = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_DECORATION then return nil end
        local result = ffi.cast("struct GridEntityDecoration*", self) return result
    end,
    ToDoor = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_DOOR then return nil end
        local result = ffi.cast("struct GridEntityDoor*", self) return result
    end,
    ToFire = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_FIRE then return nil end
        local result = ffi.cast("struct GridEntityFire*", self) return result
    end,
    ToGravity = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_GRAVITY then return nil end
        local result = ffi.cast("struct GridEntityGravity*", self) return result
    end,    
    ToLock = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_LOCK then return nil end
        local result = ffi.cast("struct GridEntityLock*", self) return result
    end,
    ToPit = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_PIT then return nil end
        local result = ffi.cast("struct GridEntityPit*", self) return result
    end,
    ToPoop = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_POOP then return nil end
        local result = ffi.cast("struct GridEntityPoop*", self) return result
    end,
    ToPressurePlate = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_PRESSURE_PLATE then return nil end
        local result = ffi.cast("struct GridEntityPressurePlate*", self) return result
    end,
    ToRock = function(self)
        for i, rock in ipairs(rocks) do
            if self.Desc.Type == rock then 
                local result = ffi.cast("struct GridEntityRock*", self) return result
            end
        end   
    end,
    ToSpikes = function(self)
        if self.Desc.Type == GridEntityType.GRID_SPIKES or self.Desc.Type == GridEntityType.GRID_SPIKES_ONOFF then 
            local result = ffi.cast("struct GridEntitySpikes*", self) return result
        end
    end,
    ToStairs = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_STAIRS then return nil end
        local result = ffi.cast("struct GridEntityStairs*", self) return result
    end,
    ToStatue = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_STATUE then return nil end
        local result = ffi.cast("struct GridEntityStatue*", self) return result
    end,
    ToTeleporter = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TELEPORTER then return nil end
        local result = ffi.cast("struct GridEntityTeleporter*", self) return result
    end,
    ToTNT = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TNT then return nil end
        local result = ffi.cast("struct GridEntityTNT*", self) return result
    end,
    ToTrapDoor = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_TRAPDOOR then return nil end
        local result = ffi.cast("struct GridEntityTrapDoor*", self) return result
    end,
    ToWall = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_WALL then return nil end
        local result = ffi.cast("struct GridEntityWall*", self) return result
    end,
    ToWeb = function(self)
        if self.Desc.Type ~= GridEntityType.GRID_SPIDERWEB then return nil end
        local result = ffi.cast("struct GridEntityWeb*", self) return result
    end,
    Update = function(self)
        -- before you accuse me of being yanderedev, this should be faster than caching the functions in a table with the jit compiler
        if self:ToDecoration() then
            self:ToDecoration():Update()
        elseif self:ToDoor() then
            self:ToDoor():Update()
        elseif self:ToFire() then
            self:ToFire():Update()
        elseif self:ToGravity() then
            self:ToGravity():Update()
        elseif self:ToLock() then
            self:ToLock():Update()
        elseif self:ToPit() then
            self:ToPit():Update()
        elseif self:ToPoop() then
            self:ToPoop():Update()
        elseif self:ToPressurePlate() then
            self:ToPressurePlate():Update()
        elseif self:ToRock() then
            self:ToRock():Update()
        elseif self:ToSpikes() then
            self:ToSpikes():Update()
        elseif self:ToTeleporter() then
            self:ToTeleporter():Update()
        elseif self:ToTrapDoor() then
            self:ToTrapDoor():Update()
        elseif self:Web() then
            self:ToWeb():Update()
        end
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
        value = ffichecks.checkinteger(1, value)
        self.Desc.State = value
    end
    if key == "VarData" then
        value = ffichecks.checkinteger(1, value)
        self.Desc.VarData = value
    end
end

local GridEntityT = ffi.metatype("struct GridEntity", GridEntityMT)
GridEntity = setmetatable({}, {__class = GridEntityMT})
