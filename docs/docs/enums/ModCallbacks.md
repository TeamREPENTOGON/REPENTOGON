# Enum "ModCallbacks"

This is a list of all new callbacks added by REPENTOGON.

## Modified Old Callbacks
### MC_USE_PILL
MC_USE_PILL now passes PillColor as an argument.
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|10 |MC_USE_PILL {: .copyable } | ([PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) Effect, [EntityPlayer](../EntityPlayer.md) Player, [UseFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/UseFlags.html) Flags, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) Color) | [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) | void |

### MC_POST_PICKUP_SELECTION

MC_POST_PICKUP_COLLISION now passes the **Requested Variant** and **Requested SubType**, as well as **RNG**.  
**Requested Variant** and **Requested SubType** represents the variant and subtype set when spawning the entity.   

A third optional `Continue` parameter has been added to the return table.  
If set to `true` the callback will instead replace the `Variant` and `SubType` parameter with the specified values and continue running.

These changes aim to make the callback a viable option for handling pickup pools.

???+ info "Usage"
    Internally the game always attempts to randomize the spawned pickup's variant and subtype, even when re-entering an already visited room, however there are checks in place to make sure that randomization only occurs if either of these values are initially set to 0.

    Therefore, any callback should always check `if RequestedVariant == 0 or RequestedSubType == 0` before attempting any modification.

    That is unless the desired effect is something akin to the APRIL FOOLS's "All nickels are sticky nickels" effect, which is applied regardless of the afore mentioned restriction.

???+ info "Requested Variant == 0"
    When **Requested Variant** is equal to 0 the game will randomize both the Variant and SubType of the pickup. In this case the value of **Requested SubType** is used as a variant blacklist.  

    This blacklist are listed in the [NullPickupSubType](NullPickupSubType.md) enum.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|37 |MC_POST_PICKUP_SELECTION {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>[PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) Variant, <br>int SubType, <br>[PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) RequestedVariant, <br>int RequestedSubType, <br>[RNG](../RNG.md) RNG ) | - | table |

### MC_PRE_PLAYER_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|33 |MC_PRE_PLAYER_COLLISION {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [Entity](../Entity.md) Collider, boolean Low) | [PlayerVariant](PlayerVariant.md) | boolean or table |

### MC_PRE_TEAR_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|42 |MC_PRE_TEAR_COLLISION {: .copyable } | ([EntityTear](../EntityTear.md) Tear, [Entity](../Entity.md) Collider, boolean Low) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | boolean or table |

### MC_PRE_FAMILIAR_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

