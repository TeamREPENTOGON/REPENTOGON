---
tags:
  - Class
---
# Class "GenericPrompt"

???+ info
	You can get this class by using its constructor or using the following functions:

	* [Game:GetGenericPrompt()](Game.md#getgenericprompt)

	???+ example "Example Code"
		This code creates a generic popup that opens when pressing the "Minus"-button. It prints the selected option to the console and displays some dummy text. 
		```lua
		local myPrompt = GenericPrompt()
		myPrompt:Initialize()
		myPrompt:SetText("Some test text")

		local wasPromptDisplayed = false

		function mod:myRenderFunction(_)
			myPrompt:Render()
		end 
		mod:AddCallback(ModCallbacks.MC_POST_RENDER, mod.myRenderFunction)

		function mod:myUpdateFunction(_) 
			myPrompt:Update(true) -- true = Process user inputs
			if wasPromptDisplayed and not myPrompt:IsActive() then -- prompt was closed by user 
				print("User selected option: "..myPrompt:GetSubmittedSelection()) 
				wasPromptDisplayed = false 
			end
			if Input.IsButtonTriggered(Keyboard.KEY_MINUS, 0)  then -- on Pressing minus button will open prompt 
				myPrompt:Show() 
				wasPromptDisplayed = true 
			end 
		end 
		mod:AddCallback(ModCallbacks.MC_POST_UPDATE, mod.myUpdateFunction)
		```

## Constructors
### GenericPrompt () {: aria-label='Constructors' }
#### [GenericPrompt](GenericPrompt.md) GenericPrompt ( ) {: .copyable aria-label='Constructors' }
Returns a GenericPrompt object. Allows for rendering a popup paper with the option to include text and tracking input for a yes/no decision.

## Functions

### GetCurrentSelection () {: aria-label='Functions' }
#### int GetCurrentSelection ( ) {: .copyable aria-label='Functions' }
Returns what selection the player is currently hovering over.

???+ info "Return info"
	- `0` - No
	- `1` - Yes

___
### GetSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetSprite ( ) {: .copyable aria-label='Functions' }
Returns the paper sprite of the prompt.

___
### GetSubmittedSelection () {: aria-label='Functions' }
#### int GetSubmittedSelection ( ) {: .copyable aria-label='Functions' }
Returns the chosen selection. 

???+ info "Return info"
	- `0` - None (Returns if the player dismisses the prompt).
	- `1` - Yes
	- `2` - No

___
### Initialize () {: aria-label='Functions' }
#### void Initialize ( boolean SmallPrompt = false ) {: .copyable aria-label='Functions' }

___
### IsActive () {: aria-label='Functions' }
#### boolean IsActive ( ) {: .copyable aria-label='Functions' }
Returns whether the prompt is active or not.

___
### Render () {: aria-label='Functions' }
#### void Render ( ) {: .copyable aria-label='Functions' }
Renders the prompt on-screen. Place this in any of the non-entity-specific RENDER callbacks.

___
### SetText () {: aria-label='Functions' }
#### void SetText ( string Text1 = "", string Text2 = "", string Text3 = "", string Text4 = "", string Text5 = "", ) {: .copyable aria-label='Functions' }
Set text that will appear on the paper.

Text strings are associated with their position on the prompt from top to bottom. The first two strings should used as header text, being bolded and at a higher font size, while the rest as description text.

___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Starts showing the prompt on-screen.

___
### Update () {: aria-label='Functions' }
#### void Update ( boolean ProcessInput ) {: .copyable aria-label='Functions' }
Updates the animation of the prompt paper. Set `ProcessInput` to `true` to track the player's input for selecting yes/no, `false` otherwise.

___
