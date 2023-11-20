# Class "ProceduralEffect"

## Functions
### Get·Action·Property () {: aria-label='Functions' }
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
### Get·Action·Type () {: aria-label='Functions' }
#### [ProceduralEffectActionType](enums/ProceduralEffectActionType.md) GetActionType ( ) {: .copyable aria-label='Functions' }
Returns what to do after the effect is triggered.

___
### Get·Condition·Property () {: aria-label='Functions' }
#### table GetConditionProperty ( ) {: .copyable aria-label='Functions' }
Returns a table that describes the condition argument.

When `GetConditionType` returns `ENTITY_SPAWN`, the returned table has the following fields.

|Field|Type|
|:--|:--|
| type |  int |
| variant |  int |

___
### Get·Condition·Type () {: aria-label='Functions' }
#### [ProceduralEffectConditionType](enums/ProceduralEffectConditionType.md) GetConditionType ( ) {: .copyable aria-label='Functions' }
Returns the timing when the effect was triggered.

___
### Get·Trigger·Chance () {: aria-label='Functions' }
#### float GetTriggerChance ( ) {: .copyable aria-label='Functions' }

Value form 0 to 1.
___