For example, you can return `{ Collide=true }` to make a familiar to collide physically with something (like an enemy, which they don't normally do) without skipping on-collision code (like you would by returning `false`).

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|26 |MC_PRE_FAMILIAR_COLLISION {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, [Entity](../Entity.md) Collider, boolean Low) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | boolean or table |

### MC_PRE_BOMB_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|60 |MC_PRE_BOMB_COLLISION {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb, [Entity](../Entity.md) Collider, boolean Low) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | boolean or table |

### MC_PRE_PICKUP_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|38 |MC_PRE_PICKUP_COLLISION {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, [Entity](../Entity.md) Collider, boolean Low) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean or table |

### MC_PRE_KNIFE_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|53 |MC_PRE_KNIFE_COLLISION {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife, [Entity](../Entity.md) Collider, boolean Low) | [KnifeSubType](KnifeSubType.md) | boolean or table |

### MC_PRE_PROJECTILE_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|46 |MC_PRE_PROJECTILE_COLLISION {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile, [Entity](../Entity.md) Collider, boolean Low) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | boolean or table |

### MC_PRE_NPC_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|30 |MC_PRE_NPC_COLLISION {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, [Entity](../Entity.md) Collider, boolean Low) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean or table |

### MC_ENTITY_TAKE_DMG
Now optionally accepts returning a table, with any combination of the following fields, to override the corresponding argument:

* Damage
* DamageFlags
* DamageCountdown

Modified values are passed along to the remaining callbacks. Returning false to cancel the damage still skips the remaining callbacks.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|11 |MC_ENTITY_TAKE_DMG {: .copyable } | ([Entity](../Entity.md) Entity, float Damage, [DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int DamageCountdown) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean or table |


## New Callbacks
### MC_PRE_ADD_COLLECTIBLE {: .copyable }
Accepts a table of parameters: `{Type, Charge, FirstTime, Slot, VarData}`

Alternatively accepts a [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) to change the type without changing any other parameters or a boolean to cancel the addition altogether (false) or force it to happen skipping the following callbacks (true).

???- example "Example Code"
    This code will transform every collectible into Money = Power once the player picks it up.
    ```lua
    function mod:myFunction(Type, Charge, FirstTime, Slot, VarData, Player)
        return CollectibleType.COLLECTIBLE_MONEY_EQUALS_POWER
    end
    mod:AddCallback(ModCallbacks.MC_PRE_ADD_COLLECTIBLE, mod.myFunction)
    ```

    This code will force active items to be uncharged on pickup.
    ```lua
    function mod:myFunction(Type, Charge, FirstTime, Slot, VarData, Player)
        return {Type, 0, FirstTime, Slot, VarData}
    end
    mod:AddCallback(ModCallbacks.MC_PRE_ADD_COLLECTIBLE, mod.myFunction)
    ```

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1004 |MC_PRE_ADD_COLLECTIBLE {: .copyable } | ([CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, <br>int Charge, <br>boolean FirstTime, <br>int Slot, <br>int VarData, <br>[EntityPlayer](../EntityPlayer.md) Player) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | table or [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) |

### MC_POST_ADD_COLLECTIBLE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1005 |MC_POST_ADD_COLLECTIBLE {: .copyable } | ([CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, <br>int Charge, <br>boolean FirstTime, <br>int Slot, <br>int VarData, <br>[EntityPlayer](../EntityPlayer.md) Player) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | void |

### MC_POST_BACKDROP_PRE_RENDER_WALLS {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1109 | MC_POST_BACKDROP_PRE_RENDER_WALLS {: .copyable } | void | - | void |

### MC_PRE_BACKDROP_CHANGE {: .copyable }
Accepts an `integer` to change [BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html).

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1141 | MC_PRE_BACKDROP_CHANGE {: .copyable } | ([BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Type) | - | ([BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Type) |

### MC_PRE_BACKDROP_RENDER_FLOOR {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1107 | MC_PRE_BACKDROP_RENDER_FLOOR {: .copyable } | ([ColorModifier](../ColorModifier.md) ColorModifier) | - | void |

### MC_PRE_BACKDROP_RENDER_WALLS {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1106 | MC_PRE_BACKDROP_RENDER_WALLS {: .copyable } | ([ColorModifier](../ColorModifier.md) ColorModifier) | - | void |

### MC_PRE_BACKDROP_RENDER_WATER {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1108 | MC_PRE_BACKDROP_RENDER_WATER {: .copyable } | void | - | void |

### MC_POST_BOMB_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1237 |MC_POST_BOMB_COLLISION {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | void |

### MC_PRE_BOMB_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1088 |MC_PRE_BOMB_RENDER {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb, <br>[Vector](../Vector.md) Offset) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.md) | [Vector](../Vector.md) or boolean |

### MC_PRE_CHALLENGE_DONE {: .copyable }
Called before a challenge is marked as completed.

Returning `false` will prevent further execution of the challenge completion-tracking function.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1471 | MC_PRE_CHALLENGE_DONE {: .copyable } | ([Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html), <br>EntityPlayer [EntityPlayer](../EntityPlayer.md)) | [Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html) | boolean |

### MC_POST_CHALLENGE_DONE {: .copyable }
Called after a challenge is marked as completed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1472 | MC_POST_CHALLENGE_DONE {: .copyable } | ([Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html)) | [Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html) | void |

### MC_PRE_CHANGE_ROOM {: .copyable }
Accepts a table of parameters: `{TargetRoomIdx, Dimension}`

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1061 |MC_PRE_CHANGE_ROOM {: .copyable } | (int TargetRoomIdx, <br>int Dimension) | - | table |

### MC_POST_ACHIEVEMENT_UNLOCK {: .copyable }
Called after an achievement is unlocked.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1476 |MC_POST_ACHIEVEMENT_UNLOCK {: .copyable } | ([Achievement](Achievement.md) AchievementID) | [Achievement](Achievement.md) | void |

### MC_PRE_COMPLETION_EVENT {: .copyable }
Can return a new [CompletionType](CompletionType.md) or `false` to cancel the completion event. Canceling it will prevent all marks and completion event related stuff to trigger for all players.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1049 |MC_PRE_COMPLETION_EVENT {: .copyable } | ([CompletionType](CompletionType.md) Completion) | - | [CompletionType](CompletionType.md), boolean |

### MC_POST_COMPLETION_EVENT {: .copyable }
Called when a completion event is recorded, such as when defeating an end boss or unlocking a tainted character.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1052 |MC_POST_COMPLETION_EVENT {: .copyable } | ([CompletionType](CompletionType.md) Completion) | - | void |

### MC_PRE_COMPLETION_MARKS_RENDER {: .copyable }
Can return `false` to prevent the completion marks from rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1216 |MC_PRE_COMPLETION_MARKS_RENDER {: .copyable } | ([Sprite](../Sprite.md) CompletionMarksSprite, <br>[Vector](../Vector.md) RenderPos, <br>[Vector](../Vector.md) RenderScale, <br>int PlayerType) | - | void |

### MC_POST_COMPLETION_MARKS_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1217 |MC_POST_COMPLETION_MARKS_RENDER {: .copyable } | ([Sprite](../Sprite.md) CompletionMarksSprite, <br>[Vector](../Vector.md) RenderPos, <br>[Vector](../Vector.md) RenderScale, <br>int PlayerType) | - | void |

### MC_COMPLETION_MARK_GET {: .copyable }
Can return `false` to cancel the completion mark.

Called when the player gets a completion mark, getting as a parameter the code for the mark in question and the playertype.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1047 |MC_COMPLETION_MARK_GET {: .copyable } | ([CompletionType](CompletionType.md) Completion, <br>int [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html)) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void or false |

### MC_POST_COMPLETION_MARK_GET {: .copyable }
Called after the player gets a completion mark, getting as a parameter the code for the mark in question and the playertype.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1048 |MC_POST_COMPLETION_MARK_GET {: .copyable } | ([CompletionType](CompletionType.md) Completion, <br>int [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html)) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_CONSOLE_AUTOCOMPLETE {: .copyable }
This is called whenever a function with the CUSTOM [AutocompleteType](AutocompleteType.md) enum is being entered into the console. Called every time the console input changes.

Accepts a table. The table can hold both string values, which will just add the string as a parameter in autocomplete for the command, and a table of two strings which will add the first string as the parameter, and second as a description. Description can be used in autocomplete as well, however pressing TAB will properly autocomplete using the ID, not description (Think the `giveitem` command, as an example- `c1` would be The Sad Onion's "parameter" and `The Sad Onion` would be the "description", and both work. Pressing TAB will turn the command into `give c1`.)

REPENTOGON handles only showing options relevant for the given input- simply return a table of options, and REPENTOGON will take care of the rest.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1120 |MC_CONSOLE_AUTOCOMPLETE {: .copyable } | (string Command, <br>string Params) | string Command | table |

### MC_PRE_DEVIL_APPLY_ITEMS {: .copyable }
This callback is run when the game starts to tally up traditional items for devil deal calculation. This is called before the stage penalty.

Most items that affect devil deal chance perform their changes here.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1130 | MC_PRE_DEVIL_APPLY_ITEMS {: .copyable } | (float Chance) | - | float |

### MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS {: .copyable }
Next, the game applies "special" items which bypass the stage penalty like Goat Head and Eucharist.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1132 | MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS {: .copyable } | (float Chance) | - | float |

### MC_PRE_DEVIL_APPLY_STAGE_PENALTY {: .copyable }
Next, the game calculates stage penalty. If a deal spawned anywhere on the previous two floors, the game decays the resulting chance by either 50% or 25% depending on how many deals have been taken.

Important to note that even though the game displays a value of ~66% or ~33% for the 50% and 25% values, respectively, this is because devil chance is *not* clamped to a value between 0 and 1, and "100%" without items generally means a value of ~133%.

Accepts a `boolean`. Return `false` to bypass the stage penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1131 |MC_PRE_DEVIL_APPLY_STAGE_PENALTY {: .copyable } | void | - | boolean |

### MC_POST_DEVIL_CALCULATE {: .copyable }
This will override *all* previous calculation values, ultimately dictating the devil chance.

Accepts a `float` to modify the chance.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1133 |MC_POST_DEVIL_CALCULATE {: .copyable } | (float Chance) | - | float |

### MC_PRE_EFFECT_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1087 |MC_PRE_EFFECT_RENDER {: .copyable } | ([EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/Entityeffect.html) Effect, <br>[Vector](../Vector.md) Offset) | [EffectVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/EffectVariant.md) | [Vector](../Vector.md) or boolean |

### MC_POST_ENTITY_TAKE_DMG {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1006 |MC_POST_ENTITY_TAKE_DMG {: .copyable } | ([Entity](../Entity.md) Entity, <br>float Damage, <br>[DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, <br>[EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, <br>int DamageCountdown) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | void |

### MC_PRE_ENTITY_THROW {: .copyable }
Accepts a [Vector](../Vector.md) which will modify the velocity of the thrown entity.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1040 |MC_PRE_ENTITY_THROW {: .copyable } | ([EntityPlayer](../EntityPlayer.md) ThrowingPlayer, <br>[Entity](../Entity.md) HeldEntity, <br>[Vector](../Vector.md) Velocity) | - | [Vector](../Vector.md) |

### MC_POST_ENTITY_THROW {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1041 |MC_POST_ENTITY_THROW {: .copyable } | ([EntityPlayer](../EntityPlayer.md) ThrowingPlayer, <br>[Entity](../Entity.md) ThrownEntity, <br>[Vector](../Vector.md) Velocity) | - | void |

### MC_POST_FAMILIAR_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1235 |MC_POST_FAMILIAR_COLLISION {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_POST_FAMILIAR_FIRE_BRIMSTONE {: .copyable }
Called when a familiar fires a brimstone laser.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1261 |MC_POST_FAMILIAR_FIRE_BRIMSTONE {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_POST_FAMILIAR_FIRE_PROJECTILE {: .copyable }
Called when a familiar fires a tear.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1252 |MC_POST_FAMILIAR_FIRE_PROJECTILE {: .copyable } | ([EntityTear](../EntityTear.md) Tear) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_POST_FAMILIAR_FIRE_TECH_LASER {: .copyable }
Called when a familiar fires a Tech laser.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1262 |MC_POST_FAMILIAR_FIRE_TECH_LASER {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_PRE_FAMILIAR_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1080 |MC_PRE_FAMILIAR_RENDER {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, <br>[Vector](../Vector.md) Offset) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | [Vector](../Vector.md) or boolean |

### MC_POST_FIRE_BOMB {: .copyable }
Called when the player fires a Dr. Fetus bomb.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1253 |MC_POST_FIRE_BOMB {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb) | - | void |

### MC_POST_FIRE_BONE_CLUB {: .copyable }
Called when the player fired The Forgotten's bone club.

This is only called when the club is initially spawned, not when swung or charged and shot.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1254 |MC_POST_FIRE_BONE_CLUB {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife) | - | void |

### MC_POST_FIRE_BRIMSTONE_BALL {: .copyable }
Called when the player fires a brimstone ball.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1256 |MC_POST_FIRE_BRIMSTONE_BALL {: .copyable } | ([EntityEffect](../EntityEffect.md) Effect) | - | void |

### MC_POST_FIRE_BRIMSTONE {: .copyable }
Called when the player fires a brimstone laser.

This is also called for delayed brimstone.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1255 |MC_POST_FIRE_BRIMSTONE {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser) | - | void |

### MC_POST_FIRE_KNIFE {: .copyable }
Called when the player fires the knife from Mom's Knife.

This is only called when the club is initially spawned, not when charged and shot.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1257 |MC_POST_FIRE_KNIFE {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife) | - | void |

### MC_POST_FIRE_SWORD {: .copyable }
Called when the player swings the sword from Spirit Sword.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1258 |MC_POST_FIRE_SWORD {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife) | - | void |

### MC_POST_FIRE_TECH_LASER {: .copyable }
Called when the player fires a Tech laser.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1259 |MC_POST_FIRE_TECH_LASER {: .copyable } | ([EntityLaser](../EntityKnife.md) Laser) | - | void |

### MC_POST_FIRE_TECH_X_LASER {: .copyable }
Called when the player fires a Tech X laser.

Returning any value will have no effect on later callback executions.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1260 |MC_POST_FIRE_TECH_X_LASER {: .copyable } | ([EntityLaser](../EntityKnife.md) Laser) | - | void |

### MC_GET_FOLLOWER_PRIORITY {: .copyable }
Accepts [FollowerPriority](FollowerPriority.md) to give familiar priority. Can accept any int.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1063 |MC_GET_FOLLOWER_PRIORITY {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.md) | [FollowerPriority](FollowerPriority.md) or int |

### MC_PRE_GET_LIGHTING_ALPHA {: .copyable }
Accepts a `float` to modify the lighting alpha. Generally this is between 0 and 1 but you can technically go higher than this.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1150 |MC_PRE_GET_LIGHTING_ALPHA {: .copyable } | (float OriginalAlpha) | - | float |

### MC_GET_SHOP_ITEM_PRICE {: .copyable }
Called after the price for a shop item was calculated.
Return an integer or [PickupPrice](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupPrice.html) to change the price of the item.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1066 |MC_GET_SHOP_ITEM_PRICE {: .copyable } | (int [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html), <br>int PickupSubType, <br>int ShopItemID, <br>int Price) | int | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) |

### MC_PRE_GRID_ENTITY_DECORATION_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1444 |MC_PRE_GRID_ENTITY_DECORATION_RENDER {: .copyable } | ([GridEntityDecoration](../GridEntityDecoration.md) Decoration, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_DECORATION_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1445 |MC_POST_GRID_ENTITY_DECORATION_RENDER {: .copyable } | ([GridEntityDecoration](../GridEntityDecoration.md) Decoration) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_DECORATION_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1400 |MC_PRE_GRID_ENTITY_DECORATION_UPDATE {: .copyable } | ([GridEntityDecoration](../GridEntityDecoration.md) Decoration) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_DECORATION_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1401 |MC_POST_GRID_ENTITY_DECORATION_UPDATE {: .copyable } | ([GridEntityDecoration](../GridEntityDecoration.md) Decoration) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_DOOR_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1446 |MC_PRE_GRID_ENTITY_DOOR_RENDER {: .copyable } | ([GridEntityDoor](../GridEntityDoor.md) Door, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_DOOR_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1447 |MC_POST_GRID_ENTITY_DOOR_RENDER {: .copyable } | ([GridEntityDoor](../GridEntityDoor.md) Door) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_DOOR_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1402 |MC_PRE_GRID_ENTITY_DOOR_UPDATE {: .copyable } | ([GridEntityDoor](../GridEntityDoor.md) Door) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_DOOR_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1403 |MC_POST_GRID_ENTITY_DOOR_UPDATE {: .copyable } | ([GridEntityDoor](../GridEntityDoor.md) Door) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_FIRE_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

???+ warning "Warning"
    Fire grid entities are largely unused, and in most cases you'll want to target the [EntityNPC](../EntityNPC.md) fireplaces.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1448 |MC_PRE_GRID_ENTITY_FIRE_RENDER {: .copyable } | ([GridEntityFire](../GridEntityFire.md) Fire, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_FIRE_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1449 |MC_POST_GRID_ENTITY_FIRE_RENDER {: .copyable } | ([GridEntityFire](../GridEntityFire.md) Fire) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_FIRE_UPDATE {: .copyable }
Accepts `false` to cancel the update.

???+ warning "Warning"
    Fire grid entities are largely unused, and in most cases you'll want to target the [EntityNPC](../EntityNPC.md) fireplaces.


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1404 |MC_PRE_GRID_ENTITY_FIRE_UPDATE {: .copyable } | ([GridEntityFire](../GridEntityFire.md) Fire) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_FIRE_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1405 |MC_POST_GRID_ENTITY_FIRE_UPDATE {: .copyable } | ([GridEntityFire](../GridEntityFire.md) Fire) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_GRAVITY_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1406 |MC_PRE_GRID_ENTITY_GRAVITY_UPDATE {: .copyable } | ([GridEntityGravity](../GridEntityGravity.md) Gravity) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_GRAVITY_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1407 |MC_POST_GRID_ENTITY_GRAVITY_UPDATE {: .copyable } | ([GridEntityGravity](../GridEntityGravity.md) Gravity) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_LOCK_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1450 |MC_PRE_GRID_ENTITY_LOCK_RENDER {: .copyable } | ([GridEntityLock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityLock.html) Lock, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_LOCK_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1451 |MC_POST_GRID_ENTITY_LOCK_RENDER {: .copyable } | ([GridEntityLock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityLock.html) Lock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_LOCK_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1408 |MC_PRE_GRID_ENTITY_LOCK_UPDATE {: .copyable } | ([GridEntityLock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityLock.html) Lock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_LOCK_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1409 |MC_POST_GRID_ENTITY_LOCK_UPDATE {: .copyable } | ([GridEntityLock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityLock.html) Lock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_PIT_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1454 |MC_PRE_GRID_ENTITY_PIT_RENDER {: .copyable } | ([GridEntityPit](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPit.html) Pit, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_PIT_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1455 |MC_POST_GRID_ENTITY_PIT_RENDER {: .copyable } | ([GridEntityPit](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPit.html) Pit) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_PIT_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1410 |MC_PRE_GRID_ENTITY_PIT_UPDATE {: .copyable } | ([GridEntityPit](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPit.html) Pit) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_PIT_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1411 |MC_POST_GRID_ENTITY_PIT_UPDATE {: .copyable } | ([GridEntityPit](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPit.html) Pit) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_POOP_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

???- warning "Warning"
    This callback does not include the [EntityNPC](../EntityNPC.md) poops used by Tainted ???.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1456 |MC_PRE_GRID_ENTITY_POOP_RENDER {: .copyable } | ([GridEntityPoop](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPoop.html) Poop, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_POOP_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1457 |MC_POST_GRID_ENTITY_POOP_RENDER {: .copyable } | ([GridEntityPoop](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPoop.html) Poop) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_POOP_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1412 |MC_PRE_GRID_ENTITY_POOP_UPDATE {: .copyable } | ([GridEntityPoop](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPoop.html) Poop) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_POOP_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1413 |MC_POST_GRID_ENTITY_POOP_UPDATE {: .copyable } | ([GridEntityPoop](https://wofsauge.github.io/IsaacDocs/rep/GridEntityPoop.html) Poop) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_PRESSUREPLATE_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1460 |MC_PRE_GRID_ENTITY_PRESSUREPLATE_RENDER {: .copyable } | ([GridEntityPressurePlate](../GridEntityPressurePlate.md) PressurePlate, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_PRESSUREPLATE_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1461 |MC_POST_GRID_ENTITY_PRESSUREPLATE_RENDER {: .copyable } | ([GridEntityPressurePlate](../GridEntityPressurePlate.md) PressurePlate) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_PRESSUREPLATE_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1414 |MC_PRE_GRID_ENTITY_PRESSUREPLATE_UPDATE {: .copyable } | ([GridEntityPressurePlate](../GridEntityPressurePlate.md) PressurePlate) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_PRESSUREPLATE_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1415 |MC_POST_GRID_ENTITY_PRESSUREPLATE_UPDATE {: .copyable } | ([GridEntityPressurePlate](../GridEntityPressurePlate.md) PressurePlate) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_ROCK_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1458 |MC_PRE_GRID_ENTITY_ROCK_RENDER {: .copyable } | ([GridEntityRock](../GridEntityRock.md) Rock, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_ROCK_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1459 |MC_POST_GRID_ENTITY_ROCK_RENDER {: .copyable } | ([GridEntityRock](../GridEntityRock.md) Rock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_ROCK_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1416 |MC_PRE_GRID_ENTITY_ROCK_UPDATE {: .copyable } | ([GridEntityRock](../GridEntityRock.md) Rock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_ROCK_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1417 |MC_POST_GRID_ENTITY_ROCK_UPDATE {: .copyable } | ([GridEntityRock](../GridEntityRock.md) Rock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_SPAWN {: .copyable }
This is called when a [GridEntity](../GridEntity.md) is spawned outside of Room initialization.

Accepts `false` to cancel spawning the grid, a table of `{Type, Variant, Vardata, SpawnSeed}` to modify it, or a [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) to override it entirely.

???+ warning "Warning"
    `Desc` will in most cases be `nil`. The exceptions to this are the poops spawned by Mole npcs, grids spawned by [TurnGold](https://wofsauge.github.io/IsaacDocs/rep/Room.html#void-turngold), or lua-made spawns called with the new `SpawnGridEntity(int GridIndex, GridEntityDesc Descriptor)` override.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1100 |MC_PRE_GRID_ENTITY_SPAWN {: .copyable } | ([GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, <br>int Variant, <br>int VarData, <br>int GridIdx, <br>int SpawnSeed, <br>[GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) Desc) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type | boolean, table, or [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) |

### MC_POST_GRID_ENTITY_SPAWN {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1101 |MC_POST_GRID_ENTITY_SPAWN {: .copyable } | ([GridEntity](../GridEntity.md) Grid) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type | void |

### MC_PRE_GRID_ENTITY_SPIKES_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1432 |MC_PRE_GRID_ENTITY_SPIKES_RENDER {: .copyable } | ([GridEntitySpikes](https://wofsauge.github.io/IsaacDocs/rep/GridEntitySpikes.html) Grid, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_SPIKES_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1433 |MC_POST_GRID_ENTITY_SPIKES_RENDER {: .copyable } | ([GridEntitySpikes](https://wofsauge.github.io/IsaacDocs/rep/GridEntitySpikes.html) Grid) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_SPIKES_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1418 |MC_PRE_GRID_ENTITY_SPIKES_UPDATE {: .copyable } | ([GridEntitySpikes](https://wofsauge.github.io/IsaacDocs/rep/GridEntitySpikes.html) Spikes) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_SPIKES_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1419 |MC_POST_GRID_ENTITY_SPIKES_UPDATE {: .copyable } | ([GridEntitySpikes](https://wofsauge.github.io/IsaacDocs/rep/GridEntitySpikes.html) Spikes) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_STAIRCASE_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1440 |MC_PRE_GRID_ENTITY_STAIRCASE_RENDER {: .copyable } | ([GridEntityStairs](../GridEntityStairs.md) Staircase, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_STAIRCASE_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1441 |MC_POST_GRID_ENTITY_STAIRCASE_RENDER {: .copyable } | ([GridEntityStairs](../GridEntityStairs.md) Staircase) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_STAIRCASE_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1420 |MC_PRE_GRID_ENTITY_STAIRCASE_UPDATE {: .copyable } | ([GridEntityStairs](../GridEntityStairs.md) Staircase) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_STAIRCASE_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1421 |MC_POST_GRID_ENTITY_STAIRCASE_UPDATE {: .copyable } | ([GridEntityStairs](../GridEntityStairs.md) Staircase) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_STATUE_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1422 |MC_PRE_GRID_ENTITY_STATUE_UPDATE {: .copyable } | ([GridEntityStatue](../GridEntityStatue.md) Statue) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_STATUE_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1423 |MC_POST_GRID_ENTITY_STATUE_UPDATE {: .copyable } | ([GridEntityStatue](../GridEntityStatue.md) Statue) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TELEPORTER_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1452 |MC_PRE_GRID_ENTITY_TELEPORTER_RENDER {: .copyable } | ([GridEntityTeleporter](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTeleporter.html) Teleporter, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_TELEPORTER_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1453 |MC_POST_GRID_ENTITY_TELEPORTER_RENDER {: .copyable } | ([GridEntityTeleporter](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTeleporter.html) Teleporter) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TELEPORTER_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1424 |MC_PRE_GRID_ENTITY_TELEPORTER_UPDATE {: .copyable } | ([GridEntityTeleporter](../GridEntityTeleporter.md) Teleporter) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_TELEPORTER_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1425 |MC_POST_GRID_ENTITY_TELEPORTER_UPDATE {: .copyable } | ([GridEntityTeleporter](../GridEntityTeleporter.md) Teleporter) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TNT_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1436 |MC_PRE_GRID_ENTITY_TNT_RENDER {: .copyable } | ([GridEntityTNT](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTNT.html) TNT, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_TNT_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1437 |MC_POST_GRID_ENTITY_TNT_RENDER {: .copyable } | ([GridEntityTNT](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTNT.html) TNT) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TNT_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1430 |MC_PRE_GRID_ENTITY_TNT_UPDATE {: .copyable } | ([GridEntityTNT](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTNT.html) TNT) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_TNT_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1431 |MC_POST_GRID_ENTITY_TNT_UPDATE {: .copyable } | ([GridEntityTNT](https://wofsauge.github.io/IsaacDocs/rep/GridEntityTNT.html) TNT) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TRAPDOOR_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1438 |MC_PRE_GRID_ENTITY_TRAPDOOR_RENDER {: .copyable } | ([GridEntityTrapDoor](../GridEntityTrapDoor.md) TrapDoor, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_TRAPDOOR_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1439 |MC_POST_GRID_ENTITY_TRAPDOOR_RENDER {: .copyable } | ([GridEntityTrapDoor](../GridEntityTrapDoor.md) TrapDoor) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_TRAPDOOR_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1426 |MC_PRE_GRID_ENTITY_TRAPDOOR_UPDATE {: .copyable } | ([GridEntityTrapDoor](../GridEntityTrapDoor.md) TrapDoor) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_TRAPDOOR_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1427 |MC_POST_GRID_ENTITY_TRAPDOOR_UPDATE {: .copyable } | ([GridEntityTrapDoor](../GridEntityTrapDoor.md) TrapDoor) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_WALL_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1462 |MC_PRE_GRID_ENTITY_WALL_RENDER {: .copyable } | ([GridEntityWall](../GridEntityWall.md) Wall, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_WALL_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1463 |MC_POST_GRID_ENTITY_WALL_RENDER {: .copyable } | ([GridEntityWall](../GridEntityWall.md) Wall) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_WEB_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset or `false` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1434 |MC_PRE_GRID_ENTITY_WEB_RENDER {: .copyable } | ([GridEntityWeb](../GridEntityWeb.md) Web, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |

### MC_POST_GRID_ENTITY_WEB_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1435 |MC_POST_GRID_ENTITY_WEB_RENDER {: .copyable } | ([GridEntityWeb](../GridEntityWeb.md) Web) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_GRID_ENTITY_WEB_UPDATE {: .copyable }
Accepts `false` to cancel the update.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1428 |MC_PRE_GRID_ENTITY_WEB_UPDATE {: .copyable } | ([GridEntityWeb](../GridEntityWeb.md) Web) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_ENTITY_WEB_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1429 |MC_POST_GRID_ENTITY_WEB_UPDATE {: .copyable } | ([GridEntityWeb](../GridEntityWeb.md) Web) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_GRID_HURT_DAMAGE {: .copyable }
Return false if entity or player should ignore damage from it.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1012 |MC_GRID_HURT_DAMAGE {: .copyable } | ([GridEntity](../GridEntity.md) GridEntity, <br>[Entity](../Entity.md) Entity, <br>int DamageAmount, <br>[DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, <br>float Unknown  boolean Unknown) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_HURT_DAMAGE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1013 |MC_POST_GRID_HURT_DAMAGE {: .copyable } | ([GridEntity](../GridEntity.md) GridEntity, <br>[Entity](../Entity.md) Entity, <br>int DamageAmount, <br>[DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, <br>float Unknown  boolean Unknown) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_POST_GRID_ROCK_DESTROY {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1011 |MC_POST_GRID_ROCK_DESTROY {: .copyable } | ([GridEntityRock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityRock.html) Rock, <br>[GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, <br>boolean Immediate) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_HUD_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1022 |MC_HUD_RENDER {: .copyable } | void | - | void |

### MC_POST_HUD_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1024 |MC_POST_HUD_RENDER {: .copyable } | void | - | void |

### MC_HUD_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1020 |MC_HUD_UPDATE {: .copyable } | void | - | void |

### MC_POST_HUD_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1021 |MC_POST_HUD_UPDATE {: .copyable } | void | - | void |

### MC_IS_PERSISTENT_ROOM_ENTITY {: .copyable }
Returning `true` allows entity to respawn.


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1263 |MC_IS_PERSISTENT_ROOM_ENTITY {: .copyable } | ([EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, <br>int Variant) | - | boolean |

### MC_PRE_ITEM_OVERLAY_SHOW {: .copyable }
Accepts an integer to change [Giantbook](Giantbook.md)

Alternatively accepts `true` to cancel item overlay show

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1076 |MC_PRE_ITEM_OVERLAY_SHOW {: .copyable } | ([Giantbook](Giantbook.md) GiantbookID, <br>int Delay, <br>[EntityPlayer](../EntityPlayer.md) Player) | [Giantbook](Giantbook.md) | [Giantbook](Giantbook.md) or boolean |

### MC_POST_ITEM_OVERLAY_SHOW {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1134 |MC_POST_ITEM_OVERLAY_SHOW {: .copyable } | ([Giantbook](Giantbook.md) GiantbookID, <br>int Delay, <br>[EntityPlayer](../EntityPlayer.md) Player) | [Giantbook](Giantbook.md) | void |

### MC_POST_ITEM_OVERLAY_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1075 |MC_POST_ITEM_OVERLAY_UPDATE {: .copyable } | void | [Giantbook](Giantbook.md) | void |

### MC_POST_KNIFE_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1243 |MC_POST_KNIFE_COLLISION {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [KnifeSubType](KnifeSubType.md) | void |

### MC_PRE_KNIFE_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1086 |MC_PRE_KNIFE_RENDER {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife, <br>[Vector](../Vector.md) Offset) | [KnifeVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/KnifeVariant.md) | [Vector](../Vector.md) or boolean |

### MC_PRE_LASER_COLLISION {: .copyable }
Runs right before a laser hits an entity. Return true to ignore the collision.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1248 |MC_PRE_LASER_COLLISION {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser, <br>[Entity](../Entity.md) Collider) | [LaserVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/LaserVariant.html) | boolean |

### MC_POST_LASER_COLLISION {: .copyable }
Runs after a laser hits an entity.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1249 |MC_POST_LASER_COLLISION {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser, <br>[Entity](../Entity.md) Collider) | [LaserVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/LaserVariant.html) | void |

### MC_PRE_LEVEL_INIT {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1060 |MC_PRE_LEVEL_INIT {: .copyable } | void | - | void |

### MC_POST_LEVEL_LAYOUT_GENERATED {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1099 |MC_POST_LEVEL_LAYOUT_GENERATED {: .copyable } | ([LevelGenerator](../LevelGenerator.md) LevelGenerator) | - | void |

### MC_PRE_LEVEL_PLACE_ROOM {: .copyable }
Return a room config to replace the room that will be placed

???+ warning "Warning"
    The new room shape must be the same, and the new available door slots must be compatible with the original room doors.


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1137 |MC_PRE_LEVEL_PLACE_ROOM {: .copyable } | ([LevelGeneratorRoom](../LevelGeneratorRoom.md) Slot, <br>[RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfig, <br>int Seed) | - | [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) Config |

### MC_PRE_LEVEL_SELECT {: .copyable }
This callback is fired when the game selects which level (also known as stage) to load, usually when the player enters a trapdoor.
The callback accepts two parameters:

* Level: the level selected by the game, as defined in the [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) enumeration.
* Type: the type of the level selected by the game, as defined in the [StageType](https://wofsauge.github.io/IsaacDocs/rep/enums/StageType.html) enumeration.

This callback can return either nothing or a table. 

* Nothing: let the game continue with the level stage / stage type pair it selected;
* Table: must contain two fields (anonymous). The first field is the desired level stage, second field is the desired stage type.

If you return a table, Repentogon will check that the values fall in the allowed ranges for the level stage and the stage type.

???+ warn "Value ranges"
    Remember that the range of level types is not the same between normal / hard on one side and greed / greedier on the other. 

    Also remember that since Repentance the stage type value 3 is deprecated.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1104 | MC_PRE_LEVEL_SELECT {: .copyable } | ([LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) Level, <br>[StageType](https://wofsauge.github.io/IsaacDocs/rep/enums/StageType.html) Type) | - | void 

### MC_MAIN_MENU_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1023 |MC_MAIN_MENU_RENDER {: .copyable } | void | - | void |

### MC_PRE_MEGA_SATAN_ENDING {: .copyable }
Called right before Mega Satan forcibly ends the game. 

* Accepts `true` to suppress the ending, guaranteeing a portal to the Void while retaining the completion mark for this character. 
* `false` or `nil` will have no effect. I might look into making `false` guarantee an ending?

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1201 |MC_PRE_MEGA_SATAN_ENDING {: .copyable } | void | - | boolean |

### MC_MENU_INPUT_ACTION {: .copyable }
Same as MC_INPUT_ACTION, but only works in the main menu.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1464 | MC_MENU_INPUT_ACTION {: .copyable } | ([Entity](../Entity.md), <br>[InputHook](InputHook.md), <br>[ButtonAction](ButtonAction.md))|[InputHook](InputHook.md) | boolean or float |

### MC_POST_MODS_LOADED {: .copyable }
Called after all Lua scripts have been loaded. Ideal for running code that is expected to run after all mods are initialized, but without the need for load order idiocy!

Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1210 |MC_POST_MODS_LOADED {: .copyable } | void | - | void |

### MC_PRE_MUSIC_LAYER_TOGGLE {: .copyable }

Accepts a music layer ID (enum pending) to change the layer, or a boolean to change the state of the layer: `true` to keep it running, `false` to stop it.

`CurrentState` returns `true` if the layer is going to be **enabled**, and `false` if it is going to be **disabled.**

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1035 |MC_PRE_MUSIC_LAYER_TOGGLE {: .copyable } | (int ID, <br>boolean CurrentState) | int | int or boolean |

### MC_PRE_MUSIC_PLAY_JINGLE {: .copyable }
Accepts a [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) to change the track

Alternatively accepts `false` to cancel the track

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1094 |MC_PRE_MUSIC_PLAY_JINGLE {: .copyable } | ([Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicID) | [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) | [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) or boolean |

### MC_PRE_MUSIC_PLAY {: .copyable }
Accepts a table of parameters: `{ID, Volume OR FadeRate}`

Alternatively accepts a [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) to change the track without changing volume or fade rate

Alternatively accepts `false` to cancel the track

???- info "Volume vs. FadeRate"
    This callback is called for *both* MusicManager::Play *and* MusicManager::Crossfade! `IsFade` is provided to distinguish the two.

???- example "Example Code"
    This code will replace all music track with the flooded caves theme (for better or for worse).
    ```lua
    function mod:myFunction(ID, Volume, IsFade)
        return Music.MUSIC_FLOODED_CAVES
    end
    mod:AddCallback(ModCallbacks.MC_PRE_MUSIC_PLAY, mod.myFunction)
    ```


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1034 |MC_PRE_MUSIC_PLAY {: .copyable } | (int ID, <br>float Volume OR float FadeRate, <br>boolean IsFade) | [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) | table or [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) or boolean |

### MC_PRE_M_MORPH_ACTIVE {: .copyable }
This callback triggers when an active gets rerolled by 'M (trinket id 138) and allows for overriding its behavior.

* Accepts a [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) to override the rerolled item id or `false` to prevent the active from rerolling entirely.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1190 |MC_PRE_M_MORPH_ACTIVE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible) | - | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) |

### MC_PRE_NEW_ROOM {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1200 |MC_PRE_NEW_ROOM {: .copyable } | ([Room](https://wofsauge.github.io/IsaacDocs/rep/Room) Room, <br>[RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor) Descriptor) | - | void |

### MC_POST_NIGHTMARE_SCENE_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1102 |MC_POST_NIGHTMARE_SCENE_RENDER {: .copyable } | void | - | void |

### MC_POST_NIGHTMARE_SCENE_SHOW {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1103 |MC_POST_NIGHTMARE_SCENE_SHOW {: .copyable } | (boolean Unknown) | - | void |

### MC_POST_NPC_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1247 |MC_POST_NPC_COLLISION {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | void |

### MC_PRE_NPC_MORPH {: .copyable }
Accepts a table of parameters: `{EntityType, Variant, SubType, Championid}` or just `{EntityType, Variant, SubType}`. 

Returning `false` cancels the morph.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1212 |MC_PRE_NPC_MORPH {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>int EntityType, <br>int Variant, <br>int SubType, <br>int Championid) | - | table or boolean |

### MC_POST_NPC_MORPH {: .copyable }
Runs after the Morph already happened.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1214 |MC_POST_NPC_MORPH {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>int PreviousType, <br>int PreviousVariant, <br>int PreviousSubType) | - | void |

### MC_NPC_PICK_TARGET {: .copyable }
Called whenever an EntityNPC selects its target, such as when EntityNPC:GetPlayerTarget() is called.

Return an entity to make the NPC target that entity instead.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1222 |MC_NPC_PICK_TARGET {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>[Entity](../Entity.md) CurrentTarget) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Entity](../Entity.md) |

### MC_POST_NPC_DARK_RED_CHAMPION_REGEN {: .copyable }
Runs right after a dark red champion regenerates out of the goo form.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1223 |MC_POST_NPC_DARK_RED_CHAMPION_REGEN {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | void |

### MC_EVALUATE_CUSTOM_CACHE {: .copyable }
Called when a custom cache is evaluated (see [items.xml](xml/items.md)). Return a number to modify the value. Modified values are passed onto the next callback.

The initial value is always 0. The most recent result can be obtained at any time using `player:GetCustomCacheValue("mycustomcache")`

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1224 |MC_EVALUATE_CUSTOM_CACHE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, CustomCacheTag string, float Value) | string CustomCacheTag | void |

### MC_EVALUATE_FAMILIAR_MULTIPLIER {: .copyable }
Called when a familiar's cached multiplier needs to be re-evaluated. Return a number to modify the multiplier. Modified values are passed to the next callback.

Effects such as BFFs or Hive Mind have already been applied at this point.

Note that the result of this callback is cached, so the callback only runs when needed. It will be triggered if an item with the `familiarmultiplier` "customcache" is added/removed (see [items.xml](xml/items.md)) or if `familiar:InvalidateCachedMultiplier()` is called.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1225 |MC_EVALUATE_FAMILIAR_MULTIPLIER {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, float Mult, [EntityPlayer](../EntityPlayer.md) Player) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_PRE_NPC_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1081 |MC_PRE_NPC_RENDER {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>[Vector](../Vector.md) Offset) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Vector](../Vector.md) or boolean |

### MC_PRE_NPC_SPLIT {: .copyable }
Called when the game is about to decide whether or not an [EntityNPC](../EntityNPC.md) can be split, ie the Meat Cleaver effect.

Return `true` to prevent the split, `false` to allow it even if blacklisted, or `nil` to continue default behavior.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1191 |MC_PRE_NPC_SPLIT {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, <br>boolean IsBlacklisted) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean |

### MC_PRE_OPENGL_RENDER {: .copyable }
Gets called right before the Manager::Render() function gets called.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1136 | MC_PRE_OPENGL_RENDER {: .copyable } | (VertexBuffer, <br>int shaderId, <br>RenderContext) | - | ? |

### MC_PRE_PAUSE_SCREEN_RENDER {: .copyable }
Can return `false` to prevent the pause screen from rendering. Doing so will also prevent the screen from darkening.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1218 |MC_PRE_PAUSE_SCREEN_RENDER {: .copyable } | ([Sprite](../Sprite.md) PauseBody, <br>[Sprite](../Sprite.md) PauseStats) | - | boolean |

### MC_POST_PAUSE_SCREEN_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1219 |MC_POST_PAUSE_SCREEN_RENDER {: .copyable } | ([Sprite](../Sprite.md) PauseBody, <br>[Sprite](../Sprite.md) PauseStats) | - | void |

### MC_POST_PICKUP_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1239 |MC_POST_PICKUP_COLLISION {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | void |

### MC_PRE_PICKUP_COMPOSTED {: .copyable }
This callback is called when a pickup is consumed with the compost active. 
Accepts `false` to cancel the pickup getting consumed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1267 |MC_PRE_PICKUP_VOIDED {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PICKUP_GET_COIN_VALUE {: .copyable }
Accepts `int` CoinValue to modify the amount of coins a coin can give you on pickup.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1250 |MC_PICKUP_GET_COIN_VALUE {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup) | [CoinSubType](https://wofsauge.github.io/IsaacDocs/rep/enums/CoinSubType.html) | int CoinValue |

### MC_PRE_PICKUP_MORPH {: .copyable }
Accepts a table of parameters: `{EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers}` or just `{EntityType, Variant, SubType}`. 

Returning `false` cancels the morph.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1213 |MC_PRE_PICKUP_MORPH {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>int EntityType, <br>int Variant, <br>int SubType, <br>boolean KeepPrice, <br>boolean KeepSeed, <br>boolean IgnoreModifiers) | - | table or boolean |

### MC_POST_PICKUP_MORPH {: .copyable }
Runs after the morph already happened.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1215 |MC_POST_PICKUP_MORPH {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>int PreviousType, <br>int PreviousVariant, <br>int SubType, <br>boolean KeptPrice, <br>boolean KeptSeed, <br>boolean IgnoredModifiers) | - | void |

### MC_PRE_PICKUP_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1083 |MC_PRE_PICKUP_RENDER {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>[Vector](../Vector.md) Offset) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.md) | [Vector](../Vector.md) or boolean |

### MC_POST_PICKUP_SHOP_PURCHASE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1062 |MC_POST_PICKUP_SHOP_PURCHASE {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>int MoneySpent) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.md) | void |

### MC_PRE_PICKUP_VOIDED_ABYSS {: .copyable }
This callback is called when a pickup is consumed with the abyss active. 
Accepts `false` to cancel the pickup getting consumed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1266 |MC_PRE_PICKUP_VOIDED_ABYSS {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PRE_PICKUP_VOIDED {: .copyable }
This callback is called when a pickup is consumed with the void active or black rune. The `IsBlackRune` parameter indicates the source.
If black rune is used, the callback gets fired both for pedestal items and for small pickups that turn into flies.
Accepts `false` to cancel the pickup getting consumed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1265 |MC_PRE_PICKUP_VOIDED {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>boolean IsBlackRune) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PRE_PLANETARIUM_APPLY_ITEMS {: .copyable }
After checking the amount of treasure rooms entered, the game applies flat item chances. This is where Crystal Ball, Magic 8 Ball and Sausage's chances get added, as well as Telescope Lens's additional 15% chance.

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1113 |MC_PRE_PLANETARIUM_APPLY_ITEMS {: .copyable } | (float Chance) | - | float |

### MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY {: .copyable }
After ensuring the stage is valid, the game then checks if a planetarium has been entered before. If so, the chance will be locked at 1% (10% with Telescope Lens).

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.

Accepts a `boolean`. Return `false` to bypass the planetarium enter penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1111 |MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY {: .copyable } | void | - | boolean |

### MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY {: .copyable }
This callback is run at the start of planetarium calculation. Before running calculation, the game first checks if the current floor is valid to spawn a planetarium. If the current floor is invalid, all further calculation (and thus, all further callbacks) will be canceled.

By default, planetariums cannot spawn past Depths II (Womb II with Telescope Lens).

This callback can be used, for example, to add custom planetarium spawn rules on custom floors, or to add new items like Telescope Lens which can augment the rules.

Accepts a `boolean`. Return `false` to bypass the planetarium stage penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1110 |MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY {: .copyable } | void | - | boolean |

### MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS {: .copyable }
Finally, after checking all of the above, Telescope Lens adds an additional 9% chance to the base chance, bringing the base generation chance to 10%.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1114 |MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS {: .copyable } | (float Chance) | - | float |

### MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY {: .copyable }
After ensuring planetariums haven't been entered before, the game then checks how many treasure rooms have been entered. If the amount of treasure rooms entered is greater than or equal to the current stage number, the chance will be locked at 1% (10% with Telescope Lens).

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.


Accepts a `boolean`. Return `false` to bypass the planetarium treasure room penalty entirely, meaning the game will act as if no treasure rooms have been entered.

Alternatively accepts an `int` to modify how many treasure rooms the game will believe has been entered.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1112 |MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY {: .copyable } | (int TreasureRoomsVisited) | - | boolean or int |

### MC_POST_PLANETARIUM_CALCULATE {: .copyable }
This will override *all* previous calculation values, ultimately dictating the planetarium chance.

Accepts a `float` to modify the chance.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1115 |MC_POST_PLANETARIUM_CALCULATE {: .copyable } | (float Chance) | - | float |

### MC_PRE_PLAYERHUD_RENDER_ACTIVE_ITEM {: .copyable }
Accepts `true` to cancel rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1119 |MC_PRE_PLAYERHUD_RENDER_ACTIVE_ITEM {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, <br>[Vector](../Vector.md) Offset, <br>float Alpha, <br>float Scale, <br>[Vector](../Vector.md) ChargeBarOffset) | - | boolean |

### MC_POST_PLAYERHUD_RENDER_ACTIVE_ITEM {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1079 |MC_POST_PLAYERHUD_RENDER_ACTIVE_ITEM {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, <br>[Vector](../Vector.md) Offset, <br>float Alpha, <br>float Scale, <br>[Vector](../Vector.md) ChargeBarOffset) | - | void |

### MC_PRE_PLAYERHUD_RENDER_HEARTS {: .copyable }
Return `true` to cancel hearts HUD rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1118 |MC_PRE_PLAYERHUD_RENDER_HEARTS {: .copyable } | ([Vector](../Vector.md) Offset(?), <br>[Sprite](../Sprite.md) HeartsSprite, <br>[Vector](../Vector.md) Position, <br>float Unknown, <br>[EntityPlayer](../EntityPlayer.md) Player) | - | boolean |

### MC_POST_PLAYERHUD_RENDER_HEARTS {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1091 |MC_POST_PLAYERHUD_RENDER_HEARTS {: .copyable } | ([Vector](../Vector.md) Offset(?), <br>[Sprite](../Sprite.md) HeartsSprite, <br>[Vector](../Vector.md) Position, <br>float Unknown, <br>[EntityPlayer](../EntityPlayer.md) Player) | - | void |

### MC_PRE_PLAYERHUD_TRINKET_RENDER {: .copyable }

Accepts returning a table, with any combination of the following fields:

* Position - Alters trinket position.
* Scale - Alters trinket scale.
* CropOffset - Alters the crop on the sprite on its spritesheet. Allows alternate sprites for the same trinket rendered this way, such as Monkey Paw.

Alternative accepts `true`, which cancels trinket rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1264 |MC_PRE_PLAYERHUD_TRINKET_RENDER {: .copyable } | (int Slot, <br>[Vector](../Vector.md) Position, <br>float Scale, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>[Vector](../Vector.md) CropOffset) | int Slot | table or boolean |

### MC_PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1092 |MC_PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM {: .copyable } | (int ModCount, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>[CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, <br>boolean OnlyCountTrueItems) | - | int |

### MC_POST_PLAYER_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1231 |MC_POST_PLAYER_COLLISION {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [PlayerVariant](PlayerVariant.md) | void |

### MC_PLAYER_GET_ACTIVE_MAX_CHARGE {: .copyable }
Accepts an integer to change chargebar for the active item.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1072 |MC_PLAYER_GET_ACTIVE_MAX_CHARGE {: .copyable } | ([CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>int VarData, <br>int CurrentMaxCharge) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | int |

### MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE {: .copyable }
Accepts an integer to change the minimum charge to use the active item. If the item currently has the minimum amount of charge, it'll also show the white outline.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1073 |MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE {: .copyable } | ([ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, [EntityPlayer](../EntityPlayer.md) Player, int CurrentMinUsableCharge) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | int |

### MC_PLAYER_GET_HEALTH_TYPE {: .copyable }
Accepts an [HealthType](HealthType.md) to change health type for the character.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1067 |MC_PLAYER_GET_HEALTH_TYPE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [HealthType](HealthType.md) |

### MC_PLAYER_GET_HEART_LIMIT {: .copyable }
Accepts an override `integer` for heart limit.

???- info
    You can set the limit to any arbitrary amount but the game can only render up to 4 lines of hearts in the HUD. However, even if they're not visible, the hearts still work properly.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1074 |MC_PLAYER_GET_HEART_LIMIT {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>int HeartLimit, <br>boolean IsKeeper) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | int |

### MC_POST_PLAYER_GET_MULTI_SHOT_PARAMS {: .copyable }
Return a [MultiShotParams](../MultiShotParams.md) object to change the properties of the players shooting behavior in regards to the MultiShotParams object properties.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1251 |MC_POST_PLAYER_GET_MULTI_SHOT_PARAMS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [MultiShotParams](../MultiShotParams.md) |

### MC_PLAYER_INIT_POST_LEVEL_INIT_STATS {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1042 |MC_PLAYER_INIT_POST_LEVEL_INIT_STATS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_PLAYER_INIT_PRE_LEVEL_INIT_STATS {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1127 |MC_PLAYER_INIT_PRE_LEVEL_INIT_STATS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_POST_FORCE_ADD_PILL_EFFECT {: .copyable }

Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1129 |MC_POST_FORCE_ADD_PILL_EFFECT {: .copyable } | [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html), [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | - | void |

### MC_POST_PLAYER_NEW_LEVEL {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1078 |MC_POST_PLAYER_NEW_LEVEL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_POST_PLAYER_NEW_ROOM_TEMP_EFFECTS {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1077 |MC_POST_PLAYER_NEW_ROOM_TEMP_EFFECTS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_PRE_PLAYER_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1082 |MC_PRE_PLAYER_RENDER {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[Vector](../Vector.md) Offset) | [PlayerVariant](PlayerVariant.md) | [Vector](../Vector.md) or boolean |

### MC_PRE_PLAYER_TAKE_DMG {: .copyable }
Runs earlier than MC_ENTITY_TAKE_DMG, and even if the player is considered invincible or has Holy Mantles.

Only accepts returning false to cancel the damage. Good for giving the player invincibility that takes precedence over other damage negation effects, such as Holy Mantle.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1008 |MC_PRE_PLAYER_TAKE_DMG {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>float Damage, <br>[DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, <br>[EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, <br>int DamageCountdown) | [PlayerVariant](PlayerVariant.md) | boolean |

### MC_PRE_PLAYER_ADD_HEARTS {: .copyable }
Runs before Add(...)Hearts functions, allows returning a value to change the given amount of health. Functions that include an argument (e.g. ignoreKeeper for AddMaxHearts) provide the value using OptionalArg.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1009 |MC_PRE_PLAYER_ADD_HEARTS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>int Amount, <br>[AddHealthType](AddHealthType.md) AddHealthType, <br>boolean OptionalArg) | [AddHealthType](AddHealthType.md) | int |

### MC_POST_PLAYER_ADD_HEARTS {: .copyable }
Runs after Add(...)Hearts functions and MC_PRE_PLAYER_ADD_HEARTS callback. Functions that include an argument (e.g. ignoreKeeper for AddMaxHearts) provide the value using OptionalArg.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1010 |MC_POST_PLAYER_ADD_HEARTS {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>int Amount, <br>[AddHealthType](AddHealthType.md) AddHealthType, <br>boolean OptionalArg) | [AddHealthType](AddHealthType.md) | void |

### MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR {: .copyable }
Accepts `false` to cancel trigger effects.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1069 |MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerVariant](PlayerVariant.md) | boolean |

### MC_PRE_PLAYER_USE_BOMB {: .copyable }
Return `false` to stop the player from using a bomb.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1020 |MC_PRE_PLAYER_USE_BOMB {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerVariant](PlayerVariant.md) | boolean |

### MC_POST_PLAYER_USE_BOMB {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1021 |MC_POST_PLAYER_USE_BOMB {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[EntityBomb](../EntityBomb.md) Bomb) | [PlayerVariant](PlayerVariant.md) | void |

### MC_POST_PROJECTILE_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1245 |MC_POST_PROJECTILE_COLLISION {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | void |

### MC_PRE_PROJECTILE_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1085 |MC_PRE_PROJECTILE_RENDER {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile, <br>[Vector](../Vector.md) Offset) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.md) | [Vector](../Vector.md) or boolean |

### MC_PRE_RENDER_CUSTOM_CHARACTER_MENU {: .copyable }
Accepts no return parameters.

???- info "Execution informations"
    This callback only triggers when a custom character is selected, it doesn't trigger on regular characters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1333 |MC_PRE_RENDER_CUSTOM_CHARACTER_MENU {: .copyable } | ([PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType, <br>[Vector](../Vector.md) RenderPos, <br>[Sprite](../Sprite.md) DefaultSprite) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_PRE_RENDER_ENTITY_LIGHTING {: .copyable }
Accepts an override [Vector](../Vector.md) for Offset.

Alternatively accepts `false` to stop the rendering.

 |ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1152 |MC_PRE_RENDER_ENTITY_LIGHTING {: .copyable } | ([Entity](../Entity.md) Entity, <br>[Vector](../Vector.md) Offset) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Vector](../Vector.md) or boolean |

### MC_PRE_RENDER_GRID_LIGHTING {: .copyable }
Accepts an override [Vector](../Vector.md) for Offset.

Alternatively accepts `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1151 |MC_PRE_RENDER_GRID_LIGHTING {: .copyable } | ([GridEntity](../GridEntity.md) GridEntity, <br>[Vector](../Vector.md) Offset) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](../Vector.md) or boolean |
 
### MC_PRE_RENDER_PLAYER_BODY {: .copyable }
Accepts an override `vector` for RenderPos.

Alternatively accepts a `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1039 |MC_PRE_RENDER_PLAYER_BODY {: .copyable } | ([EntityPlayer](../EntityPlayer.md) player, <br>[Vector](../Vector.md) RenderPos) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [Vector](../Vector.md) or boolean |

### MC_PRE_RENDER_PLAYER_HEAD {: .copyable }
Accepts an override [Vector](../Vector.md) for RenderPos.

Alternatively accepts a `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1038 |MC_PRE_RENDER_PLAYER_HEAD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) player, <br>[Vector](../Vector.md) RenderPos) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [Vector](../Vector.md) or boolean |

### MC_PRE_RENDER {: .copyable }
Gets called right before the Manager::Render() function gets called.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1135 | MC_PRE_RENDER {: .copyable } | void | - | void |

### MC_PRE_REPLACE_SPRITESHEET {: .copyable }
Accepts a table of parameters: `{int LayerID, string PNGFilename}`

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1116 |MC_PRE_REPLACE_SPRITESHEET {: .copyable } | (int LayerID, <br>string PNGFilename) | string ANM2Filename | table |

### MC_POST_REPLACE_SPRITESHEET {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1117 |MC_POST_REPLACE_SPRITESHEET {: .copyable } | (int LayerID, <br>string PNGFilename) | string ANM2Filename | void |

### MC_PRE_RESTOCK_SHOP {: .copyable }
Accepts `false` to cancel the restock, blocking shop rerolls from restock machines or restocks from Restock altogether.

???- info "Partial"
    This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1070 |MC_PRE_RESTOCK_SHOP {: .copyable } | (boolean Partial) | - | void |

### MC_POST_RESTOCK_SHOP {: .copyable }
Accepts no return parameters.

???- info "Partial"
    This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1071 |MC_POST_RESTOCK_SHOP {: .copyable } | (boolean Partial) | - | void |

### MC_PRE_ROOM_EXIT {: .copyable }
Accepts no return parameters.

???- info "NewLevel"
    NewLevel returns `true` when entering a new level or when exiting a run.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1043 |MC_PRE_ROOM_EXIT {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>boolean NewLevel) | - | void |

### MC_PRE_ROOM_GRID_ENTITY_SPAWN {: .copyable }
This is called during Room initiization when [GridEntities](../GridEntity.md) from the layout are being spawned.

Accepts `false` to cancel spawning the grid or a table of `{Type, Variant, Vardata, SpawnSeed}` to modify it.

???+ warning "Warning"
    This does _not_ fire for random decorations spawned by the game! Use `MC_PRE_GRID_ENTITY_SPAWN` for these.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1192 |MC_PRE_ROOM_GRID_ENTITY_SPAWN {: .copyable } | ([GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, <br>int Variant, <br>int VarData, <br>int GridIdx, <br>int SpawnSeed) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type | boolean or table |

### MC_PRE_ROOM_TRIGGER_CLEAR {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1068 |MC_PRE_ROOM_TRIGGER_CLEAR {: .copyable } | (boolean PlaySound) | - | void |

### MC_POST_SAVESLOT_LOAD {: .copyable }
This is called wheenver a saveslot is loaded by the game.

This is the callback you should use to handle savedata loads, ideally, from normal Mod::LoadData to Repentogon Marks/Achievement checks, since it's the callback that triggers when those are loaded.

The first parameter is the slot you should care about, the 2nd parameter (isslotselected) indicates if the slot that is being loaded has actually been selected from the save menu screen (you can limit your save handling to when this is true if you want to get fancy), and the 3rd parameter(rawslot) is the actual saveslot the game uses (not the one the api uses since it can be 0!).

???+ warning "Warning"
    This callback is called many times before a run is started, either from changing saveslots naturally or because the game doesnt give a damn, so code accounting for that, clearing previous data when necessary.
	The 3rd param is actually only useful to check for the 0 slot state, which is the one the game defaults to before the actual slot is loaded by the player. When on this state, the moddata and the gamedata WONT BE IN SYNC (moddata is slot 1, while vanilla game data is 3)

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1470 |MC_POST_SAVESLOT_LOAD {: .copyable } | (int saveslot, <br>boolean isslotselected, <br>int rawslot) | void | - | void |

### MC_PRE_SFX_PLAY {: .copyable }
Accepts a table of parameters: `{ID, Volume, FrameDelay, Loop, Pitch, Pan}`

Alternatively accepts `false` to cancel the sound.

???- example "Example Code"
    This code will forcibly loop every sound (for better or worse).
    ```lua
    function mod:myFunction(ID, Volume, FrameDelay, Loop, Pitch, Pan)
        return {ID, Volume, FrameDelay, true, Pitch, Pan}
    end
    mod:AddCallback(ModCallbacks.MC_PRE_SFX_PLAY, mod.myFunction)
    ```

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1030 |MC_PRE_SFX_PLAY {: .copyable } | (int ID, <br>float Volume, <br>int FrameDelay, <br>boolean Loop, <br>float Pitch, <br>float Pan) | [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/SoundEffect.html) | table or boolean |

### MC_POST_SFX_PLAY {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1031 |MC_POST_SFX_PLAY {: .copyable } | (int ID, <br>float Volume, <br>int FrameDelay, <br>boolean Loop, <br>float Pitch, <br>float Pan) | [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/SoundEffect.html) | void |

### MC_PRE_SLOT_COLLISION {: .copyable }
Just like the collision callbacks in the vanilla API, the Low value is true if the entity collided with the collider first, and false if the opposite is true.

Return `true` to ignore collision, `false` to collide but not execute internal code.

Also accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1240 |MC_PRE_SLOT_COLLISION {: .copyable } | ([EntitySlot](../EntitySlot.md), <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [SlotVariant](SlotVariant.md) | boolean |

### MC_POST_SLOT_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1241 |MC_POST_SLOT_COLLISION {: .copyable } | ([EntitySlot](../EntitySlot.md), <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_CREATE_EXPLOSION_DROPS {: .copyable }
Return `false` to stop explosions from dropping the standard consumables. This is useful, for example, to allow custom slots to drop their own loot on explosion.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1123 |MC_PRE_SLOT_CREATE_EXPLOSION_DROPS {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | boolean |

### MC_POST_SLOT_CREATE_EXPLOSION_DROPS {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1124 |MC_POST_SLOT_CREATE_EXPLOSION_DROPS {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_POST_SLOT_INIT {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1121 |MC_POST_SLOT_INIT {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1089 |MC_PRE_SLOT_RENDER {: .copyable } | ([EntitySlot](../EntitySlot.md) Slot, <br>[Vector](../Vector.md) Offset) | [SlotVariant](SlotVariant.md) | [Vector](../Vector.md) or boolean |

### MC_POST_SLOT_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1090 |MC_POST_SLOT_RENDER {: .copyable } | ([EntitySlot](../EntitySlot.md) Slot, <br>[Vector](../Vector.md) Offset) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable }
Used by Shell Game, Hell Game, and Crane Game.

Accepts a [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) to override what the game will pay out with.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1125 |MC_PRE_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable } | ([EntitySlot](../EntitySlot.md), <br>[CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type) | [SlotVariant](SlotVariant.md) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) |

### MC_POST_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1126 |MC_POST_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable } | ([EntitySlot](../EntitySlot.md), <br>[CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1169 |MC_PRE_SLOT_UPDATE {: .copyable } | ([EntitySlot](../EntitySlot.md) Slot) | [SlotVariant](SlotVariant.md) | boolean |

### MC_POST_SLOT_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1122 |MC_POST_SLOT_UPDATE {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_POST_TEAR_COLLISION {: .copyable }
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1233 |MC_POST_TEAR_COLLISION {: .copyable } | ([EntityTear](../EntityTear.md) Tear, <br>[Entity](../Entity.md) Collider, <br>boolean Low) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | void |

### MC_PRE_TEAR_RENDER {: .copyable }
Accepts a [Vector](../Vector.md) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1084 |MC_PRE_TEAR_RENDER {: .copyable } | ([EntityTear](../EntityTear.md) Tear, <br>[Vector](../Vector.md) Offset) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.md) | [Vector](../Vector.md) or boolean |

### MC_POST_TRIGGER_COLLECTIBLE_REMOVED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1095 |MC_POST_TRIGGER_COLLECTIBLE_REMOVED {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | void |

### MC_PRE_TRIGGER_PLAYER_DEATH {: .copyable }
Fires right before the game over screen, but BEFORE the game checks for vanilla revive effects like 1UP.

Return `false` or call `player:Revive()` to cancel the death, reviving the player in-place with half a heart. This will also prevent later callbacks from running.

???- warning "Warning"
    Returning false or calling `player:Revive()` may remove the current run's ability to save. This occurs because the game immediately deletes the save file during the death animation if there's no pending revives, and this callback doesn't fire fast enough to register a revive on death as it occurs right before the game over screen.

    In order to prevent this, only attempt to revive the player if they have an item or effect with REPENTOGON's "revive" customtag, which allows the item/effect to count as an extra life on the HUD and prevents the game from deleting the run save upon player death. See the page on [items.xml](../xml/items.md) for more information and some example XML/code.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1050 |MC_PRE_TRIGGER_PLAYER_DEATH {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | - | boolean |

### MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES {: .copyable }
Fires right before the game over screen, but AFTER the game checks for vanilla revive effects like 1UP.

Return `false` or call `player:Revive()` to cancel the death, reviving the player in-place with half a heart. This will also prevent later callbacks from running.

???- warning "Warning"
    Returning false or calling `player:Revive()` may remove the current run's ability to save. This occurs because the game immediately deletes the save file during the death animation if there's no pending revives, and this callback doesn't fire fast enough to register a revive on death as it occurs right before the game over screen.

    In order to prevent this, only attempt to revive the player if they have an item or effect with REPENTOGON's "revive" customtag, which allows the item/effect to count as an extra life on the HUD and prevents the game from deleting the run save upon player death. See the page on [items.xml](../xml/items.md) for more information and some example XML/code.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1051 |MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | - | boolean |

### MC_PRE_PLAYER_REVIVE {: .copyable }
Called before the player revives.

Return `false` to cancel it."

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1481 |MC_PRE_PLAYER_REVIVE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType]([https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | boolean |

### MC_POST_PLAYER_REVIVE {: .copyable }
Called after the player revives, assuming it wasn't cancelled.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1482 |MC_POST_PLAYER_REVIVE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_POST_TRIGGER_TRINKET_ADDED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1096 |MC_POST_TRIGGER_TRINKET_ADDED {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Type, <br>boolean FirstTimePickingUp) | [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) | void |

### MC_POST_TRIGGER_TRINKET_REMOVED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1097 |MC_POST_TRIGGER_TRINKET_REMOVED {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Type) | [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) | void |

### MC_POST_TRIGGER_WEAPON_FIRED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1098 |MC_POST_TRIGGER_WEAPON_FIRED {: .copyable } | ([Vector](../Vector.md) FireDirection, <br>int FireAmount, <br>[Entity](../Entity.md) Owner, <br>[Weapon](../Weapon.md) Weapon) | [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) | void |

### MC_PRE_USE_CARD {: .copyable }
Accepts `true` to cancel card use.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1064 |MC_PRE_USE_CARD {: .copyable } | ([Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.md) ID, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>int UseFlag) | - | boolean |

### MC_PRE_USE_PILL {: .copyable }
Accepts `true` to cancel pill use.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1065 |MC_PRE_USE_PILL {: .copyable } | ([PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.md) ID, <br>[PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.md) PillColor, <br>[EntityPlayer](../EntityPlayer.md) Player, <br>int UseFlag) | - | boolean |

### MC_POST_WEAPON_FIRE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1105 |MC_POST_WEAPON_FIRE {: .copyable } | ([Weapon](../Weapon.md) Weapon, <br>[Vector](../Vector.md) FireDirection, <br>boolean IsShooting, <br>boolean IsInterpolated) | [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) | void |

### MC_PRE_PICKUP_GET_LOOT_LIST {: .copyable }
Called before the pickup determines its loot content. Accepts a [LootList](../LootList.md) to change loot content.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1333 |MC_PRE_PICKUP_GET_LOOT_LIST {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, <br>boolean ShouldAdvance) | - | [LootList](../LootList.md) |

### MC_PRE_PICKUP_UPDATE_GHOST_PICKUPS {: .copyable }
Called before a ghost pickup of the loot content is applied to the pickup. Return `true` to apply pickup ghost to your pickup entity, `false` to cancel it.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1334 |MC_PRE_PICKUP_UPDATE_GHOST_PICKUPS {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_POST_PLAYER_TRIGGER_EFFECT_REMOVED {: .copyable }
Called after the player's `ItemConfigItem` TemporaryEffect is removed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1268 |MC_POST_PLAYER_TRIGGER_EFFECT_REMOVED {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, <br>[ItemConfigItem][../ItemConfig_Item.md]) | - | void |

### MC_POST_ROOM_TRIGGER_EFFECT_REMOVED {: .copyable }
Called after the room's [TemporaryEffects](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.md) is removed.

[Room](../Room.md) has its own [TemporaryEffects](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.md) that are accessed through [Room::GetEffects()](../Room.md#geteffects)

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1269 |MC_POST_ROOM_TRIGGER_EFFECT_REMOVED {: .copyable } | ([ItemConfigItem][../ItemConfig_Item.md]) | - | void |

### MC_PRE_PLAYER_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1171 |MC_PRE_PLAYER_GRID_COLLISION {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [PlayerVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerVariant.html) | boolean |

### MC_PLAYER_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1172 |MC_PLAYER_GRID_COLLISION {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [PlayerVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerVariant.html) | boolean |

### MC_PRE_TEAR_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1173 |MC_PRE_TEAR_GRID_COLLISION {: .copyable } | ([EntityTear](../EntityTear.md) Tear, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | boolean |

### MC_TEAR_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1174 |MC_TEAR_GRID_COLLISION {: .copyable } | ([EntityTear](../EntityTear.md) Tear, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | boolean |

### MC_PRE_FAMILIAR_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1175 |MC_PRE_FAMILIAR_GRID_COLLISION {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | boolean |

### MC_FAMILIAR_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1176 |MC_FAMILIAR_GRID_COLLISION {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | boolean |

### MC_PRE_BOMB_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1177 |MC_PRE_BOMB_GRID_COLLISION {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | boolean |

### MC_BOMB_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1178 |MC_BOMB_GRID_COLLISION {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | boolean |

### MC_PRE_PICKUP_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1179 |MC_PRE_PICKUP_GRID_COLLISION {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PICKUP_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1180 |MC_PICKUP_GRID_COLLISION {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PRE_PROJECTILE_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1181 |MC_PRE_PROJECTILE_GRID_COLLISION {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | boolean |

### MC_PROJECTILE_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1182 |MC_PROJECTILE_GRID_COLLISION {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | boolean |

### MC_PRE_NPC_GRID_COLLISION {: .copyable }
Called before this entity collides with a [GridEntity](../GridEntity.md) or other solid grid tile.

Return `true` to ignore collision.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1183 |MC_PRE_NPC_GRID_COLLISION {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean |

### MC_NPC_GRID_COLLISION {: .copyable }
Called when this entity collides with the grid, assuming it wasn't skipped.

???+ warning "Warning"
    `GridEntity` can be nil, as entities may collide with "empty" grid indexes if the [GridPath](https://wofsauge.github.io/IsaacDocs/rep/Room.html#getgridpath) value is >= 1000.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1184 |MC_NPC_GRID_COLLISION {: .copyable } | ([EntityNPC](../EntityNPC.md) NPC, int GridIndex, [GridEntity](../GridEntity.md) GridEntity) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean |

### MC_POST_PROJECTILE_DEATH {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1032 |MC_POST_PROJECTILE_DEATH {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | void |

### MC_POST_TEAR_DEATH {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1032 |MC_POST_TEAR_DEATH {: .copyable } | ([EntityTear](../EntityTear.md) Tear) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | void |

### MC_POST_BOSS_INTRO_SHOW {: .copyable }
Accepts no return parameters.

Called right after the boss intro is initialized. `BossID2` is for Double Trouble.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1270 |MC_POST_BOSS_INTRO_SHOW {: .copyable } | ([BossType](BossType.md) BossID1, [BossType](BossType.md) BossID2) | - | void |

### MC_POST_ROOM_TRANSITION_UPDATE {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1271 |MC_POST_ROOM_TRANSITION_UPDATE {: .copyable } | void | int TransitionMode | void |

### MC_POST_ROOM_TRANSITION_RENDER {: .copyable }

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1272 |MC_POST_ROOM_TRANSITION_RENDER {: .copyable } | void | int TransitionMode | void |

### MC_PRE_PLAYER_ADD_COSTUME {: .copyable }
Called before the costume is added to the player. Return [ItemConfigItem](../ItemConfig_Item.md) to replace the costume or `true` to cancel adding it entirely.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1281 |MC_PRE_PLAYER_ADD_COSTUME {: .copyable } | ([ItemConfigItem](../ItemConfig_Item.md) ItemConfig, [EntityPlayer](../EntityPlayer.md) Player, boolean ItemStateOnly) | - | [ItemConfigItem](../ItemConfig_Item.md) ItemConfig or boolean |

### MC_PRE_PLAYER_REMOVE_COSTUME {: .copyable }
Called before the costume is added to the player. Return `true` to cancel the removal.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1282 |MC_PRE_PLAYER_ADD_COSTUME {: .copyable } | ([ItemConfigItem](../ItemConfig_Item.md) ItemConfig, [EntityPlayer](../EntityPlayer.md) Player, boolean ItemStateOnly) | - | boolean |

### MC_POST_PLAYER_ADD_COSTUME {: .copyable }
Called after the costume is added to the player, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1283 |MC_POST_PLAYER_ADD_COSTUME {: .copyable } | ([ItemConfigItem](../ItemConfig_Item.md) ItemConfig, [EntityPlayer](../EntityPlayer.md) Player, boolean ItemStateOnly) | - | void |

### MC_POST_PLAYER_REMOVE_COSTUME {: .copyable }
Called after the costume is added to the player, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1284 |MC_POST_PLAYER_ADD_COSTUME {: .copyable } | ([ItemConfigItem](../ItemConfig_Item.md) ItemConfig, [EntityPlayer](../EntityPlayer.md) Player, boolean ItemStateOnly) | - | void |

### MC_PRE_PLAYER_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1160 |MC_PRE_PLAYER_UPDATE {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player) | [PlayerVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerVariant.html) | boolean |

### MC_PRE_TEAR_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1161 |MC_PRE_TEAR_UPDATE {: .copyable } | ([EntityTear](../EntityTear.md) Tear) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | boolean |

### MC_PRE_FAMILIAR_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1162 |MC_PRE_FAMILIAR_UPDATE {: .copyable } | ([EntityFamiliar](../EntityFamiliar.md) Familiar) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | boolean |

### MC_PRE_BOMB_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1163 |MC_PRE_BOMB_UPDATE {: .copyable } | ([EntityBomb](../EntityBomb.md) Bomb) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | boolean |

### MC_PRE_PICKUP_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1164 |MC_PRE_PICKUP_UPDATE {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean |

### MC_PRE_KNIFE_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1165 |MC_PRE_KNIFE_UPDATE {: .copyable } | ([EntityKnife](../EntityKnife.md) Knife) | [KnifeSubType](KnifeSubType.md) | boolean |

### MC_PRE_PROJECTILE_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1166 |MC_PRE_PROJECTILE_UPDATE {: .copyable } | ([EntityProjectile](../EntityProjectile.md) Projectile) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | boolean |

### MC_PRE_LASER_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1167 |MC_PRE_LASER_UPDATE {: .copyable } | ([EntityLaser](../EntityLaser.md) Laser) | [LaserVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/LaserVariant.html) | boolean |

### MC_PRE_EFFECT_UPDATE {: .copyable }
Called before this entity is updated.

Return `true` if the internal AI should be ignored, `false` or `nil`/nothing otherwise.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1168 |MC_PRE_EFFECT_UPDATE {: .copyable } | ([EntityEffect](../EntityEffect.md) Effect) | [EffectVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/EffectVariant.html) | boolean |

### MC_PRE_FORTUNE_DISPLAY {: .copyable }
Called before a fortune is displayed.

Return `false` to cancel showing the fortune.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1483 |MC_PRE_FORTUNE_DISPLAY {: .copyable } | void | - | boolean |

### MC_PRE_ITEM_TEXT_DISPLAY {: .copyable }
Called before an item text is displayed.

`IsSticky` is `true` is if the item text stays on screen for an indefinite amount of time, namely when holding down the map key. `IsCurseDisplay` is `true` if the text is for displaying a curse.

Return `false` to cancel showing the item text.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1484 |MC_PRE_ITEM_TEXT_DISPLAY {: .copyable } | (string Title, string Subtitle, boolean IsSticky, boolean IsCurseDisplay) | - | boolean |

### MC_PRE_GET_RANDOM_ROOM_INDEX {: .copyable }
Called when the game wishes to get a random available room index on the floor.

Return an integer to override the target room index.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1290 |MC_PRE_FORTUNE_DISPLAY {: .copyable } | (int RoomIndex, bool IAmErrorRoom, int Seed) | - | int |

### MC_PRE_GLOWING_HOURGLASS_SAVE {: .copyable }
Called after the Glowing Hourglass state is saved.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1302 |MC_PRE_GLOWING_HOURGLASS_SAVE {: .copyable } | (int Slot) | - | void |

### MC_POST_GLOWING_HOURGLASS_SAVE {: .copyable }
Called after the Glowing Hourglass state is saved.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1300 |MC_POST_GLOWING_HOURGLASS_SAVE {: .copyable } | (int Slot) | - | void |

### MC_PRE_GLOWING_HOURGLASS_LOAD {: .copyable }
Called after the Glowing Hourglass state is loaded.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1303 |MC_PRE_GLOWING_HOURGLASS_LOAD {: .copyable } | (int Slot) | - | void |

### MC_POST_GLOWING_HOURGLASS_LOAD {: .copyable }
Called after the Glowing Hourglass state is loaded.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1301 |MC_POST_GLOWING_HOURGLASS_LOAD {: .copyable } | (int Slot) | - | void |

### MC_PRE_PLAYER_ADD_CARD {: .copyable }
Called before a card gets added to the player's inventory.

Return [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) to change the card being added or `false` to cancel adding it entirely.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1350 |MC_PRE_PLAYER_ADD_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) CardID, [PillCardSlot](PillCardSlot.md) Slot) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | boolean or int |

### MC_POST_PLAYER_ADD_CARD {: .copyable }
Called after a card gets added to the player's inventory.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1351 |MC_POST_PLAYER_ADD_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) CardID, [PillCardSlot](PillCardSlot.md) Slot) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | void |

### MC_PRE_PLAYER_ADD_PILL {: .copyable }
Called before a pill gets added to the player's inventory.

Return [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) to change the pill being added or `false` to cancel adding it entirely.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1352 |MC_PRE_PLAYER_ADD_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) PillColor, [PillCardSlot](PillCardSlot.md) Slot) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | boolean or int |

### MC_POST_PLAYER_ADD_PILL {: .copyable }
Called after a pill gets added to the player's inventory.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1353 |MC_POST_PLAYER_ADD_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) PillColor, [PillCardSlot](PillCardSlot.md) Slot) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | void |

### MC_POST_PLAYER_REMOVE_CARD {: .copyable }
Called after a card gets removed from the player's inventory by any means (dropped, direct removal, use, etc).

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1354 |MC_POST_PLAYER_REMOVE_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) CardID, [PillCardSlot](PillCardSlot.md) Slot) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | void |

### MC_POST_PLAYER_REMOVE_PILL {: .copyable }
Called after a pill gets removed from the player's inventory by any means (dropped, direct removal, use, etc).

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1355 |MC_POST_PLAYER_REMOVE_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) CardID, [PillCardSlot](PillCardSlot.md) Slot) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | void |

### MC_PRE_PLAYER_COLLECT_CARD {: .copyable }
Called before a player picks up a card off the ground.

Return `false` to prevent the card from being picked up.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1356 |MC_PRE_PLAYER_COLLECT_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | boolean |

### MC_POST_PLAYER_COLLECT_CARD {: .copyable }
Called after a player picks up a card off the ground.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1357 |MC_POST_PLAYER_COLLECT_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | void |

### MC_PRE_PLAYER_COLLECT_PILL {: .copyable }
Called before a player picks up a pill off the ground.

Return `false` to prevent the pill from being picked up.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1358 |MC_PRE_PLAYER_COLLECT_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | boolean |

### MC_POST_PLAYER_COLLECT_PILL {: .copyable }
Called after a player picks up a pill off the ground.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1359 |MC_POST_PLAYER_COLLECT_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | void |

### MC_POST_PLAYER_DROP_CARD {: .copyable }
Called after a player drops a card onto the ground from their inventory.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1360 |MC_POST_PLAYER_DROP_CARD {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup, [PillCardSlot](PillCardSlot.md) Slot) | [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) | void |

### MC_POST_PLAYER_DROP_PILL {: .copyable }
Called after a player drops a pill onto the ground from their inventory.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1361 |MC_POST_PLAYER_DROP_PILL {: .copyable } | ([EntityPlayer](../EntityPlayer.md) Player, [EntityPickup](../EntityPickup.md) Pickup, [PillCardSlot](PillCardSlot.md) Slot) | [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) | void |