ffi.cdef [[
    struct EntityConfigEntity {
        private int Type : 0x0;
        private int Variant : 0x4;
        private int SubType : 0x8;
        private struct StdString Name : 0xc;
        private float ShadowSize : 0x24;
        private float CollisionDamage : 0x28;
        private bool Boss : 0x2c;
        private int BossID : 0x30;
        private bool Champion : 0x34;
        private float CollisionRadius : 0x38;
        private struct Vector CollisionRadiusMulti : 0x3c;
        private float Mass : 0x44;
        private unsigned int GridCollisionPoints : 0x48;
        private float Friction : 0x4c;
        private float BaseHP : 0x50;
        private float StageHP : 0x54;
        private bool ShutDoors : 0x58;
        private unsigned int GibsAmount : 0x5c;
        private unsigned int GibFlags : 0x60;
        private int Portrait : 0x64;
        private struct StdString Anm2Path : 0x74;
        private bool Reroll : 0xcc;
        private bool FloorAlts : 0xcd;
        private unsigned int CollisionInterval : 0xd0;
        private unsigned int Tags : 0xd8;
        private float ShieldStrength : 0xe0;
        private struct Vector BestiaryOffset : 0xec;
        private float BestiaryScale : 0xf4;
        private struct StdString BestiaryAnm2Path : 0xf8;
        private struct StdString BestiaryAnim : 0x110;
        private struct StdString BestiaryOverlay : 0x128;
        private struct StdString BestiaryFloorAlt : 0x140;
    } : 0x198;
    typedef struct EntityConfigEntity* EntityConfigEntityPtr;

    const char* L_EntityConfigEntity_GetModName(struct EntityConfigEntity*);
    bool L_EntityConfigEntity_HasCustomTag(struct EntityConfigEntity*, const char*);
    int L_EntityConfigEntity_GetCustomTags(struct EntityConfigEntity*, const char**, int);
    struct EntityConfigEntity* L_EntityConfigEntity_GetDevolvedEntity(struct EntityConfigEntity*);
]]

local repentogon = ffidll
local ffi = ffi

local function HasFlags(value, flags)
    if flags <= 0 then
        return false
    end
    return (flags & value) == flags
end

local EntityConfigEntityMT
EntityConfigEntityMT = {
    __type = "EntityConfigEntity",
    CanBeChampion = function(self)
        local result = ffi.getprivate(self, "Champion") return result
    end,
    CanBeRerolledInto = function(self)
        local result = ffi.getprivate(self, "Reroll") return result
    end,
    CanShutDoors = function(self)
        local result = ffi.getprivate(self, "ShutDoors") return result
    end,
    GetAnm2Path = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Anm2Path"))
    end,
    GetBaseHP = function(self)
        local result = ffi.getprivate(self, "BaseHP") return result
    end,
    GetBestiaryAnimation = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BestiaryAnim"))
    end,
    GetBestiaryAnm2Path = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BestiaryAnm2Path"))
    end,
    GetBestiaryFloorAlt = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BestiaryFloorAlt"))
    end,
    GetBestiaryOffset = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "BestiaryOffset"))
    end,
    GetBestiaryOverlay = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BestiaryOverlay"))
    end,
    GetBestiaryScale = function(self)
        local result = ffi.getprivate(self, "BestiaryScale") return result
    end,
    GetBossID = function(self)
        local result = ffi.getprivate(self, "BossID") return result
    end,
    GetCollisionDamage = function(self)
        local result = ffi.getprivate(self, "CollisionDamage") return result
    end,
    GetCollisionInterval = function(self)
        local result = ffi.getprivate(self, "CollisionInterval") return result
    end,
    GetCollisionRadius = function(self)
        local result = ffi.getprivate(self, "CollisionRadius") return result
    end,
    GetCollisionRadiusMultiplier = function(self)
        return ffichecks.copyvector(ffi.getprivate(self, "CollisionRadiusMulti"))
    end,
    GetCustomTags = function(self)
        local count = repentogon.L_EntityConfigEntity_GetCustomTags(self, nil, 0)
        local result = {}
        if count > 0 then
            local buffer = ffi.new("const char*[?]", count)
            repentogon.L_EntityConfigEntity_GetCustomTags(self, buffer, count)
            for i = 0, count - 1 do
                result[i + 1] = ffi.string(buffer[i])
            end
        end
        return result
    end,
    GetDevolvedEntity = function(self)
        local result = repentogon.L_EntityConfigEntity_GetDevolvedEntity(self) return result
    end,
    GetEntityTags = function(self)
        local result = ffi.getprivate(self, "Tags") return result
    end,
    GetFriction = function(self)
        local result = ffi.getprivate(self, "Friction") return result
    end,
    GetGibFlags = function(self)
        local result = ffi.getprivate(self, "GibFlags") return result
    end,
    GetGibsAmount = function(self)
        local result = ffi.getprivate(self, "GibsAmount") return result
    end,
    GetGridCollisionPoints = function(self)
        local result = ffi.getprivate(self, "GridCollisionPoints") return result
    end,
    GetMass = function(self)
        local result = ffi.getprivate(self, "Mass") return result
    end,
    GetModName = function(self)
        local name = repentogon.L_EntityConfigEntity_GetModName(self)
        if name == nil then
            return nil
        end
        local result = ffi.string(name) return result
    end,
    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Name"))
    end,
    GetPortraitID = function(self)
        local result = ffi.getprivate(self, "Portrait") return result
    end,
    GetShadowSize = function(self)
        local result = ffi.getprivate(self, "ShadowSize") return result
    end,
    GetShieldStrength = function(self)
        local result = ffi.getprivate(self, "ShieldStrength") return result
    end,
    GetStageHP = function(self)
        local result = ffi.getprivate(self, "StageHP") return result
    end,
    GetSubType = function(self)
        local result = ffi.getprivate(self, "SubType") return result
    end,
    GetType = function(self)
        local result = ffi.getprivate(self, "Type") return result
    end,
    GetVariant = function(self)
        local result = ffi.getprivate(self, "Variant") return result
    end,
    HasCustomTag = function(self, tag)
        tag = ffichecks.checkstring(1, tag)
        local result = repentogon.L_EntityConfigEntity_HasCustomTag(self, tag) return result
    end,
    HasEntityTags = function(self, tags)
        tags = ffichecks.checkinteger(1, tags)
        return HasFlags(ffi.getprivate(self, "Tags"), tags)
    end,
    HasFloorAlts = function(self)
        local result = ffi.getprivate(self, "FloorAlts") return result
    end,
    HasGibFlags = function(self, flags)
        flags = ffichecks.checkinteger(1, flags)
        return HasFlags(ffi.getprivate(self, "GibFlags"), flags)
    end,
    IsBoss = function(self)
        local result = ffi.getprivate(self, "Boss") return result
    end,
}

setmetatable(EntityConfigEntityMT, { __index = function() end })
EntityConfigEntityMT.__index = EntityConfigEntityMT

ffi.metatype("struct EntityConfigEntity", EntityConfigEntityMT)

EntityConfigEntity = setmetatable({}, {
    __class = EntityConfigEntityMT,
})
