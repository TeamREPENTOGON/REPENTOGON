---
tags:
  - Class
---
# Class "ProceduralEffect"

???+ info
    You can get this class by using the following functions:

    * [ProceduralItem.GetEffect()](ProceduralItem.md#geteffect)

    ???+ example "Example Code"
        ```lua
        local pItemEffect = ProceduralItemManager.GetProceduralItem(0):GetEffect(0)
        ```

## Functions
### GetActionProperty () {: aria-label='Functions' }
#### table GetActionProperty ( ) {: .copyable aria-label='Functions' }
Returns a table that describes the action argument.

When `GetActionType` returns `USE_ACTIVE_ITEM`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| id | int | |

When `GetActionType` returns `ADD_TEMPRORY_EFFECT`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| id | int | |

When `GetActionType` returns `SPAWN_ENTITY`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| type | int | |
| variant | int | |

When `GetActionType` returns `CONVERT_ENTITY`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| fromType | int | |
| fromVariant | int | |
| toType | int | target type |
| toVariant | int | target variant |

When `GetActionType` returns `AREA_DAMAGE`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| radius | float | |
| damage | float | |

When `GetActionType` returns `FART`, the returned table has the following fields.

|Field|Type|Comment|
|:--|:--|:--|
| scale | float | |
| radius | float | |

___
### GetActionType () {: aria-label='Functions' }
#### [ProceduralEffectActionType](enums/ProceduralEffectActionType.md) GetActionType ( ) {: .copyable aria-label='Functions' }
Returns what to do after the effect is triggered.

___
### GetConditionProperty () {: aria-label='Functions' }
#### table GetConditionProperty ( ) {: .copyable aria-label='Functions' }
Returns a table that describes the condition argument.

When `GetConditionType` returns `ENTITY_SPAWN`, the returned table has the following fields.

|Field|Type|
|:--|:--|
| type |  int |
| variant |  int |

___
### GetConditionType () {: aria-label='Functions' }
#### [ProceduralEffectConditionType](enums/ProceduralEffectConditionType.md) GetConditionType ( ) {: .copyable aria-label='Functions' }
Returns the timing when the effect was triggered.

___
### GetScore () {: aria-label='Functions' }
#### float GetScore ( ) {: .copyable aria-label='Functions' }

The score is used to generate the `ProceduralItem`. Each `ProceduralItem` has a score limit when generating its effects. If the limit is reached, no more effect will be added.
___
### GetTriggerChance () {: aria-label='Functions' }
#### float GetTriggerChance ( ) {: .copyable aria-label='Functions' }

This is the chance that the game actually uses. In most cases, this value ranges from `0` to `1`. This is the value that the result of `GetTriggerChanceScale` has been applied.

___
### GetTriggerChanceScale () {: aria-label='Functions' }
#### float GetTriggerChanceScale ( ) {: .copyable aria-label='Functions' }

In most cases, this value should be `1`.
___
