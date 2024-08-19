#pragma once

bool __stdcall RunPreLaserCollisionCallback(Entity_Laser* laser, Entity* entity);
void PatchPreSampleLaserCollision();
void PatchPreLaserCollision();
void PatchPreEntityTakeDamageCallbacks();
void PatchPostEntityTakeDamageCallbacks();
void ASMPatchPrePlayerUseBomb();
void ASMPatchPostPlayerUseBomb();
void ASMPatchPreMMorphActiveCallback();
void ASMPatchTrySplit();
void ASMPatchInputAction();
void ASMPatchPostNightmareSceneCallback();
void ASMPatchPrePickupVoided();
void ASMPatchPrePickupVoidedBlackRune();
void ASMPatchPrePickupVoidedAbyss();
void ASMPatchPrePickupComposted();
void ASMPatchPostChampionRegenCallback();
void ASMPatchTrinketRender();
void ASMPatchPickupUpdatePickupGhosts();
void ASMPatchProjectileDeath();
void ASMPatchTearDeath();
void ASMPatchPrePlayerGiveBirth();
