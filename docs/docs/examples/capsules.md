# Capsules Example
Capsules are used by the game for collision detection. All entities have a collision capsule whose shape is defined by their XML markup. Additionally, you can use a null layer frame's size and position in a sprite's animation to generate a null capsule.

Null layers are added in the sprite's .anm2 file by setting the type to "Null" when adding a layer. The size and relative position of the null layer is dependant on the position and scale of the frame.

You can use [Entity:GetNullCapsule()](../Entity.md#getnullcapsule) to generate a capsule of the current frame in a specified null layer.

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-null-capsules.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/capsule_null_layer.mp4" type="video/mp4">
        </video>
        <figcaption>An example of a capsule hitbox using a large lead pipe.</figcaption>
    </figure>

???+ example Code
    ```lua
    local exampleMod = RegisterMod("Repentogon Null Capsule Example", 1)
    local sfx = SFXManager()

    -- Setup some constants.
    local PIPE_ITEM_ID = Isaac.GetItemIdByName("Swinging Pipe")
    local PIPE_EFFECT_ID = Isaac.GetEntityVariantByName("Pipe Swing")
    local DAMAGE_MULTIPLIER = 2.5

    ---@param player EntityPlayer
    function exampleMod:ActivatePipeItem(_, _, player, flags)
        -- Don't run a second time for Car Battery, because it would be strange for an item like this.
        if flags & UseFlag.USE_CARBATTERY == UseFlag.USE_CARBATTERY then
            return
        end

        -- Spawn the pipe effect. We will be checking for things to damage in the update callback.
        local effect = Isaac.Spawn(EntityType.ENTITY_EFFECT, PIPE_EFFECT_ID, 0, player.Position, Vector.Zero, player):ToEffect()
        effect:FollowParent(player)

        -- Sound effects.
        sfx:Play(SoundEffect.SOUND_SWORD_SPIN)
    end

    -- Connect the callback, only for our item.
    exampleMod:AddCallback(ModCallbacks.MC_USE_ITEM, exampleMod.ActivatePipeItem, PIPE_ITEM_ID)

    -- Now, let's handle capsules.
    -- Capsules are our hitboxes.
    ---@param pipe EntityEffect
    function exampleMod:PipeEffectUpdate(pipe)
        local sprite = pipe:GetSprite()
        local player = pipe.Parent:ToPlayer()
        local data = pipe:GetData()

        -- We are going to use this table as a way to make sure enemies are only hurt once in a swing.
        -- This line will either set the hit blacklist to itself, or create one if it doesn't exist.
        data.HitBlacklist = data.HitBlacklist or {}

        -- Handle removing the pipe when the spin is done.
        if sprite:IsFinished("Spin") then
            pipe:Remove()
            return
        end

        -- We're doing a for loop before because the effect is based off of Spirit Sword's anm2.
        -- Spirit Sword's anm2 has two hitboxes with the same name with a different number at the ending, so we use a for loop to avoid repeating code.
        for i = 1, 2 do
            -- Get the "null capsule", which is the hitbox defined by the null layer in the anm2.
            local capsule = pipe:GetNullCapsule("Hit" .. i)

            -- Search for all enemies within the capsule.
            for _, enemy in ipairs(Isaac.FindInCapsule(capsule, EntityPartition.ENEMY)) do
                -- Make sure it can be hurt.
                if enemy:IsVulnerableEnemy()
                and enemy:IsActiveEnemy()
                and not data.HitBlacklist[GetPtrHash(enemy)] then
                    -- Now hurt it.
                    enemy:TakeDamage(player.Damage * DAMAGE_MULTIPLIER, 0, EntityRef(player), 0)

                    -- Add it to the blacklist, so it can't be hurt again.
                    data.HitBlacklist[GetPtrHash(enemy)] = true

                    -- Do some fancy effects, while we're at it.
                    enemy:BloodExplode()
                    enemy:MakeBloodPoof(enemy.Position, nil, 0.5)
                    sfx:Play(SoundEffect.SOUND_DEATH_BURST_LARGE)
                end
            end
        end
    end

    -- Connect the callback, only for our effect.
    exampleMod:AddCallback(ModCallbacks.MC_POST_EFFECT_UPDATE, exampleMod.PipeEffectUpdate, PIPE_EFFECT_ID)
    ```