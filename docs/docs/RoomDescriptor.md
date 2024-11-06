---
tags:
  - Class
---
# Class "RoomDescriptor"

## Modified Variables

### AllowedDoors {: aria-label='Modified Variables' }
#### DoorSet AllowedDoors {: .copyable aria-label='Modified Variables' }
Now properly returns a value.

Returns a bitmask corresponding to which door slots are currently enabled.

Doors are typically only included in this bitmask when there is a door currently present, even if the room would allow a door in that slot.

???+ example "Example"
    This tests if the DoorSlot `LEFT0` is enabled.
    ```lua
    if roomDesc.AllowedDoors & (1 << DoorSlot.LEFT0) ~= 0 then
        print("Room has a door in slot LEFT0")
    end
    ```

___

## Functions

### AddRestrictedGridIndex () {: aria-label='Functions' }
#### void AddRestrictedGridIndex ( int GridIndex ) {: .copyable aria-label='Functions' }

___
### GetDecoSaveState () {: aria-label='Functions' }
#### [EntitiesSaveStateVector](EntitiesSaveStateVector.md) GetDecoSaveState ( ) {: .copyable aria-label='Functions' }

___
### GetDimension () {: aria-label='Functions' }
#### [Dimension](https://wofsauge.github.io/IsaacDocs/rep/enums/Dimension.html) GetDimension ( ) {: .copyable aria-label='Functions' }
Returns the [Dimension](enums/Dimension.md) that this room exists in.

___
### GetEntitiesSaveState () {: aria-label='Functions' }
#### [EntitiesSaveStateVector](EntitiesSaveStateVector.md) GetEntitiesSaveState ( ) {: .copyable aria-label='Functions' }

___
### GetGridEntitiesSaveState () {: aria-label='Functions' }
#### [GridEntitiesSaveStateVector](GridEntitiesSaveStateVector.md) GetGridEntitiesSaveState ( ) {: .copyable aria-label='Functions' }

___
### GetNeighboringRooms () {: aria-label='Functions' }
#### table GetNeighboringRooms ( ) {: .copyable aria-label='Functions' }
Returns a table that maps [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) to [RoomDescriptor](https://wofsauge.github.io/IsaacDocs/rep/RoomDescriptor.html) for all of the current neighbors of this room.

Don't use `ipairs` to iterate over this, use `pairs`!

???- example "Example Code"
	```lua
	-- Returns true if the room has a neighboring secret room.
	local function HasSecretRoomNeighbor(roomDesc)
		for doorSlot, neighborDesc in pairs(roomDesc:GetNeighboringRooms()) do
			local roomType = neighborDesc.Data.Type
			if roomType == RoomType.ROOM_SECRET or roomType == RoomType.ROOM_SUPERSECRET or roomType == RoomType.ROOM_ULTRASECRET then
				return true
			end
		end
		return false
	end
	```

___
### GetRestrictedGridIndexes () {: aria-label='Functions' }
#### int[] GetRestrictedGridIndexes ( ) {: .copyable aria-label='Functions' }

___
### InitSeeds () {: aria-label='Functions' }
#### void InitSeeds ( [RNG](RNG.md) RNG ) {: .copyable aria-label='Functions' }

___

## Variables

### BossDeathSeed {: aria-label='Variables' }
#### int BossDeathSeed {: .copyable aria-label='Variables' }

___
### Doors {: aria-label='Variables' }
#### int[] Doors {: .copyable aria-label='Variables' }
Allows you to check which level grid index each [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) in the room connects to.

For example, `roomdesc.Doors[DoorSlot.UP0]` provides the level grid index that the upwards door would connect to.

The value will be `-1` if the [RoomShape](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomShape.html) does not allow a door in that slot.

Note that this typically provides a valid index even if there is no door present, and even if the room itself does not allow a door in that slot.

___
