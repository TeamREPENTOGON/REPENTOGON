# Weighted Outcomes Example
The [WeightedOutcomePicker](../WeightedOutcomePicker.md) class allows choosing a random outcome from a weighted list using an RNG object.

A weighted chance system is commonly used by games to determine loot tables and other things where some items in a list of possible outcomes should have a lower chance than others. An outcome's chance to be chosen is determined by its weight divided by the sum of the weight of all outcomes.

The weight of each outcome don't have to add up to 100. The following code snippet adds up to each outcome having a 50% chance to chosen. 50 (weight) divided by sum of the weight of all outcomes (100) is 0.5 (50%).
```lua
local weightedOutcomePicker = WeightedOutcomePicker()

-- Each outcome has a 50% chance to be chosen. (50 / 100)
weightedOutcomePicker:AddOutcomeWeight(10, 50)
weightedOutcomePicker:AddOutcomeWeight(20, 50)
```

Meanwhile, this code snippet has each outcome having a 33% chance to be chosen. 50 (weight) divided by sum of the weight of all outcomes (150) is 0.33 (33%).
```lua
local weightedOutcomePicker = WeightedOutcomePicker()

-- Each outcome has a 33% chance to be chosen. (50 / 150)
weightedOutcomePicker:AddOutcomeWeight(10, 50)
weightedOutcomePicker:AddOutcomeWeight(20, 50)
weightedOutcomePicker:AddOutcomeWeight(30, 50)
```

The follow example mod adds a new chest variant that only gives blue items, and uses a WeightedOutcomePicker filled with collectible types to make stronger items more rare.

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-weighted-outcome.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/weighted_outcome_chest.mp4" type="video/mp4">
        </video>
        <figcaption>A blue chest giving blue items.</figcaption>
    </figure>

