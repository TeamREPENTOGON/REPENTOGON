# Altering Deal Chance Example
There are four callbacks that allow us to manipulate devil and angel chances.

[MC_PRE_DEVIL_APPLY_ITEMS](../enums/ModCallbacks.md#mc_pre_devil_apply_items) is run when most items that effect deal chance modify the calculation. It's called before the stage penalty is applied. If a deal has spawned anywhere on the previous two floors, the game decays the resulting deal chance by a stage penalty of either 25% or 50%, depending on how many deals have been taken.

It's important to note that even though they game displays a value of ~33% or ~66% for the 25% and 50% values respectively, devil chance is *not* clamped to a value between 0 and 1, and "100%" chance without items generally means a value of around ~133%.

Return a `float` to modify the chance in this step of the calculation.

[MC_PRE_DEVIL_APPLY_STAGE_PENALTY](../enums/ModCallbacks.md#mc_pre_devil_apply_stage_penalty) allows you to return `false` to bypass the stage penalty.

[MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS](../enums/ModCallbacks.md#mc_pre_devil_apply_special_items) is where the game applies modifications that bypass the stage penalty, for items such as Goat Head and Eucharist. Return a `float` to modify the chance in this step of the calculation.

[MC_POST_DEVIL_CALCULATE](../enums/ModCallbacks.md#mc_post_devil_calculate) is called after the deal chance has been calculated. You can return a `float` here to override the calculated chance.

The following example mod adds an item that will randomize the deal chance on MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS while in an uncleared boss room, and keeps that chance applied after the room is cleared.

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-devil-chance.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/devil_chance.mp4" type="video/mp4">
        </video>
        <figcaption>The devil deal chance is randomized every frame while fighting Baby Plum.</figcaption>
    </figure>

???+ example Code
    ```lua
    local mod = RegisterMod("Repentogon Devil Chance", 1)
    local game = Game()

    -- The item "Dark Spellcasting Tome" will set the devil deal chance to
    -- a random percentage every frame while in an uncleared boss room.
    local ITEM_ID = Isaac.GetItemIdByName("Dark Spellcasting Tome")

    -- Store the random chance that will be set when the boss room is cleared.
    local modifiedChance = 0

    -- There's four callbacks for devil chance that are applied at different times.
    -- The one we're using is MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS.
    -- This is where the game applies chances that bypass the stage penalty, such as Goat Head and Eucharist.
    -- There's also MC_PRE_DEVIL_APPLY_ITEMS, which is where most items apply their modified chance.

    -- If in a boss room and it isn't cleared, set the chance to something random.
    function mod:RandomizeDevilChance()
        -- Check if we should actually do the item's effects.
        -- We need the RNG for later so we'll check if the following variable is nil.
        local firstPlayer = PlayerManager.FirstCollectibleOwner(ITEM_ID)
        if firstPlayer then
            local room = game:GetRoom()

            -- Only do this in a boss room.
            if room:GetType() == RoomType.ROOM_BOSS then
                -- Check if in a boss room and if isn't clear, and randomize if so.
                if not room:IsClear() then
                    local rng = firstPlayer:GetCollectibleRNG(ITEM_ID)

                    -- Random chance between 0% and 100%.
                    modifiedChance = rng:RandomFloat()
                end

                return modifiedChance
            end
        end
    end

    mod:AddCallback(ModCallbacks.MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS, mod.RandomizeDevilChance)
    ```