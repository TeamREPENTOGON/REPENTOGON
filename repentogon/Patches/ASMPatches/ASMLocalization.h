#pragma once

#include <string>

#include "IsaacRepentance.h"

// Returns one of the following strings, whichever one comes first that matches an animation in the provided ANM2.
// 1. The player's localized name
// 2. The raw player name as defined in the XML
// 3. The player's english name
// 4. The player's localized name (there were no matches)
std::string GetLocalizedPlayerAnimationForAnm2(EntityConfig_Player* player, ANM2* sprite);

void ASMPatchRedirectToLocalizationFolders();
void ASMPatchLocalizedPlayerAnimations();
