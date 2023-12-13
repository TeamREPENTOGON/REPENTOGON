---
tags:
  - Class
---
# Class "Beam"

This class provides access to the rendering system used for cords as used by Evis, Scrouge, Dogma, etc.

## Constructors
### Beam () {: aria-label='Constructors' }
#### [Beam](Beam.md) Beam ( [Sprite](../Sprite.md) Sprite, int Layer, boolean UseOverlay, boolean UnkBool, int PointsPreallocateSize = 8) {: .copyable aria-label='Constructors' }
#### [Beam](Beam.md) Beam ( [Sprite](../Sprite.md) Sprite, string LayerName, boolean UseOverlay, boolean UnkBool int PointsPreallocateSize = 8) {: .copyable aria-label='Constructors' }

???- note "Notes"
	`PointsPreallocateSize` is the amount of space that `Beam` will reserve to hold points. This is not a hard limit, but going past this amount will incur a performance cost as more memory must be allocated to hold new ones.

???- example "Example Code"
	Here is an example of how you would use this class:

    ```lua
	local sprite = Sprite()
	sprite:Load("gfx/893.000_ball and chain.anm2", true)
	chain = Beam(sprite, "chain", true, false)
	
	mod:AddCallback(ModCallbacks.MC_POST_PLAYER_RENDER, function(_, player)
		chain:GetSprite():PlayOverlay("Chain", false)
		local center = game:GetLevel():GetCurrentRoom():GetCenterPos()
		chain:Add(Isaac.WorldToScreen(center))
		chain:Add(Isaac.WorldToScreen(player.Position))
		chain:Render()
	end)
    ```

## Functions

### Add () {: aria-label='Functions' }
#### void Add ( [Vector](../Vector.md) Position, float Height = 1.0, float Width = 1.0, [Color](../Color.md) Color = Color ) {: .copyable aria-label='Functions' }   
Adds a point to the beam. Points are stored in order of adding.

___
### Get·Layer () {: aria-label='Functions' }
#### int GetLayer ( ) {: .copyable aria-label='Functions' }   

___
### Get·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetSprite ( ) {: .copyable aria-label='Functions' }   

___
### Get·Unk·Bool () {: aria-label='Functions' }
#### boolean GetUnkBool ( ) {: .copyable aria-label='Functions' }   

___
### Get·Use·Overlay () {: aria-label='Functions' }
#### boolean GetUseOverlay ( ) {: .copyable aria-label='Functions' }   

___
### Render () {: aria-label='Functions' }
#### void Render ( boolean ClearPoints = true ) {: .copyable aria-label='Functions' }

___
### Set·Layer () {: aria-label='Functions' }
#### void SetLayer ( int LayerID ) {: .copyable aria-label='Functions' }   
#### void SetLayer ( string LayerName ) {: .copyable aria-label='Functions' }   
___
### Set·Sprite () {: aria-label='Functions' }
#### void SetSprite ( [Sprite](../Sprite.md) Sprite ) {: .copyable aria-label='Functions' }   

___
### Set·Unk·Bool () {: aria-label='Functions' }
#### void SetUnkBool ( boolean UnkBool ) {: .copyable aria-label='Functions' }   

___
### Set·Use·Overlay () {: aria-label='Functions' }
#### void SetUseOverlay ( boolean UseOverlay ) {: .copyable aria-label='Functions' }   

___
