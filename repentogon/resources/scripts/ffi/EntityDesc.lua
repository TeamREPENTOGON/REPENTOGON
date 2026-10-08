ffi.cdef [[
    struct EntityDesc {
        private unsigned int Type;
        private unsigned int Variant;
        private unsigned int Subtype;
        private int ChampionId;
        private float Health;
        private float MaxHealth;
        private bool PlayerControlled;
    } : 0x1c;

    typedef struct EntityDesc* EntityDescPtr;
]]

local repentogon = ffidll
local ffi = ffi

local function CheckChampionId(arg, id) 
    if id < -1 or id > 25 then
        ffichecks.argerror(arg, "ChampionId must be between -1 and 25", 3)
    end
    return id
end

local function CheckEntityType(arg, entityType) 
    if entityType < 0 or (entityType >= 1 and entityType <= 9) or entityType >= 1000 then
        ffichecks.argerror(arg, string.format("Invalid Entity Type %d", entityType), 3)
    end
    return entityType
end

local EntityDescMT
EntityDescMT = {
    __type = "EntityDesc",
    GetChampionId = function(self)
        return ffi.getprivate(self, "ChampionId")
    end,
    GetHealth = function(self)
        return ffi.getprivate(self, "Health")
    end,
    GetMaxHealth = function(self)
        return ffi.getprivate(self, "MaxHealth")
    end,
    GetSubtype = function(self)
        return ffi.getprivate(self, "Subtype")
    end,
    GetType = function(self)
        return ffi.getprivate(self, "Type")
    end,
    GetVariant = function(self)
        return ffi.getprivate(self, "Variant")
    end,
    IsPlayerControlled = function(self)
        return ffi.getprivate(self, "PlayerControlled")
    end,
    SetChampionId = function(self, id)
        id = ffichecks.checkinteger(1, id)
        ffi.setprivate(self, "ChampionId", CheckChampionId(1, id))
    end,
    SetHealth = function(self, health)
        health = ffichecks.checknumber(1, health)
        ffi.setprivate(self, "Health", health)
    end,
    SetMaxHealth = function(self, maxHealth)
        maxHealth = ffichecks.checknumber(1, maxHealth)
        ffi.setprivate(self, "MaxHealth", maxHealth)
    end,
    SetPlayerControlled = function(self, playerControlled)
        playerControlled = ffichecks.checkboolean(1, playerControlled)
        ffi.setprivate(self, "PlayerControlled", playerControlled)
    end,
    SetSubtype = function(self, subtype)
        subtype = ffichecks.checkinteger(1, subtype)
        ffi.setprivate(self, "Subtype", subtype)
    end,    
    SetType = function(self, entityType)
        entityType = ffichecks.checkinteger(1, entityType)
        ffi.setprivate(self, "Type", CheckEntityType(1, entityType))
    end,
    SetVariant = function(self, variant)
        variant = ffichecks.checkinteger(1, variant)
        ffi.setprivate(self, "Variant", variant)
    end,
}

setmetatable(EntityDescMT, { __index = function() end })
EntityDescMT.__index = EntityDescMT

local EntityDescT = ffi.metatype("struct EntityDesc", EntityDescMT)

EntityDesc = setmetatable({}, {
    __call = function(_, entityType, variant, subtype, championId, health, maxHealth, playerControlled) 
        local desc = EntityDescT(CheckEntityType(1, entityType or 0), variant or 0, subtype or 0, CheckChampionId(4, championId or -1), health or 0, maxHealth or 0, playerControlled or false) 
        return desc 
    end,
    __class = EntityDescMT,
})