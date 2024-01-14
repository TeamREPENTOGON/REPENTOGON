#pragma once

#include <bitset>

#include "IsaacRepentance.h"

extern std::bitset<500> CallbackState;
extern int ambushWaves;
extern bool overrideMegaSatanEnding;

void __stdcall LogMessageCallback(const char* logMessage);
bool __stdcall RunPreLaserCollisionCallback(Entity_Laser* laser, Entity* entity);
void PerformASMPatches();