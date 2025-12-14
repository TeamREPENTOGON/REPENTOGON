---@class REPENTOGON._Internals.ESSM
local Module = {}

--#region Data

---Set of registered mod ids
---@type table<string, true|nil>
local s_registeredMods = {}

---Set of entities PtrHash that exist
---@type table<integer, true|nil>
local s_validEntities = {}

---Persistent mod data for existing entities
---@type table<ModReference, table<integer, table>>
local s_entityData = {}

---Persistent mdo data for entity save states
---@type table<ModReference, table<integer, table>>
local s_saveStateData = {}

--#endregion

local type = type
local pairs = pairs
local string_format = string.format
local GetPtrHash = GetPtrHash
local GetModId = _CBindings.GetModId
local GetEntitySaveStateId = _CBindings.ESSM.GetEntitySaveStateId

---@param tbl table
---@param memoTable table<table, table>
---@return table
local function _deep_copy(tbl, memoTable)
    local memo = memoTable[tbl]
    if memo then
        return memo
    end

    local copy = {}
    memoTable[tbl] = copy

    for key, value in pairs(tbl) do
        local keyType = type(key)
        local valueType = type(value)

        if keyType == "userdata" or keyType == "table" then
            goto continue
        end

        if valueType == "userdata" then
            -- skip
        elseif valueType == "table" then
            copy[key] = _deep_copy(value, memoTable)
        else
            copy[key] = value
        end
        ::continue::
    end

    return copy
end

---@param tbl table
---@return table
local function deep_copy(tbl)
    return _deep_copy(tbl, {})
end

--#region Internals

local function _OnNewEntity()
end

local function _OnDeleteEntity()
end

local function _OnStoreEntity()
end

local function _OnRestoreEntity()
end

local function _OnCopySaveStates()
end

local function _OnClearSaveStates()
end

local function _Serialize()
end

local function _Deserialize()
end

--#endregion

--#region API

---@param mod ModReference
local function RegisterMod(mod)
    local modId = GetModId(mod)
    if not modId then
        error("ModReference expected, got table", 2)
    end

    if s_registeredMods[modId] then
        error("Mod has already been registered", 2)
    end

    s_registeredMods[modId] = true
    s_entityData[modId] = {}
    s_saveStateData[modId] = {}
end

local VALID_ENTITY_USERDATA = {
    ["Entity"] = true,
    ["EntityPlayer"] = true,
    ["EntityFamiliar"] = true,
    ["EntityTear"] = true,
    ["EntityBomb"] = true,
    ["EntityPickup"] = true,
    ["EntitySlot"] = true,
    ["EntityProjectile"] = true,
    ["EntityKnife"] = true,
    ["EntityLaser"] = true,
    ["EntityNPC"] = true,
    ["EntityEffect"] = true,
}

---@param dataTable table
---@param id integer
---@return table
---@return boolean
local function get_data(dataTable, id)
    local entityData = dataTable[id]
    local exists = true

    if not entityData then
        exists = false
        entityData = {}
        dataTable[id] = entityData
    end

    return entityData, exists
end

---@param mod ModReference
---@param entity Entity
---@return table
---@return boolean
local function GetEntityData(mod, entity)
    local entityDataTable = s_entityData[mod]
    if not entityDataTable then
        error("bad argument #1 (expected registered ModReference)", 2)
    end

    local entityArgType = type(entity)
    if entityArgType ~= "userdata" then
        error(string_format("bad argument #2 (expected userdata, got %s)", entityArgType), 2)
    end

    local id = GetPtrHash(entity)
    if not s_validEntities[id] then
        local specificError
        local userdataName = getmetatable(entity).__name
        if not VALID_ENTITY_USERDATA[userdataName] then
            specificError = string_format("got %s", userdataName)
        else
            specificError = "Entity no longer exists"
        end
        error(string_format("bad argument #2 (got invalid Entity, %s)", specificError), 2)
    end

    return get_data(entityDataTable, id)
end

---@param mod ModReference
---@param ess EntitiesSaveState
---@return table
---@return boolean
local function GetEntitySaveStateData(mod, ess)
    local saveStateTable = s_saveStateData[mod]
    if not saveStateTable then
        error("bad argument #1 (expected registered ModReference)", 2)
    end

    local essArgType = type(ess)
    if essArgType ~= "userdata" then
        error(string_format("bad argument #2 (expected userdata, got %s)", essArgType), 2)
    end

    local userdataName = getmetatable(ess).__name
    if userdataName ~= "EntitySaveState" then
        error(string_format("bad argument #2 (expected EntitySaveState, got %s)", userdataName), 2)
    end

    ---@cast ess EntitiesSaveState
    return get_data(mod, GetEntitySaveStateId(ess))
end

--#endregion

Module._OnNewEntity = _OnNewEntity
Module._OnDeleteEntity = _OnDeleteEntity
Module._OnStoreEntity = _OnStoreEntity
Module._OnRestoreEntity = _OnRestoreEntity
Module._OnCopySaveStates = _OnCopySaveStates
Module._OnClearSaveStates = _OnClearSaveStates
Module._Serialize = _Serialize
Module._Deserialize = _Deserialize
Module.RegisterMod = RegisterMod
Module.GetEntityData = GetEntityData
Module.GetEntitySaveStateData = GetEntitySaveStateData

return Module