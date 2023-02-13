# Callbacks
##ModCallbacks.MC_PRE_ADD_COLLECTIBLE(int Type, int Charge, bool FirstTime, int Slot, int VarData)
Accepts a table of parameters: `{Type, Charge, FirstTime, Slot, VarData}`
Example, which turns every item into Money = Power: `return {CollectibleType.COLLECTIBLE_MONEY_EQUALS_POWER, Charge, FirstTime, Slot, VarData}`

## ModCallbacks.MC_POST_TAKE_DMG(Entity Ent, float Damage, int DamageFlags, EntityRef Source, int DamageCooldown)
Accepts no return parameters.

## ModCallbacks.MD_GRID_ROCK_UPDATE(GridEntityRock Rock)
Accepts no return parameters.

## ModCallbacks.MC_HUD_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_POST_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_CHARACTER_MENU_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_PRE_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Accepts a table of parameters: `{ID, Volume, FrameDelay, Loop, Pitch, Pan}`
Example, which forcibly loops every sound: `return {ID, Volume, FrameDelay, true, Pitch, Pan}`
Alternatively accepts `false` to cancel the sound

## ModCallbacks.MC_POST_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Accepts no return parameters.

## ModCallbacks.MC_PRE_MUSIC_PLAY(int ID, float Volume OR float FadeRate (see below), bool IsFade)
Accepts a table of parameters: `{ID, Volume}`
Example, which replaces every song with the Flooded Caves track: `return {Music.MUSIC_FLOODED_CAVES, Volume}`
Alternatively accepts a single number to change the track without changing volume (effectively shorthand for the above)
Alternatively accepts `false` to cancel the track

This callback is called for *both* MusicManager::Play *and* MusicManager::Crossfade! `IsFade` is provided to distinguish the two.


## ModCallbacks.MC_PRE_ENTITY_THROW(Vector Velocity)
Accepts a `Vector` which will modify the velocity of the thrown entity.

## ModCallbacks.MC_POST_ENTITY_THROW(Entity ThrownEntity, Vector Velocity)
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