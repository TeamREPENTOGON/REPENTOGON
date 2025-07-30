#pragma once

#include <bitset>

#include "IsaacRepentance.h"

extern std::bitset<500> CallbackState;

bool AddSmeltedTrinketToPlayer(Entity_Player* player, int trinketID, bool firstTime);

void PatchGetGreedDonationBreakChanceForModdedCharacters();
void PatchIncreaseGreedDonationCoinCountForModdedCharacters();
