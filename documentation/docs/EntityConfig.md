# Class "EntityConfig"

Can be obtained via [Isaac.GetEntityConfig()](Isaac.md#getentityconfig).

## Functions

### Get·Entity () {: aria-label='Functions' }
#### [EntityConfigEntity](EntityConfigEntity.md) GetEntity ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, int Variant = -1, int SubType = -1 ) {: .copyable aria-label='Functions' }
Returns nil if no entity exists with the specified Type.

Providing Variant and/or SubType is optional. If a non-existent Variant/SubType is requested, returns the base version of that entity.

___

