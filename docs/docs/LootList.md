---
tags:
  - Class
---
# Class "LootList"

## Constructors
### LootList () {: aria-label='Constructors' }
#### [LootList](LootList.md) Beam ( )  {: .copyable aria-label='Constructors' }

## Functions

### GetEntries () {: aria-label='Functions' }
#### [LootListEntry](LoostListEntry.md) GetEntries ( ) {: .copyable aria-label='Functions' }
Returns a table of LootListEntries contained in the `LootList`.

__
### PushEntry () {: aria-label='Functions' }
#### void PushEntry ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) int Variant, int SubType, int Seed = Random(), [RNG](RNG.md) RNG = nil ) {: .copyable aria-label='Functions' }
Creates and pushes a [LootListEntry](LoostListEntry.md) into the `LootList`.

While usually reserved for chests and sacks that give pickups like hearts, bombs, etc, every `EntityPickup` has a `LootList` and you can push any type, variant, and subtype as a LootListEntry.

???+ example "Example Code"
    This code makes all regular chests spawn the best boss in the entire game. As a bonus, use Guppy's Eye for a horrifying image.

    ```lua
		local mod = RegisterMod("Delirium Unboxing", 1)

		function mod:OnPickupUpdate(pickup)
			if pickup.FrameCount ~= 1 then return end
			local lootList = pickup:GetLootList()
			
			lootList:PushEntry(EntityType.ENTITY_DELIRIUM, 0, 0)
		end
		mod:AddCallback(ModCallbacks.MC_POST_PICKUP_UPDATE, mod.OnPickupUpdate, PickupVariant.PICKUP_CHEST)
    ```

__