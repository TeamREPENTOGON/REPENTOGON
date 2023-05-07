# Class "EntityPickup"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Get·Var·Data () {: aria-label='Functions' }
#### int GetVarData ( ) {: .copyable aria-label='Functions' }

___
### Is·Blind () {: aria-label='Functions' }
#### boolean IsBlind ( ) {: .copyable aria-label='Functions' }

Returns whether a collectible is being hidden by the Curse of the Blind question mark graphics. Always returns false for non-collectible EntityPickups.

___
### Set·Alternate·Pedestal () {: aria-label='Functions' }
#### void SetAlternatePedestal ( int PedestalType ) {: .copyable aria-label='Functions' }

Sets the graphics of the item pedestal. Does nothing for non-collectible EntityPickups.

___
### Set·Var·Data () {: aria-label='Functions' }
#### void SetVarData ( int VarData ) {: .copyable aria-label='Functions' }

___
### Set·Force·Blind () {: aria-label='Functions' }
#### void SetForceBlind ( boolean SetBlind ) {: .copyable aria-label='Functions' }

Hides pedestal items similar to Curse of the Blind. Does nothing for non-collectible EntityPickups.

___
### Try·Remove·Collectible () {: aria-label='Functions' }
#### boolean TryRemoveCollectible ( ) {: .copyable aria-label='Functions' }

Attempts to remove the collectible from an item pedestal.

Returns true if a collectible was successfully removed from the pedestal. Returns false if the pedestal was already empty, or if called on a non-collectible EntityPickup.

___