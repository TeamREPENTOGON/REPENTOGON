error("This file should not have been included")

---@class REPENTOGON._CBindings
---@field ESSM REPENTOGON._CBindings.ESSM
---@field RaiseModError fun(modName: string)
---@field GetModId fun(mod: ModReference): string?

---@class REPENTOGON._CBindings.ESSM
---@field GetEntitySaveStateId fun(ess: EntitiesSaveState): integer
---@field SaveData fun(mod: ModReference, fileName: string, data: string)
---@field LoadData fun(mod: ModReference, fileName: string): string?
---@field DeleteData fun(mod: ModReference, fileName: string)

---@type REPENTOGON._CBindings
---@diagnostic disable-next-line: missing-fields
_G._CBindings = {}