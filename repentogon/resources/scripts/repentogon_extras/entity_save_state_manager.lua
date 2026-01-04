---@class REPENTOGON._Internals.ESSM
local Module = {}

--#region Data

---@class REPENTOGON._Internals.ESSM.ModLookup
---@field entityData table<integer, table>
---@field saveStateData table<integer, table>

---@class REPENTOGON._Internals.ESSM.SaveData
---@field version integer
---@field checksum integer
---@field data REPENTOGON._Internals.ESSM.SerializedTable

---@class REPENTOGON._Internals.ESSM.SerializedTable
---@field _data table -- connects a string key to plain data
---@field _links table -- connects a string key to a table id
---@field _specialKeyData table -- connects a special key (number and boolean) to plain data
---@field _specialKeyLinks table -- connects a special key (number and boolean) to a table id

local SAVE_DATA_VERSION = 1

---Map of registered mod ids and the ModReference that will be used for save and load
---@type table<string, ModReference>
local s_registeredMods = {}

---Set of entities PtrHash that exist
---@type table<integer, true|nil>
local s_validEntities = {}

---Persistent mod data for existing entities
---@type table<string, table<integer, table>>
local s_entityData = {}

---Persistent mod data for entity save states
---@type table<string, table<integer, table>>
local s_saveStateData = {}

---Lookup for individual ModReference.
---mod references for the same mod point to the same data.
---@type table<ModReference, REPENTOGON._Internals.ESSM.ModLookup>
local s_modLookup = {}

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
local table_insert = table.insert
local json_encode = json.encode
local json_decode = json.decode
local GetPtrHash = GetPtrHash
local Isaac_DebugString = Isaac.DebugString
local GetModId = _CBindings.GetModId
local GetEntitySaveStateId = _CBindings.ESSM.GetEntitySaveStateId
local SaveEntityData = _CBindings.ESSM.SaveData
local LoadEntityData = _CBindings.ESSM.LoadData
local DeleteEntityData = _CBindings.ESSM.DeleteData

local NON_SERIALIZABLE_KEYS = {
    ["userdata"] = true,
    ["table"] = true,
    ["function"] = true,
    ["thread"] = true,
}

local NON_SERIALIZABLE_VALUES = {
    ["userdata"] = true,
    ["function"] = true,
    ["thread"] = true,
}

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

        if NON_SERIALIZABLE_KEYS[keyType] then
            goto continue
        end

        if NON_SERIALIZABLE_VALUES[valueType] then
            goto continue
        end

        if valueType == "table" then
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

---@param mod ModReference
---@param modId string
local function register_mod_id(mod, modId)
    local exists = s_registeredMods[modId] ~= nil
    local entityData
    local saveStateData

    if not exists then
        s_registeredMods[modId] = mod
        entityData = {}
        saveStateData = {}
        s_entityData[modId] = entityData
        s_saveStateData[modId] = saveStateData
    else
        entityData = s_entityData[modId]
        saveStateData = s_saveStateData[modId]
    end

    s_modLookup[mod] = {entityData = entityData, saveStateData = saveStateData}
end

---@param mod table
---@return boolean, string
local function register_mod(mod)
    local varType = type(mod)
    if varType ~= "table" then
        return false, string_format("ModReference expected, got %s", varType)
    end

    local modId = GetModId(mod)
    if not modId then
        return false, "ModReference expected, got table"
    end

    register_mod_id(mod, modId)

    return true, "success"
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
    for modId, entityTbl in pairs(s_entityData) do
        local entityData = entityTbl[entityId]
        if entityData then
            s_saveStateData[modId][saveStateId] = deep_copy(entityData)
        end
    end
end

---@param entityId integer
---@param saveStateId integer
local function _OnRestoreEntity(entityId, saveStateId)
    for modId, saveStateTbl in pairs(s_saveStateData) do
        local stateData = saveStateTbl[saveStateId]
        local data = nil
        if stateData then
            data = deep_copy(stateData)
        end
        s_entityData[modId][entityId] = data
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

