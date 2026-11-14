#pragma once

#include <string>

#include "IsaacRepentance.h"

struct MinimapIcon {
	std::string group;
	std::string animation;
	int frame = -1;
	int priority = 0;
	ANM2* anm2 = nullptr;
};

void AddEntityMinimapIcon(int id, int variant, int subtype, const MinimapIcon& icon);
void AddEntityMinimapIcon(int id, int variant, int subtype, const std::string& group, const std::string& animation, int frame, int priority, ANM2* customAnm2);

namespace MinimapPatches
{
    void ApplyPatches();
}
