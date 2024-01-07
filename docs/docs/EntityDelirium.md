---
tags:
  - Class
---
# Class "EntityDelirium"

## Functions

### GetTeleportationTimer () {: aria-label='Functions' }
#### int GetTeleportationTimer ( ) {: .copyable aria-label='Functions' }
Return the number of frames before Delirium teleports.
    
___
### IsRedMode () {: aria-label='Functions' }
#### boolean IsRedMode ( ) {: .copyable aria-label='Functions' }
Returns a boolean indicating if the red mode is activated or not.

???+ info "About red mode"
    Red mode is mechanic in the Delirium fight where Delirium's sprite is tinted red. 
    While this mode is active, Delirium's movement speed is increased in a way that cannot be observed through the modding API as it occurs outside of the update callbacks.
    
___
### SetRedMode () {: aria-label='Functions' }
#### void SetRedMode ( boolean On ) {: .copyable aria-label='Functions' }
Enable or disable red mode according to the parameter `on`. 

???+ info "About red mode"
    Please refer to the note in the documentation of [IsRedMode](EntityDelirium.md#IsRedMode) for an explanation of red mode.
    
___
### SetTeleportationTimer () {: aria-label='Functions' }
#### void SetTeleportationTimer ( int Timer ) {: .copyable aria-label='Functions' }
Set the number of frames before Delirium teleports. Negative values are not allowed.

___
### Transform () {: aria-label='Functions' }
#### void Transform ( int Type, int Variant = 0 ) {: .copyable aria-label='Functions' }
Transform Delirium into the entity with the specified type and variant. 

???+ warn "Warning"
    The validation of the type and variant is only as strong as the validation performed by the game when it attempts to transform Delirium.
    In other words, this behaves exactly as if the game itself attempted to transform Delirium, with everything it implies if the specified entity is invalid.
    
???+ warn "Warning"
    In order to properly handle transformations, we use the native transformation mechanic of Delirium. 
    As such, the transformation will not be effective immediately, but instead on the next frame.
    Internally, this function forces the transformation timer to 1 frame and lets Delirium's AI update as needed.
    
___    
## Attributes

### Angle {: aria-label='Variables' }
#### int8 Angle {: .copyable aria-label='Variables' }
Angle of Delirium's projectiles. 

???+ warn "Geometric system"
    This variable is an eight bit integers, so the allowed values are the integers from 0 to 255 (inclusive).
    You can use a linear interpolation between the \[0: 255] and the \[0: 360\[ ranges in order to convert angles in degree to this system.

???+ info "WTH"
    All of Delirium bullet hell patterns can be influenced by this variable. Unlike most bosses, Delirium does not aim its projectiles at the player, instead spawning them in random directions (with some control to prevent "absurd" patterns).
    For instance, if Delirium spawns eight tears around it, and `Angle` is set to 0, the 8 tears will fire in the cardinal and ordinal directions. If `Angle` is set to 32, all tears will be rotated by 45°.
    No, I have no idea why Nicalis used an eight bits integer to represent an angle (analyzing the memory layout of Delirium shows that it would have made no difference using a 32 bits float).
___
### AttackID {: aria-label='Variables' }
#### int AttackID {: .copyable aria-label='Variables' }
Internal [I1](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#I1) value used by Delirium to identify the bullet hell pattern it is currently executing.

___
### BossType {: aria-label='Variables' }
[ ](#){: .const .tooltip .badge }
#### int BossType {: .copyable aria-label='Variables' }
(EntityType)(https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) of the boss Delirium is currently transformed as.

___
### BossVariant {: aria-label='Variables' }
[ ](#){: .const .tooltip .badge }
#### int BossVariant {: .copyable aria-label='Variables' }
Variant of the boss Delirium is currently transformed as.

___
### Cycle {: aria-label='Variables' }
#### int Cycle {: .copyable aria-label='Variables' }
Internal [I2](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#I2) value used by Delirium to identify whether red mode is activated and the amount of time before a teleportation.
You should not use this variable directly and instead rely on the [GetTeleportationTimer](EntityDelirium.md#GetTeleportationTimer), [SetTeleportationTimer](EntityDelirium.md#SetTeleportationTimer), [IsRedMode](EntityDelirium.md#IsRedMode) and [SetRedMode](EntityDelirium.md#SetRedMode) functions instead.
The only reason to use this variable directly is if you want to freeze it to a certain value that you know will do exactly what you want (for instance disable red mode and prevent teleportation).

???+ info "Format of the variable"
    The variable is 32 bits wide and is structured as follows: bits 0 to 6 (inclusive) are unknown, bits 7 to 14 (inclusive) indicate whether red mode is active (it is active if any of the bits is set) and bits 15 to 25 (inclusive) are the teleportation timer.

???+ info "Preservation of state"
    The aforementioned methods used to manipulate this variable preserve the bits of the variable that are irrelevant to the operation performed.
    For instance, enabling or disabling red mode will not change the transformation timer. 
___
### RemainingAttacks {: aria-label='Variables' }
#### int RemainingAttacks {: .copyable aria-label='Variables' }
Number of attacks remaining before Delirium transforms into another boss.

???+ info "About remaining attacks" 
    This variable is Nicalis answer to prevent Delirium from performing too many attacks as a single boss before transforming.
    Under certain conditions, the game will decrement this value by 1. If it reaches 0, Delirium transforms regardless of the transformation timer.
    The conditions that must be met (simultaneously) are: the [StateFrame](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#StateFrame) variable must be 1 during the current frame, and the [State](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.md#State) variable must be set to any of the attack states.
    This is the reason why Delirium will sometimes initiate an attack as a boss and immediately transform. 
    You can refer to the [complete breakdown of bosses AI configurations](https://wofsauge.github.io/IsaacDocs/rep/customData/bosses.xlsx) to see the AI configuration of each attack.
___
### StateD {: aria-label='Variables' }
#### [NpcState](https://wofsauge.github.io/IsaacDocs/rep/enums/NpcState.html) StateD {: .copyable aria-label='Variables' }
Internal [State](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#State) of Delirium.

___
### TransformationTimer {: aria-label='Variables' }
#### int TransformationTimer {: .copyable aria-label='Variables' }
Get or set the amount of time before Delirium transforms into another boss.

???+ warn "On transformations" 
    Delirium can transform in two situations: either this value reaches 0, or the value of [RemainingAttacks](EntityDelirium.md#RemainingAttacks) reaches 0. 
    Refer to the documentation of [RemainingAttacks](EntityDelirium.md#RemainingAttacks) for a more detailed explanation of that mechanic.
    
___
