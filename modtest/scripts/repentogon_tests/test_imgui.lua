local imgui = Isaac.GetImGui()

-- Create Menu entry
imgui:CreateMenu("testMenu", "Test Menu")
-- Create Menu item that opens an empty window
imgui:AddElement("testMenu", "menuElem1", ImGuiElement.MenuItem, "Minimal Window test")

imgui:CreateWindow("testWindow", "Minimal Window test")
imgui:LinkWindowToElement("testWindow", "menuElem1")
-- Menu item callback
imgui:AddElement("testMenu", "menuElem2", ImGuiElement.MenuItem, "Menu Item Callback test")
imgui:AddCallback("menuElem2", ImGuiCallback.Clicked, function (i) print("Clicked Counter "..i) end)

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
imgui:AddCallback("button1", ImGuiCallback.Hovered, function (i) print("Hover") end)
imgui:AddElement("cat1", "", ImGuiElement.SameLine)
imgui:AddElement("cat1", "button2", ImGuiElement.Button, "A button in the same line")


imgui:AddElement("testWindow2", "", ImGuiElement.Separator)

-- open window via button
imgui:AddElement("testWindow2", "button_window", ImGuiElement.Button, "open a window")
imgui:CreateWindow("testWindow3", "button Window")
imgui:LinkWindowToElement("testWindow3", "button_window")

-- open popup via button
imgui:AddElement("testWindow2", "button_popup2", ImGuiElement.Button, "open a popup")

imgui:AddElement("testWindow2", "popup", ImGuiElement.Popup, "Test Popup")
imgui:AddElement("popup", "", ImGuiElement.Text, "Test popup text")
imgui:LinkWindowToElement("popup", "button_popup2")

-- open popup via SetVisible
imgui:AddElement("testWindow2", "button_popup3", ImGuiElement.Button, "open a popup via SetVisible()")
imgui:AddCallback("button_popup3", ImGuiCallback.Clicked, function (i) imgui:SetVisible("popup", true) end)