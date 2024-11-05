-- Create Menu entry
ImGui.CreateMenu("testMenu", "Test Menu")
-- Create Menu item that opens an empty window
ImGui.AddElement("testMenu", "menuElem1", ImGuiElement.MenuItem, "Minimal Window test")

ImGui.CreateWindow("testWindow", "Minimal Window test")
ImGui.LinkWindowToElement("testWindow", "menuElem1")
ImGui.SetColor("testWindow", ImGuiColor.WindowBg, 0, 0.25, 0.75, 1)
-- Menu item callback
ImGui.AddElement("testMenu", "menuElem2", ImGuiElement.MenuItem, "Menu Item Callback test")
ImGui.AddCallback("menuElem2", ImGuiCallback.Clicked, function(i) print("Clicked Counter " .. i) end)
ImGui.SetColor("menuElem2", ImGuiColor.Text, 0.75, 0.75, 0.75, 1)

-- Add big testing window
ImGui.AddElement("testMenu", "menuElem3", ImGuiElement.MenuItem, "Awesome Test Menu")

ImGui.CreateWindow("testWindow2", "Awesome Test Window")
ImGui.LinkWindowToElement("testWindow2", "menuElem3")


ImGui.AddElement("testWindow2", "cat1", ImGuiElement.CollapsingHeader, "Test Category")
ImGui.AddText("cat1", "A test text thats pretty long and stuff to test out text wrapping", true)
ImGui.AddElement("cat1", "", ImGuiElement.SeparatorText, "Separator text")
ImGui.AddElement("cat1", "tree1", ImGuiElement.TreeNode, "Test SubTree")
ImGui.SetTextColor("tree1", 0.75, 0.75, 0)
ImGui.AddElement("tree1", "", ImGuiElement.BulletText, "Bullet point 1")
ImGui.AddElement("tree1", "testbullet2", ImGuiElement.BulletText, "Bullet point 2")
ImGui.SetColor("testbullet2", ImGuiColor.Text, 0, 0.5, 1, 1)
ImGui.AddElement("tree1", "", ImGuiElement.BulletText, "Bullet point 3")
ImGui.AddElement("cat1", "", ImGuiElement.Separator)
ImGui.AddElement("cat1", "button1", ImGuiElement.Button, "A funky button")
ImGui.AddCallback("button1", ImGuiCallback.Hovered, function(_) print("Hover") end)
ImGui.AddElement("cat1", "", ImGuiElement.SameLine)
ImGui.AddElement("cat1", "button2", ImGuiElement.Button, "A button in the same line")
ImGui.SetTextColor("button2", 1, 0, 0, 0.75)
ImGui.SetColor("button2", ImGuiColor.Button, 1, 1, 0, 1)
ImGui.AddElement("cat1", "button3", ImGuiElement.Button, "A button causing an error")
ImGui.AddCallback("button3", ImGuiCallback.Clicked, function(_) VARIABLE_DOESNT_EXIST:Test() end)
ImGui.SetColor("button3", ImGuiColor.Button, 1, 0, 0, 1)

--------- CATEGORY WINDOWS ---------
ImGui.AddElement("testWindow2", "catWindows", ImGuiElement.CollapsingHeader, "Windows / Popups")
-- open window via button
ImGui.AddElement("catWindows", "button_window", ImGuiElement.Button, "open a window")
ImGui.CreateWindow("testWindow3", "button Window")
ImGui.LinkWindowToElement("testWindow3", "button_window")
--------- Render callback ---------
ImGui.AddText("testWindow3", "", true, "mousePosText")
ImGui.AddCallback("mousePosText", ImGuiCallback.Render, function(val)
    local mousePos = ImGui.GetMousePosition() -- this is already in imgui coordinates
    ImGui.UpdateText("mousePosText", "ImGui MousePos:\t" .. mousePos.X .. "\t" .. mousePos.Y)
    ImGui.SetWindowPosition("testWindow3", inputMousepos.X,inputMousepos.Y)
end)

-- open popup via button
ImGui.AddElement("catWindows", "button_popup2", ImGuiElement.Button, "open a popup")

ImGui.AddElement("catWindows", "popup", ImGuiElement.Popup, "Test Popup")
ImGui.AddText("popup", "Test popup text")
ImGui.LinkWindowToElement("popup", "button_popup2")

-- open popup via SetVisible
ImGui.AddElement("catWindows", "button_popup3", ImGuiElement.Button, "open a popup via SetVisible()")
ImGui.AddCallback("button_popup3", ImGuiCallback.Clicked, function(val) ImGui.SetVisible("popup", true) end)

ImGui.AddElement("catWindows", "tooltipButton", ImGuiElement.Button, "A button with tooltip and helpmarker")
-- ImGui.AddCallback("tooltipButton", ImGuiCallback.Hovered, function(val) print("Hovered") end) -- sanity check if tooltip on an object with hover callback works -- yes it works :) deactivated for less spam
ImGui.SetTooltip("tooltipButton", "My cool tooltip")
ImGui.SetHelpmarker("tooltipButton", "My cool Helpmarker")

