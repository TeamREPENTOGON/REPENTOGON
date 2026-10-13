---@class REPENTOGON._Internals.SSM.ModData
---@field gameData table
---@field gameStates table[]

---@class REPENTOGON.SSM.SaveData
---@field version integer
---@field checksum integer
---@field gameData REPENTOGON.SerializedTable

--#region Data

---@type table<REPENTOGON.ModId, REPENTOGON._Internals.SSM.ModData>
local s_modData = {}

--#endregion

local MAIN_GAME_STATE = 0
local SAVE_DATA_VERSION = 1

local json = require("json")
local ModManager = _GetModule("repentogon_extras.mod_manager")
local TableUtils = _GetModule("repentogon_extras.table_utils")
local HasModData = _CBindings.HasModData
local SaveModData = _CBindings.SaveModData
local LoadModData = _CBindings.LoadModData
local RemoveModData = _CBindings.RemoveModData
local RemoveAllData = _CBindings.RemoveAllData

---@param gameStateInfo REPENTOGON.GameStateInfo
---@return string
local function get_file_name(gameStateInfo)
    return "SaveStateManager/" .. gameStateInfo.fileName
end

---Tries to initialize mod's gameData and returns.
---@param modId REPENTOGON.ModId
---@return REPENTOGON._Internals.SSM.ModData
local function get_data(modId)
    local modData = s_modData[modId]
    if not modData then
        modData = {gameData = {}, gameStates = {}}
        s_modData[modId] = modData
    end

    return modData
end

---Gets the mod's gameData
---@param mod ModReference
local function GetGameData(mod)
    local modId = ModManager.GetModIdByReference(mod)
    if not modId then
        error(string.format("bad argument #1 (ModReference expected, got %s)", type(mod)), 2)
    end

    local modData = get_data(modId)
    return modData.gameData
end

---Resets the mod's gameData
---@param mod ModReference
local function ClearGameData(mod)
    local modId = ModManager.GetModIdByReference(mod)
    if not modId then
        error(string.format("bad argument #1 (ModReference expected, got %s)", type(mod)), 2)
    end

    local modData = get_data(modId)
    modData.gameData = {}
end

---Copies all mods' gameData to gameState
---@param gameStateId integer
local function StoreGameState(gameStateId)
    for _, modData in pairs(s_modData) do
        modData.gameStates[gameStateId] = TableUtils.DeepCopy(modData.gameData)
    end
end

---Copies all mods' gameState to gameData
---@param gameStateId integer
local function RestoreGameState(gameStateId)
    for _, modData in pairs(s_modData) do
        local state = modData.gameStates[gameStateId] or {}
        modData.gameData = TableUtils.DeepCopy(state)
    end
end

---Clears all mods' gameState
---@param gameStateId integer
local function ClearGameState(gameStateId)
    for _, modData in pairs(s_modData) do
        modData.gameStates[gameStateId] = nil
    end
end

---Serializes mods' main gameState and saves it to a file in the 'data' folder
---@param gameStateInfo REPENTOGON.GameStateInfo
local function SerializeGameState(gameStateInfo)
    for modId, modData in pairs(s_modData) do
        local gameData = modData.gameStates[MAIN_GAME_STATE] or {}
        local serializedData = TableUtils.Serialize(gameData)

        ---@type REPENTOGON.SSM.SaveData
        local saveData = {
            version = SAVE_DATA_VERSION,
            checksum = gameStateInfo.checksum,
            gameData = serializedData
        }

        local modReference = ModManager.GetModReferenceById(modId)
        if not modReference then
            goto continue
        end

        local fileName = get_file_name(gameStateInfo)
        local success = SaveModData(modReference, fileName, json.encode(saveData))
        if not success then
            Isaac.DebugString(string.format("[ERROR] [SSM] Unable to Save mod data for '%s'", tostring(modReference.Name)))
        end
        ::continue::
    end
end

---@param gameStateInfo REPENTOGON.GameStateInfo
---@param serializedData string
---@return boolean ok
---@return table | string
local function deserialize_game_state(gameStateInfo, serializedData)
    local ok, saveData = pcall(json.decode, serializedData)
    if not ok then
        return false, saveData
    end

    if type(saveData) ~= "table" then
        return false, "decoded data is not a table"
    end

    ---@cast saveData REPENTOGON.SSM.SaveData
    if saveData.checksum ~= gameStateInfo.checksum then
        return false, "checksum mismatch"
    end

    local gameData = saveData.gameData
    if type(gameData) ~= "table" then
        return false, "no mod data found"
    end

    local ok, data = TableUtils.Deserialize(gameData)
    if not ok then
        return false, data
    end

    return true, data
end

---Clears each mod's main gameState and tries to load the game state file.
---If successful it loads the data into the main gameState.
---@param gameStateInfo REPENTOGON.GameStateInfo
local function DeserializeGameState(gameStateInfo)
    for modId, modReferences in ModManager.GetModIterator() do
        local modRef = modReferences[1]
        if not modRef then
            goto continue
        end

        local fileName = get_file_name(gameStateInfo)
        local noDataInit = not s_modData[modId] and not HasModData(modRef, fileName)
        if noDataInit then
            goto continue
        end

        local modData = get_data(modId)
        modData.gameStates[MAIN_GAME_STATE] = nil

        local serializedData = LoadModData(modRef, fileName)
        if not serializedData then
            goto continue
        end

        local ok, data = deserialize_game_state(gameStateInfo, serializedData)
        if not ok then
            Isaac.DebugString(string.format("[ERROR] [SSM] Unable to decode mod data for '%s' in '%s': %s", tostring(modRef.Name), fileName, data))
            RemoveModData(modRef, fileName)
            goto continue
        end

        ---@cast data table
        modData.gameStates[MAIN_GAME_STATE] = data
        ::continue::
    end
end

---Delete all mod files for that filePath
---@param filePath string
local function DeleteGameState(filePath)
    RemoveAllData("SaveStateManager/" .. filePath)
end

---Reset game data for all mods
local function CleanupState()
    for _, modData in pairs(s_modData) do
        modData.gameData = {}
    end
end

---@class REPENTOGON.Scripts.SSM
local Module = {}

Module.GetGameData = GetGameData
Module.ClearGameData = ClearGameData
Module._StoreGameState = StoreGameState
Module._RestoreGameState = RestoreGameState
Module._ClearGameState = ClearGameState
Module._SerializeGameState = SerializeGameState
Module._DeserializeGameState = DeserializeGameState
Module._DeleteGameState = DeleteGameState
Module._CleanupState = CleanupState

return Module