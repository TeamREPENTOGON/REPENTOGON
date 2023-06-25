local imgui = Isaac.GetImGui()

-- Create Menu entry
imgui:CreateMenu("testMenu", "Test Menu")
-- Create Menu item that opens an empty window
imgui:AddElement("testMenu", "menuElem1", ImGuiElement.MenuItem, "Minimal Window test")

imgui:CreateWindow("testWindow", "Minimal Window test")
imgui:LinkWindowToElement("testWindow", "menuElem1")
-- Menu item callback
imgui:AddElement("testMenu", "menuElem2", ImGuiElement.MenuItem, "Menu Item Callback test")
imgui:AddCallback("menuElem2", ImGuiCallback.Clicked, function(i) print("Clicked Counter " .. i) end)

-- Add big testing window
imgui:AddElement("testMenu", "menuElem3", ImGuiElement.MenuItem, "Awesome Test Menu")

imgui:CreateWindow("testWindow2", "Awesome Test Window")
imgui:LinkWindowToElement("testWindow2", "menuElem3")


imgui:AddElement("testWindow2", "cat1", ImGuiElement.CollapsingHeader, "Test Category")
imgui:AddElement("cat1", "", ImGuiElement.Text, "A test text thats pretty long and stuff")
imgui:AddElement("cat1", "", ImGuiElement.SeparatorText, "Separator text")
imgui:AddElement("cat1", "tree1", ImGuiElement.TreeNode, "Test SubTree")
imgui:AddElement("tree1", "", ImGuiElement.BulletText, "Bullet point 1")
imgui:AddElement("tree1", "", ImGuiElement.BulletText, "Bullet point 2")
imgui:AddElement("tree1", "", ImGuiElement.BulletText, "Bullet point 3")
imgui:AddElement("cat1", "", ImGuiElement.Separator)
imgui:AddElement("cat1", "button1", ImGuiElement.Button, "A funky button")
imgui:AddCallback("button1", ImGuiCallback.Hovered, function(i) print("Hover") end)
imgui:AddElement("cat1", "", ImGuiElement.SameLine)
imgui:AddElement("cat1", "button2", ImGuiElement.Button, "A button in the same line")

--------- CATEGORY WINDOWS ---------
imgui:AddElement("testWindow2", "catWindows", ImGuiElement.CollapsingHeader, "Windows / Popups")
-- open window via button
imgui:AddElement("catWindows", "button_window", ImGuiElement.Button, "open a window")
imgui:CreateWindow("testWindow3", "button Window")
imgui:LinkWindowToElement("testWindow3", "button_window")

-- open popup via button
imgui:AddElement("catWindows", "button_popup2", ImGuiElement.Button, "open a popup")

imgui:AddElement("catWindows", "popup", ImGuiElement.Popup, "Test Popup")
imgui:AddElement("popup", "", ImGuiElement.Text, "Test popup text")
imgui:LinkWindowToElement("popup", "button_popup2")

-- open popup via SetVisible
imgui:AddElement("catWindows", "button_popup3", ImGuiElement.Button, "open a popup via SetVisible()")
imgui:AddCallback("button_popup3", ImGuiCallback.Clicked, function(val) imgui:SetVisible("popup", true) end)

imgui:AddElement("catWindows", "tooltipButton", ImGuiElement.Button, "A button with tooltip and helpmarker")
-- imgui:AddCallback("tooltipButton", ImGuiCallback.Hovered, function(val) print("Hovered") end) -- sanity check if tooltip on an object with hover callback works -- yes it works :) deactivated for less spam
imgui:SetTooltip("tooltipButton", "My cool tooltip")
imgui:SetHelpmarker("tooltipButton", "My cool Helpmarker")

--------- CATEGORY INPUTS ---------
imgui:AddElement("testWindow2", "catInput", ImGuiElement.CollapsingHeader, "Input types")
imgui:AddButton("catInput", "testButton1","Button", function(clickCount) print("Button clicked "..clickCount.." times") end)
imgui:AddInputInteger("catInput", "inputInt", "int input", function(val) print(val) end, 10, 5, 500)
imgui:AddInputFloat("catInput", "inputFloat", "float input", function(val) print(val) end, 2.5, 0.2, 5)
imgui:AddDragInteger("catInput", "DragInt", "int Drag", function(val) print(val) end, 5, 1, 0, 100)
imgui:AddDragFloat("catInput", "DragFloat", "float Drag", function(val) print(val) end, 2.5, 1, 0, 100)
imgui:AddSliderInteger("catInput", "SliderInt", "int Slider", function(val) print(val) end, 0, 0, 100)
imgui:AddSliderFloat("catInput", "SliderFloat", "float Slider", function(val) print(val) end, 0, 0, 100)
imgui:AddInputColor("catInput", "inputColorRGB", "RGB input", function(r, g, b) print(r, g, b) end, 1, 0.25, 0.45)
imgui:AddInputColor("catInput", "inputColorRGBA", "RGBA input", function(r, g, b, a) print(r, g, b, a) end, 0.5, 0.5, 0.5,
    0.5)
imgui:AddCheckbox("catInput", "checkbox", "Test checkbox", function(val) print(val) end, true)
imgui:AddRadioButtons("catInput", "radioButtons", function(val) print(val) end, { "Radio 1", "Radio 2", "Radio 3" }, 1)
imgui:AddCombobox("catInput", "combobox1", "Combobox", function(index, val) print(index, val) end,
    { "Item 1", "Item 2", "Item 3" }, 1)
imgui:AddCombobox("catInput", "combobox2", "Combobox slider", function(index, val) print(index, val) end,
    { "Item 1", "Item 2", "Item 3" }, 1, true)
imgui:AddInputText("catInput", "inputText", "text input", function(val) print(val) end, "")
imgui:AddInputText("catInput", "inputTextwHint", "text input w hint", function(val) print(val) end, "", "Some Hint")
imgui:AddInputTextMultiline("catInput", "inputTextMulti", "Multiline text input", function(val) print(val) end,
    "Some\nCool text with linebreaks\nAnd Stuff", 7)

imgui:AddInputKeyboard("catInput", "keyboardInput", "Keyboard Input", function(keyID, keyName) print(keyID, keyName) end, 0)
imgui:AddInputController("catInput", "controllerInput", "Controller Input", function(keyID, keyName) print(keyID, keyName) end, 0) -- also tests ImGui bugfix for https://github.com/ocornut/imgui/issues/74

--------- Tab Testing ---------
imgui:AddTabBar("testWindow2", "tabBar1")
imgui:AddTab("tabBar1", "tab1", "Tab 1")
imgui:AddElement("tab1", "", ImGuiElement.Text, "This is tab 1")
imgui:AddTab("tabBar1", "tab2", "Tab 2")
imgui:AddElement("tab2", "", ImGuiElement.Text, "This is tab 2")
imgui:AddTab("tabBar1", "tab3", "Tab 3")
imgui:AddElement("tab3", "", ImGuiElement.Text, "This is tab 3")

imgui:AddElement("testWindow2", "", ImGuiElement.Separator)

--------- Render callback ---------
imgui:AddElement("testWindow2", "mousePosText", ImGuiElement.Text, "")
imgui:AddCallback("mousePosText", ImGuiCallback.Render, function(val)
    local mousePos = imgui:GetMousePosition()
    imgui:UpdateText("mousePosText", "MousePos: " .. mousePos.X .. "\t" .. mousePos.Y)
end)
