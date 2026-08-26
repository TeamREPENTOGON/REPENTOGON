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

int CalculateAutoDoors(const int* roomMap, XY& base_coords, int shape) {
	int doors = 0;
	auto check = [&](int slot, int dx, int dy) {
		XY target(base_coords.x + dx, base_coords.y + dy);
		int grid_idx = target.ToGridIdx();
		if (grid_idx >= 0 && grid_idx < 169) {
			if (roomMap[grid_idx] > -1) {
				doors |= (1 << slot);
			}
		}
	};

	switch (shape) {
	case ROOMSHAPE_1x1:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 1, 0);
		check(DOOR_SLOT_DOWN0, 0, 1);
		break;
	case ROOMSHAPE_IH:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_RIGHT0, 1, 0);
		break;
	case ROOMSHAPE_IV:
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_DOWN0, 0, 1);
		break;
	case ROOMSHAPE_1x2:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 1, 0);
		check(DOOR_SLOT_DOWN0, 0, 2);
		check(DOOR_SLOT_LEFT1, -1, 1);
		check(DOOR_SLOT_RIGHT1, 1, 1);
		break;
	case ROOMSHAPE_IIV:
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_DOWN0, 0, 2);
		break;
	case ROOMSHAPE_2x1:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		check(DOOR_SLOT_DOWN0, 0, 1);
		check(DOOR_SLOT_UP1, 1, -1);
		check(DOOR_SLOT_DOWN1, 1, 1);
		break;
	case ROOMSHAPE_IIH:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		break;
	case ROOMSHAPE_2x2:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		check(DOOR_SLOT_DOWN0, 0, 2);
		check(DOOR_SLOT_LEFT1, -1, 1);
		check(DOOR_SLOT_UP1, 1, -1);
		check(DOOR_SLOT_RIGHT1, 2, 1);
		check(DOOR_SLOT_DOWN1, 1, 2);
		break;
	case ROOMSHAPE_LTL:
		check(DOOR_SLOT_LEFT0, 0, 0);
		check(DOOR_SLOT_UP0, 0, 0);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		check(DOOR_SLOT_DOWN0, 0, 2);
		check(DOOR_SLOT_LEFT1, -1, 1);
		check(DOOR_SLOT_UP1, 1, -1);
		check(DOOR_SLOT_RIGHT1, 2, 1);
		check(DOOR_SLOT_DOWN1, 1, 2);
		break;
	case ROOMSHAPE_LTR:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 1, 0);
		check(DOOR_SLOT_UP1, 1, 0);
		check(DOOR_SLOT_DOWN0, 0, 2);
		check(DOOR_SLOT_LEFT1, -1, 1);
		check(DOOR_SLOT_RIGHT1, 2, 1);
		check(DOOR_SLOT_DOWN1, 1, 2);
		break;
	case ROOMSHAPE_LBL:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		check(DOOR_SLOT_DOWN0, 0, 1);
		check(DOOR_SLOT_LEFT1, 0, 1);
		check(DOOR_SLOT_UP1, 1, -1);
		check(DOOR_SLOT_RIGHT1, 2, 1);
		check(DOOR_SLOT_DOWN1, 1, 2);
		break;
	case ROOMSHAPE_LBR:
		check(DOOR_SLOT_LEFT0, -1, 0);
		check(DOOR_SLOT_UP0, 0, -1);
		check(DOOR_SLOT_RIGHT0, 2, 0);
		check(DOOR_SLOT_DOWN0, 0, 2);
		check(DOOR_SLOT_LEFT1, -1, 1);
		check(DOOR_SLOT_UP1, 1, -1);
		check(DOOR_SLOT_RIGHT1, 1, 1);
		check(DOOR_SLOT_DOWN1, 1, 1);
		break;
	}
	return doors;
}

#pragma endregion
