---
tags:
  - Global
  - Class
---
# Global Class "EntitySaveStateManager"

???+ info
    You can get this class by using the `EntitySaveStateManager` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local data, exists = EntitySaveStateManager.GetEntityData(modRef, entity)
        ```

???+ info
    All data tracked by this class is saved and restored automatically.

## Functions

### GetEntityData () {: aria-label='Functions' }
#### table, boolean GetEntityData ( [ModReference](https://wofsauge.github.io/IsaacDocs/rep/ModReference.html) mod, [Entity](Entity.md) entity ) {: .copyable aria-label='Functions' }
Returns a Lua table containing mod-related data associated with the entity.
Additionally returns whether or not the table is newly created or pre-existing.

___
### GetEntitySaveStateData () {: aria-label='Functions' }
#### table, boolean GetEntitySaveStateData ( [ModReference](https://wofsauge.github.io/IsaacDocs/rep/ModReference.html) mod, [EntitiesSaveState](EntitiesSaveState.md) saveState ) {: .copyable aria-label='Functions' }
Returns a Lua table containing mod-related data associated with the entity save state.
Additionally returns whether or not the table is newly created or pre-existing.

___
### TryGetEntityData () {: aria-label='Functions' }
#### table? TryGetEntityData ( [ModReference](https://wofsauge.github.io/IsaacDocs/rep/ModReference.html) mod, [Entity](Entity.md) entity ) {: .copyable aria-label='Functions' }
Like `GetEntityData` but only returns a table if it already exists.

___
### TryGetEntitySaveStateData () {: aria-label='Functions' }
#### table? TryGetEntitySaveStateData ( [ModReference](https://wofsauge.github.io/IsaacDocs/rep/ModReference.html) mod, [EntitiesSaveState](EntitiesSaveState.md) saveState ) {: .copyable aria-label='Functions' }
Like `GetEntitySaveStateData` but does only returns a table if it already exists.

___