---
tags:
  - Class
---
# Class "LootListEntry"

???+ info
    You can get this class by using the following function:

    - [LootList:GetEntries()](LootList.md#getentries)

    ???+ example "Example Code"
		```lua
		local mod = RegisterMod("Print Pickups", 1)
		
		function mod:OnPickupUpdate(pickup)
			if pickup.FrameCount ~= 1 then return end
			local lootList = pickup:GetLootList()
			local lootListEntries = lootListEntries:GetEntries()

			for _, lootListEntry in ipairs(lootListEntries) do
				print("PickupVariant:", lootListEntry:GetVariant(), "Pickup SubType:", lootListEntry:GetSubType())
			end
		end
		mod:AddCallback(ModCallbacks.MC_POST_PICKUP_UPDATE, mod.OnPickupUpdate)
		```

## Functions

### GetSeed () {: aria-label='Functions' }
#### int GetSeed ( ) {: .copyable aria-label='Functions' }

___
### GetSubType () {: aria-label='Functions' }
#### int GetSubType ( ) {: .copyable aria-label='Functions' }

___
### GetType () {: aria-label='Functions' }
#### [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) GetType ( ) {: .copyable aria-label='Functions' }

___
### GetVariant () {: aria-label='Functions' }
#### int GetVariant ( ) {: .copyable aria-label='Functions' }

___
### GetRNG () {: aria-label='Functions' }
#### [RNG](RNG.md) GetRNG ( ) {: .copyable aria-label='Functions' }

???+ warning "Warning"
	May return `nil`.
___