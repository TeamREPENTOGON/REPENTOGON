#pragma once

#include <bitset>

#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"
#include "Log.h"
#include "../REPENTOGONOptions.h"

extern std::bitset<1500> CallbackState;
extern int ambushWaves;
extern bool overrideMegaSatanEnding;

void __stdcall LogMessageCallback(const char* logMessage);
void PerformASMPatches();