---
tags:
  - Class
---
# Class "Level"

## Functions

### CanPlaceRoom () {: aria-label='Functions' }
#### boolean CanPlaceRoom ( [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfigToPlace, int GridIndex, [Dimension](enums/Dimension.md) Dimension = -1, bool AllowMultipleDoors = true, bool AllowSpecialNeighbors = false, bool AllowNoNeighbors = false ) {: .copyable aria-label='Functions' }
Returns true if the provided room could be successfully placed at this location using [TryPlaceRoom](Level.md#tryplaceroom).

See documentation for [TryPlaceRoom](Level.md#tryplaceroom) for more information.

___
### CanPlaceRoomAtDoor () {: aria-label='Functions' }
#### boolean CanPlaceRoomAtDoor ( [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfigToPlace, [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) NeighborRoomDescriptor, [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) DoorSlot, bool AllowMultipleDoors = true, bool AllowSpecialNeighbors = false ) {: .copyable aria-label='Functions' }
Returns true if the provided room (the [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html)) could be successfully placed as a neighbor of an existing room (the [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html)) at the specified [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html), using [TryPlaceRoomAtDoor](Level.md#tryplaceroomatdoor).

See documentation for [TryPlaceRoomAtDoor](Level.md#tryplaceroomatdoor) and [TryPlaceRoom](Level.md#tryplaceroom) for more information.

___
### CanSpawnDoorOutline () {: aria-label='Functions' }
#### boolean CanSpawnDoorOutline ( int RoomIdx, [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) DoorSlot ) {: .copyable aria-label='Functions' }

___
### FindValidRoomPlacementLocations () {: aria-label='Functions' }
#### int[] FindValidRoomPlacementLocations ( [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfig, [Dimension](enums/Dimension.md) Dimension = -1, bool AllowMultipleDoors = true, bool AllowSpecialNeighbors = false ) {: .copyable aria-label='Functions' }
Returns a table of room grid indices that would be valid locations to place the specified room using [TryPlaceRoom](Level.md#tryplaceroom).

Note that if you set `AllowSpecialNeighbors` to `true`, you can get weird placements next to the ultra secret room. You can use [GetNeighboringRooms](Level.md#getneighboringrooms) to confirm that potential neighbors are desired before placing your room.

See [TryPlaceRoom](Level.md#tryplaceroom) for more information on room placement and example code.

___
### GetDimension () {: aria-label='Functions' }
#### [Dimension](enums/Dimension.md) GetDimension ( ) {: .copyable aria-label='Functions' }
Get's the current [Dimension](enums/Dimension.md) the player is in.

___
### GetForceSpecialQuest () {: aria-label='Functions' }
#### [SpecialQuest](enums/SpecialQuest.md) GetForceSpecialQuest ( ) {: .copyable aria-label='Functions' }
If set, the level will automatically attempt to place the Knife Piece puzzle door for this [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html).
???+ info "Info"
	This is set to `SpecialQuest.DEFAULT` immediately before calling `MC_PRE_LEVEL_INIT`.

___
### GetGreedWavesClearedWithoutRedHeartDamage () {: aria-label='Functions' }
#### int GetGreedWavesClearedWithoutRedHeartDamage ( ) {: .copyable aria-label='Functions' }

___
### GetMyosotisPickups () {: aria-label='Functions' }
#### [EntitiesSaveStateVector](EntitiesSaveStateVector.md) GetMyosotisPickups ( ) {: .copyable aria-label='Functions' }
Returns the pickups that will be transferred to the next floor by the Myosotis trinket effect.

___
### GetNeighboringRooms () {: aria-label='Functions' }
#### table GetNeighboringRooms ( int GridIndex, [RoomShape](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomShape.html), [Dimension](enums/Dimension.md) Dimension = -1 ) {: .copyable aria-label='Functions' }
Returns a table that maps [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) to [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) for all of the neighbors that a room of the specified shape would have if placed at this location.

Don't use `ipairs` to iterate over this, use `pairs`!

Note that this does not give you any information on if a room would actually fit here, or if the neighbors would even allow the connection.

If you want to get the neighbors of an existing room, you can simply use [RoomDescriptor:GetNeighboringRooms()](RoomDescriptor.md#getneighboringrooms) instead.

???- example "Example Code"
	```lua
	-- Returns true if a room placed at this GridIndex with this RoomShape would have a neighboring secret room.
	local function WouldHaveSecretRoomNeighbor(gridIndex, roomShape)
		for doorSlot, neighborDesc in pairs(Game():GetLevel():GetNeighboringRooms(gridIndex, roomShape)) do
			local roomType = neighborDesc.Data.Type
			if roomType == RoomType.ROOM_SECRET or roomType == RoomType.ROOM_SUPERSECRET or roomType == RoomType.ROOM_ULTRASECRET then
				return true
			end
		end
		return false
	end
	```

___
### HasAbandonedMineshaft () {: aria-label='Functions' }
#### boolean HasAbandonedMineshaft ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the mineshaft room used for the second Knife Piece puzzle.

___
### HasMirrorDimension () {: aria-label='Functions' }
#### boolean HasMirrorDimension ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the mirror dimension used for the first Knife Piece puzzle.

___
### HasPhotoDoor () {: aria-label='Functions' }
#### boolean HasPhotoDoor ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the photo door used to enter Mausoleum/Gehenna leading to the Ascent sequence.

___
### IsStageAvailable () {: aria-label='Functions' }
#### void IsStageAvailable ( [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) Level, [StageType](https://wofsauge.github.io/IsaacDocs/rep/enums/StageType.html) Stage ) {: .copyable aria-label='Functions' }
Returns `true` if the provided `Level` and `Stage` combination is available to be generated in any given run. Returns `false` if locked behind an achievement.

___
### PlaceRoom () {: aria-label='Functions' }
#### boolean PlaceRoom ( [LevelGeneratorEntry](LevelGeneratorEntry.md) Room, [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfig, int Seed ) {: .copyable aria-label='Functions' }
Places a room into the game. Returns `true` if successful.

Note that this function does not really check if a room placement would be considered valid, nor does it create the doors necessary to connect the new room to its neighbors.

See [TryPlaceRoom](Level.md#tryplaceroom) and related functions if you want to properly add new rooms to the floor after level generation, similarly to what Red Key does.

___
### SetForceSpecialQuest () {: aria-label='Functions' }
#### void SetForceSpecialQuest ( [SpecialQuest](enums/SpecialQuest.md) Quest ) {: .copyable aria-label='Functions' }
Sets whether the level should attempt to place the Knife Piece puzzle door for this [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html).
???+ info "Info"
	This is set to `SpecialQuest.DEFAULT` immediately before calling `MC_PRE_LEVEL_INIT`.

___
### SetGreedWavesClearedWithoutRedHeartDamage () {: aria-label='Functions' }
#### void SetGreedWavesClearedWithoutRedHeartDamage ( int WavesCleared ) {: .copyable aria-label='Functions' }

___
### SetName () {: aria-label='Functions' }
#### void SetName ( string Name ) {: .copyable aria-label='Functions' }

___
### TryPlaceRoom () {: aria-label='Functions' }
#### [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) TryPlaceRoom ( [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfigToPlace, int GridIndex, [Dimension](enums/Dimension.md) Dimension = -1, int Seed = 0, bool AllowMultipleDoors = true, bool AllowSpecialNeighbors = false, bool AllowNoNeighbors = false ) {: .copyable aria-label='Functions' }
Attempts to place the provided room at the specified location.

If successful, returns the newly initialized [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html). Otherwise, returns nil.

This function will ONLY place the room if it fits (does not overlap any other existing rooms, and can mutually connect to all neighboring room with doors).

If a seed of 0 or nil is provided, a deterministic seed with be auto-generated based on the location, room shape, and level seed.

The boolean parameters enable or disable additional restrictions/safeties for room placement:

- AllowMultipleDoors: Set to `false` to only allow placement if the room would only have one door (useful for placing special rooms). Secret rooms don't count as a door.
- AllowSpecialNeighbors: Set to `true` to allow connections to existing special rooms (note that secret rooms are always permitted, but boss rooms never are).
- AllowNoNeighbors: Set to `true` to permit room placements out in the void with no neighbors at all.

???- example "Example Code"
	```lua
	-- Adds another treasure room to the floor at a random valid location.
	local function AddAnotherTreasureRoomToTheFloorAtARandomValidLocation()
		local level = game:GetLevel()
		
		local dimension = -1  -- current dimension
		local seed = level:GetDungeonPlacementSeed()
		
		-- Fetch a random RoomConfig for a new treasure room.
		local roomConfig = RoomConfigHolder.GetRandomRoom(seed, true, StbType.SPECIAL_ROOMS, RoomType.ROOM_TREASURE, RoomShape.ROOMSHAPE_1x1)
		
		-- Disallow placements with multiple doors, or placements that connect to other special rooms.
		local allowMultipleDoors = false
		local allowSpecialNeighbors = false
		
		-- Fetch all valid locations.
		local options = level:FindValidRoomPlacementLocations(roomConfig, dimension, allowMultipleDoors, allowSpecialNeighbors)
		
		for _, gridIndex in pairs(options) do
			-- You may have additional conditions or priorities when it comes to where you would prefer to place your room.
			-- For the purposes of this example we arbitarily forbid the new room from being connected to the starting room,
			-- and otherwise just place the room at the first place we check.
			
			-- Get the RoomDescriptors of all rooms that would be neighboring the room if placed here.
			local neighbors = level:GetNeighboringRooms(gridIndex, roomConfig.Shape)
			
			local connectsToStartingRoom = false
			
			for doorSlot, neighborDesc in pairs(neighbors) do
				if neighborDesc.GridIndex == level:GetStartingRoomIndex() then
					connectsToStartingRoom = true
				end
			end
			
			if not connectsToStartingRoom then
				-- Try to place the room.
				local room = level:TryPlaceRoom(roomConfig, gridIndex, dimension, seed, allowMultipleDoors, allowSpecialNeighbors)
				if room then
					-- The room was placed successfully!
					return
				end
			end
		end
	end
	```

___
### TryPlaceRoomAtDoor () {: aria-label='Functions' }
#### [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) TryPlaceRoomAtDoor ( [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfigToPlace, [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) NeighborRoomDescriptor, [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) DoorSlot, int Seed = 0, bool AllowMultipleDoors = true, bool AllowSpecialNeighbors = false ) {: .copyable aria-label='Functions' }
Similar to [TryPlaceRoom](Level.md#tryplaceroom), but attempts to place the provided room (the [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html)) as a neighbor of an existing room (the [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html)) at the specified [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html).

Otherwise, the details are the same as for [TryPlaceRoom](Level.md#tryplaceroom).

???- example "Example Code"
	```lua
	-- Example code for an active item that creates a new 1x1 room at every doorslot available in the current room.
	mod:AddCallback(ModCallbacks.MC_USE_ITEM, function(_, id, rng, player, flags, slot)
		local level = game:GetLevel()
		local room = game:GetRoom()
		local roomDesc = level:GetCurrentRoomDesc()
		
		local success = false
		
		for doorSlot=0, 7 do
			-- Bitmask check to identify if the current room would even allow a door to be placed here.
			local doorSlotAllowed = roomDesc.Data.Doors & (1 << doorSlot) ~= 0
			if doorSlotAllowed and not room:GetDoor(doorSlot) then
				local seed = rng:Next()
				local requiredDoors = 15  -- Bitmask value that requires the new 1x1 room to have all 4 doorslots available, just so we're sure it fits.
				local roomConfig = RoomConfigHolder.GetRandomRoom(seed, true, Isaac.GetCurrentStageConfigId(), RoomType.ROOM_DEFAULT, RoomShape.ROOMSHAPE_1x1, nil, nil, nil, nil, requiredDoors)
				if roomConfig then
					local newRoom = level:TryPlaceRoomAtDoor(roomConfig, roomDesc, doorSlot, seed, true, false)
					if newRoom then
						success = true
					end
				end
			end
		end
		
		if success then
			return {ShowAnim=true}
		else
			return {Discharge=false}
		end
	end, MY_ACTIVE_ITEM_ID)
	```

___
