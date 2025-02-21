# Color Modifier Example
The [ColorModifier](../ColorModifier.md) class allows us to alter the color correction applied onto the game. This is used by items such as Astral Projection.

You can use [Game:SetColorModifier()](../Game.md#setcolormodifier) to apply a modifier, and [Room:UpdateColorModifier()](../Room.md#updatecolormodifier) to set the color modifier back to what is defined in the room's [FXParams](../FXParams.md), which are the values defined in [fxlayers.xml](https://wofsauge.github.io/IsaacDocs/rep/xml/fxlayers.html) by default.

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-color-modifier.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/color_modifiers.mp4" type="video/mp4">
        </video>
        <figcaption>A brief, red color correction whenever the player is damaged.</figcaption>
    </figure>

???+ example Code
    ```lua
    local mod = RegisterMod("Repentogon Color Modifiers", 1)
    local game = Game()

    local RED_LENGTH = 3

    -- Make our red color modifier.
    local redModifier = ColorModifier(1, 0, 0, 0.33, 0, 2)

    -- Add a red color modifier when the player is hurt.
    function mod:OnPlayerHurt()
        -- Set the red effect. "lerp" stands for linear interpolation, and means it'll "fade" to our color modifier from the current one.
        game:SetColorModifier(redModifier, true, 0.3)

        -- Fade back to normal after RED_LENGTH frames.
        Isaac.CreateTimer(function ()
            -- Set's the color modifier back to what was specified in "fxparams.xml".
            game:GetRoom():UpdateColorModifier(true, true, 0.05)
        end, RED_LENGTH, 1, false)
    end

    mod:AddCallback(ModCallbacks.MC_ENTITY_TAKE_DMG, mod.OnPlayerHurt, EntityType.ENTITY_PLAYER)
    ```