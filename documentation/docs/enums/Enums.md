
# Enumerations
### Achievement {: aria-label='Constructors' }
Taken by PersistentGameData::TryUnlock.

### BossType {: aria-label='Constructors' }
Return value of Entity::GetBossID and Room::GetBossID.

### Dimension {: aria-label='Constructors' }
Dimensions

### EntityPoopVariant {: aria-label='Constructors' }
Variant attribute of EntityType.ENTITY_POOP.

### FollowerPriority {: aria-label='Constructors' }
Return parameter for MC_GET_FOLLOWER_PRIORITY.

### GridPoopVariant {: aria-label='Constructors' }
Variant attribute of GridEntityType.GRID_POOP. Is not stored in STB, or passed through MC_PRE_ROOM_ENTITY_SPAWN.

### HealthType {: aria-label='Constructors' }
Return value for EntityPlayer::GetHealthType. Return parameter for MC_PLAYER_GET_HEALTH_TYPE.

### KnifeSubType {: aria-label='Constructors' }
*VERY* incomplete.

### KnifeVariant {: aria-label='Constructors' }
Knife variants

### LineCheckMode {: aria-label='Constructors' }
Taken by Room::CheckLine.

### NullPickupSubType {: aria-label='Constructors' }
5.0.x

### PlayerVariant {: aria-label='Constructors' }
Optional argument of MC_POST_PLAYER_INIT, MC_POST_PLAYER_UPDATE, MC_PRE_PLAYER_RENDER, MC_POST_PLAYER_RENDER, and MC_PRE_PLAYER_COLLISION.

### PressurePlateVariant {: aria-label='Constructors' }
Variant attribute of StbGridType.PRESSURE_PLATE stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of GridEntityType.GRID_PRESSURE_PLATE.

### ProjectileMode {: aria-label='Constructors' }
Taken by EntityNPC::FireProjectiles.

### RetractingSpikesVariant {: aria-label='Constructors' }
Variant attribute of StbGridType.RETRACTING_SPIKES stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of GridEntityType.GRID_SPIKES_ONOFF.

### RoomSubType {: aria-label='Constructors' }
Excludes Super Secret Room backdrop (use [BackdropType]), and Boss Room boss (use [BossType]).

### SlotVariant {: aria-label='Constructors' }
Slot variants

### StbType {: aria-label='Constructors' }
Taken by RoomConfigHolder::GetRoomByStageTypeAndVariant.

### StbGridType {: aria-label='Constructors' }
Type attribute of grid entities stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

### StbRockSubType {: aria-label='Constructors' }
SubType attribute of StbGridType.ROCK stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

### StbPoopSubType {: aria-label='Constructors' }
SubType attribute of StbGridType.POOP stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

### StbTeleporterSubType {: aria-label='Constructors' }
SubType attribute of StbGridType.TELEPORTER stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.

### StbRailVariant {: aria-label='Constructors' }
Variant attribute of StbGridType.RAIL and StbGridType.RAIL_PIT stored in STB. (And while passing through MC_PRE_ROOM_ENTITY_SPAWN?)

### TeleporterVariant {: aria-label='Constructors' }
Variant attribute of StbGridType.TELEPORTER stored in STB and while passing through MC_PRE_ROOM_ENTITY_SPAWN.
Variant attribute of StbGridType.GRID_TELEPORTER.
