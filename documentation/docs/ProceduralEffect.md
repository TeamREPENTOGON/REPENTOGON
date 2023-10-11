# Class "ProceduralEffect"

## Functions
### Get·Condition·Type () {: aria-label='Functions' }
#### [ProceduralEffectConditionType](enums/ProceduralEffectConditionType.md) GetConditionType ( ) {: .copyable aria-label='Functions' }
Returns the timing when the effect was triggered.

___
### Get·Condition·Property () {: aria-label='Functions' }
#### table GetConditionProperty ( ) {: .copyable aria-label='Functions' }
Returns a table that describes the condition argument.

|Field|ActionType|Type|
|:--|:--|:--|
| type | ENTITY_SPAWN |  int |
| variant | ENTITY_SPAWN |  int |

___
### Get·Action·Type () {: aria-label='Functions' }
#### [ProceduralEffectActionType](enums/ProceduralEffectActionType.md) GetActionType ( ) {: .copyable aria-label='Functions' }
Returns what to do after the effect is triggered.

___
### Get·Action·Property () {: aria-label='Functions' }
#### table GetActionProperty ( ) {: .copyable aria-label='Functions' }
Returns a table that describes the action argument. The returned table will contains the following fields.

|Field|Type|ActionType|Comment|
|:--|:--|:--|:--|
| id | int | USE_ACTIVE_ITEM | |
| id | int | ADD_TEMPRORY_EFFECT | |
| type | int | SPAWN_ENTITY | |
| variant | int | SPAWN_ENTITY | |
| fromType | int | CONVERT_ENTITY | |
| fromVariant | int | CONVERT_ENTITY | |
| toType | int | CONVERT_ENTITY | target type |
| toVariant | int | CONVERT_ENTITY | target variant |
| radius | float | AREA_DAMAGE | |
| value | float | AREA_DAMAGE | |
| flags1 | uint64 | AREA_DAMAGE | |
| flags2 | uint64 | AREA_DAMAGE | |
| scale | float | FART | |
| radius | float | FART | |

___

### Get·Trigger·Rate () {: aria-label='Functions' }
#### float GetTriggerRate ( ) {: .copyable aria-label='Functions' }

Value form 0 to 1.