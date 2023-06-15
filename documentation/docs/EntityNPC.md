# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Get·Dirt·Color() {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetDirtColor ( ) {: .copyable aria-label='Functions' }
Returns the dynamic dirt color of the entity. This lets entities like Nightcrawler blend in to the environment.

___
### Update·Dirt·Color() {: aria-label='Functions' }
#### void UpdateDirtColor ( boolean unk ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this reagard.

___
### Get·Controller·Id() {: aria-label='Functions' }
#### int GetControllerId ( ) {: .copyable aria-label='Functions' }
Returns the ControllerId for the NPC, which indicates which player is controlling it. Will return -1 when its not being controlled by any player.

___
### Set·Controller·Id() {: aria-label='Functions' }
#### int SetControllerId ( int ControllerId ) {: .copyable aria-label='Functions' }
Sets the ControllerId for the NPC, which indicates which player will control it. Set it to -1 for no player controls(back to normal behaviour).

___
### Try·Force·Target() {: aria-label='Functions' }
#### boolean TryForceTarget ( [Entity](Entity.md) target, int duration ) {: .copyable aria-label='Functions' }

___