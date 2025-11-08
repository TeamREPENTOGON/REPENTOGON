#include "LevelGenUtils.h"
#include <vector>
#include <IsaacRepentance.h>

#pragma region RoomCoords struct impl

RoomCoords::RoomCoords(uint32_t col, uint32_t row) {
	this->col = col;
	this->row = row;
}

bool RoomCoords::IsValid() const {
	return row >= 0 && row <= 13 && col >= 0 && col <= 13;
}

int RoomCoords::ToGridIndex() const {
	return col + row * 13;
}

bool RoomCoords::operator==(RoomCoords const& other) const {
	return this->row == other.row && this->col == other.col;
}

#pragma endregion

#pragma region Helpers

std::vector<RoomCoords> GetOccupiedCoords(RoomCoords& base_coords, int shape) {
	std::vector<RoomCoords> occupied_coords = {};

	switch (shape)
	{
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		break;
	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		break;
	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		break;
	case ROOMSHAPE_2x2:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_LTL:
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_LTR:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_LBL:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_LBR:
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		occupied_coords.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		break;
	default:
		break;
	}

	return occupied_coords;
}

std::vector<RoomCoords> GetForbiddenNeighbors(RoomCoords& base_coords, int shape, int doors) {
	std::vector<RoomCoords> forbidden_neighbors = {};

	switch (shape)
	{
	case ROOMSHAPE_1x1:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		}
		break;
	case ROOMSHAPE_IH:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		break;
	case ROOMSHAPE_IV:
		forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		}
		break;
	case ROOMSHAPE_1x2:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		}
		break;
	case ROOMSHAPE_IIV:
		forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_2x1:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		}
		break;
	case ROOMSHAPE_IIH:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		break;
	case ROOMSHAPE_2x2:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 2));
		}
		break;
	case ROOMSHAPE_LTL:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0 || (doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 2));
		}
		break;
	case ROOMSHAPE_LTR:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0 || (doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 2));
		}
		break;
	case ROOMSHAPE_LBL:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0 || (doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 2));
		}
		break;
	case ROOMSHAPE_LBR:
		if ((doors & (1 << DOOR_SLOT_LEFT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_UP0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 2, base_coords.row));
		}
		if ((doors & (1 << DOOR_SLOT_DOWN0)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col, base_coords.row + 2));
		}
		if ((doors & (1 << DOOR_SLOT_LEFT1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col - 1, base_coords.row + 1));
		}
		if ((doors & (1 << DOOR_SLOT_UP1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row - 1));
		}
		if ((doors & (1 << DOOR_SLOT_RIGHT1)) == 0 || (doors & (1 << DOOR_SLOT_DOWN1)) == 0) {
			forbidden_neighbors.push_back(RoomCoords(base_coords.col + 1, base_coords.row + 1));
		}
		break;
	default:
		break;
	}

	return forbidden_neighbors;
}

#pragma endregion