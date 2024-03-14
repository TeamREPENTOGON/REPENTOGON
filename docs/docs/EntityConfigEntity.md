---
tags:
  - Class
---
# Class "EntityConfigEntity"

???+ info
    You can get this class by using the following functions:

    * [EntityConfig.GetEntity()](EntityConfig.md#getentity)

    ???+ example "Example Code"
        ```lua
        local gaperConfig = EntityConfig.GetEntity(EntityType.ENTITY_GAPER)
        ```
        
## Functions

### CanBeChampion () {: aria-label='Functions' }
#### boolean CanBeChampion ( ) {: .copyable aria-label='Functions' }

___
### CanBeRerolledInto () {: aria-label='Functions' }
#### boolean CanBeRerolledInto ( ) {: .copyable aria-label='Functions' }

___
### CanShutDoors () {: aria-label='Functions' }
#### boolean CanShutDoors ( ) {: .copyable aria-label='Functions' }

___
### GetAnm2Path () {: aria-label='Functions' }
#### string GetAnm2Path ( ) {: .copyable aria-label='Functions' }

___
### GetBaseHP () {: aria-label='Functions' }
#### float GetBaseHP ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryAnimation () {: aria-label='Functions' }
#### string GetBestiaryAnimation ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryAnm2Path () {: aria-label='Functions' }
#### string GetBestiaryAnm2Path ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryFloorAlt () {: aria-label='Functions' }
#### string GetBestiaryFloorAlt ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryOffset () {: aria-label='Functions' }
#### [const Vector](Vector.md) GetBestiaryOffset ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryOverlay () {: aria-label='Functions' }
#### string GetBestiaryOverlay ( ) {: .copyable aria-label='Functions' }

___
### GetBestiaryScale () {: aria-label='Functions' }
#### float GetBestiaryScale ( ) {: .copyable aria-label='Functions' }

___
### GetBossID () {: aria-label='Functions' }
#### int GetBossID ( ) {: .copyable aria-label='Functions' }

___
### GetCollisionDamage () {: aria-label='Functions' }
#### float GetCollisionDamage ( ) {: .copyable aria-label='Functions' }

___
### GetCollisionInterval () {: aria-label='Functions' }
#### int GetCollisionInterval ( ) {: .copyable aria-label='Functions' }

___
### GetCollisionRadius () {: aria-label='Functions' }
#### float GetCollisionRadius ( ) {: .copyable aria-label='Functions' }
Also known as "Size".

___
### GetCollisionRadiusMultiplier () {: aria-label='Functions' }
#### [const Vector](Vector.md) GetCollisionRadiusMultiplier ( ) {: .copyable aria-label='Functions' }
Also known as "SizeMulti".

___
### GetCustomTags () {: aria-label='Functions' }
#### table GetCustomTags ( ) {: .copyable aria-label='Functions' }
Returns a table containing all the strings specified in the entity's `customtags` attribute in [entities2.xml](xml/entities.md). Tags are always provided in all lowercase. See [entities2.xml](xml/entities.md) for more information on `customtags`.

___
### GetEntityTags () {: aria-label='Functions' }
#### int GetEntityTags ( ) {: .copyable aria-label='Functions' }
Returns the bitmask of [EntityTagFlag](enums/EntityTagFlag.md)s for this entity.

___
### GetFriction () {: aria-label='Functions' }
#### float GetFriction ( ) {: .copyable aria-label='Functions' }

___
### GetGibFlags () {: aria-label='Functions' }
#### int GetGibFlags ( ) {: .copyable aria-label='Functions' }
Returns the bitmask of [GibFlag](enums/GibFlag.md)s for this entity.

___
### GetGibsAmount () {: aria-label='Functions' }
#### int GetGibsAmount ( ) {: .copyable aria-label='Functions' }

___
### GetGridCollisionPoints () {: aria-label='Functions' }
#### int GetGridCollisionPoints ( ) {: .copyable aria-label='Functions' }

___
### GetMass () {: aria-label='Functions' }
#### float GetMass ( ) {: .copyable aria-label='Functions' }

___
### GetModName () {: aria-label='Functions' }
#### string GetModName ( ) {: .copyable aria-label='Functions' }
Name string of the mod that entity is from.

Returns nil for vanilla entities.

___
### GetName () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### GetPortraitID () {: aria-label='Functions' }
#### int GetPortraitID ( ) {: .copyable aria-label='Functions' }

___
### GetShadowSize () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }
Note that this value is the "shadowSize" specified in the XML divided by 100.

___
### GetShieldStrength () {: aria-label='Functions' }
#### float GetShieldStrength ( ) {: .copyable aria-label='Functions' }
The amount of armor the entity has.

___
### GetStageHP () {: aria-label='Functions' }
#### float GetStageHP ( ) {: .copyable aria-label='Functions' }

___
### GetSubType () {: aria-label='Functions' }
#### int GetSubType ( ) {: .copyable aria-label='Functions' }

___
### GetType () {: aria-label='Functions' }
#### int GetType ( ) {: .copyable aria-label='Functions' }

___
### GetVariant () {: aria-label='Functions' }
#### int GetVariant ( ) {: .copyable aria-label='Functions' }

___
### HasCustomTag () {: aria-label='Functions' }
#### boolean HasCustomTag ( string tag ) {: .copyable aria-label='Functions' }
Returns true if the entity has the provided string specified in its `customtags` attribute in [entities2.xml](xml/entities.md). Capitalization does not matter. See [entities2.xml](xml/entities.md) for more information on `customtags`.

___
### HasEntityTags () {: aria-label='Functions' }
#### boolean HasEntityTags ( int Tags ) {: .copyable aria-label='Functions' }
Returns true if the entity has all [EntityTagFlag](enums/EntityTagFlag.md)s specified in the provided bitset.

___
### HasFloorAlts () {: aria-label='Functions' }
#### boolean HasFloorAlts ( ) {: .copyable aria-label='Functions' }

___
### HasGibFlags () {: aria-label='Functions' }
#### boolean HasGibFlags ( int Flags ) {: .copyable aria-label='Functions' }
Returns true if the entity has all [GibFlag](enums/GibFlag.md)s specified in the provided bitset.

___
### IsBoss () {: aria-label='Functions' }
#### boolean IsBoss ( ) {: .copyable aria-label='Functions' }

___

