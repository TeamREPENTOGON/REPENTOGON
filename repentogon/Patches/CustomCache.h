#pragma once

#include <string>

#include "IsaacRepentance.h"

int __stdcall GetMaxCoins();
int __stdcall GetMaxKeys();
int __stdcall GetMaxBombs();

double GetCustomCacheValue(Entity_Player* player, const std::string& customcache);

void ASMPatchesForCustomCache();
