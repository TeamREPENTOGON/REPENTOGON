# Beams Example
[Beams](../renderer/Beam.md) are used internally for rendering cords, such as Gello's umbilical cord. Note that this *only handles rendering* and not physics.

Beams are best demonstrated by looking at the Ball and Chain enemies found in Gehenna, which have a chain that dynamically moves and stretches depending on how far away the spiked ball is from where it is chained.

Beams are comprised of any number of [Points](../renderer/Point.md).

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-beam.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/beam_damage_ray.mp4" type="video/mp4">
        </video>
        <figcaption>A laser Beam of death incinerating enemies.</figcaption>
    </figure>

???+ example Code
    ```lua
    local mod = RegisterMod("Repentogon Beam Example", 1)

    -- Some constants.
    local DAMAGE_MULTIPLIER = 0.01
    local ITEM_ID = Isaac.GetItemIdByName("Life-Drain Gem")

    -- First make a sprite.
    local sprite = Sprite()
    sprite:Load("gfx/chain_beam.anm2", true)
    sprite:Play("Idle", true)

    -- Handles the beam and damaging of the closest enemy to the player.
    function mod:HandleLifeSteam(player)
        local data = player:GetData()

        -- Check if the player has our item, first.
        if not player:HasCollectible(ITEM_ID) then
            return
        end

        -- Get the beam stored in player:GetData(), or create one if it doesn't exist.
        local beam = data.LifeStealBeam
        if not beam then
            -- Create the beam. We don't need to use the overlay here.
            data.LifeStealBeam = Beam(sprite, "chain", false, false)
            beam = data.LifeStealBeam
        end

        -- Get the closest enemy
        local closestDistance, closestEnemy
        for _, enemy in ipairs(Isaac.GetRoomEntities()) do
            if enemy:IsActiveEnemy() and enemy:IsVulnerableEnemy() then
                local distanceToPlayer = enemy.Position:Distance(player.Position)
                if not closestDistance or closestDistance > distanceToPlayer then
                    closestEnemy = enemy:ToNPC()
                    closestDistance = distanceToPlayer
                end
            end
        end

        -- If there's no enemy in the room, end the function early.
        if not closestEnemy then
            return
        end

        -- Deal damage every frame.
        closestEnemy:TakeDamage(player.Damage * DAMAGE_MULTIPLIER, 0, EntityRef(player), 0)

        -- Render the beam. The position is in screen coordinates.
        -- The second argument, SpritesheetCoordinate, is what the Y position of the spritesheet is by the time this Point is reached.
        -- So for example, two points of 0 and 64 SpritesheetCoordinate will render the sprite starting from y0 to 64.
        -- An additional Point with a SpritesheetCoordinate of 0 will draw it in reverse from y64 to y0.
        -- Width is an optional third argument that is a multiplier used to determine the width of the Beam.
        beam:Add(Isaac.WorldToScreen(player.Position), 0)
        beam:Add(Isaac.WorldToScreen(closestEnemy.Position), 64) -- 64 is the height of the spritesheet.

        -- Render the beam. By default, this will clear all points.
        beam:Render()
    end

    mod:AddCallback(ModCallbacks.MC_POST_PLAYER_RENDER, mod.HandleLifeSteam)
    ```