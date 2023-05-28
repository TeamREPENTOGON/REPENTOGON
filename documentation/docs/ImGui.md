# Class "ImGui"

## Reference
For element types we use the same names as in ImGui itself. You can find an interactive ImGui example here:

**[https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)**

## Functions

### Add·Callback () {: aria-label='Functions' }
#### void AddCallback ( string elementId, [ImGuiCallback](enums/ImGuiCallback.md) type , function func ) {: .copyable aria-label='Functions' }
Add a callback to an ImGui-element. An element can have one callback per type.
___
### Add·Element () {: aria-label='Functions' }
#### void AddElement ( string parentId, string elementId = "", [ImGuiElement](enums/ImGuiElement.md) type, string text = "" ) {: .copyable aria-label='Functions' }
Adds an element to a given parent.
___
### Create·Menu () {: aria-label='Functions' }
#### void CreateMenu ( string elementId, string text = "" ) {: .copyable aria-label='Functions' }
Creates an entry to the main menu bar of Repentogon.
___
### Create·Window () {: aria-label='Functions' }
#### void CreateWindow ( string elementId, string title = "" ) {: .copyable aria-label='Functions' }
Creates a window object. You need to use `LinkWindowToElement()` or `SetVisible()` to toggle the visibility of the window.
___
### Element·Exists () {: aria-label='Functions' }
#### boolean ElementExists ( string elementId ) {: .copyable aria-label='Functions' }
Returns true if an element with the given ID exists already.
___
### Hide () {: aria-label='Functions' }
#### void Hide ( ) {: .copyable aria-label='Functions' }
Closes ImGui.
___
### Link·Window·To·Element () {: aria-label='Functions' }
#### void LinkWindowToElement ( string windowId, string elementId ) {: .copyable aria-label='Functions' }
Connects a Window or Popup element to another element, making said element act as a "toggle" for that window.

???- example "Example Code"
    this code creates a new menu entry with one menuitem, which on click toggles a window
    ```lua
    local imgui = Isaac.GetImGui()
    imgui:CreateMenu("myMenu", "Test Menu")
    imgui:AddElement("myMenu", "myButton", ImGuiElement.MenuItem, "Some Text")
    imgui:CreateWindow("myWindow", "Some Window title")
    imgui:LinkWindowToElement("myWindow", "myButton")
    ```

___
### Remove·Callback () {: aria-label='Functions' }
#### void RemoveCallback ( string elementId, [ImGuiCallback](enums/ImGuiCallback.md) type ) {: .copyable aria-label='Functions' }
Remove the callback of the given type from the element.
___
### Remove·Menu () {: aria-label='Functions' }
#### void RemoveMenu ( string elementId ) {: .copyable aria-label='Functions' }

___
### Remove·Window () {: aria-label='Functions' }
#### void RemoveWindow ( string elementId ) {: .copyable aria-label='Functions' }

___
### Reset () {: aria-label='Functions' }
#### void Reset ( ) {: .copyable aria-label='Functions' }
Removes all custom defined Imgui elements and resets imgui back to its original state.
___
### Set·Visible () {: aria-label='Functions' }
#### void SetVisible ( string elementId, boolean visible ) {: .copyable aria-label='Functions' }

___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Opens ImGui.
___
### Update·Text () {: aria-label='Functions' }
#### void UpdateText ( string elementId, string text ) {: .copyable aria-label='Functions' }

___