---@param tbl table
---@param saveData REPENTOGON._Internals.ESSM.SerializedTable
---@param memoTable table<table, integer>
---@return integer
local function _serialize_table(tbl, saveData, memoTable)
    local memoId = memoTable[tbl]
    if memoId then
        return memoId
    end

    local tables = saveData._data
    local id = #tables + 1
    memoTable[tbl] = id

    local plainData = {}
    local links = {}
    local specialKeyPlainData = {}
    local specialKeyLinks = {}

    table_insert(tables, plainData)
    table_insert(saveData._links, links)
    table_insert(saveData._specialKeyData, specialKeyPlainData)
    table_insert(saveData._specialKeyLinks, specialKeyLinks)

    for key, value in pairs(tbl) do
        local keyType = type(key)
        local valueType = type(value)

        if NON_SERIALIZABLE_KEYS[keyType] then
            goto continue
        end

        if NON_SERIALIZABLE_VALUES[valueType] then
            goto continue
        end

        local dataTable
        local specialKey = false

        if valueType == "table" then
            value = _serialize_table(value, saveData, memoTable)
            if keyType == "number" or keyType == "boolean" then
                dataTable = specialKeyLinks
                specialKey = true
            else
                dataTable = links
            end
        else
            if keyType == "number" or keyType == "boolean" then
                dataTable = specialKeyPlainData
                specialKey = true
            else
                dataTable = plainData
            end
        end

        if specialKey then
            table_insert(dataTable, {key, value})
        else
            dataTable[key] = value
        end
        ::continue::
    end

    return id
end

---@param saveStateTbl table<integer, table>
---@param idMap table<integer, integer>
---@param checksum integer
---@return string data
local function serialize_data(saveStateTbl, idMap, checksum)
    ---@type REPENTOGON._Internals.ESSM.SerializedTable
    local serializedTable = {
        _data = {},
        _specialKeyData = {},
        _links = {},
        _specialKeyLinks = {},
    }

    local remappedData = {}
    for serializationId, id in pairs(idMap) do
        remappedData[serializationId] = saveStateTbl[id]
    end

    _serialize_table(remappedData, serializedTable, {})

    ---@type REPENTOGON._Internals.ESSM.SaveData
    local saveData = {
        version = SAVE_DATA_VERSION,
        checksum = checksum,
        data = serializedTable,
    }
    local data = json_encode(saveData)
    return data
end

---@param idMap table<integer, integer>
---@param fileName string
---@param checksum integer
local function _Serialize(idMap, fileName, checksum)
    for modId, saveStateTbl in pairs(s_saveStateData) do
        local success, result = pcall(serialize_data, saveStateTbl, idMap, checksum)
        local mod = s_registeredMods[modId]
        if not success then
            Isaac_DebugString(string_format("[ERROR] [ESSM] Unable to save data for Mod '%s' in '%s': %s", mod.Name, fileName, result))
            DeleteEntityData(mod, fileName)
        else
            SaveEntityData(mod, fileName, result)
        end
    end
end

-- Register mods forcefully so that the system works as expected, even
-- if a mod has not accessed any data yet.
---@param mods ModReference[]
---@param modIds string[]
local function _PreDeserialize(mods, modIds)
    for i = 1, #mods, 1 do
        local modId = modIds[i]
        if not s_registeredMods[modId] then
            register_mod_id(mods[i], modId)
        end
    end
end

---@param serializeTable REPENTOGON._Internals.ESSM.SerializedTable
local function _assert_serialized_table_size(serializeTable)
    local size = #serializeTable._data
    if size == #serializeTable._specialKeyData and
        size == #serializeTable._links and
        size == #serializeTable._specialKeyLinks then
            return true
    end

    return false
end

---@param serializeTable REPENTOGON._Internals.ESSM.SerializedTable
---@return table
local function _deserialize_table(serializeTable)
    assert(_assert_serialized_table_size(serializeTable), "table is corrupted (sizes don't match)")
    local tables = serializeTable._data

    for i = 1, #tables, 1 do
        local tbl = tables[i]
        local specialTbl = serializeTable._specialKeyData[i]
        local links = serializeTable._links[i]
        local specialLinks = serializeTable._specialKeyLinks[i]

        for j = 1, #specialTbl, 1 do
            local keyVal = specialTbl[j]
            tbl[keyVal[1]] = keyVal[2]
        end

        for k, v in pairs(links) do
            tbl[k] = tables[v]
        end

        for j = 1, #specialLinks, 1 do
            local keyVal = specialLinks[j]
            tbl[keyVal[1]] = tables[keyVal[2]]
        end
    end

    return tables[1]
