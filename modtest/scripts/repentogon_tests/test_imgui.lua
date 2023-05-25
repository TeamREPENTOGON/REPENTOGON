local imgui =Isaac.GetImGui() 

-- Create Menu entry
Isaac.GetImGui():AddElementToMenu("Test Menu", ImGuiElement.Menu)

-- Create Menu item that opens an empty window
Isaac.GetImGui():AddElementToMenu("Minimal Window test", ImGuiElement.MenuItem,"Test Menu")
Isaac.GetImGui():AddWindow("Minimal Window test", "Minimal Window")

-- Menu item callback
Isaac.GetImGui():AddElementToMenu("Menu Item Callback test", ImGuiElement.MenuItem, "Test Menu")
Isaac.GetImGui():AddCallbackToMenuElement("Menu Item Callback test", ImGuiCallback.Clicked, function (i) print("Clicked Counter "..i) end)

-- Add big testing window
imgui:AddElementToMenu("Test Window", ImGuiElement.MenuItem,"Test Menu")
local testWindow = "Super cool test window"
imgui:AddWindow("Test Window", testWindow) 

imgui:AddElementToWindow(testWindow, "Test Category", ImGuiElement.CollapsingHeader, "") 
imgui:AddElementToWindow(testWindow, "A test text thats pretty long and stuff", ImGuiElement.Text, "Test Category") 
imgui:AddElementToWindow(testWindow, "Separator text", ImGuiElement.SeparatorText, "Test Category") 
imgui:AddElementToWindow(testWindow, "Test SubTree", ImGuiElement.TreeNode, "Test Category") 
imgui:AddElementToWindow(testWindow, "Bullet point 1", ImGuiElement.BulletText, "Test SubTree") 
imgui:AddElementToWindow(testWindow, "Bullet point 2", ImGuiElement.BulletText, "Test SubTree") 
imgui:AddElementToWindow(testWindow, "Bullet point 3", ImGuiElement.BulletText, "Test SubTree") 
imgui:AddElementToWindow(testWindow, "", ImGuiElement.Separator, "Test Category") 
imgui:AddElementToWindow(testWindow, "A funky button", ImGuiElement.Button, "Test Category")
imgui:AddCallbackToWindowElement(testWindow, "A hoverable button", ImGuiCallback.Hovered, function (i) print("Hover") end) 
imgui:AddElementToWindow(testWindow, "", ImGuiElement.SameLine, "Test Category") 
imgui:AddElementToWindow(testWindow, "A button in the same line", ImGuiElement.Button, "Test Category")