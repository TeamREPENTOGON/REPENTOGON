#pragma once

void ApplyVanillaItemMetadata();
void RefreshVanillaFeatureStats(const bool* toggle);
struct Entity;
struct Entity_Familiar;
struct Entity_Player;

bool VanillaAbyssRework();
bool VanillaAbyssLocustHit(Entity_Familiar* familiar, Entity* collider);
bool VanillaAbyssUse(Entity_Player* player);
