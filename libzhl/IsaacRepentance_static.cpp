#include "IsaacRepentance.h"

bool Room::IsValidRailType(lua_Integer rail) {
	if (rail < 0) {
		return false;
	}
	else if (rail > RailType::RAIL_NONE) {
		return false;
	}
	else {
		return rail <= RailType::RAIL_CROSSING || 
			rail == RAIL_DOOR_LEFT || 
			rail == RAIL_DOOR_RIGHT || 
			rail == RAIL_DOOR_TOP || 
			rail == RAIL_DOOR_BOTTOM;
	}
}

bool RoomConfig::IsValidGridIndex(lua_Integer index, bool includeWalls) const {
	if (index < 0) {
		return false;
	}

	switch (Shape) {
	case ROOMSHAPE_1x1: {
		bool validWall = includeWalls && (index <= 15 || index % 15 == 0 || index % 15 == 14 || (index >= 120 && index <= 134));
		return validWall || index <= 119;
	}

	default:
		return true;
	}
}

void KAGE::_LogMessage(int flag, const char* fmt, ...) {
	char buffer[4096];
	va_list va;
	va_start(va, fmt);
	int n = vsnprintf(buffer, 4095, fmt, va);
	va_end(va);

	if (n >= 0) {
		LogMessage(flag, buffer);
	}
}

bool Game::IsErased(int type, int variant, int subtype) {
	for (EntityId const& entity : _erasedEntities) {
		if (entity.type == type) {
			if (variant == -1) {
				return true;
			}
			else {
				if (entity.variant == variant) {
					if (subtype == -1) {
						return true;
					}
					else {
						return entity.subtype == subtype;
					}
				}
			}
		}
	}

	return false;
}