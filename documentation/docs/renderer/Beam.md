---
tags:
  - Class
---
# Class "Beam"

This class provides more streamlined access to the `BeamRenderer` used internally for rendering cords, ie Evis, Gello, Vis Fatty, etc.
Note that this is a low-level class that strictly handles rendering. We hope to later provide an extension of this class capable of handling the physics calculations and automatic point adjustment required for cords, but this is a complex system that will require a non-trivial amount of effort to implement.

???+ warning "Warning"
    The params in this class, especially `UnkBool`, `Width`, and `Height`, are largely unknown and for now will require trial and error to effectively use.
		These documents will be updated with the latest findings.

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
#### void Add ( [Vector](../Vector.md) Position, float Height, float Width = 1.0, [Color](../Color.md) Color = Color ) {: .copyable aria-label='Functions' }   
Adds a point to the beam. Points are stored in order of adding.

???+ info "Info"
    `Height` is, to our current understanding, how much of the sprite will render, ie. a value of `10` would render the sprite up to `10` pixels vertically.
	`Width` determines how large the beam should be. A larger value will upscale the sprite. This is interpolated between points.

___
### GetLayer () {: aria-label='Functions' }
#### int GetLayer ( ) {: .copyable aria-label='Functions' }   

___

### GetPoints () {: aria-label='Functions' }
#### [Point](Point.md)[] GetPoints ( ) {: .copyable aria-label='Functions' }   
Returns a table of the [Points](Point.md) currently stored.

___

### GetSprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetSprite ( ) {: .copyable aria-label='Functions' }   

___
### GetUnkBool () {: aria-label='Functions' }
#### boolean GetUnkBool ( ) {: .copyable aria-label='Functions' }   

___
### GetUseOverlay () {: aria-label='Functions' }
#### boolean GetUseOverlay ( ) {: .copyable aria-label='Functions' }   

___
### Render () {: aria-label='Functions' }
#### void Render ( boolean ClearPoints = true ) {: .copyable aria-label='Functions' }

___
### SetLayer () {: aria-label='Functions' }
#### void SetLayer ( int LayerID ) {: .copyable aria-label='Functions' }   
#### void SetLayer ( string LayerName ) {: .copyable aria-label='Functions' } 
  
___
### SetPoints () {: aria-label='Functions' }
#### void SetPoints ( [Point](Point.md)[] Points) {: .copyable aria-label='Functions' }   
Sets the [Points](Point.md) used by this.

___
### SetSprite () {: aria-label='Functions' }
#### void SetSprite ( [Sprite](../Sprite.md) Sprite ) {: .copyable aria-label='Functions' }   

___
### SetUnkBool () {: aria-label='Functions' }
#### void SetUnkBool ( boolean UnkBool ) {: .copyable aria-label='Functions' }   

___
### SetUseOverlay () {: aria-label='Functions' }
#### void SetUseOverlay ( boolean UseOverlay ) {: .copyable aria-label='Functions' }   

___
