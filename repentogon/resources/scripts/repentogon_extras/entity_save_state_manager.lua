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

local json = require("json")

local type = type
local pairs = pairs
local error = error
local assert = assert
local pcall = pcall
local tostring = tostring
local getmetatable = getmetatable
local string_format = string.format
local json_encode = json.encode
local json_decode = json.decode
local GetPtrHash = GetPtrHash
local Isaac_DebugString = Isaac.DebugString
local GetModId = _CBindings.GetModId
local GetEntitySaveStateId = _CBindings.ESSM.GetEntitySaveStateId
local SaveEntityData = _CBindings.ESSM.SaveData
local LoadEntityData = _CBindings.ESSM.LoadData
local DeleteEntityData = _CBindings.ESSM.DeleteData

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

---@param entityId integer
local function _OnDeleteEntity(entityId)
    local exists = s_validEntities[entityId] ~= nil
    s_validEntities[entityId] = nil

    if not exists then
        return
    end

    for _, entityTbl in pairs(s_entityData) do
        entityTbl[entityId] = nil
    end
end

---@param entityId integer
local function _OnNewEntity(entityId)
    if s_validEntities[entityId] then
        _OnDeleteEntity(entityId)
    end

    s_validEntities[entityId] = true
end

---@param entityId integer
---@param saveStateId integer
local function _OnStoreEntity(entityId, saveStateId)
    for mod, entityTbl in pairs(s_entityData) do
        local entityData = entityTbl[entityId]
        if entityData then
            s_saveStateData[mod][saveStateId] = deep_copy(entityData)
        end
    end
end

---@param entityId integer
---@param saveStateId integer
local function _OnRestoreEntity(entityId, saveStateId)
    for mod, saveStateTbl in pairs(s_saveStateData) do
        local stateData = saveStateTbl[saveStateId]
        if stateData then
            s_entityData[mod][entityId] = deep_copy(stateData)
        end
    end
end

---@param saveStateIds integer[]
local function _OnClearSaveStates(saveStateIds)
    for _, saveStateTbl in pairs(s_saveStateData) do
        for i = 1, #saveStateIds, 1 do
            local id = saveStateIds[i]
            saveStateTbl[id] = nil
        end
    end
end

---@param sourceIds integer[]
---@param destIds integer[]
local function _OnCopySaveStates(sourceIds, destIds)
    assert(#sourceIds == #destIds)
    for _, saveStateTbl in pairs(s_saveStateData) do
        for i = 1, #sourceIds, 1 do
            local sourceTbl = saveStateTbl[sourceIds[i]]
            local dest = destIds[i]
            if sourceTbl then
                saveStateTbl[dest] = deep_copy(sourceTbl)
            end
        end
    end
end

---@param saveStateTbl table<integer, table>
---@param idMap table<integer, integer>
---@param checksum integer
---@return string data
local function serialize_data(saveStateTbl, idMap, checksum)
    local serializedData = {}
    for serializationId, id in pairs(idMap) do
        local stateTbl = saveStateTbl[id]
        if stateTbl then
            serializedData[serializationId] = deep_copy(stateTbl)
        end
    end

    -- guard against json being bad with just number keys
    serializedData["json_guard"] = true

    local saveData = {
        checksum = checksum,
        data = serializedData,
    }

    local data = json_encode(saveData)
    return data
end

---@param idMap table<integer, integer>
---@param fileName string
---@param checksum integer
local function _Serialize(idMap, fileName, checksum)
    for mod, saveStateTbl in pairs(s_saveStateData) do
        local success, result = pcall(serialize_data, saveStateTbl, idMap, checksum)
        if not success then
            Isaac_DebugString(string_format("[ERROR] [ESSM] Unable to save data for Mod '%s' in '%s': %s", mod.Name, fileName, result))
            DeleteEntityData(mod, fileName)
        else
            SaveEntityData(mod, fileName, result)
        end
    end
end

---@param serializedIds integer[]
---@param destIds integer[]
---@param encodedData string
---@param checksum integer
---@param outData table
---@return boolean, string
local function deserialize_data(serializedIds, destIds, encodedData, checksum, outData)
    local success, decodedData = pcall(json_decode, encodedData)
    if not success then
        return false, string_format("Unable to decode data: %s", decodedData)
    end

    if type(decodedData) ~= "table" then
        return false, "decoded data is not a table"
    end

    local tblChecksum = decodedData.checksum
    if not tblChecksum or tblChecksum ~= checksum then
        return false, "checksum mismatch"
    end

    local data = decodedData.data
    if type(data) ~= "table" then
        return false, "entity data is not a table"
    end

    for i = 1, #serializedIds, 1 do
        local sourceId = tostring(serializedIds[i]) -- thanks json.encode
        local deserialized = data[sourceId]
        if type(deserialized) == "table" then
            local id = destIds[i]
            outData[id] = deep_copy(deserialized)
        end
    end

    return true, "success"
end

---@param serializedIds integer[]
---@param destIds integer[]
---@param fileName string
---@param checksum integer
local function _Deserialize(serializedIds, destIds, fileName, checksum)
    assert(#serializedIds, #destIds)
    for mod, saveStateTbl in pairs(s_saveStateData) do
        local data = LoadEntityData(mod, fileName)

        if not data then
            Isaac_DebugString(string_format("[INFO] [ESSM] Mod '%s' does not have any data for '%s'", mod.Name, fileName))
            goto continue
        end

        local success, err = deserialize_data(serializedIds, destIds, data, checksum, saveStateTbl)
        if not success then
            Isaac_DebugString(string_format("[ERROR] [ESSM] Unable to load data for Mod '%s' in '%s': %s", mod.Name, fileName, tostring(err)))
            DeleteEntityData(mod, fileName)
        end
        ::continue::
    end
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
    s_entityData[mod] = {}
    s_saveStateData[mod] = {}
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
        local metatable = getmetatable(entity)
        local userdataName = metatable.__name or metatable.__type
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