#pragma once

#include <bitset>

extern std::bitset<500> CallbackState;
extern int ambushWaves;
extern bool overrideMegaSatanEnding;

void __stdcall LogMessageCallback(const char* logMessage);
void PerformASMPatches();