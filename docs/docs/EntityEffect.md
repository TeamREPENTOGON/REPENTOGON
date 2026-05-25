---
tags:
  - Class
---
# Class "EntityEffect"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### AddTearFlags () {: aria-label='Functions' }
#### void AddTearFlags ( [TearFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/TearFlags.html) ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.PLAYER_CREEP_HOLYWATER_TRAIL`, `EffectVariant.BRIMSTONE_BALL`, `EffectVariant.TECH_DOT`, and `EffectVariant.CHAIN_LIGHTNING`.

___
### ClearTearFlags () {: aria-label='Functions' }
#### void ClearTearFlags ( ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.PLAYER_CREEP_HOLYWATER_TRAIL`, `EffectVariant.BRIMSTONE_BALL`, `EffectVariant.TECH_DOT`, and `EffectVariant.CHAIN_LIGHTNING`.

___
### CreateLight () {: aria-label='Functions' }
#### static [EntityEffect](EntityEffect.md) CreateLight ( [Vector](Vector.md) Position, float Scale = RandomFloat[0.0-1.0], int Lifespan = -1, int State = 6, [Color](Color.md) Color = Default) {: .copyable aria-label='Functions' }
???+ warning "Warning"
    This is a static function and must be called via `EntityEffect.CreateLight`.

___
### CreateLootPreview () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) CreateLootPreview ( [Vector](Vector.md) Position, [EntityPickup](EntityPickup.md) Owner, [EntityEffect](EntityEffect.md) Effect ) {: .copyable aria-label='Functions' }

___
### GetGridEntityDesc () {: aria-label='Functions' }
#### [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) GetGridEntityDesc ( ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.GRID_ENTITY_PROJECTILE_HELPER`.

___
### GetTearFlags () {: aria-label='Functions' }
#### [TearFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/TearFlags.html) GetTearFlags ( ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.PLAYER_CREEP_HOLYWATER_TRAIL`, `EffectVariant.BRIMSTONE_BALL`, `EffectVariant.TECH_DOT`, and `EffectVariant.CHAIN_LIGHTNING`.

___
### HasTearFlags () {: aria-label='Functions' }
#### boolean HasTearFlags ( [TearFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/TearFlags.html) ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.PLAYER_CREEP_HOLYWATER_TRAIL`, `EffectVariant.BRIMSTONE_BALL`, `EffectVariant.TECH_DOT`, and `EffectVariant.CHAIN_LIGHTNING`.

___
### SetTearFlags () {: aria-label='Functions' }
#### void SetTearFlags ( [TearFlags](https://wofsauge.github.io/IsaacDocs/rep/enums/TearFlags.html) ) {: .copyable aria-label='Functions' }
???+ info "Applicable effects only"
	Only works for `EffectVariant.PLAYER_CREEP_HOLYWATER_TRAIL`, `EffectVariant.BRIMSTONE_BALL`, `EffectVariant.TECH_DOT`, and `EffectVariant.CHAIN_LIGHTNING`.

___