???+ example Code
    ```lua
    local mod = RegisterMod("Repentogon Custom Chest Example", 1)
    local sfx = SFXManager()

    local BLUE_CHEST_SUBTYPE = Isaac.GetEntitySubTypeByName("Blue Chest")

    -- First, let's create our WeightedOutcomePicker.
    -- This allows us to assign weights to a number outcome, and randomly chooses an outcome based on the total weight.
    -- To be specific, it's (itemWeight / sumOfItemWeights).
    -- We will be making a WeightedOutcomePicker for the items in which the chest can give.
    local itemOutcomes = WeightedOutcomePicker()

    -- Just add a few blue items. You don't HAVE to make the weights add up to 100, they can add up to anything, but it's easier to understand this way.
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_CONVERTER, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_ISAACS_TEARS, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_BLUE_BOX, 10) -- Pandora's Box
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_TELEPORT, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_D12, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_CUBE_BABY, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_PISCES, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_BLUE_MAP, 10)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_BLANKET, 5)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_EVIL_CHARM, 5)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_FATE, 1)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_RELIC, 1)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_GLOWING_HOUR_GLASS, 1)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_PARASITOID, 1)
    itemOutcomes:AddOutcomeWeight(CollectibleType.COLLECTIBLE_VENUS, 1)

    -- Creates a pedestal that looks like our chest.
    function mod:CreateChestPedestal(location, rng, isCoinChest)
        -- Get an outcome, aka a collectible type.
        local collectibleId = itemOutcomes:PickOutcome(rng)

        -- Spawn a pedestal with that item, and change
        local itemPedestal = Isaac.Spawn(EntityType.ENTITY_PICKUP, PickupVariant.PICKUP_COLLECTIBLE, collectibleId, location, Vector.Zero, nil):ToPickup()

        -- Replace the item pedestal's altar spritesheet with our own.
        local sprite = itemPedestal:GetSprite()
        sprite:ReplaceSpritesheet(5, "gfx/items/blue_chest_altar.png", true)

        -- Remove the poof effect from the pedestal spawning.
        itemPedestal:ClearEntityFlags(EntityFlag.FLAG_APPEAR)

        -- Change the sprite. In the anm2, 5 is the frame with the opened golden chest, and 16 is the frame with the coin version.
        -- Our altar spritesheet replaces those two sprites, so we will use those frames.
        if isCoinChest then
            itemPedestal:SetAlternatePedestal(16)
        else
            itemPedestal:SetAlternatePedestal(4)
        end

        --! To be clear, leaving and entering the room again or leaving and continuing the run will cause the pedestal to turn into a normal golden chest pedestal.
        --! This can be fixed by using save data to track what pedestals are blue chest pedestals.
        --! That is outside the scope of this example mod, but keep this in mind.
    end

    -- Handle giving an item.
    function mod:HandleGivingLoot(chest, collider)
        local player = collider:ToPlayer()
        local sprite = chest:GetSprite()

        -- If a player didn't touch the chest, return (collider:ToPlayer() returns nil if it's not a player).
        -- Also return if it's not our chest.
        if not player or chest.SubType ~= BLUE_CHEST_SUBTYPE then
            return
        end

        -- Make sure it's not already being opened.
        if not sprite:IsPlaying("Open")  then
            -- Check if we should open the chest, and remove either a coin or a key if so.
            local shouldOpen = false
            local someoneHasPayToPlay = PlayerManager.AnyoneHasCollectible(CollectibleType.COLLECTIBLE_PAY_TO_PLAY)
            if someoneHasPayToPlay and player:GetNumCoins() > 0 then
                player:AddCoins(-1)
                shouldOpen = true
            elseif player:HasGoldenKey() or player:GetNumKeys() > 0 then
                player:AddKeys(-1)
                shouldOpen = true
            end

            -- If we are opening the chest, make an RNG object based on the drop seed of the chest and run the pedestal creation code.
            if shouldOpen then
                local rng = RNG()
                rng:SetSeed(chest.DropSeed)

                -- Spawn our pedestal.
                mod:CreateChestPedestal(chest.Position, rng, someoneHasPayToPlay)

                -- Remove the opened chest.
                -- Wait 4 frames to do this so that there isn't a brief period where there's no visible sprite.
                -- We can delay the function by a frame by using a timer.
                Isaac.CreateTimer(function ()
                    chest:Remove()
                end, 4, 1, false)

                -- Play the animation so that this doesn't happen again.
                sprite:Play("Open")

                -- Play the sound.
                sfx:Play(SoundEffect.SOUND_CHEST_OPEN)
            end
        end

        -- Cancel internal code of the normal chest and just allow collision.
        return {
            Collide = true,
            SkipCollisionEffects = true,
        }
    end

    mod:AddCallback(ModCallbacks.MC_PRE_PICKUP_COLLISION, mod.HandleGivingLoot, PickupVariant.PICKUP_CHEST)

    -- Handle general update logic.
    function mod:HandleBlueChestUpdate(chest)
        local sprite = chest:GetSprite()

        -- End the callback early if it's not our chest.
        if chest.SubType ~= BLUE_CHEST_SUBTYPE then
            return
        end

        -- Play sound.
        if sprite:IsEventTriggered("DropSound") then
            sfx:Play(SoundEffect.SOUND_CHEST_DROP)
        end

        -- Handle switching animations.
        if sprite:IsFinished("Appear") then
            sprite:Play("Idle", true)
        end
    end

    mod:AddCallback(ModCallbacks.MC_POST_PICKUP_UPDATE, mod.HandleBlueChestUpdate, PickupVariant.PICKUP_CHEST)

    -- Handle the chest using the pay-to-play sprite.
    function mod:HandlePayToPlay(chest)
        if chest.SubType == BLUE_CHEST_SUBTYPE and PlayerManager.AnyoneHasCollectible(CollectibleType.COLLECTIBLE_PAY_TO_PLAY) then
            local sprite = chest:GetSprite()
            sprite:ReplaceSpritesheet(0, "gfx/items/pick ups/blue_chest_coinslot.png", true)
        end
    end

    mod:AddCallback(ModCallbacks.MC_POST_PICKUP_INIT, mod.HandlePayToPlay, PickupVariant.PICKUP_CHEST)
    ```