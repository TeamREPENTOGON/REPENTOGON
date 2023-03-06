# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Get·Dirt·Color() {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetDirtColor ( ) {: .copyable aria-label='Functions' }
Returns the dynamic dirt color of the entity. This lets entities like Nightcrawler blend in to the environment.

___
### Update·Dirt·Color() {: aria-label='Functions' }
#### void UpdateDirtColor ( bool unk ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this reagard.

___