#pragma once

#include <map>
#include <vector>
#include "IsaacRepentance.h"

namespace ASMChampion {
    extern std::vector<std::pair<eChampionColor, float>> championChances[];
    extern WeightedOutcomePicker weightedOutcomePicker[];

    void ASMPatchChampionInitialization();
    int __stdcall SelectChampionColor(unsigned int seed);
};