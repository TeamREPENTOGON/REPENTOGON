local Entity = require("ffi.Entity.Entity")
local NPC = require("ffi.Entity.EntityNPC")

ffi.cdef("struct EntityDelirium { " .. Entity.Fields .. NPC.Fields .. " } : 0xf60;\ntypedef struct EntityDelirium* EntityDeliriumPtr;")

ffi.cdef [[
    void L_EntityDelirium_Transform(struct EntityDelirium*, int, int, bool);
]]

local ffi = ffi
local repentogon = ffidll

local helpers = Entity.Helpers
local Getter = helpers.Getter

local TYPE_DELIRIUM = 412
local TYPE_ENTITY_GAPER = 10

local deliriumType = ffi.typeof("struct EntityDelirium*")

local function IntegerSetter(field)
    return function(self, value)
        value = ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, value)
    end
end

local TELEPORTATION_TIMER_MAX = 0x3FF

local getters = {
    AttackID = Getter("DeliriumAttackIdValue"),
    Angle = Getter("DeliriumAttackAngleValue"),
    BossType = Getter("DeliriumBossTypeValue"),
    BossVariant = Getter("DeliriumBossVariantValue"),
    Cycle = Getter("DeliriumCycleValue"),
    RemainingAttacks = Getter("DeliriumRemainingAttacksValue"),
    StateD = Getter("DeliriumStateValue"),
    TransformationTimer = Getter("DeliriumTransformationTimerValue"),
}

local setters = {
    AttackID = IntegerSetter("DeliriumAttackIdValue"),
    Angle = IntegerSetter("DeliriumAttackAngleValue"),
    Cycle = IntegerSetter("DeliriumCycleValue"),
    RemainingAttacks = IntegerSetter("DeliriumRemainingAttacksValue"),
    StateD = IntegerSetter("DeliriumStateValue"),
    TransformationTimer = IntegerSetter("DeliriumTransformationTimerValue"),
}

local methods = {
    -- Delirium is red if any bit between 7 and 14 (inclusive) is 1
    IsRedMode = function(self)
        return ((ffi.getprivate(self, "DeliriumCycleValue") >> 7) & 0xFF) ~= 0
    end,
    GetTeleportationTimer = function(self)
        return (ffi.getprivate(self, "DeliriumCycleValue") >> 0xF) & TELEPORTATION_TIMER_MAX
    end,
    SetRedMode = function(self, on)
        local cycle = ffi.getprivate(self, "DeliriumCycleValue")
        if on then
            cycle = cycle | 0x00007F80
        else
            cycle = cycle & ~0x00007F80
        end
        ffi.setprivate(self, "DeliriumCycleValue", cycle)
    end,
    SetTeleportationTimer = function(self, timer)
        timer = ffichecks.checkinteger(1, timer)
        if timer < 0 then
            error(string.format("Invalid transformation timer %d (positive number required)\n", timer), 2)
        elseif timer > TELEPORTATION_TIMER_MAX then
            error(string.format("Invalid transformation timer %d (max value %d)\n", timer, TELEPORTATION_TIMER_MAX), 2)
        end

        -- The timer is bits 15 to 24 of the cycle
        local cycle = ffi.getprivate(self, "DeliriumCycleValue") & 0xFE007FFF
        ffi.setprivate(self, "DeliriumCycleValue", cycle | (timer << 0xF))
    end,
    Transform = function(self, entityType, variant, callback)
        entityType = ffichecks.checkinteger(1, entityType)
        if entityType < TYPE_ENTITY_GAPER then
            error(string.format("Invalid EntityType %d for Delirium\n", entityType), 2)
        end
        if variant == nil then
            variant = 0
        else
            variant = ffichecks.checkinteger(2, variant)
        end
        repentogon.L_EntityDelirium_Transform(self, entityType, variant, ffichecks.optboolean(callback, false))
    end,
}

local DeliriumMT = Entity.Inherit("EntityDelirium", methods, getters, setters, NPC.Class)
ffi.metatype("struct EntityDelirium", DeliriumMT)

Entity.Methods.ToDelirium = function(self)
    local entityType = ffi.getprivate(self, "TypeValue")
    if entityType == TYPE_DELIRIUM then
        local result = ffi.cast(deliriumType, self) return result
    end

    local npc = self:ToNPC()
    if npc ~= nil and ffi.getprivate(npc, "DeliriumBossTypeValue") == entityType
        and ffi.getprivate(npc, "DeliriumBossVariantValue") == ffi.getprivate(npc, "VariantValue") then
        local result = ffi.cast(deliriumType, npc) return result
    end
    return nil
end
