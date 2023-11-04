# Class "ImGui"

## Reference
For element types we use the same names as in ImGui itself. You can find an interactive ImGui example here:

**[https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)**

## Icons
All imgui text supports the usage of icons. Right now, we use "FontAwesome 6", which provides ~1400 icons. You can search for fitting icons here:[https://fontawesome.com/search?o=r&m=free&s=solid](https://fontawesome.com/search?o=r&m=free&s=solid)

### Usage in Lua:
If you want to add an Icon into your widget, just use the "Unicode" representation of the icon and put it in between a `\u{ }` string. You can find this, by selecting the icon on the fontawesome page, and looking in the top right corner of the popup-window. You can add it to your element like this:

`"\u{f0f9} My Text"`

This will add the "truck-medical" icon in front of the text "My text".

Result: ":fontawesome-solid-truck-medical: My Text"


## Functions

### Add·Button () {: aria-label='Functions' }
#### void AddButton ( string parentId, string elementId, string label = "", function clickCallback = nil, boolean isSmall = false ) {: .copyable aria-label='Functions' }
___
### Add·Callback () {: aria-label='Functions' }
#### void AddCallback ( string elementId, [ImGuiCallback](enums/ImGuiCallback.md) type, function func ) {: .copyable aria-label='Functions' }
Add a callback to an ImGui-element. An element can have one callback per type.
___
### Add·Checkbox () {: aria-label='Functions' }
#### void AddCheckbox ( string parentId, string elementId, string label = "", function changeCallback = nil, boolean isActive = false ) {: .copyable aria-label='Functions' }
___
### Add·Combobox () {: aria-label='Functions' }
#### void AddCombobox ( string parentId, string elementId, string label = "", function changeCallback = nil, table options, int selectedIndex = 0, boolean isSlider = false ) {: .copyable aria-label='Functions' }
Adds a Combobox element which represents a single line element that allows you to select a value from a dropdown menu. If `isSlider` is set to true, instead of a dropdown menu, the values can be selected by interacting with a slider element.

???+ example "Example Code"
    ```lua
    local imgui = Isaac.GetImGui()
    imgui:AddCombobox("catInput", "combobox1", "Combobox", function(index, val) print(index, val) end, { "Item 1", "Item 2", "Item 3" }, 1)
    ```
___
### Add·Drag·Float () {: aria-label='Functions' }
#### void AddDragFloat ( string parentId, string elementId, string label = "", function changeCallback = nil, float defaultVal = 0, float speed = 1, float min = INTEGER_MIN, float min = INTEGER_MAX, string formatting = "%.3f" ) {: .copyable aria-label='Functions' }
___
### Add·Drag·Integer () {: aria-label='Functions' }
#### void AddDragInteger ( string parentId, string elementId, string label = "", function changeCallback = nil, int defaultVal = 0, float speed = 1, int min = INTEGER_MIN, int min = INTEGER_MAX, string formatting = "%d%" ) {: .copyable aria-label='Functions' }
___
### Add·Element () {: aria-label='Functions' }
#### void AddElement ( string parentId, string elementId = "", [ImGuiElement](enums/ImGuiElement.md) type, string label = "" ) {: .copyable aria-label='Functions' }
Adds a generic element to a given parent. Useful to add control elements like "SameLine", "Bullet" or "Text".
___
### Add·Input·Color () {: aria-label='Functions' }
#### void AddInputColor ( string parentId, string elementId, string label = "", function changeCallback = nil, float r = 0, float g = 0, float b = 0) {: .copyable aria-label='Functions' }
#### void AddInputColor ( string parentId, string elementId, string label = "", function changeCallback = nil, float r = 0, float g = 0, float b = 0, float a = 1 ) {: .copyable aria-label='Functions' }
Adds a color input element. If the parameter `a` is set, it acts as an RGBA input. Otherwise its just an RGB input. The float values are between 0 and 1.

The callback gets passed the r,g,b and a values as seperate parameters.

???+ example "Example Code"
    ```lua
    local imgui = Isaac.GetImGui()
    imgui:AddInputColor("catInput", "inputColorRGB", "RGB input", function(r, g, b) print(r, g, b) end, 1, 0.25, 0.45)
    imgui:AddInputColor("catInput", "inputColorRGBA", "RGBA input", function(r, g, b, a) print(r, g, b, a) end, 0.5, 0.5, 0.5,
        0.5)
    ```
___
### Add·Input·Controller () {: aria-label='Functions' }
#### void AddInputController ( string parentId, string elementId, string buttonLabel = "", function changeCallback = nil, float defaultVal = 0 ) {: .copyable aria-label='Functions' }
Adds an input for Gamepad / controller buttons.

The callback gets passed the [ButtonAction ID](https://wofsauge.github.io/IsaacDocs/rep/enums/ButtonAction.html) and the ImGuiKey name of the new button.
___
### Add·Input·Float () {: aria-label='Functions' }
#### void AddInputFloat ( string parentId, string elementId, string label = "", function changeCallback = nil, float defaultVal = 0, float step = 1, float stepFast = 100  ) {: .copyable aria-label='Functions' }
___
### Add·Input·Integer () {: aria-label='Functions' }
#### void AddInputInteger ( string parentId, string elementId, string label = "", function changeCallback = nil, int defaultVal = 0, int step = 1, int stepFast = 100  ) {: .copyable aria-label='Functions' }
___
### Add·Input·Keyboard () {: aria-label='Functions' }
#### void AddInputKeyboard ( string parentId, string elementId, string buttonLabel = "", function changeCallback = nil, float defaultVal = 0 ) {: .copyable aria-label='Functions' }
Adds an input for keyboard buttons.

The callback gets passed the [Keyboard key ID](https://wofsauge.github.io/IsaacDocs/rep/enums/Keyboard.html) and the ImGuiKey name of the new button.
___
### Add·Input·Text () {: aria-label='Functions' }
#### void AddInputText ( string parentId, string elementId, string description = "", function changeCallback = nil, string defaultVal = "", string hintText = "" ) {: .copyable aria-label='Functions' }
Adds a text input element. The text from `hintText` will get displayed as a "placeholder" inside the input element, if the input of the element is empty.
___
### Add·Input·Text·Multiline () {: aria-label='Functions' }
#### void AddInputTextMultiline ( string parentId, string elementId, string description = "", function changeCallback = nil, string defaultVal = "", float displayedLines = 6 ) {: .copyable aria-label='Functions' }
Adds a text input element that allows to input multiple lines of text. The attribute `displayedLines` can be used to change the height of the element.
___
### Add·Plot·Histogram () {: aria-label='Functions' }
#### void AddPlotHistogram ( string parentId, string elementId, string label = "", table values, string overlayText = "", float minimum = FLT_MIN, float maximum = FLT_MAX, float height = 40 ) {: .copyable aria-label='Functions' }
Adds a bar-diagram displaying the given data as vertical bars. On default, minimum and maximum are set "dynamicaly", making the diagram fit its content perfectly.
___
### Add·Plot·Lines () {: aria-label='Functions' }
#### void AddPlotLines ( string parentId, string elementId, string label = "", table values, string overlayText = "", float minimum = FLT_MIN, float maximum = FLT_MAX, float height = 40 ) {: .copyable aria-label='Functions' }
Adds a line-diagram connecting the given values using lines. On default, minimum and maximum are set "dynamicaly", making the diagram fit its content perfectly.
___
### Add·Progress·Bar () {: aria-label='Functions' }
#### void AddProgressBar ( string parentId, string elementId, string label = "", float progress = 0, string overlayText = "__DEFAULT__" ) {: .copyable aria-label='Functions' }
Adds a progressbar element. The `progress` value defines the fill percentage (0 to 1). 

If the `overlayText` was not defined, the progressbar will display the current fill state in percent inside the progressbar (for example 50% when progress is set to 0.5).

If the `label` is empty, the progressbar will render over the full width of the parent element.
___
### Add·Radio·Buttons () {: aria-label='Functions' }
#### void AddRadioButtons ( string parentId, string elementId, function changeCallback = nil, table options, int selectedIndex = 0, boolean renderSameLine = true ) {: .copyable aria-label='Functions' }

???+ example "Example Code"
    ```lua
    local imgui = Isaac.GetImGui()
    imgui:AddRadioButtons("catInput", "radioButtons", function(index) print(index) end, { "Radio 1", "Radio 2", "Radio 3" }, 1)
    ```
___
### Add·Slider·Float () {: aria-label='Functions' }
#### void AddSliderFloat ( string parentId, string elementId, string label = "", function changeCallback = nil, float defaultVal = 0, float min = INTEGER_MIN, float min = INTEGER_MAX, string formatting = "%.3f" ) {: .copyable aria-label='Functions' }
___
### Add·Slider·Integer () {: aria-label='Functions' }
#### void AddSliderInteger ( string parentId, string elementId, string label = "", function changeCallback = nil, int defaultVal = 0, int min = INTEGER_MIN, int min = INTEGER_MAX, string formatting = "%d%" ) {: .copyable aria-label='Functions' }
___
### Add·Tab·Bar () {: aria-label='Functions' }
#### void AddTabBar ( string parentId, string elementId ) {: .copyable aria-label='Functions' }
A TabBar is a container which is used to store Tab elements.
___
### Add·Tab () {: aria-label='Functions' }
#### void AddTab ( string parentId, string elementId, string label ) {: .copyable aria-label='Functions' }
A tab is a clickable area that shows another page or area.

The parent object needs to be a TabBar.
___
### Add·Text () {: aria-label='Functions' }
#### void AddText ( string parentId, string text, boolean wrapText = false, string elementId = "" ) {: .copyable aria-label='Functions' }
Creates a text element. If `wrapText` is set to `true`, the text will wrap on the window borders. If set to `false` it will expand the window content till it fits.

The elementID can be set as well, if the text should be able to be edited by later code.
___
### Create·Menu () {: aria-label='Functions' }
#### void CreateMenu ( string elementId, string label = "" ) {: .copyable aria-label='Functions' }
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
### Get·Mouse·Position () {: aria-label='Functions' }
#### void GetMousePosition ( ) {: .copyable aria-label='Functions' }
Returns the mouse position in Screen coordinates.

Use this instead of `Input.GetMousePosition()` when working with imgui!
___
### Get·Window·Pinned () {: aria-label='Functions' }
#### boolean GetWindowPinned ( string windowId ) {: .copyable aria-label='Functions' }
Get the pinned state of a window.
___
### Get·Visible () {: aria-label='Functions' }
#### boolean GetVisible ( string elementId ) {: .copyable aria-label='Functions' }
Get if a window element is visible or not.
___
### Hide () {: aria-label='Functions' }
#### void Hide ( ) {: .copyable aria-label='Functions' }
Closes ImGui.
___
### Im·Gui·To·World () {: aria-label='Functions' }
#### void ImGuiToWorld ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position ) {: .copyable aria-label='Functions' }
Converts ImGui coordinates into World coordinates.

???+ bug "Bug"
    This function does not work correctly when the game's scale factor exceeds MaxRenderScale.
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
### Push·Notification () {: aria-label='Functions' }
#### void PushNotification ( const text, int severity = 0, int lifetime = 5000 ) {: .copyable aria-label='Functions' }        
Displays a pop-up message window in the style of a notification.
Severity values are 0 = Info, 1 = success, 2 = warning, 3 = error
___
### Remove·Callback () {: aria-label='Functions' }
#### void RemoveCallback ( string elementId, [ImGuiCallback](enums/ImGuiCallback.md) type ) {: .copyable aria-label='Functions' }
Remove the callback of the given type from the element.
___
### Remove·Color () {: aria-label='Functions' }
#### void RemoveColor ( string elementId, [ImGuiColor](enums/ImGuiColor.md) colorType ) {: .copyable aria-label='Functions' }
Remove a color modifier of the given type from the element.
___
### Remove·Element () {: aria-label='Functions' }
#### void RemoveElement ( string elementId ) {: .copyable aria-label='Functions' }
General function to remove any kind of element.
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
### Set·Color () {: aria-label='Functions' }
#### void SetColor ( string elementId, [ImGuiColor](enums/ImGuiColor.md) colorType, float red, float green, float blue, float alpha = 1.0 ) {: .copyable aria-label='Functions' }
Adds a color modifier to a given element.
___
### Set·Text·Color () {: aria-label='Functions' }
#### void SetTextColor ( string elementId, float red, float green, float blue, float alpha = 1.0 ) {: .copyable aria-label='Functions' }
Shortcut function to add a color modifier to text of a given element.
___
### Set·Helpmarker () {: aria-label='Functions' }
#### void SetHelpmarker ( string elementId, string text ) {: .copyable aria-label='Functions' }
Adds a helpmarker to a given element. A Helpmarker is a `(?)` element rendered on the right of an element, which when hovered displays a tooltip.
___
### Set·Tooltip () {: aria-label='Functions' }
#### void SetTooltip ( string elementId, string text ) {: .copyable aria-label='Functions' }
Adds a tooltip to a given element. The tooltip is visible when the user hovers over the element.
___
### Set·Visible () {: aria-label='Functions' }
#### void SetVisible ( string elementId, boolean visible ) {: .copyable aria-label='Functions' }

___
### Set·Window·Pinned () {: aria-label='Functions' }
#### void SetWindowPinned ( string windowId, boolean pinned ) {: .copyable aria-label='Functions' }
Set the pinned state of a window, making it visible when the ImGui interface is not active.
___
### Set·Window·Position () {: aria-label='Functions' }
#### void SetWindowPosition ( string windowId, float x, float y ) {: .copyable aria-label='Functions' }
Set the position of a window in screen coordinates.
___
### Set·Window·Size () {: aria-label='Functions' }
#### void SetWindowSize ( string windowId, float width, float height ) {: .copyable aria-label='Functions' }
Set the width and height of a window, in pixels.
___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Opens ImGui.
___
### Update·Data () {: aria-label='Functions' }
#### void UpdateData ( string elementId, [ImGuiData](enums/ImGuiData.md) dataType, int newDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string elementId, [ImGuiData](enums/ImGuiData.md) dataType, float newDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string elementId, [ImGuiData](enums/ImGuiData.md) dataType, bool newDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string elementId, [ImGuiData](enums/ImGuiData.md) dataType, string newDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string elementId, [ImGuiData](enums/ImGuiData.md) dataType, table newDataValues ) {: .copyable aria-label='Functions' }
Update arbitrary data of a given element. See [ImGuiData](enums/ImGuiData.md) for possible data to update.

The dataTypes and the expected newDataValue are evaluated per element. Therefore, if you try to update data of an element where this data is not used, this function will throw an error for you.
___
### Update·Text () {: aria-label='Functions' }
#### void UpdateText ( string elementId, string text ) {: .copyable aria-label='Functions' }
Shortcut function to update an element text or label.
___
### World·To·Im·Gui () {: aria-label='Functions' }
#### void WorldToImGui ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position ) {: .copyable aria-label='Functions' }
Converts world coordinates into ImGui coordinates.

???+ bug "Bug"
    This function does not work correctly when the game's scale factor exceeds MaxRenderScale.
___
