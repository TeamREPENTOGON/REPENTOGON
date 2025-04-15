#pragma once

#include <cstdint>
#include <optional>

#include "IsaacRepentance.h"

class GridEntityPlus {
public:
	virtual ~GridEntityPlus() {}

	std::optional<uint32_t> waterClipInfoFlagsOverride = std::nullopt;
};

GridEntityPlus* GetGridEntityPlus(GridEntity* gridEntity);
