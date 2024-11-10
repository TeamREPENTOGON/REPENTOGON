---
tags:
  - Class
---
# Class "ImGui"

An example mod using the ImGui class can be found [here.](./examples/ImGuiMenu.md)

???+ info
    You can get this class by using the global table "ImGui"


    ???+ example "Example Code"
        ```lua
        local isblind = ImGui.GetVisible("braillemenu")
        ```
    ### Reference
    For element types we use the same names as in ImGui itself. Check out the **[interactive ImGui example](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)**.
    
    ### Icons
    All imgui text supports the usage of icons. Right now, we use "FontAwesome 6", which provides ~1400 icons. You can search for fitting icons here: [https://fontawesome.com/search?o=r&m=free&s=solid](https://fontawesome.com/search?o=r&m=free&s=solid)

    **Icon usage in Lua:**

    If you want to add an Icon into your widget, just use the "Unicode" representation of the icon and put it in between a `\u{ }` string. You can find this, by selecting the icon on the fontawesome page, and looking in the top right corner of the popup-window. You can add it to your element like this:

    `"\u{f0f9} My Text"`

    This will add the "truck-medical" icon in front of the text "My text".

    Result: ":fontawesome-solid-truck-medical: My Text"


## Functions

### AddButton () {: aria-label='Functions' }
#### void AddButton ( string ParentId, string ElementId, string Label = "", function ClickCallback = nil, boolean IsSmall = false ) {: .copyable aria-label='Functions' }
___
### AddCallback () {: aria-label='Functions' }
#### void AddCallback ( string ElementId, [ImGuiCallback](enums/ImGuiCallback.md) Type, Function func ) {: .copyable aria-label='Functions' }
Add a callback to an ImGui-element. An element can have one callback per type.
___
### AddCheckbox () {: aria-label='Functions' }
#### void AddCheckbox ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, boolean IsActive = false ) {: .copyable aria-label='Functions' }
___
### AddCombobox () {: aria-label='Functions' }
#### void AddCombobox ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, table Options, int SelectedIndex = 0, boolean IsSlider = false ) {: .copyable aria-label='Functions' }
Adds a Combobox element which represents a single line element that allows you to select a value from a dropdown menu. If `isSlider` is set to true, instead of a dropdown menu, the values can be selected by interacting with a slider element.

???+ example "Example Code"
    ```lua
    ImGui.AddCombobox("catInput", "combobox1", "Combobox", function(index, val) print(index, val) end, { "Item 1", "Item 2", "Item 3" }, 1)
    ```
___
### AddDragFloat () {: aria-label='Functions' }
#### void AddDragFloat ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, float DefaultVal = 0, float Speed = 1, float Min = INTEGER_MIN, float Min = INTEGER_MAX, string Formatting = "%.3f" ) {: .copyable aria-label='Functions' }
___
### AddDragInteger () {: aria-label='Functions' }
#### void AddDragInteger ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, int DefaultVal = 0, float Speed = 1, int Min = INTEGER_MIN, int Min = INTEGER_MAX, string Formatting = "%d%" ) {: .copyable aria-label='Functions' }
___
### AddElement () {: aria-label='Functions' }
#### void AddElement ( string ParentId, string ElementId = "", [ImGuiElement](enums/ImGuiElement.md) type, string Label = "" ) {: .copyable aria-label='Functions' }
Adds a generic element to a given parent. Useful to add control elements like "SameLine", "Bullet" or "Text".
___
### AddInputColor () {: aria-label='Functions' }
#### void AddInputColor ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, float r = 0, float g = 0, float b = 0) {: .copyable aria-label='Functions' }
#### void AddInputColor ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, float r = 0, float g = 0, float b = 0, float a = 1 ) {: .copyable aria-label='Functions' }
Adds a color input element. If the parameter `a` is set, it acts as an RGBA input. Otherwise its just an RGB input. The float values are between `0` and `1`.

The callback gets passed the r,g,b and a values as seperate parameters.

???+ example "Example Code"
    ```lua
    ImGui.AddInputColor("catInput", "inputColorRGB", "RGB input", function(r, g, b) print(r, g, b) end, 1, 0.25, 0.45)
    ImGui.AddInputColor("catInput", "inputColorRGBA", "RGBA input", function(r, g, b, a) print(r, g, b, a) end, 0.5, 0.5, 0.5,
        0.5)
    ```
___
### AddInputController () {: aria-label='Functions' }
#### void AddInputController ( string ParentId, string ElementId, string ButtonLabel = "", function ChangeCallback = nil, float DefaultVal = 0 ) {: .copyable aria-label='Functions' }
Adds an input for Gamepad / controller buttons.

The callback gets passed the [ButtonAction ID](https://wofsauge.github.io/IsaacDocs/rep/enums/ButtonAction.html) and the ImGuiKey name of the new button.
___
### AddInputFloat () {: aria-label='Functions' }
#### void AddInputFloat ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, float DefaultVal = 0, float Step = 1, float StepFast = 100  ) {: .copyable aria-label='Functions' }
___
### AddInputInteger () {: aria-label='Functions' }
#### void AddInputInteger ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, int DefaultVal = 0, int Step = 1, int StepFast = 100  ) {: .copyable aria-label='Functions' }
___
### AddInputKeyboard () {: aria-label='Functions' }
#### void AddInputKeyboard ( string ParentId, string ElementId, string ButtonLabel = "", function ChangeCallback = nil, float DefaultVal = 0 ) {: .copyable aria-label='Functions' }
Adds an input for keyboard buttons.

The callback gets passed the [Keyboard key ID](https://wofsauge.github.io/IsaacDocs/rep/enums/Keyboard.html) and the ImGuiKey name of the new button.
___
### AddInputText () {: aria-label='Functions' }
#### void AddInputText ( string ParentId, string ElementId, string Description = "", function ChangeCallback = nil, string DefaultVal = "", string HintText = "" ) {: .copyable aria-label='Functions' }
Adds a text input element. The text from `HintText` will get displayed as a "placeholder" inside the input element, if the input of the element is empty.
___
### AddInputTextMultiline () {: aria-label='Functions' }
#### void AddInputTextMultiline ( string ParentId, string ElementId, string Description = "", function ChangeCallback = nil, string DefaultVal = "", float DisplayedLines = 6 ) {: .copyable aria-label='Functions' }
Adds a text input element that allows to input multiple lines of text. The attribute `displayedLines` can be used to change the height of the element.
___
### AddPlotHistogram () {: aria-label='Functions' }
#### void AddPlotHistogram ( string ParentId, string ElementId, string Label = "", table Values, string OverlayText = "", float Minimum = FLT_MIN, float Maximum = FLT_MAX, float Height = 40 ) {: .copyable aria-label='Functions' }
Adds a bar-diagram displaying the given data as vertical bars. On default, minimum and maximum are set "dynamicaly", making the diagram fit its content perfectly.
___
### AddPlotLines () {: aria-label='Functions' }
#### void AddPlotLines ( string ParentId, string ElementId, string Label = "", table Values, string OverlayText = "", float Minimum = FLT_MIN, float Maximum = FLT_MAX, float Height = 40 ) {: .copyable aria-label='Functions' }
Adds a line-diagram connecting the given values using lines. On default, minimum and maximum are set "dynamicaly", making the diagram fit its content perfectly.
___
### AddProgressBar () {: aria-label='Functions' }
#### void AddProgressBar ( string ParentId, string ElementId, string Label = "", float Progress = 0, string OverlayText = "__DEFAULT__" ) {: .copyable aria-label='Functions' }
Adds a progressbar element. The `progress` value defines the fill percentage (`0` to `1`). 

If the `overlayText` was not defined, the progressbar will display the current fill state in percent inside the progressbar (for example 50% when progress is set to 0.5).

If the `label` is empty, the progressbar will render over the full width of the parent element.
___
### AddRadioButtons () {: aria-label='Functions' }
#### void AddRadioButtons ( string ParentId, string ElementId, function ChangeCallback = nil, table options, int SelectedIndex = 0, boolean renderSameLine = true ) {: .copyable aria-label='Functions' }

???+ example "Example Code"
    ```lua
    ImGui.AddRadioButtons("catInput", "radioButtons", function(index) print(index) end, { "Radio 1", "Radio 2", "Radio 3" }, 1)
    ```
___
### AddSliderFloat () {: aria-label='Functions' }
#### void AddSliderFloat ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, float DefaultVal = 0, float Min = INTEGER_MIN, float Max = INTEGER_MAX, string Formatting = "%.3f" ) {: .copyable aria-label='Functions' }
___
### AddSliderInteger () {: aria-label='Functions' }
#### void AddSliderInteger ( string ParentId, string ElementId, string Label = "", function ChangeCallback = nil, int DefaultVal = 0, int Min = INTEGER_MIN, int Max = INTEGER_MAX, string Formatting = "%d%" ) {: .copyable aria-label='Functions' }
___
### AddTab () {: aria-label='Functions' }
#### void AddTab ( string ParentId, string ElementId, string Label ) {: .copyable aria-label='Functions' }
A tab is a clickable area that shows another page or area.

The parent object needs to be a TabBar.
___
### AddTabBar () {: aria-label='Functions' }
#### void AddTabBar ( string ParentId, string ElementId ) {: .copyable aria-label='Functions' }
A TabBar is a container which is used to store Tab elements.
___
### AddText () {: aria-label='Functions' }
#### void AddText ( string ParentId, string Text, boolean WrapText = false, string ElementId = "" ) {: .copyable aria-label='Functions' }
Creates a text element. If `wrapText` is set to `true`, the text will wrap on the window borders. If set to `false` it will expand the window content till it fits.

The ElementId can be set as well, if the text should be able to be edited by later code.
___
### CreateMenu () {: aria-label='Functions' }
#### void CreateMenu ( string ElementId, string Label = "" ) {: .copyable aria-label='Functions' }
Creates an entry to the main menu bar of Repentogon.
___
### CreateWindow () {: aria-label='Functions' }
#### void CreateWindow ( string ElementId, string Title = "" ) {: .copyable aria-label='Functions' }
Creates a window object. You need to use `LinkWindowToElement()` or `SetVisible()` to toggle the visibility of the window.
___
### ElementExists () {: aria-label='Functions' }
#### boolean ElementExists ( string ElementId ) {: .copyable aria-label='Functions' }
Returns true if an element with the given ID exists already.
___
### GetMousePosition () {: aria-label='Functions' }
#### void GetMousePosition ( ) {: .copyable aria-label='Functions' }
Returns the mouse position in Screen coordinates.

Use this instead of `Input.GetMousePosition()` when working with imgui!
___
### GetVisible () {: aria-label='Functions' }
#### boolean GetVisible ( string ElementId ) {: .copyable aria-label='Functions' }
Get if a window element is visible or not.
___
### GetWindowChildFlags () {: aria-label='Functions' }
#### [ImGuiChildFlags](enums/ImGuiChildFlags.md) GetWindowChildFlags ( ) {: .copyable aria-label='Functions' }
Get visual setting flags for the window, specific for its usecase as a child.
___
### GetWindowFlags () {: aria-label='Functions' }
#### [ImGuiWindowFlags](enums/ImGuiWindowFlags.md) GetWindowFlags ( ) {: .copyable aria-label='Functions' }
Get visual setting flags for the window.
___
### GetWindowPinned () {: aria-label='Functions' }
#### boolean GetWindowPinned ( string WindowId ) {: .copyable aria-label='Functions' }
Get the pinned state of a window.
___
### Hide () {: aria-label='Functions' }
#### void Hide ( ) {: .copyable aria-label='Functions' }
Closes ImGui.
___
### ImGuiToWorld () {: aria-label='Functions' }
#### void ImGuiToWorld ( [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }
Converts ImGui coordinates into World coordinates.

???+ bug "Bug"
    This function does not work correctly when the game's scale factor exceeds MaxRenderScale.
___
### IsVisible () {: aria-label='Functions' }
#### boolean IsVisible ( ) {: .copyable aria-label='Functions' }
Called when the player is actively in ImGui. This isn't triggered by "pinned" windows.
___
### LinkWindowToElement () {: aria-label='Functions' }
#### void LinkWindowToElement ( string WindowId, string ElementId ) {: .copyable aria-label='Functions' }
Connects a Window or Popup element to another element, making said element act as a "toggle" for that window.

???- example "Example Code"
    this code creates a new menu entry with one menuitem, which on click toggles a window
    ```lua
    ImGui.CreateMenu("myMenu", "Test Menu")
    ImGui.AddElement("myMenu", "myButton", ImGuiElement.MenuItem, "Some Text")
    ImGui.CreateWindow("myWindow", "Some Window title")
    ImGui.LinkWindowToElement("myWindow", "myButton")
    ```

___
### PushNotification () {: aria-label='Functions' }
#### void PushNotification ( const Text, [ImGuiNotificationType](enums/ImGuiNotificationType.md) notificationType = 0, int lifetime = 5000 ) {: .copyable aria-label='Functions' }        
Displays a pop-up message window in the style of a notification.
___
### RemoveCallback () {: aria-label='Functions' }
#### void RemoveCallback ( string ElementId, [ImGuiCallback](enums/ImGuiCallback.md) type ) {: .copyable aria-label='Functions' }
Remove the callback of the given type from the element.
___
### RemoveColor () {: aria-label='Functions' }
#### void RemoveColor ( string ElementId, [ImGuiColor](enums/ImGuiColor.md) colorType ) {: .copyable aria-label='Functions' }
Remove a color modifier of the given type from the element.
___
### RemoveElement () {: aria-label='Functions' }
#### void RemoveElement ( string ElementId ) {: .copyable aria-label='Functions' }
General function to remove any kind of element.
___
### RemoveMenu () {: aria-label='Functions' }
#### void RemoveMenu ( string ElementId ) {: .copyable aria-label='Functions' }

___
### RemoveWindow () {: aria-label='Functions' }
#### void RemoveWindow ( string ElementId ) {: .copyable aria-label='Functions' }

___
### Reset () {: aria-label='Functions' }
#### void Reset ( ) {: .copyable aria-label='Functions' }
Removes all custom defined Imgui elements and resets imgui back to its original state.
___
### SetColor () {: aria-label='Functions' }
#### void SetColor ( string ElementId, [ImGuiColor](enums/ImGuiColor.md) ColorType, float r, float g, float b, float a = 1.0 ) {: .copyable aria-label='Functions' }
Adds a color modifier to a given element.
___
### SetHelpmarker () {: aria-label='Functions' }
#### void SetHelpmarker ( string ElementId, string Text ) {: .copyable aria-label='Functions' }
Adds a helpmarker to a given element. A Helpmarker is a `(?)` element rendered on the right of an element, which when hovered displays a tooltip.
___
### SetSize () {: aria-label='Functions' }
#### void SetSize ( string elementID, float width, float Height ) {: .copyable aria-label='Functions' }
Sets the width and height of an element in pixels. Most regular form-elements only allow for width changes. Windows, Buttons and Plots do allow for width and height changes.

If the width is equal 0, the element will try to fill all available space in the window (Default behavior). 

If the value is negative, the element will fill the full width of the window, minus the number of pixels defined.
Example: The window is 500px wide. A width of -75 will make the element 425px wide.
___
### SetTextColor () {: aria-label='Functions' }
#### void SetTextColor ( string ElementId, float r, float g, float b, float a = 1.0 ) {: .copyable aria-label='Functions' }
Shortcut function to add a color modifier to text of a given element.
___
### SetTooltip () {: aria-label='Functions' }
#### void SetTooltip ( string ElementId, string Text ) {: .copyable aria-label='Functions' }
Adds a tooltip to a given element. The tooltip is visible when the user hovers over the element.
___
### SetVisible () {: aria-label='Functions' }
#### void SetVisible ( string ElementId, boolean Visible ) {: .copyable aria-label='Functions' }

___
### SetWindowChildFlags () {: aria-label='Functions' }
#### void SetWindowChildFlags ( [ImGuiChildFlags](enums/ImGuiChildFlags.md) newFlags ) {: .copyable aria-label='Functions' }
Set visual setting flags for the window, specific for its usecase as a child.
___
### SetWindowFlags () {: aria-label='Functions' }
#### void SetWindowFlags ( [ImGuiWindowFlags](enums/ImGuiWindowFlags.md) newFlags ) {: .copyable aria-label='Functions' }
Set visual setting flags for the window.
___
### SetWindowPinned () {: aria-label='Functions' }
#### void SetWindowPinned ( string WindowId, boolean Pinned ) {: .copyable aria-label='Functions' }
Set the pinned state of a window, making it visible when the ImGui interface is not active.
___
### SetWindowPosition () {: aria-label='Functions' }
#### void SetWindowPosition ( string WindowId, float x, float y ) {: .copyable aria-label='Functions' }
Set the position of a window in screen coordinates.
___
### SetWindowSize () {: aria-label='Functions' }
#### void SetWindowSize ( string WindowId, float width, float Height ) {: .copyable aria-label='Functions' }
Deprecated. Now does the same as [SetSize()](#setsize).
___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Opens ImGui.
___
### UpdateData () {: aria-label='Functions' }
#### void UpdateData ( string ElementId, [ImGuiData](enums/ImGuiData.md) DataType, int NewDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string ElementId, [ImGuiData](enums/ImGuiData.md) DataType, float NewDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string ElementId, [ImGuiData](enums/ImGuiData.md) DataType, boolean NewDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string ElementId, [ImGuiData](enums/ImGuiData.md) DataType, string NewDataValue ) {: .copyable aria-label='Functions' }
#### void UpdateData ( string ElementId, [ImGuiData](enums/ImGuiData.md) DataType, table NewDataValues ) {: .copyable aria-label='Functions' }
Update arbitrary data of a given element. See [ImGuiData](enums/ImGuiData.md) for possible data to update.

The dataTypes and the expected NewDataValue are evaluated per element. Therefore, if you try to update data of an element where this data is not used, this function will throw an error for you.
___
### UpdateText () {: aria-label='Functions' }
#### void UpdateText ( string ElementId, string Text ) {: .copyable aria-label='Functions' }
Shortcut function to update an element text or label.
___
### WorldToImGui () {: aria-label='Functions' }
#### void WorldToImGui ( [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }
Converts world coordinates into ImGui coordinates.

???+ bug "Bug"
    This function does not work correctly when the game's scale factor exceeds MaxRenderScale.
___
