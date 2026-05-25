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

#pragma endregion