# General
## Callback Typechecking
Still a work in progress for the new callbacks added by REPENTOGON, but all vanilla callbacks are now typechecked.
  * An error is thrown when a callback return type is invalid.
  * A traceback will be printed into the debug console when a callback error is thrown.
  * This fixes some vanilla callbacks crashing the game when given a bad return type (MC_POST_CURSE_EVAL, MC_GET_CARD, MC_EXECUTE_CMD, etc);
  
## Old Callback Error Behavior
Callback error behavior has been reverted back to how it was before Repentance's 1.7.9b patch. 
  * A callback with an error will no longer stop every future callback from running.
  * This deprecates [Mod Error Container](https://steamcommunity.com/sharedfiles/filedetails/?id=2917616737).

# Callbacks
## ModCallbacks.MC_PRE_ADD_COLLECTIBLE(CollectibleType Type, int Charge, bool FirstTime, int Slot, int VarData)
Optional argument: `CollectibleType Type`

Accepts a table of parameters: `{Type, Charge, FirstTime, Slot, VarData}`

Example, which turns every item into Money = Power: `return {CollectibleType.COLLECTIBLE_MONEY_EQUALS_POWER, Charge, FirstTime, Slot, VarData}`

Alternatively accepts an integer to change the CollectibleType without changing any other parameters (effectively shorthand for the above)

## ModCallbacks.MC_HUD_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_POST_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_CHARACTER_MENU_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_PRE_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Optional argument: `SoundEffect Sound`

Accepts a table of parameters: `{ID, Volume, FrameDelay, Loop, Pitch, Pan}`

Example, which forcibly loops every sound: `return {ID, Volume, FrameDelay, true, Pitch, Pan}`

Alternatively accepts `false` to cancel the sound

## ModCallbacks.MC_POST_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Optional argument: `SoundEffect Sound`

Accepts no return parameters.

## ModCallbacks.MC_PRE_MUSIC_PLAY(int ID, float Volume OR float FadeRate (see below), bool IsFade)
Accepts a table of parameters: `{ID, Volume}`

Example, which replaces every song with the Flooded Caves track: `return {Music.MUSIC_FLOODED_CAVES, Volume}`

Alternatively accepts a single number to change the track without changing volume (effectively shorthand for the above)

Alternatively accepts `false` to cancel the track

This callback is called for *both* MusicManager::Play *and* MusicManager::Crossfade! `IsFade` is provided to distinguish the two.


## ModCallbacks.MC_PRE_RENDER_PLAYER_HEAD(EntityPlayer player, Vector renderpos)
Accepts an override `vector` for renderpos.

Alternatively accepts a `false` to stop the rendering.

## ModCallbacks.MC_PRE_RENDER_PLAYER_BODY(EntityPlayer player, Vector renderpos)
Accepts an override `vector` for renderpos.

Alternatively accepts a `false` to stop the rendering.


## ModCallbacks.MC_PRE_ENTITY_THROW(EntityPlayer ThrowingPlayer, Entity HeldEntity, Vector Velocity)
Accepts a `Vector` which will modify the velocity of the thrown entity.

## ModCallbacks.MC_POST_ENTITY_THROW(EntityPlayer ThrowingPlayer, Entity ThrownEntity, Vector Velocity)
Accepts no return parameters.

## ModCallbacks.MC_PLAYER_INIT_POST_LEVEL_INIT_STATS()
Accepts no return parameters.

## ModCallbacks.MC_PRE_ROOM_EXIT(EntityPlayer Player, Bool Unknown)
Accepts no return parameters.

## ModCallbacks.MC_PRE_LEVEL_INIT()
Accepts no return parameters.

## ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH(EntityPlayer Player)
Accepts `false` to cancel the death, reviving the player in-place, `true` or `nil` to allow the death to go through.

**BUG!** Much like the vanilla Lua Revive() function, this removes the current run's ability to save. This is being investigated.

## ModCallbacks.MC_PRE_RESTOCK_SHOP(bool Partial)
Accepts `false` to cancel the restock, blocking shop rerolls from restock machines or restocks from Restock altogether.

This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

## ModCallbacks.MC_POST_RESTOCK_SHOP(bool Partial)
Accepts no return parameters.

This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

## ModCallbacks.MC_PRE_CHANGE_ROOM(int TargetRoomIdx, int Dimension)
Accepts a table of parameters: `{TargetRoomIdx, Dimension}`

## ModCallbacks.MC_POST_PICKUP_SHOP_PURCHASE(EntityPickup Pickup, EntityPlayer Player, int MoneySpent)
Accepts no return parameters.

## ModCallbacks.MC_GET_FOLLOWER_PRIORITY(EntityFamiliar Familiar)
Accepts FollowerPriority to give familiar priority. Can accept any int.

## ModCallbacks.MC_PRE_PICKUP_MORPH(EntityPickup Pickup,int EntityType,int Variant,int SubType,bool KeepPrice,bool KeepSeed,bool IgnoreModifiers)
Accepts a table of parameters: `{EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers}` or just `{EntityType, Variant, SubType}`. Returning `false` cancels the morph.

## ModCallbacks.MC_PRE_NPC_MORPH(int EntityType,int Variant,int SubType,int Championid)
Accepts a table of parameters: `{EntityType, Variant, SubType, Championid}` or just `{EntityType, Variant, SubType}`. Returning `false` cancels the morph.

## ModCallbacks.MC_PRE_USE_CARD(Card ID, EntityPlayer Player, int UseFlag)
Accepts `true` to cancel card use.

## ModCallbacks.MC_PRE_USE_PILL(PillEffect ID, PillColor PillColor, EntityPlayer Player, int UseFlag)
Accepts `true` to cancel pill use.

## ModCallbacks.MC_GET_SHOP_ITEM_PRICE(int EntityVariant, int EntitySubType, int ShopItemID)
Accepts an integer to change the price of the item.

## ModCallbacks.MC_PLAYER_GET_HEALTH_TYPE(EntityPlayer Player)
Accepts an integer to change health type for the character.

## ModCallbacks.MC_PRE_FAMILIAR_RENDER(EntityFamiliar Familiar, Vector Offset)
Optional argument: `FamiliarVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_NPC_RENDER(EntityNPC NPC, Vector Offset)
Optional argument: `EntityType Type`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_PLAYER_RENDER(EntityPlayer Player, Vector Offset)
Optional argument: `PlayerVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_PICKUP_RENDER(EntityPickup Pickup, Vector Offset)
Optional argument: `PickupVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_TEAR_RENDER(EntityTear Tear, Vector Offset)
Optional argument: `TearVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_PROJECTILE_RENDER(EntityProjectile Projectile, Vector Offset)
Optional argument: `ProjectileVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_KNIFE_RENDER(EntityKnife Knife, Vector Offset)
Optional argument: `ProjectileVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_EFFECT_RENDER(EntityEffect Effect, Vector Offset)
Optional argument: `EffectVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_BOMB_RENDER(EntityBomb Bomb, Vector Offset)
Optional argument: `BombVariant Variant`

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_SLOT_RENDER(Entity Slot, Vector Offset)
Optional argument: `SlotVariant Variant`

As we don't (yet) have an EntitySlot metatable, this simply returns an Entity. Sorry! :(

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_POST_SLOT_RENDER(Entity Slot, Vector Offset)
Optional argument: `SlotVariant Variant`

As we don't (yet) have an EntitySlot metatable, this simply returns an Entity. Sorry! :(

Accepts a Vector to modify render offset

Alternatively accepts `false` to cancel rendering

## ModCallbacks.MC_PRE_GRID_INIT(GridEntity Grid, int Seed)
Optional argument: `GridEntityType Type`

This is called when a grid is being placed into a room for the first time.

Accepts `false` to cancel grid initialization and immediately remove the residual grid from the room. **This will cause the game to call PRE_GRID_INIT again the next time the room is entered.**

Alternatively accepts an int to modify the init seed.

## ModCallbacks.MC_POST_GRID_INIT(GridEntity Grid, int Seed)
Optional argument: `GridEntityType Type`

Accepts no return parameters.

## ModCallbacks.MC_PRE_ROOM_TRIGGER_CLEAR(bool PlaySound)
Accepts no return parameters.

## ModCallbacks.MC_PRE_PLAYER_TRIGGER_ROOM_CLEAR(EntityPlayer Player)
Accepts `false` to cancel trigger effects.

## ModCallbacks.MC_PLAYER_GET_ACTIVE_MAX_CHARGE(CollectibleType Collectible, int VarData)
Optional argument: `CollectibleType Collectible`

Accepts an integer to change chargebar for the active item.

## ModCallbacks.MC_PLAYER_GET_ACTIVE_MIN_USABLE_CHARGE(ActiveSlot Slot)
Optional argument: `CollectibleType Collectible`

Accepts an integer to change the minimum charge to use the active item.

# Functions
## PosVel EntityPlayer:GetMultiShotPositionVelocity(int LoopIndex, WeaponType WeaponType, Vector ShotDirection, Vector ShotSpeed, MultiShotParams Params)
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than MultiShotParams.NumTears.

## bool Game:AchievementUnlocksDisallowed()
Returns `true` if achievements can't be unlocked this run (challenges, seeded, etc)

## RoomConfigHolder Game:GetRoomConfigHolder()

## RoomConfigRoom RoomConfigHolder::GetRoomByStageTypeAndVariant(StbType Stage, int Type, int Variant, int Difficulty)
Returns a RoomConfigRoom corresponding to the given params.

## LevelGeneratorEntry Isaac.LevelGeneratorEntry
Fetches a blank LevelGeneratorEntry from the game.

## void LevelGeneratorEntry:SetColIdx(LevelGeneratorEntry Room, int ColIdx)

## void LevelGeneratorEntry:SetLineIdx(LevelGeneratorEntry Room, int ColIdx)

## void LevelGeneratorEntry:SetAllowedDoors(LevelGeneratorEntry Room, int Doors)
I *believe* Doors here is a bitmask (1 << DoorSlot)

## bool Level:PlaceRoom(LevelGeneratorEntry Room, RoomConfigRoom RoomConfig, int Seed)
Places a room into the game. Returns `true` if successful.

## PlayerManager Game:GetPlayerManager()

## EntityPlayer PlayerManager:FirstCollectibleOwner(CollectibleType Collectible, RNG RNG, bool Unknown)
Returns `nil` if the specified collectible has never been picked up.

## EntityPlayer PlayerManager:SpawnCoPlayer2(PlayerType PlayerType)

## void Entity:AddBleeding(EntityRef Source, int Duration)

## void Entity:AddMagnetized(EntityRef Source, int Duration)

## void Entity:AddBaited(EntityRef Source, int Duration)

## void Entity:AddBrimstoneMark(EntityRef Source, int Duration)

## void Entity:AddWeakness(EntityRef Source, int Duration)

## void Entity:AddIce(EntityRef Source, int Duration)

## void Entity:AddKnockback(EntityRef Source, Vector PushDirection, int Duration, bool Unknown)

## float Entity:GetShadowSize()

## float Entity:SetShadowSize(float Size)

## Vector Entity:GetNullOffset(string NullLayerName)
Returns position of null layer mark. Alternatively returns Vector.Zero if layer is not visible or for other unknown reasons.

## EntityPlayer EntityPlayer:InitTwin(PlayerType PlayerType)
I believe this is used by J&E, Strawman etc

## void EntityPlayer:InitPostLevelInitStats()
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

## void EntityPlayer:SetItemState(CollectibleType CollectibleType)
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

## void EntityPlayer:AddCacheFlags(CacheFlag CacheFlag, bool EvaluateItems = false)
Now accepts an optional `bool` to determine if EntityPlayer:EvaluateItems() should be automatically called after adding cache flags. In most cases, you'll want to do this.

## HealthType EntityPlayer:GetHealthType()

## int EntityPlayer:GetTotalActiveCharge(ActiveSlot Slot)

## void EntityPlayer:SetActiveVarData(int VarData, ActiveSlot Slot)

## int EntityPlayer:GetActiveMaxCharge(CollectibleType Collectible, int VarData)

## int EntityPlayer:GetActiveMinUsableCharge(ActiveSlot Slot)

## int EntityPlayer:AddActiveCharge(int Charge, ActiveSlot Slot, bool Unknown, bool Overcharge, bool Force)

## MultiShotParams EntityPlayer:GetMultiShotParams(WeaponType WeaponType)
Now returns a proper `MultiShotParams` object.

## int MultiShotParams:GetNumTears()
Returns the amount of tears the player can currently simultaneously fire.

This would be more ideal as a field- I'm looking into this.

## void EntityPickup:SetAlternatePedestal(int PedestalType)

## bool EntityPickup:TryRemoveCollectible()

## bool EntityPickup:IsBlind()

## void EntityPickup:SetForceBlind(bool SetBlind)

## int Room:GetShopItemPrice(int EntityVariant, int EntitySubType, int ShopItemID)
Returns the price of the item.

## void Room:RemoveGridEntityImmediate(int GridIndex, int PathTrail, bool KeepDecoration)
*Immediately* removes the GridEntity at the given index. This means grids can be properly removed and immediately replaced, *without* the need to call Room:Update.

## PersistentGameData Isaac:GetPersistentGameData()
Should probably be moved to Game for consistency.

## bool PersistentGameData:TryUnlock(Achievement Unlock)
Returns `true` if successful, `false` if unlocking failed or the secret was already unlocked.

## Console Game:GetConsole()

## void Console:PrintError(string Error)
Prints an error to the console.

## Ambush Game:GetAmbushEx()

## void Ambush:StartChallenge()
Triggers Challenge/Boss Rush ambush.

## void Sprite:ReplaceSpritesheet (int LayerId, string PngFilename, bool︎ LoadGraphics = false);
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

# Enumerations
## StbType
Taken by RoomConfigHolder::GetRoomByStageTypeAndVariant.

## BossType
Return value of Entity::GetBossID and Room::GetBossID.

## NullPickupSubType
5.0.x

## KnifeVariant

## KnifeSubType
*VERY* incomplete.

## SlotVariant

## RoomSubType
Excludes Super Secret Room backdrop (use [BackdropType]), and Boss Room boss (use [BossType]).

## Achievement
Taken by PersistentGameData::TryUnlock.

## FollowerPriority
Return parameter for MC_GET_FOLLOWER_PRIORITY.

## HealthType
Return value for EntityPlayer::GetHealthType. Return parameter for MC_PLAYER_GET_HEALTH_TYPE.

## Dimension

## LineCheckMode
Taken by Room::CheckLine.

## ProjectileMode
Taken by EntityNPC::FireProjectiles.

## StbGridType
Type attribute of grid entities stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

## StbRockSubType
SubType attribute of StbGridType.ROCK stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

## StbPoopSubType
SubType attribute of StbGridType.POOP stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

## StbTeleporterSubType
SubType attribute of StbGridType.TELEPORTER stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

## StbRailVariant
Variant attribute of StbGridType.RAIL and StbGridType.RAIL_PIT stored in STB. (And while passing through MC_PRE_ROOM_ENTITY_SPAWN?)

## RetractingSpikesVariant
Variant attribute of StbGridType.RETRACTING_SPIKES stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of GridEntityType.GRID_SPIKES_ONOFF.

## PressurePlateVariant
Variant attribute of StbGridType.PRESSURE_PLATE stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of GridEntityType.GRID_PRESSURE_PLATE.

## TeleporterVariant
Variant attribute of StbGridType.TELEPORTER stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of StbGridType.GRID_TELEPORTER.

## GridPoopVariant
Variant attribute of GridEntityType.GRID_POOP. Is not stored in STB, or passed through MC_PRE_ROOM_ENTITY_SPAWN.

## EntityPoopVariant
Variant attribute of EntityType.ENTITY_POOP.

## PlayerVariant
Optional argument of MC_POST_PLAYER_INIT, MC_POST_PLAYER_UPDATE, MC_PRE_PLAYER_RENDER, MC_POST_PLAYER_RENDER, and MC_PRE_PLAYER_COLLISION.