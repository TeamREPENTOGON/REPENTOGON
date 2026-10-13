---@class REPENTOGON.SerializedTable
---@field _data table -- connects a string key to plain data
---@field _links table -- connects a string key to a table id
---@field _specialKeyData table -- connects a special key (number and boolean) to plain data
---@field _specialKeyLinks table -- connects a special key (number and boolean) to a table id

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

-- the json library does not properly restore these types of json keys
local SPECIAL_JSON_KEYS = {
    ["boolean"] = true,
    ["number"] = true,
}

---@param tbl table
---@param memoTable table<table, table>
---@return table
local function deep_copy(tbl, memoTable)
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
            copy[key] = deep_copy(value, memoTable)
        else
            copy[key] = value
        end
        ::continue::
    end

    return copy
end

---@param tbl table
---@return table
local function DeepCopy(tbl)
    return deep_copy(tbl, {})
end

---@param tbl table
---@param saveData REPENTOGON.SerializedTable
---@param memoTable table<table, integer>
---@return integer
local function serialize_table(tbl, saveData, memoTable)
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

    table.insert(tables, plainData)
    table.insert(saveData._links, links)
    table.insert(saveData._specialKeyData, specialKeyPlainData)
    table.insert(saveData._specialKeyLinks, specialKeyLinks)

    for key, value in pairs(tbl) do
        local keyType = type(key)
        local valueType = type(value)

        if NON_SERIALIZABLE_KEYS[keyType] then
            goto continue
        end

        if NON_SERIALIZABLE_VALUES[valueType] then
            goto continue
        end

        local dataTable = plainData
        local specialKey = SPECIAL_JSON_KEYS[keyType] ~= nil

        if valueType == "table" then
            value = serialize_table(value, saveData, memoTable)
            if specialKey then
                dataTable = specialKeyLinks
            else
                dataTable = links
            end
        elseif specialKey then
            dataTable = specialKeyPlainData
        end

        if specialKey then
            table.insert(dataTable, {key, value})
        else
            dataTable[key] = value
        end
        ::continue::
    end

    return id
end

---@param tbl table
---@return REPENTOGON.SerializedTable
local function Serialize(tbl)
    ---@type REPENTOGON.SerializedTable
    local serializedTable = {
        _data = {},
        _specialKeyData = {},
        _links = {},
        _specialKeyLinks = {},
    }

    serialize_table(tbl, serializedTable, {})
    return serializedTable
end

---@param serializeTable REPENTOGON.SerializedTable
local function assert_serialized_table_size(serializeTable)
    local size = #serializeTable._data
    if size == #serializeTable._specialKeyData and
        size == #serializeTable._links and
        size == #serializeTable._specialKeyLinks then
            return true
    end

    return false
end

---@param serializeTable REPENTOGON.SerializedTable
---@return boolean ok
---@return table | string -- table or error
local function Deserialize(serializeTable)
    if not assert_serialized_table_size(serializeTable) then
        return false, "table is corrupted (sizes don't match)"
    end

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

    return true, tables[1]
end

---@class REPENTOGON.Scripts.TableUtils
local Module = {}

Module.DeepCopy = DeepCopy
Module.Serialize = Serialize
Module.Deserialize = Deserialize

return Module