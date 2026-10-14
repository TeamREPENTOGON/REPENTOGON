error("This file should not have been included")

---@class REPENTOGON._CBindings
---@field ESSM REPENTOGON._CBindings.ESSM
---@field RaiseModError fun(modName: string)
---@field GetModId fun(mod: ModReference): string?
---@field HasModData fun(mod: ModReference, fileName: string): boolean
---@field SaveModData fun(mod: ModReference, fileName: string, data: string): boolean
---@field LoadModData fun(mod: ModReference, fileName: string): string?
---@field RemoveModData fun(mod: ModReference, fileName: string)
---@field RemoveAllData fun(fileName: string)
---@field ModReference_HasModFile fun(mod: ModReference, fileName: string): boolean
---@field ModReference_SaveModFile fun(mod: ModReference, fileName: string, data: string): boolean
---@field ModReference_LoadModFile fun(mod: ModReference, fileName: string): string?
---@field ModReference_RemoveModFile fun(mod: ModReference, fileName: string)


---@class REPENTOGON._CBindings.ESSM
---@field GetEntitySaveStateId fun(ess: EntitiesSaveState): integer
---@field SaveData fun(mod: ModReference, fileName: string, data: string)
---@field LoadData fun(mod: ModReference, fileName: string): string?
---@field DeleteData fun(mod: ModReference, fileName: string)

---@class REPENTOGON.GameStateInfo
---@field checksum integer
---@field fileName string

---@type REPENTOGON._CBindings
---@diagnostic disable-next-line: missing-fields
_G._CBindings = {}