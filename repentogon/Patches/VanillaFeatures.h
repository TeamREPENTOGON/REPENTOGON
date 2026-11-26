#pragma once

void ApplyVanillaItemMetadata();
struct Entity;
struct Entity_Familiar;
struct Entity_Player;

bool VanillaAbyssRework();
bool VanillaAbyssLocustHit(Entity_Familiar* familiar, Entity* collider);
bool VanillaAbyssUse(Entity_Player* player);