end

---@param serializedIds integer[]
---@param destIds integer[]
---@param encodedData string
---@param checksum integer
---@param outData table
---@return boolean, string
local function deserialize_data(serializedIds, destIds, encodedData, checksum, outData)
    ---@type boolean, REPENTOGON._Internals.ESSM.SaveData
    local decodeSuccess, decodedData = pcall(json_decode, encodedData)
    if not decodeSuccess then
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
        return false, "decoded data does not have any data"
    end

    local deserializeSuccess, deserializedData = pcall(_deserialize_table, data)
    if not deserializeSuccess then
        return false, string_format("Unable to deserialize data: %s", deserializedData)
    end

    for i = 1, #serializedIds, 1 do
        local sourceId = serializedIds[i]
        local deserialized = deserializedData[sourceId]
        if type(deserialized) == "table" then
            local id = destIds[i]
            outData[id] = deserialized
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
    for modId, saveStateTbl in pairs(s_saveStateData) do
        local mod = s_registeredMods[modId]
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
---@param errLevel integer
---@return table, integer
local function get_entity_params(mod, entity, errLevel)
    local modLookup = s_modLookup[mod]
    if not modLookup then
        local success, err = register_mod(mod)
        modLookup = s_modLookup[mod]

        if not success then
            error(string_format("bad argument #1 (%s)", err), errLevel + 1)
        end
    end

    local entityArgType = type(entity)
    if entityArgType ~= "userdata" then
        error(string_format("bad argument #2 (expected userdata, got %s)", entityArgType), errLevel + 1)
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
        error(string_format("bad argument #2 (got invalid Entity, %s)", specificError), errLevel + 1)
    end

    return modLookup.entityData, id
end

---@param mod ModReference
---@param ess EntitiesSaveState
---@param errLevel integer
---@return table, integer
local function get_entity_save_state_params(mod, ess, errLevel)
    local modLookup = s_modLookup[mod]
    if not modLookup then
        local success, err = register_mod(mod)
        modLookup = s_modLookup[mod]

        if not success then
            error(string_format("bad argument #1 (%s)", err), errLevel + 1)
        end
    end

    local essArgType = type(ess)
    if essArgType ~= "userdata" then
        error(string_format("bad argument #2 (expected userdata, got %s)", essArgType), errLevel + 1)
    end

    local userdataName = getmetatable(ess).__name
    if userdataName ~= "EntitySaveState" then
        error(string_format("bad argument #2 (expected EntitySaveState, got %s)", userdataName), errLevel + 1)
    end

    ---@cast ess EntitiesSaveState
    return modLookup.saveStateData, GetEntitySaveStateId(ess)
end

---@param mod ModReference
---@param entity Entity
---@return table
---@return boolean
local function GetEntityData(mod, entity)
    local dataTable, id = get_entity_params(mod, entity, 2)
    return get_data(dataTable, id)
end

---@param mod ModReference
---@param ess EntitiesSaveState
---@return table
---@return boolean
local function GetEntitySaveStateData(mod, ess)
    local dataTable, id = get_entity_save_state_params(mod, ess, 2)
    return get_data(dataTable, id)
end

---@param mod ModReference
---@param entity Entity
---@return table?
local function TryGetEntityData(mod, entity)
    local dataTable, id = get_entity_params(mod, entity, 2)
    return dataTable[id]
end

---@param mod ModReference
---@param ess EntitiesSaveState
---@return table?
local function TryGetEntitySaveStateData(mod, ess)
    local dataTable, id = get_entity_save_state_params(mod, ess, 2)
    return dataTable[id]
end

--#endregion

Module._OnNewEntity = _OnNewEntity
Module._OnDeleteEntity = _OnDeleteEntity
Module._OnStoreEntity = _OnStoreEntity
Module._OnRestoreEntity = _OnRestoreEntity
Module._OnCopySaveStates = _OnCopySaveStates
Module._OnClearSaveStates = _OnClearSaveStates
Module._Serialize = _Serialize
Module._PreDeserialize = _PreDeserialize
Module._Deserialize = _Deserialize
Module.GetEntityData = GetEntityData
Module.GetEntitySaveStateData = GetEntitySaveStateData
Module.TryGetEntityData = TryGetEntityData
Module.TryGetEntitySaveStateData = TryGetEntitySaveStateData

return Module