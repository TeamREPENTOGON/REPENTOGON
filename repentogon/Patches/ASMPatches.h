#pragma once

#include <bitset>

#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"
#include "Log.h"
#include "../REPENTOGONOptions.h"

extern std::bitset<500> CallbackState;
extern std::bitset<75> VanillaCallbackState;
extern int ambushWaves;
extern bool overrideMegaSatanEnding;

extern void HookImGui();

void __stdcall LogMessageCallback(const char* logMessage);
void PerformASMPatches();