-- open a window with window flags
ImGui.AddInputInteger("catWindows", "inputWindowFlags", "window flags", function(val) ImGui.SetWindowFlags("testWindow4", val) end, 0, 1, 1)
ImGui.AddElement("catWindows", "button_windowflags", ImGuiElement.Button, "open a window with window flags")
ImGui.CreateWindow("testWindow4", "button Window Flags")
ImGui.LinkWindowToElement("testWindow4", "button_windowflags")

--------- CATEGORY INPUTS ---------
ImGui.AddElement("testWindow2", "catInput", ImGuiElement.CollapsingHeader, "Input types")
ImGui.AddButton("catInput", "testButton1","Button", function(clickCount) print("Button clicked "..clickCount.." times") end)
ImGui.AddInputInteger("catInput", "inputInt", "int input", function(val) print(val) end, 10, 5, 500)
ImGui.AddInputFloat("catInput", "inputFloat", "float input", function(val) print(val) end, 2.5, 0.2, 5)
ImGui.AddDragInteger("catInput", "DragInt", "int Drag", function(val) print(val) end, 5, 1, 0, 100)
ImGui.AddDragFloat("catInput", "DragFloat", "float Drag", function(val) print(val) end, 2.5, 1, 0, 100)
ImGui.AddSliderInteger("catInput", "SliderInt", "int Slider", function(val) print(val) end, 0, 0, 100)
ImGui.AddSliderFloat("catInput", "SliderFloat", "float Slider", function(val) print(val) end, 0, 0, 100)
ImGui.AddSliderFloat("catInput", "SliderFloatScience", "float Slider Scientific", function(val) print(val) end, 500000000, 0, 999999999999999, "%e")
ImGui.AddInputColor("catInput", "inputColorRGB", "RGB input", function(r, g, b) print(r, g, b) end, 1, 0.25, 0.45)
ImGui.AddInputColor("catInput", "inputColorRGBA", "RGBA input", function(r, g, b, a) print(r, g, b, a) end, 0.5, 0.5, 0.5,
    0.5)
ImGui.AddCheckbox("catInput", "checkbox", "Test checkbox", function(val) print(val) end, true)
ImGui.AddRadioButtons("catInput", "radioButtons", function(val) print(val) end, { "Radio 1", "Radio 2", "Radio 3" }, 1)
ImGui.AddCombobox("catInput", "combobox1", "Combobox", function(index, val) print(index, val) end,
    { "Item 1", "Item 2", "Item 3" }, 1)
ImGui.AddCombobox("catInput", "combobox2", "Combobox slider", function(index, val) print(index, val) end,
    { "Item 1", "Item 2", "Item 3" }, 1, true)
ImGui.AddInputText("catInput", "inputText", "text input", function(val) print(val) end, "")
ImGui.AddInputText("catInput", "inputTextwHint", "text input w hint", function(val) print(val) end, "", "Some Hint")
ImGui.AddInputTextMultiline("catInput", "inputTextMulti", "Multiline text input", function(val) print(val) end,
    "Some\nCool text with linebreaks\nAnd Stuff", 7)

ImGui.AddInputKeyboard("catInput", "keyboardInput", "Keyboard Input", function(keyID, keyName) print(keyID, keyName) end, 0)
ImGui.AddInputController("catInput", "controllerInput", "Controller Input", function(keyID, keyName) print(keyID, keyName) end, 0) -- also tests ImGui bugfix for https://github.com/ocornut/imgui/issues/74

--------- Tab Testing ---------
ImGui.AddTabBar("testWindow2", "tabBar1")
ImGui.AddTab("tabBar1", "tab1", "Tab 1")
ImGui.AddText("tab1", "This is tab 1")
ImGui.AddTab("tabBar1", "tab2", "Tab 2")
ImGui.AddText("tab2", "This is tab 2")
ImGui.AddTab("tabBar1", "tab3", "Tab 3")
ImGui.AddText("tab3", "This is tab 3")

ImGui.AddElement("testWindow2", "", ImGuiElement.Separator)
ImGui.AddPlotLines("testWindow2", "plotLines", "Line Plot", { 0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2 }, "Test")
ImGui.AddPlotHistogram("testWindow2", "plotHistogram", "Histogram Plot", { 0.6, 0.1, 1.0, 0.5, 0.92, 0.1, 0.2 }, nil, nil, nil, 50)
ImGui.AddProgressBar("testWindow2", "progressbar1", "Progressbar", 0.5 )

--------- Window following the player ---------
-- open window via button
ImGui.AddElement("testWindow2", "button_windowFollowPlayer", ImGuiElement.Button, "open window following the player")
ImGui.CreateWindow("testWindowFollowPlayer", "Player")
ImGui.LinkWindowToElement("testWindowFollowPlayer", "button_windowFollowPlayer")
ImGui.SetWindowPinned("testWindowFollowPlayer", true)
--------- Render callback ---------
ImGui.AddText("testWindowFollowPlayer", "<-- Player")
ImGui.AddCallback("testWindowFollowPlayer", ImGuiCallback.Render, function(val)
    if Isaac.GetPlayer() ~= nil then
        local playerPos = ImGui.WorldToImGui(Isaac.GetPlayer().Position)
        ImGui.SetWindowPosition("testWindowFollowPlayer", playerPos.X, playerPos.Y)
    end
end)
