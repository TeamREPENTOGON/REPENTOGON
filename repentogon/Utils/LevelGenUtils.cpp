#include "LevelGenUtils.h"
#include <vector>
#include <IsaacRepentance.h>

#pragma region Helpers

void PushCoordsIfValid(std::vector<XY>& list, XY& coords) {
	if (coords.ToGridIdx() >= 0) {
		list.push_back(coords);
	}
}

std::vector<XY> GetOccupiedCoords(XY& base_coords, int shape) {
	std::vector<XY> occupied_coords = {};

	switch (shape)
	{
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		break;
	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x, base_coords.y + 1));
		break;
	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y));
		break;
	case ROOMSHAPE_2x2:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x, base_coords.y + 1));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_LTL:
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x, base_coords.y + 1));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_LTR:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x, base_coords.y + 1));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_LBL:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_LBR:
		occupied_coords.push_back(XY(base_coords.x, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x + 1, base_coords.y));
		occupied_coords.push_back(XY(base_coords.x, base_coords.y + 1));
		break;
	default:
		break;
	}

	return occupied_coords;
}

std::vector<XY> GetForbiddenNeighbors(XY& base_coords, int shape, int doors) {
	std::vector<XY> forbidden_neighbors = {};

	switch (shape)
	{
	case ROOMSHAPE_1x1:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		}
		break;
	case ROOMSHAPE_IH:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		break;
	case ROOMSHAPE_IV:
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		}
		break;
	case ROOMSHAPE_1x2:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 1));
		}
		break;
	case ROOMSHAPE_IIV:
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_2x1:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 1));
		}
		break;
	case ROOMSHAPE_IIH:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 1));
		break;
	case ROOMSHAPE_2x2:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 2));
		}
		break;
	case ROOMSHAPE_LTL:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0 || (doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 2));
		}
		break;
	case ROOMSHAPE_LTR:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0 || (doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 2));
		}
		break;
	case ROOMSHAPE_LBL:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0 || (doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 2));
		}
		break;
	case ROOMSHAPE_LBR:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 2, base_coords.y));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x, base_coords.y + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x - 1, base_coords.y + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0 || (doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			PushCoordsIfValid(forbidden_neighbors, XY(base_coords.x + 1, base_coords.y + 1));
		}
		break;
	default:
		break;
	}

	return forbidden_neighbors;
}

XY GetDoorTargetCoords(const XY& base_coords, int shape, int slot) {
	int dx = 0, dy = 0;

	switch (shape) {
	case ROOMSHAPE_1x1:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 1; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 1; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_IH:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_RIGHT0: dx = 1; dy = 0; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_IV:
		switch (slot) {
		case DOOR_SLOT_UP0:   dx = 0; dy = -1; break;
		case DOOR_SLOT_DOWN0: dx = 0; dy = 1; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_1x2:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 1; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 2; break;
		case DOOR_SLOT_LEFT1:  dx = -1; dy = 1; break;
		case DOOR_SLOT_RIGHT1: dx = 1; dy = 1; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_IIV:
		switch (slot) {
		case DOOR_SLOT_UP0:   dx = 0; dy = -1; break;
		case DOOR_SLOT_DOWN0: dx = 0; dy = 2; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_2x1:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 1; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = -1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 1; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_IIH:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_2x2:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 2; break;
		case DOOR_SLOT_LEFT1:  dx = -1; dy = 1; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = -1; break;
		case DOOR_SLOT_RIGHT1: dx = 2; dy = 1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 2; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_LTL:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = 0; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = 0; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 2; break;
		case DOOR_SLOT_LEFT1:  dx = -1; dy = 1; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = -1; break;
		case DOOR_SLOT_RIGHT1: dx = 2; dy = 1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 2; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_LTR:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 1; dy = 0; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 2; break;
		case DOOR_SLOT_LEFT1:  dx = -1; dy = 1; break;
		case DOOR_SLOT_RIGHT1: dx = 2; dy = 1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 2; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_LBL:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 1; break;
		case DOOR_SLOT_LEFT1:  dx = 0; dy = 1; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = -1; break;
		case DOOR_SLOT_RIGHT1: dx = 2; dy = 1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 2; break;
		default: return XY(-1, -1);
		}
		break;
	case ROOMSHAPE_LBR:
		switch (slot) {
		case DOOR_SLOT_LEFT0:  dx = -1; dy = 0; break;
		case DOOR_SLOT_UP0:    dx = 0; dy = -1; break;
		case DOOR_SLOT_RIGHT0: dx = 2; dy = 0; break;
		case DOOR_SLOT_DOWN0:  dx = 0; dy = 2; break;
		case DOOR_SLOT_LEFT1:  dx = -1; dy = 1; break;
		case DOOR_SLOT_UP1:    dx = 1; dy = -1; break;
		case DOOR_SLOT_RIGHT1: dx = 1; dy = 1; break;
		case DOOR_SLOT_DOWN1:  dx = 1; dy = 1; break;
		default: return XY(-1, -1);
		}
		break;
	default:
		return XY(-1, -1);
	}

	return XY(base_coords.x + dx, base_coords.y + dy);
}

int CalculateAutoDoors(const int* roomMap, XY& base_coords, int shape) {
	int doors = 0;
	for (int slot = 0; slot < NUM_DOOR_SLOTS; slot++) {
		XY target = GetDoorTargetCoords(base_coords, shape, slot);
		int grid_idx = target.ToGridIdx();
		if (grid_idx >= 0 && grid_idx < 169) {
			if (roomMap[grid_idx] > -1) {
				doors |= (1 << slot);
			}
		}
	}
	return doors;
}

#pragma endregion
