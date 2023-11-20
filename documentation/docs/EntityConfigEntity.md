# Class "EntityConfigEntity"

???+ info
    Can be obtained via the `EntityConfig` global table.

    ???+ example "Example Code"
        ```lua
        local gaperConfig = EntityConfig.GetEntity(EntityType.ENTITY_GAPER)
        ```
        
## Functions

### Can·Be·Champion () {: aria-label='Functions' }
#### boolean CanBeChampion ( ) {: .copyable aria-label='Functions' }

___
### Can·Be·Rerolled·Into () {: aria-label='Functions' }
#### boolean CanBeRerolledInto ( ) {: .copyable aria-label='Functions' }

___
### Can·Shut·Doors () {: aria-label='Functions' }
#### boolean CanShutDoors ( ) {: .copyable aria-label='Functions' }

___
### Get·Anm2·Path () {: aria-label='Functions' }
#### string GetAnm2Path ( ) {: .copyable aria-label='Functions' }

___
### Get·Base·HP () {: aria-label='Functions' }
#### float GetBaseHP ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Animation () {: aria-label='Functions' }
#### string GetBestiaryAnimation ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Anm2·Path () {: aria-label='Functions' }
#### string GetBestiaryAnm2Path ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Floor·Alt () {: aria-label='Functions' }
#### string GetBestiaryFloorAlt ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Offset () {: aria-label='Functions' }
#### [const Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetBestiaryOffset ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Overlay () {: aria-label='Functions' }
#### string GetBestiaryOverlay ( ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Scale () {: aria-label='Functions' }
#### float GetBestiaryScale ( ) {: .copyable aria-label='Functions' }

___
### Get·Boss·ID () {: aria-label='Functions' }
#### int GetBossID ( ) {: .copyable aria-label='Functions' }

___
### Get·Collision·Damage () {: aria-label='Functions' }
#### float GetCollisionDamage ( ) {: .copyable aria-label='Functions' }

___
### Get·Collision·Interval () {: aria-label='Functions' }
#### int GetCollisionInterval ( ) {: .copyable aria-label='Functions' }

___
### Get·Collision·Radius () {: aria-label='Functions' }
#### float GetCollisionRadius ( ) {: .copyable aria-label='Functions' }
Also known as "Size".

___
### Get·Collision·Radius·Multiplier () {: aria-label='Functions' }
#### [const Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetCollisionRadiusMultiplier ( ) {: .copyable aria-label='Functions' }
Also known as "SizeMulti".

___
### Get·Entity·Tags () {: aria-label='Functions' }
#### int GetEntityTags ( ) {: .copyable aria-label='Functions' }
Returns the bitmask of [EntityTag](enums/EntityTag.md)s for this entity.

___
### Get·Friction () {: aria-label='Functions' }
#### float GetFriction ( ) {: .copyable aria-label='Functions' }

___
### Get·Gibs·Amount () {: aria-label='Functions' }
#### int GetGibsAmount ( ) {: .copyable aria-label='Functions' }

___
### Get·Gib·Flags () {: aria-label='Functions' }
#### int GetGibFlags ( ) {: .copyable aria-label='Functions' }
Returns the bitmask of [GibFlag](enums/GibFlag.md)s for this entity.

___
### Get·Grid·Collision·Points () {: aria-label='Functions' }
#### int GetGridCollisionPoints ( ) {: .copyable aria-label='Functions' }

___
### Get·Mass () {: aria-label='Functions' }
#### float GetMass ( ) {: .copyable aria-label='Functions' }

___
### Get·Mod·Name () {: aria-label='Functions' }
#### string GetModName ( ) {: .copyable aria-label='Functions' }
Name string of the mod that entity is from.

Returns nil for vanilla entities.

___
### Get·Name () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### Get·Portrait·ID () {: aria-label='Functions' }
#### int GetPortraitID ( ) {: .copyable aria-label='Functions' }

___
### Get·Shadow·Size () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }
Note that this value is the "shadowSize" specified in the XML divided by 100.

___
### Get·Shield·Strength () {: aria-label='Functions' }
#### float GetShieldStrength ( ) {: .copyable aria-label='Functions' }
The amount of armor the entity has.

___
### Get·Stage·HP () {: aria-label='Functions' }
#### float GetStageHP ( ) {: .copyable aria-label='Functions' }

___
### Get·Sub·Type () {: aria-label='Functions' }
#### int GetSubType ( ) {: .copyable aria-label='Functions' }

___
### Get·Type () {: aria-label='Functions' }
#### int GetType ( ) {: .copyable aria-label='Functions' }

___
### Get·Variant () {: aria-label='Functions' }
#### int GetVariant ( ) {: .copyable aria-label='Functions' }

___
### Has·Entity·Tags () {: aria-label='Functions' }
#### boolean HasEntityTags ( int tags ) {: .copyable aria-label='Functions' }
Returns true if the entity has all [EntityTag](enums/EntityTag.md)s specified in the provided bitset.

___
### Has·Gib·Flags () {: aria-label='Functions' }
#### boolean HasGibFlags ( int flags ) {: .copyable aria-label='Functions' }
Returns true if the entity has all [GibFlag](enums/GibFlag.md)s specified in the provided bitset.

___
### Is·Boss () {: aria-label='Functions' }
#### boolean IsBoss ( ) {: .copyable aria-label='Functions' }

___

