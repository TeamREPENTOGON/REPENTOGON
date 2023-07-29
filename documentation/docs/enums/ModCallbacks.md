# Enum "ModCallbacks"

This is a list of all new callbacks added by REPENTOGON.

## Modified Old Callbacks
### MC_USE_PILL
MC_USE_PILL now passes PillColor as an argument.
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1064 |MC_USE_PILL {: .copyable } | ([PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) Effect, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [UseFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/UseFlags.html) Flags, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/PillColor.html) Color) | [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) | void |

### MC_PRE_PLAYER_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1065 |MC_PRE_PLAYER_COLLISION {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [PlayerVariant](PlayerVariant.md) | boolean or table |

### MC_PRE_TEAR_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1232 |MC_PRE_TEAR_COLLISION {: .copyable } | ([EntityTear](https://wofsauge.github.io/IsaacDocs/rep/EntityTear.html) Tear, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | boolean or table |

### MC_PRE_FAMILIAR_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

For example, you can return `{ Collide=true }` to make a familiar to collide physically with something (like an enemy, which they don't normally do) without skipping on-collision code (like you would by returning `false`).

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1234 |MC_PRE_FAMILIAR_COLLISION {: .copyable } | ([EntityFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html) Familiar, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | boolean or table |

### MC_PRE_BOMB_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1236 |MC_PRE_BOMB_COLLISION {: .copyable } | ([EntityBomb](https://wofsauge.github.io/IsaacDocs/rep/EntityBomb.html) Bomb, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | boolean or table |

### MC_PRE_PICKUP_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1238 |MC_PRE_PICKUP_COLLISION {: .copyable } | ([EntityPickup](https://wofsauge.github.io/IsaacDocs/rep/EntityPickup.html) Pickup, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | boolean or table |

### MC_PRE_KNIFE_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1242 |MC_PRE_KNIFE_COLLISION {: .copyable } | ([EntityKnife](https://wofsauge.github.io/IsaacDocs/rep/EntityKnife.html) Knife, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [KnifeSubType](KnifeSubType.md) | boolean or table |

### MC_PRE_PROJECTILE_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1244 |MC_PRE_PROJECTILE_COLLISION {: .copyable } | ([EntityProjectile](https://wofsauge.github.io/IsaacDocs/rep/EntityProjectile.html) Projectile, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | boolean or table |

### MC_PRE_NPC_COLLISION
Now optionally accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity, such as dealing collision damage. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1246 |MC_PRE_NPC_COLLISION {: .copyable } | ([EntityNPC](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html) NPC, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | boolean or table |


## New Callbacks
### MC_PRE_ADD_COLLECTIBLE
Accepts a table of parameters: `{Type, Charge, FirstTime, Slot, VarData, Player}`

Alternatively accepts a [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) to change the type without changing any other parameters

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
|1004 |MC_PRE_ADD_COLLECTIBLE {: .copyable } | ([CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, int Charge, boolean FirstTime, int Slot, int VarData, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | table or [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) |

### MC_POST_TAKE_DMG {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1006 |MC_POST_TAKE_DMG {: .copyable } | ([Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Entity, float Damage, [DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int DamageCountdown) | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | void |

### MC_GRID_ROCK_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1010 |MC_GRID_ROCK_UPDATE {: .copyable } | ([GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, [GridEntityRock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityRock.html) Rock) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_POST_GRID_ROCK_DESTROY {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1011 |MC_POST_GRID_ROCK_DESTROY {: .copyable } | ([GridEntityRock](https://wofsauge.github.io/IsaacDocs/rep/GridEntityRock.html) Rock, [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, boolean Immediate) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_GRID_HURT_DAMAGE {: .copyable }
Return false if entity or player should ignore damage from it.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1012 |MC_GRID_HURT_DAMAGE {: .copyable } | ([GridEntity](https://wofsauge.github.io/IsaacDocs/rep/GridEntity.html) GridEntity, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Entity, int DamageAmount, [DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, float Unknown  boolean Unknown) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean |

### MC_POST_GRID_HURT_DAMAGE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1013 |MC_POST_GRID_HURT_DAMAGE {: .copyable } | ([GridEntity](https://wofsauge.github.io/IsaacDocs/rep/GridEntity.html) GridEntity, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Entity, int DamageAmount, [DamageFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) DamageFlags, float Unknown  boolean Unknown) | [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_HUD_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1020 |MC_HUD_UPDATE {: .copyable } | - | - | void |

### MC_HUD_POST_UPDATE {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1021 |MC_HUD_POST_UPDATE {: .copyable } | - | - | void |

### MC_HUD_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1022 |MC_HUD_RENDER {: .copyable } | - | - | void |

### MC_MAIN_MENU_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1023 |MC_MAIN_MENU_RENDER {: .copyable } | - | - | void |


### MC_PRE_PAUSE_SCREEN_RENDER {: .copyable }
Can return `false` to prevent the completion marks from rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1218 |MC_PRE_PAUSE_SCREEN_RENDER {: .copyable } | ([Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) PauseBody,[Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) PauseStats) | - | void |

### MC_POST_PAUSE_SCREEN_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1219 |MC_POST_PAUSE_SCREEN_RENDER {: .copyable } | ([Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) PauseBody,[Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) PauseStats) | - | void |


### MC_PRE_COMPLETION_MARKS_RENDER {: .copyable }
Can return `false` to prevent the completion marks from rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1216 |MC_PRE_COMPLETION_MARKS_RENDER {: .copyable } | ([Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) CompletionMarksSprite,[Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderPos,[Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderScale,int PlayerType) | - | void |

### MC_POST_COMPLETION_MARKS_RENDER {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1217 |MC_POST_COMPLETION_MARKS_RENDER {: .copyable } | ([Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) CompletionMarksSprite,[Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderPos,[Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderScale,int PlayerType) | - | void |


### MC_PRE_SFX_PLAY
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
|1030 |MC_PRE_SFX_PLAY {: .copyable } | (int ID, float Volume, int FrameDelay, boolean Loop, float Pitch, float Pan)| [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/SoundEffect.html) | table or boolean |

### MC_POST_SFX_PLAY
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1031 |MC_POST_SFX_PLAY {: .copyable } | (int ID, float Volume, int FrameDelay, boolean Loop, float Pitch, float Pan)| [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/SoundEffect.html) | void |

### MC_PRE_MUSIC_PLAY
Accepts a table of parameters: `{ID, Volume OR FadeRate}`

Alternatively accepts a [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) to change the track without changing volume or fade rate

Alternatively accepts `false` to cancel the track

???- info "Volume vs. FadeRate"
    This callback is called for *both* MusicManager::Play *and* MusicManager::Crossfade! `IsFade` is provided to distinguish the two.

???- example "Example Code"
    This code will forcibly loop every sound (for better or worse).
    ```lua
    function mod:myFunction(ID, Volume, IsFade)
        return Music.MUSIC_FLOODED_CAVES
    end
    mod:AddCallback(ModCallbacks.MC_PRE_MUSIC_PLAY, mod.myFunction)
    ```


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1034 |MC_PRE_MUSIC_PLAY {: .copyable } | (int ID, float Volume OR float FadeRate, boolean IsFade)| [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) | table or [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) or boolean |

### MC_PRE_MUSIC_LAYER_TOGGLE

Accepts a music layer ID (enum pending) to change the layer, or a bool to change the state of the layer: `true` to keep it running, `false` to stop it.

`CurrentState` returns `true` if the layer is going to be **enabled**, and `false` if it is going to be **disabled.**

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1035 |MC_PRE_MUSIC_LAYER_TOGGLE {: .copyable } | (int ID, boolean CurrentState)| int | int or boolean |

### MC_PRE_RENDER_PLAYER_HEAD
Accepts an override [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) for RenderPos.

Alternatively accepts a `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1038 |MC_PRE_RENDER_PLAYER_HEAD {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) player, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderPos)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_RENDER_PLAYER_BODY
Accepts an override `vector` for RenderPos.

Alternatively accepts a `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1039 |MC_PRE_RENDER_PLAYER_BODY {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) player, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RenderPos)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |


### MC_PRE_ENTITY_THROW
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) which will modify the velocity of the thrown entity.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1040 |MC_PRE_ENTITY_THROW {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) ThrowingPlayer, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) HeldEntity, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Velocity)| - | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) |

### MC_POST_ENTITY_THROW
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1041 |MC_POST_ENTITY_THROW {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) ThrowingPlayer, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) ThrownEntity, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Velocity)| - | void |

### MC_PLAYER_INIT_POST_LEVEL_INIT_STATS {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1042 |MC_PLAYER_INIT_POST_LEVEL_INIT_STATS {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void |

### MC_PRE_ROOM_EXIT
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1043 |MC_PRE_ROOM_EXIT {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, boolean Unknown)| - | void |

### MC_PRE_COMPLETION_EVENT
Can return `false` to cancel the completion event. Canceling it will prevent all marks and completion event related stuff to trigger for all players.

Triggers when a completion even gets triggered, getting as a parameter the code for the event in question. 

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1049 |MC_PRE_COMPLETION_EVENT {: .copyable } | ([CompletionType](CompletionType.md) Completion) | - | void or false |

### MC_COMPLETION_MARK_GET
Can return `false` to cancel the completion mark.

Triggers when the player gets a completion mark, getting as a parameter the code for the mark in question and the playertype.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1047 |MC_COMPLETION_MARK_GET {: .copyable } | ([CompletionType](CompletionType.md) Completion, int [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html)) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | void or false |

### MC_POST_COMPLETION_MARK_GET
Triggers after the player gets a completion mark, getting as a parameter the code for the mark in question and the playertype.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1048 |MC_POST_COMPLETION_MARK_GET {: .copyable } | ([CompletionType](CompletionType.md) Completion, int [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html)) | [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) |


### MC_PRE_LEVEL_INIT {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1060 |MC_PRE_LEVEL_INIT {: .copyable } | - | - | void |

### MC_PRE_TRIGGER_PLAYER_DEATH
Fires right before the game over screen.

Accepts `false` to cancel the death, reviving the player in-place with half a heart, or `true` or `nil` to allow the death to go through.


???- warning "Warning"
    Much like the vanilla Lua Revive() function, this removes the current run's ability to save. This occurs because the game immediately deletes the save file during the death animation if there's no pending revives, and this callback doesn't fire fast enough to register a revive on death as it occurs right before the game over screen.

    There is no workaround for this behavior at this time, but this may change in the future.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1050 |MC_PRE_TRIGGER_PLAYER_DEATH {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| - | boolean |

### MC_PRE_RESTOCK_SHOP
Accepts `false` to cancel the restock, blocking shop rerolls from restock machines or restocks from Restock altogether.

???- info "Partial"
    This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.


|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1070 |MC_PRE_RESTOCK_SHOP {: .copyable } | (boolean Partial)| - | void |

### MC_POST_RESTOCK_SHOP
Accepts no return parameters.

???- info "Partial"
    This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1071 |MC_POST_RESTOCK_SHOP {: .copyable } | (boolean Partial)| - | void |

### MC_PRE_CHANGE_ROOM
Accepts a table of parameters: `{TargetRoomIdx, Dimension}`

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1061 |MC_PRE_CHANGE_ROOM {: .copyable } | (int TargetRoomIdx, int Dimension) | - | table |

### MC_POST_PICKUP_SHOP_PURCHASE
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1062 |MC_POST_PICKUP_SHOP_PURCHASE {: .copyable } | ([EntityPickup](../EntityPickup.md) Pickup, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, int MoneySpent)| [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.md) | void |

### MC_GET_FOLLOWER_PRIORITY
Accepts [FollowerPriority](FollowerPriority.md) to give familiar priority. Can accept any int.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1063 |MC_GET_FOLLOWER_PRIORITY {: .copyable } | ([EntityFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html) Familiar) | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.md) | [FollowerPriority](FollowerPriority.md) or int |

### MC_PRE_PICKUP_MORPH
Accepts a table of parameters: `{EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers}` or just `{EntityType, Variant, SubType}`. 

Returning `false` cancels the morph.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1213 |MC_PRE_PICKUP_MORPH {: .copyable } | ([EntityPickup](https://wofsauge.github.io/IsaacDocs/rep/EntityPickup.html) Pickup, int EntityType, int Variant, int SubType, boolean KeepPrice, boolean KeepSeed, boolean IgnoreModifiers)| - | table or boolean |

### MC_POST_PICKUP_MORPH
Runs after the morph already happened.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1215 |MC_POST_PICKUP_MORPH {: .copyable } | ([EntityPickup](https://wofsauge.github.io/IsaacDocs/rep/EntityPickup.html) Pickup, int PreviousType, int PreviousVariant, int SubType, boolean KeptPrice, boolean KeptSeed, boolean IgnoredModifiers)| - | void |


### MC_PRE_NPC_MORPH
Accepts a table of parameters: `{EntityType, Variant, SubType, Championid}` or just `{EntityType, Variant, SubType}`. 

Returning `false` cancels the morph.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1212 |MC_PRE_NPC_MORPH {: .copyable } | (int EntityType, int Variant, int SubType, int Championid)| - | table or boolean |

### MC_POST_NPC_MORPH
Runs after the Morph already happened.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1214 |MC_POST_NPC_MORPH {: .copyable } | (int PreviousType, int PreviousVariant, int PreviousSubType)| - | void |

### MC_PRE_USE_CARD
Accepts `true` to cancel card use.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1064 |MC_PRE_USE_CARD {: .copyable } | ([Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.md) ID, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, int UseFlag)| - | boolean |

### MC_PRE_USE_PILL
Accepts `true` to cancel pill use.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1065 |MC_PRE_USE_PILL {: .copyable } | ([PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.md) ID, [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.md) PillColor, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, int UseFlag)| - | boolean |

### MC_GET_SHOP_ITEM_PRICE
Accepts an integer to change the price of the item.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1066 |MC_GET_SHOP_ITEM_PRICE {: .copyable } | (int EntityVariant, int EntitySubType, int ShopItemID)| int | int |

### MC_PLAYER_GET_HEALTH_TYPE
Accepts an [HealthType](HealthType.md) to change health type for the character.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1067 |MC_PLAYER_GET_HEALTH_TYPE {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| - | [HealthType](HealthType.md) |

### MC_PRE_FAMILIAR_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1080 |MC_PRE_FAMILIAR_RENDER {: .copyable } | ([EntityFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html) Familiar, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_NPC_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1081 |MC_PRE_NPC_RENDER {: .copyable } | ([EntityNPC](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html) NPC, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_PLAYER_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1082 |MC_PRE_PLAYER_RENDER {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [PlayerVariant](PlayerVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_PICKUP_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1083 |MC_PRE_PICKUP_RENDER {: .copyable } | ([EntityPickup](https://wofsauge.github.io/IsaacDocs/rep/EntityPickup.html) Pickup, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_TEAR_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1084 |MC_PRE_TEAR_RENDER {: .copyable } | ([EntityTear](https://wofsauge.github.io/IsaacDocs/rep/EntityTear.html) Tear, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_PROJECTILE_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1085 |MC_PRE_PROJECTILE_RENDER {: .copyable } | ([EntityProjectile](https://wofsauge.github.io/IsaacDocs/rep/EntityProjectile.html) Projectile, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_KNIFE_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1086 |MC_PRE_KNIFE_RENDER {: .copyable } | ([EntityKnife](https://wofsauge.github.io/IsaacDocs/rep/EntityKnife.html) Knife, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [KnifeVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/KnifeVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_EFFECT_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1087 |MC_PRE_EFFECT_RENDER {: .copyable } | ([EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/Entityeffect.html) Effect, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [EffectVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/EffectVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_BOMB_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1088 |MC_PRE_BOMB_RENDER {: .copyable } | ([EntityBomb](https://wofsauge.github.io/IsaacDocs/rep/EntityBomb.html) Bomb, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_SLOT_RENDER
Accepts a [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) to modify render offset

Alternatively accepts `false` to cancel rendering

???- info "Shadows"
    Canceling this callback will not stop rendering of entity shadows. This is being investigated, but in the meantime call SetShadowSize(0) on the entity in MC_POST_UPDATE.
    
|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1089 |MC_PRE_SLOT_RENDER {: .copyable } | ([EntitySlot](../EntitySlot.md) Slot, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [SlotVariant](SlotVariant.md) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_POST_SLOT_RENDER
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1090 |MC_POST_SLOT_RENDER {: .copyable } | ([EntitySlot](../EntitySlot.md) Slot, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [SlotVariant](SlotVariant.md) | void |

### MC_PRE_GRID_INIT
This is called when a grid is being placed into a room for the first time.

Accepts `false` to cancel grid initialization and immediately remove the residual grid from the room. **This will cause the game to call PRE_GRID_INIT again the next time the room is entered.**

Alternatively accepts an int to modify the init seed.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1100 |MC_PRE_GRID_INIT {: .copyable } | ([GridEntity](https://wofsauge.github.io/IsaacDocs/rep/GridEntity.html) Grid, int Seed)| [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | boolean or int |

### MC_POST_GRID_INIT
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1101 |MC_POST_GRID_INIT {: .copyable } | ([GridEntity](https://wofsauge.github.io/IsaacDocs/rep/GridEntity.html) Grid, int Seed)| [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | void |

### MC_PRE_ROOM_TRIGGER_CLEAR
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1068 |MC_PRE_ROOM_TRIGGER_CLEAR {: .copyable } | (boolean PlaySound)| - | void |

### MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR
Accepts `false` to cancel trigger effects.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1069 |MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [PlayerVariant](PlayerVariant.md) | boolean |

### MC_PLAYER_GET_ACTIVE_MAX_CHARGE
Accepts an integer to change chargebar for the active item.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1072 |MC_PLAYER_GET_ACTIVE_MAX_CHARGE {: .copyable } | ([CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, int VarData)| [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | int |

### MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE
Accepts an integer to change the minimum charge to use the active item.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1073 |MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE {: .copyable } | ([ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot)| [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | int |

### MC_PRE_REPLACE_SPRITESHEET
Accepts a table of parameters: `{int LayerID, string PNGFilename}`

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1116 |MC_PRE_REPLACE_SPRITESHEET {: .copyable } | (int LayerID, string PNGFilename)| string ANM2Filename | table |

### MC_POST_REPLACE_SPRITESHEET
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1117 |MC_POST_REPLACE_SPRITESHEET {: .copyable } | (int LayerID, string PNGFilename)| string ANM2Filename | void |

### MC_PLAYER_GET_HEART_LIMIT
Accepts an override `integer` for heart limit.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1074 |MC_PLAYER_GET_HEART_LIMIT {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, int HeartLimit, boolean IsKeeper)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | int |

### MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY
This callback is run at the start of planetarium calculation. Before running calculation, the game first checks if the current floor is valid to spawn a planetarium. If the current floor is invalid, all further calculation (and thus, all further callbacks) will be canceled.

By default, planetariums cannot spawn past Depths II (Womb II with Telescope Lens).

This callback can be used, for example, to add custom planetarium spawn rules on custom floors, or to add new items like Telescope Lens which can augment the rules.

Accepts a `boolean`. Return `false` to bypass the planetarium stage penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1110 |MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY {: .copyable } | ()| - | boolean |

### MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY
After ensuring the stage is valid, the game then checks if a planetarium has been entered before. If so, the chance will be locked at 1% (10% with Telescope Lens).

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.

Accepts a `boolean`. Return `false` to bypass the planetarium enter penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1111 |MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY {: .copyable } | ()| - | boolean |

### MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY
After ensuring planetariums haven't been entered before, the game then checks how many treasure rooms have been entered. If the amount of treasure rooms entered is greater than or equal to the current stage number, the chance will be locked at 1% (10% with Telescope Lens).

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.


Accepts a `boolean`. Return `false` to bypass the planetarium treasure room penalty entirely, meaning the game will act as if no treasure rooms have been entered.

Alternatively accepts an `int` to modify how many treasure rooms the game will believe has been entered.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1112 |MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY {: .copyable } | (int TreasureRoomsVisited)| - | boolean or int |

### MC_PRE_PLANETARIUM_APPLY_ITEMS
After checking the amount of treasure rooms entered, the game applies flat item chances. This is where Crystal Ball, Magic 8 Ball and Sausage's chances get added, as well as Telescope Lens's additional 15% chance.

If you're looking to add an item like Telescope Lens which modifies the *base* chance, have a look at MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS instead.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1113 |MC_PRE_PLANETARIUM_APPLY_ITEMS {: .copyable } | (float Chance)| - | float |

### MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS
Finally, after checking all of the above, Telescope Lens adds an additional 9% chance to the base chance, bringing the base generation chance to 10%.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1114 |MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS {: .copyable } | (float Chance)| - | float |

### MC_POST_PLANETARIUM_CALCULATE
This will override *all* previous calculation values, ultimately dictating the planetarium chance.

Accepts a `float` to modify the chance.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1115 |MC_POST_PLANETARIUM_CALCULATE {: .copyable } | (float Chance)| - | float |

### MC_POST_SLOT_INIT
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1121 |MC_PRE_SLOT_INIT {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_POST_SLOT_UPDATE
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1122 |MC_POST_SLOT_UPDATE {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_COLLISION
Just like the collision callbacks in the vanilla API, the Low value is true if the entity collided with the collider first, and false if the opposite is true.

Return `true` to ignore collision, `false` to collide but not execute internal code.

Also accepts returning a table, with any combination of the following fields:

* Collide: Set to true to force the entities to physically collide (push each other away), unless the "Collider" ignores the collision. Set to false to ignore the physical collision without necessarily skipping on-collision effects.
* SkipCollisionEffects: Set to true to skip the on-collision code of this entity. Does not affect the physical collision. Won't skip the on-collision code of the "Collider".

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1240 |MC_PRE_SLOT_COLLISION {: .copyable } | ([EntitySlot](../EntitySlot.md), [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low) | [SlotVariant](SlotVariant.md) | bool |

### MC_POST_SLOT_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1241 |MC_POST_SLOT_COLLISION {: .copyable } | ([EntitySlot](../EntitySlot.md), [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_CREATE_EXPLOSION_DROPS
Return `false` to stop explosions from dropping the standard consumables. This is useful, for example, to allow custom slots to drop their own loot on explosion.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1123 |MC_PRE_SLOT_CREATE_EXPLOSION_DROPS {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | bool |

### MC_POST_SLOT_CREATE_EXPLOSION_DROPS
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1124 |MC_POST_SLOT_CREATE_EXPLOSION_DROPS {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_PRE_SLOT_SET_PRIZE_COLLECTIBLE
Used by Shell Game and Hell Game. 

Accepts a [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) to override what the game will pay out with.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1125 |MC_PRE_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) |

### MC_POST_SLOT_SET_PRIZE_COLLECTIBLE
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1126 |MC_POST_SLOT_SET_PRIZE_COLLECTIBLE {: .copyable } | ([EntitySlot](../EntitySlot.md)) | [SlotVariant](SlotVariant.md) | void |

### MC_POST_PLAYER_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1231 |MC_POST_PLAYER_COLLISION {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [PlayerVariant](PlayerVariant.md) | void |

### MC_POST_TEAR_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1233 |MC_POST_TEAR_COLLISION {: .copyable } | ([EntityTear](https://wofsauge.github.io/IsaacDocs/rep/EntityTear.html) Tear, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [TearVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/TearVariant.html) | void |

### MC_POST_FAMILIAR_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1235 |MC_POST_FAMILIAR_COLLISION {: .copyable } | ([EntityFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html) Familiar, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/FamiliarVariant.html) | void |

### MC_POST_BOMB_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1237 |MC_POST_BOMB_COLLISION {: .copyable } | ([EntityBomb](https://wofsauge.github.io/IsaacDocs/rep/EntityBomb.html) Bomb, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [BombVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/BombVariant.html) | void |

### MC_POST_PICKUP_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1239 |MC_POST_PICKUP_COLLISION {: .copyable } | ([EntityPickup](https://wofsauge.github.io/IsaacDocs/rep/EntityPickup.html) Pickup, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [PickupVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/PickupVariant.html) | void |

### MC_POST_KNIFE_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1243 |MC_POST_KNIFE_COLLISION {: .copyable } | ([EntityKnife](https://wofsauge.github.io/IsaacDocs/rep/EntityKnife.html) Knife, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [KnifeSubType](KnifeSubType.md) | void |

### MC_POST_PROJECTILE_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1245 |MC_POST_PROJECTILE_COLLISION {: .copyable } | ([EntityProjectile](https://wofsauge.github.io/IsaacDocs/rep/EntityProjectile.html) Projectile, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [ProjectileVariant](https://wofsauge.github.io/IsaacDocs/rep/enums/ProjectileVariant.html) | void |

### MC_POST_NPC_COLLISION
Runs after the on-collision code of this entity, assuming it wasn't skipped.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1247 |MC_POST_NPC_COLLISION {: .copyable } | ([EntityNPC](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html) NPC, [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Collider, bool Low | [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | void |

### MC_PRE_DEVIL_APPLY_ITEMS
This callback is run when the game starts to tally up traditional items for devil deal calculation. This is called before the stage penalty.

Most items that affect devil deal chance perform their changes here.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1130 | MC_PRE_DEVIL_APPLY_ITEMS {: .copyable } | ()| - | float |

### MC_PRE_DEVIL_APPLY_STAGE_PENALTY
Next, the game calculates stage penalty. If a deal spawned anywhere on the previous two floors, the game decays the resulting chance by either 50% or 25% depending on how many deals have been taken.

Important to note that even though the game displays a value of ~66% or ~33% for the 50% and 25% values, respectively, this is because devil chance is *not* clamped to a value between 0 and 1, and "100%" without items generally means a value of ~133%.

Accepts a `boolean`. Return `false` to bypass the stage penalty.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1131 |MC_PRE_DEVIL_APPLY_STAGE_PENALTY {: .copyable } | ()| - | boolean |

### MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS
Next, the game applies "special" items which bypass the stage penalty like Goat Head and Eucharist.

Accepts a `float` to modify the chance in this step of the calculation.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1132 | MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS {: .copyable } | ()| - | float |

### MC_POST_DEVIL_CALCULATE
This will override *all* previous calculation values, ultimately dictating the devil chance.

Accepts a `float` to modify the chance.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1133 |MC_POST_DEVIL_CALCULATE {: .copyable } | (float Chance)| - | float |

### MC_POST_ITEM_OVERLAY_UPDATE
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1075 |MC_POST_ITEM_OVERLAY_UPDATE {: .copyable } | ([ItemOverlay](../ItemOverlay.md) ItemOverlay)| [GiantBook](Giantbook.md) | void |

### MC_PRE_ITEM_OVERLAY_SHOW
Accepts an integer to change [GiantBook](Giantbook.md)

Alternatively accepts ``true`` to cancel item overlay show

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1076 |MC_PRE_ITEM_OVERLAY_SHOW {: .copyable } | ([ItemOverlay](../ItemOverlay.md) ItemOverlay, [GiantBook](Giantbook.md) GiantBookID, int Delay, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [GiantBook](Giantbook.md) | [GiantBook](Giantbook.md) or boolean |

### MC_POST_PLAYER_NEW_ROOM_TEMP_EFFECTS

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1077 |MC_POST_PLAYER_NEW_ROOM_TEMP_EFFECTS {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | - |

### MC_POST_PLAYER_NEW_LEVEL

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1078 |MC_POST_PLAYER_NEW_LEVEL {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) | - |

### MC_POST_PLAYERHUD_RENDER_ACTIVE_ITEM

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1079 |MC_POST_PLAYERHUD_RENDER_ACTIVE_ITEM {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset, float Alpha)| - | - |

### MC_POST_PLAYERHUD_RENDER_HEARTS

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1091 |MC_POST_PLAYERHUD_RENDER_HEARTS {: .copyable } | ([Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset(?), [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) HeartsSprite, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position, float Unknown)| - | - |

### MC_PRE_GET_LIGHTING_ALPHA
Accepts a `float` to modify the lighting alpha. Generally this is between 0 and 1 but you can technically go higher than this.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1150 |MC_PRE_GET_LIGHTING_ALPHA {: .copyable } | (float OriginalAlpha)| - | float |

### MC_PRE_RENDER_GRID_LIGHTING
Accepts an override [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) for Offset.

Alternatively accepts `false` to stop the rendering.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1151 |MC_PRE_RENDER_GRID_LIGHTING {: .copyable } | ([GridEntity](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) grid, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |
 
### MC_PRE_RENDER_ENTITY_LIGHTING
Accepts an override [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) for Offset.

Alternatively accepts `false` to stop the rendering.

 |ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1152 |MC_PRE_RENDER_ENTITY_LIGHTING {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) player, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset)| [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) | [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) or boolean |

### MC_PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1092 |MC_PRE_PLAYER_APPLY_INNATE_COLLECTIBLE_NUM {: .copyable } | int ModCount, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, boolean OnlyCountTrueItems)| - | int |

### MC_PRE_PLAYER_HAS_COLLECTIBLE

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1093 |MC_PRE_PLAYER_HAS_COLLECTIBLE {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type, boolean OnlyCountTrueItems)| [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | bool |

### MC_PRE_MUSIC_PLAY_JINGLE
Accepts a [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) to change the track

Alternatively accepts `false` to cancel the track

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1094 |MC_PRE_MUSIC_PLAY_JINGLE {: .copyable } | ([Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicID)| [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) | [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) or boolean |

### MC_POST_TRIGGER_COLLECTIBLE_REMOVED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1095 |MC_POST_TRIGGER_COLLECTIBLE_REMOVED {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Type) | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | void |

### MC_POST_TRIGGER_TRINKET_ADDED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1096 |MC_POST_TRIGGER_TRINKET_ADDED {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Type, boolean FirstTimePickingUp) | [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) | void |

### MC_POST_TRIGGER_TRINKET_REMOVED {: .copyable }
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1097 |MC_POST_TRIGGER_TRINKET_REMOVED {: .copyable } | ([EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Type) | [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) | void |

### MC_CONSOLE_AUTOCOMPLETE
This is called whenever a function with the CUSTOM [AutocompleteType](AutocompleteType.md) enum is being entered into the console. Called every time the console input changes.

Accepts a table. The table can hold both string values, which will just add the string as a parameter in autocomplete for the command, and a table of two strings which will add the first string as the parameter, and second as a description. Description can be used in autocomplete as well, however pressing TAB will properly autocomplete using the ID, not description (Think the `giveitem` command, as an example- `c1` would be The Sad Onion's "parameter" and `The Sad Onion` would be the "description", and both work. Pressing TAB will turn the command into `give c1`.)

REPENTOGON handles only showing options relevant for the given input- simply return a table of options, and REPENTOGON will take care of the rest.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1120 |MC_CONSOLE_AUTOCOMPLETE {: .copyable } | (string Command, string Params)| string Command | table |

### MC_CUSTOM_CHARACTER_UNLOCKED
This is called every frame a custom character with the `needsunlock` tag is currently selected on the menu.

By default, characters with this tag are **locked.** Return `true` in this callback to unlock the character, `false` or `nil` to keep it locked.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1140 |MC_CUSTOM_CHARACTER_UNLOCKED {: .copyable } | ( int PlayerType ) | int PlayerType | bool |

### MC_PRE_NEW_ROOM
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1200 |MC_PRE_NEW_ROOM {: .copyable } | ( [Room](https://wofsauge.github.io/IsaacDocs/rep/Room) Room, [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor) Descriptor ) | - | - |

### MC_PRE_MEGA_SATAN_ENDING
Called right before Mega Satan forcibly ends the game. 

* Accepts `true` to suppress the ending, guaranteeing a portal to the Void while retaining the completion mark for this character. 
* `false` or `nil` will have no effect. I might look into making `false` guarantee an ending?

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1201 |MC_PRE_MEGA_SATAN_ENDING {: .copyable } | - | - | boolean |

### MC_POST_MODS_LOADED
Called after all Lua scripts have been loaded. Ideal for running code that is expected to run after all mods are initialized, but without the need for load order idiocy!

Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1210 |MC_POST_MODS_LOADED {: .copyable } | - | - | void |

### MC_POST_ITEM_OVERLAY_SHOW
Accepts no return parameters.

|ID|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|:--|
|1134 |MC_POST_ITEM_OVERLAY_SHOW {: .copyable } | ([ItemOverlay](../ItemOverlay.md) ItemOverlay, [GiantBook](Giantbook.md) GiantBookID, int Delay, [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player)| [GiantBook](Giantbook.md) | void |