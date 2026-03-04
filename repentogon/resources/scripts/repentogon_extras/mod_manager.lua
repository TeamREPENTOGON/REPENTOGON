---@alias REPENTOGON.ModId string

--#region Data

---@type table<ModReference, REPENTOGON.ModId>
local s_modIds = {}

---@type table<REPENTOGON.ModId, ModReference[]>
local s_modReferences = {}

--#endregion

local table_insert = table.insert
local table_remove = table.remove
local GetModId = _CBindings.GetModId

---@param modReference ModReference
local function _RegisterMod(modReference)
    local id = GetModId(modReference)
    if not id then
        return
    end

    assert(s_modIds[modReference] == nil, "The same mod has been registered multiple times")
    s_modIds[modReference] = id
    local modReferences = s_modReferences[id]
    if not modReferences then
        modReferences = {}
        s_modReferences[id] = modReferences
    end

    table_insert(modReferences, modReference)
end

---@param modReference ModReference
local function _UnloadMod(modReference)
    local id = s_modIds[modReference]
    if not id then
        return
    end

    s_modIds[modReference] = nil

    local modReferences = s_modReferences[id]
    for i = 1, #modReferences, 1 do
        if modReferences[i] == modReference then
            table_remove(modReferences, i)
            break -- there should never be multiple of the same mod references in the list
        end
    end
end

---@param modReference ModReference
---@return REPENTOGON.ModId?
local function GetModIdByReference(modReference)
    return s_modIds[modReference]
end

---@param modId REPENTOGON.ModId
---@return ModReference?
local function GetModReferenceById(modId)
    local modReferences = s_modReferences[modId]
    if modReferences then
        return modReferences[1]
    end
end

Module = {}
Module.detail = {}

Module.detail.RegisterMod = _RegisterMod
Module.detail.UnloadMod = _UnloadMod
Module.GetModIdByReference = GetModIdByReference
Module.GetModReferenceById = GetModReferenceById

return Module