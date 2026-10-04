ffi.cdef [[
    struct ProceduralEffect {
        private int ConditionType : 0x0;
        private int ActionType : 0x4;
        private int16_t SpawnConditionVariant : 0xc;
        private int16_t SpawnConditionType : 0xe;
        private int ActionCollectible : 0x10;
        private int16_t ActionShort0 : 0x10;
        private int16_t ActionShort1 : 0x12;
        private int16_t ActionShort2 : 0x14;
        private int16_t ActionShort3 : 0x16;
        private float FartScale : 0x10;
        private float AreaRadius : 0x18;
        private float AreaDamage : 0x1c;
        private float FartRadius : 0x1c;
        private int64_t AreaFlags1 : 0x20;
        private int64_t AreaFlags2 : 0x28;
        private float TriggerChanceScale : 0x30;
        private float Score : 0x60;
    } : 0x68;
    typedef struct ProceduralEffect* ProceduralEffectPtr;
]]

local ffi = ffi

local CONDITION_TEAR_FIRE = 1
local CONDITION_ENEMY_HIT = 2
local CONDITION_ENEMY_KILL = 3
local CONDITION_ENTITY_SPAWN = 6

local ACTION_USE_ACTIVE_ITEM = 0
local ACTION_ADD_TEMPORARY_EFFECT = 1
local ACTION_CONVERT_ENTITIES = 2
local ACTION_AREA_DAMAGE = 3
local ACTION_SPAWN_ENTITY = 4
local ACTION_FART = 5

local function ToFloat(value)
    return tonumber(ffi.new("float", value))
end

local ProceduralEffectMT
ProceduralEffectMT = {
    __type = "ProceduralEffect",
    GetActionProperty = function(self)
        local actionType = ffi.getprivate(self, "ActionType")
        if actionType == ACTION_USE_ACTIVE_ITEM or actionType == ACTION_ADD_TEMPORARY_EFFECT then
            return { id = ffi.getprivate(self, "ActionCollectible") }
        elseif actionType == ACTION_SPAWN_ENTITY then
            return {
                type = ffi.getprivate(self, "ActionShort1"),
                variant = ffi.getprivate(self, "ActionShort0"),
            }
        elseif actionType == ACTION_CONVERT_ENTITIES then
            return {
                fromType = ffi.getprivate(self, "ActionShort1"),
                fromVariant = ffi.getprivate(self, "ActionShort0"),
                toType = ffi.getprivate(self, "ActionShort3"),
                toVariant = ffi.getprivate(self, "ActionShort2"),
            }
        elseif actionType == ACTION_AREA_DAMAGE then
            return {
                radius = ffi.getprivate(self, "AreaRadius"),
                damage = ffi.getprivate(self, "AreaDamage"),
                flags1 = tonumber(ffi.getprivate(self, "AreaFlags1")),
                flags2 = tonumber(ffi.getprivate(self, "AreaFlags2")),
            }
        elseif actionType == ACTION_FART then
            return {
                scale = ToFloat(ffi.getprivate(self, "FartScale") * 6),
                radius = ToFloat(ToFloat(ffi.getprivate(self, "FartRadius") * 6) / 85),
            }
        end
        return {}
    end,
    GetActionType = function(self)
        return ffi.getprivate(self, "ActionType")
    end,
    GetConditionProperty = function(self)
        if ffi.getprivate(self, "ConditionType") == CONDITION_ENTITY_SPAWN then
            return {
                type = ffi.getprivate(self, "SpawnConditionType"),
                variant = ffi.getprivate(self, "SpawnConditionVariant"),
            }
        end
        return {}
    end,
    GetConditionType = function(self)
        return ffi.getprivate(self, "ConditionType")
    end,
    GetScore = function(self)
        return ffi.getprivate(self, "Score")
    end,
    GetTriggerChance = function(self)
        local chance = ffi.getprivate(self, "TriggerChanceScale")
        local conditionType = ffi.getprivate(self, "ConditionType")
        if conditionType == CONDITION_TEAR_FIRE or conditionType == CONDITION_ENEMY_HIT or conditionType == CONDITION_ENTITY_SPAWN then
            chance = ToFloat(chance * ToFloat(0.05))
        elseif conditionType == CONDITION_ENEMY_KILL then
            chance = ToFloat(chance * ToFloat(0.2))
        end
        return chance
    end,
    GetTriggerChanceScale = function(self)
        return ffi.getprivate(self, "TriggerChanceScale")
    end,
}

setmetatable(ProceduralEffectMT, { __index = function() end })
ProceduralEffectMT.__index = ProceduralEffectMT

ffi.metatype("struct ProceduralEffect", ProceduralEffectMT)

ProceduralEffect = setmetatable({}, {
    __class = ProceduralEffectMT,
})
