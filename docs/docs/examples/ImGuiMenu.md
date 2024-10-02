# Custom ImGui Menu
The [ImGui](../ImGui.md) class allows us to create custom menus for our mods. This can be used to create configuration menus, and also to hold useful information while debugging.

ImGui menus are found in the top bar when pressing the Debug Console button, which is `~` by default.

For a very thorough and generic usage example, please see the [ImGui Lua Test Menu](https://github.com/TeamREPENTOGON/REPENTOGON/blob/main/repentogon/resources/scripts/repentogon_tests/test_imgui.lua), which can also be found in game.

## Code

[You can download the full mod and its assets by clicking here.](./zips/repentogon-imgui.zip)

???- example "Video Demonstration"
    <figure class="video_container">
        <video controls="true" allowfullscreen="true" muted="true" style="width:25rem">
            <source src="videos/imgui_fun_menu.mp4" type="video/mp4">
        </video>
        <figcaption>The menu created by the example mod.</figcaption>
    </figure>

???+ example Code
    ```lua
    local mod = RegisterMod("Repentogon ImGui", 1)
    local sfx = SFXManager()
    local game = Game()

    -- We'll use this later.
    local notificationText = "What do you want to shout..."

    -- This example mod demonstrates many general applications for ImGui.
    -- For a very thorough and generic usage example, please see the ImGui Lua Test Menu:
    -- https://github.com/TeamREPENTOGON/REPENTOGON/blob/main/repentogon/resources/scripts/repentogon_tests/test_imgui.lua

    -- Create a menu, which is a tab that appears in the top bar after pressing the debug console button.
    -- The element id should be something unique. There can only be one element per ID.
    -- This "\u{f0c3}" represents a unicode character. The font used for ImGui is powered by fontawesome.com, so it has many emojis you can use.
    -- Find an emoji, then click on it and find the Unicode representation at the top right.
    -- https://fontawesome.com/search?m=free&o=r
    ImGui.CreateMenu("ExampleMenu", "\u{f0c3} ImGui Example")

    -- To add a button under the dropdown for the menu, we need to use the generic AddElement function.
    -- This can be used to add any element, or you can use the dedicated function for the type of element if there is one.
    ImGui.AddElement("ExampleMenu", "ExampleMenuMainWindowButton", ImGuiElement.MenuItem, "\u{f005} Fun Menu")

    -- Create Fun Menu. This will hold some buttons and sliders that we will make do fun things.
    ImGui.CreateWindow("ExampleMenuFunMenu", "\u{f005} Fun Menu")

    -- Link it to ExampleMenuMainWindowButton, the button we made earlier.
    -- It'll now open or close when that button is pressed.
    ImGui.LinkWindowToElement("ExampleMenuFunMenu", "ExampleMenuMainWindowButton")

    -- The fun menu will have four things:
        -- A progress bar showing how close we are to missing Boss Rush.
        -- A that updates some text.
        -- A text input and button to cause a notification to appear.
        -- A button that plays a funny sound (SoundEffect.SOUND_DERP).

    -- First, add a progress bar showing how close we are to missing Boss Rush.
    -- The code controlling the value of it as at the bottom of this file.
    ImGui.AddProgressBar(
        "ExampleMenuFunMenu",
        "ExampleMenuBossRushProgress",
        "Boss Rush deadline!",
        0
    )

    -- Add that slider.
    ImGui.AddSliderInteger(
        "ExampleMenuFunMenu",
        "ExampleMenuHpSlider",
        "Cool slider", -- Slider label.
        function (newValue) -- Function ran whenever the slider is changed.
            print(newValue)
        end,
        50, -- Default value
        0, -- Minimum value
        100, -- Maximum value
        "Coolness factor: %d%" -- Number label formatting. So, at default, the label will say "Coolness factor: 50"
    )

    -- Add the text input and notification.
    ImGui.AddInputText(
        "ExampleMenuFunMenu",
        "ExampleMenuNotificationInput",
        "", -- We'll use the hint text to indicate what this does instead of a label.
        function (newValue) -- Function ran whenever the text input is updated.
            notificationText = newValue
        end,
        "", -- Default value.
        "What do you want to shout..." -- Hint text; This is the text that will appear when there is no user-inputted text in the box.
    )

    -- By default, ImGui elements are on separate lines.
    -- Add a "SameLine" element to make the next element appear on the same line.
    ImGui.AddElement(
        "ExampleMenuFunMenu",
        "", -- No element id needed since we're never gonna need to reference this later. We're allowed to add multiple elements to an empty id.
        ImGuiElement.SameLine
    )

    ImGui.AddButton(
        "ExampleMenuFunMenu",
        "ExampleMenuNotificationButton",
        "\u{f0a1}", -- Since this button will be small, we'll just put an emoji to indicate what it does.
        function (clickCount)
            -- Send a notification of type INFO with a lifetime of 5000 (in milliseconds).
            ImGui.PushNotification(notificationText, ImGuiNotificationType.INFO, 5000)
        end,
        true -- It'll be a small button.
    )

    -- Let's also add a help marker to indicator what the button does.
    -- Help markers look like: (?) and can be hovered over with your mouse to display a box of text.

    ImGui.SetHelpmarker("ExampleMenuNotificationButton", "Send a notification with the text of your choice using this fashionable button!")

    -- Finally, make a button that plays a funny sound.
    ImGui.AddButton(
        "ExampleMenuFunMenu",
        "ExampleMenuFunnyButton",
        "Funny button!", -- Button label.
        function (clickCount)
            sfx:Play(SoundEffect.SOUND_DERP, 1, 0, false, math.random(50, 150)/100) -- A random pitch is pretty funny...
        end,
        false -- Normal sized button.
    )

    -- Update the progress bar
    mod:AddCallback(ModCallbacks.MC_POST_UPDATE, function ()
        ImGui.UpdateData("ExampleMenuBossRushProgress", ImGuiData.Value, game.TimeCounter / game.BossRushParTime)
    end)
    ```