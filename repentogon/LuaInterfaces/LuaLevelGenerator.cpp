#include <iterator>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>

#include "IsaacRepentance.h"
#include "Exception.h"
#include "Log.h"
#include "../MiscFunctions.h"

#ifdef max
#undef max
#endif

enum LinkDirection {
	LINK_DIRECTION_INVALID = -1,
	LINK_DIRECTION_RIGHT,
	LINK_DIRECTION_DOWN,
	LINK_DIRECTION_LEFT,
	LINK_DIRECTION_UP,
	LINK_DIRECTION_ADJ_RIGHT,
	LINK_DIRECTION_ADJ_DOWN,
	LINK_DIRECTION_ADJ_LEFT,
	LINK_DIRECTION_ADJ_UP
};

static std::tuple<bool, std::optional<std::vector<int>>> ValidateRoomPlacement(LevelGenerator* generator, int col, int line, eRoomShape shape);
static bool ValidateRoomPlacement(LevelGenerator_Room const& source, int col, int line, eRoomShape shape);
static inline int ComposeGridIndex(int col, int line);
static inline std::tuple<int, int> DecomposeGridIndex(int index);
static std::vector<int> ShapeToIndices(int col, int line, eRoomShape shape);
static std::vector<int> ShapeToIndices(int index, eRoomShape shape);
static std::tuple<int, int> ShapeToDimensions(eRoomShape shape);
static std::tuple<bool, std::pair<int, int>> Connects(LevelGenerator_Room const& source, LevelGenerator_Room const& target);
static LinkDirection ComputeLinkDirection(int source, int target);
static bool RequiresAdjustment(eRoomShape shape);
static LinkDirection ComputeAdjustedLinkDirection(LevelGenerator_Room const& source, int index);
static std::tuple<int, int> ComputeSafeConnection(LevelGenerator_Room const& source, LevelGenerator_Room const& target);

int ComposeGridIndex(int col, int line) {
	return line * 13 + col;
}

std::tuple<int, int> ShapeToDimensions(eRoomShape shape) {
	switch (shape) {
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		return std::make_tuple(1, 1);

	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		return std::make_tuple(1, 2);

	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		return std::make_tuple(2, 1);

	default:
		return std::make_tuple(2, 2);
	}
}

std::tuple<int, int> DecomposeGridIndex(int index) {
	int line = index / 13;
	int col = index % 13;

	return std::make_tuple(line, col);
}

std::vector<int> ShapeToIndices(int col, int line, eRoomShape shape) {
	return ShapeToIndices(line * 13 + col, shape);
}

std::vector<int> ShapeToIndices(int index, eRoomShape shape) {
	std::vector<int> indices;
	switch (shape) {
		case ROOMSHAPE_1x1:
		case ROOMSHAPE_IH:
		case ROOMSHAPE_IV:
			indices.push_back(index);
			break;

		case ROOMSHAPE_1x2:
		case ROOMSHAPE_IIV:
			indices.push_back(index);
			indices.push_back(index + 13);
			break;

		case ROOMSHAPE_2x1:
		case ROOMSHAPE_IIH:
			indices.push_back(index);
			indices.push_back(index + 1);
			break;

		case ROOMSHAPE_2x2:
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + 13);
			indices.push_back(index + 14);
			break;

		case ROOMSHAPE_LTL:
			indices.push_back(index + 1);
			indices.push_back(index + 13);
			indices.push_back(index + 14);
			break;

		case ROOMSHAPE_LTR:
			indices.push_back(index);
			indices.push_back(index + 13);
			indices.push_back(index + 14);
			break;

		case ROOMSHAPE_LBL:
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + 14);
			break;

		case ROOMSHAPE_LBR:
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + 13);
			break;

		default:
			break;
	}

	return indices;
}

std::tuple<bool, std::optional<std::vector<int>>> ValidateRoomPlacement(LevelGenerator* generator, int col, int line, eRoomShape shape) {
	std::vector<int> errors;

	for (LevelGenerator_Room const& gen : *generator->GetAllRooms()) {
		if (!ValidateRoomPlacement(gen, col, line, shape)) {
			errors.push_back(gen._generationIndex);
		}
	}

	if (errors.empty()) {
		return std::make_tuple(true, std::nullopt);
	}
	else {
		return std::make_tuple(false, std::make_optional(std::move(errors)));
	}
}

bool ValidateRoomPlacement(LevelGenerator_Room const& source, int col, int line, eRoomShape shape) {
	std::vector<int> sourceIndices = ShapeToIndices(ComposeGridIndex(source._gridColIdx, source._gridLineIdx), (eRoomShape)source._shape);
	std::vector<int> ourIndices = ShapeToIndices(ComposeGridIndex(col, line), shape);
	std::set<int> intersection;
	std::insert_iterator<std::set<int>> output = std::inserter(intersection, intersection.begin());

	std::set_intersection(sourceIndices.begin(), sourceIndices.end(), ourIndices.begin(), ourIndices.end(), output);
	return intersection.empty();
}

std::tuple<bool, std::pair<int, int>> Connects(LevelGenerator_Room const& source, LevelGenerator_Room const& target) {
	std::vector<int> sourceIndices, targetIndices;
	sourceIndices = ShapeToIndices(source._gridColIdx, source._gridLineIdx, (eRoomShape)source._shape);
	targetIndices = ShapeToIndices(target._gridColIdx, target._gridLineIdx, (eRoomShape)target._shape);

	for (int sourceIndex : sourceIndices) {
		for (int targetIndex : targetIndices) {
			if (sourceIndex == targetIndex - 1 ||
				sourceIndex == targetIndex + 1 ||
				sourceIndex == targetIndex + 13 ||
				sourceIndex == targetIndex - 13) {
				return std::make_tuple(true, std::make_pair(sourceIndex, targetIndex));
			}
		}
	}

	return std::make_tuple(false, std::make_pair(-1, -1));
}

LinkDirection ComputeLinkDirection(int source, int target) {
	if (target == source - 13) {
		return LINK_DIRECTION_UP;
	}
	else if (target == source + 13) {
		return LINK_DIRECTION_DOWN;
	}
	else if (target == source - 1) {
		return LINK_DIRECTION_LEFT;
	}
	else if (target == source + 1) {
		return LINK_DIRECTION_RIGHT;
	}
	else {
		return LINK_DIRECTION_INVALID;
	}
}

bool RequiresAdjustment(eRoomShape shape) {
	return !(shape == ROOMSHAPE_1x1 || shape == ROOMSHAPE_IH || shape == ROOMSHAPE_IV);
}

LinkDirection ComputeAdjustedLinkDirection(LevelGenerator_Room const& source, int index) {
	int sourceIndex = ComposeGridIndex(source._gridColIdx, source._gridLineIdx);
	if (index == sourceIndex - 12) {
		return LINK_DIRECTION_ADJ_LEFT;
	}
	else if (index == sourceIndex + 14 || index == sourceIndex + 15) { // 14 : 1x2 or LBR, 15 : 2x2 or other Ls
		return LINK_DIRECTION_ADJ_RIGHT;
	}
	else if (index == sourceIndex - 12) {
		return LINK_DIRECTION_ADJ_UP;
	}
	else if (index == sourceIndex + 27) {
		return LINK_DIRECTION_ADJ_DOWN;
	}
	else {
		return LINK_DIRECTION_INVALID;
	}
}

// Return col, line
std::tuple<int, int> ComputeSafeConnection(LevelGenerator_Room const& source, LevelGenerator_Room const& target) {
	switch (target._shape) {
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		return std::make_tuple(target._gridColIdx, target._gridLineIdx);

	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		switch (target._originNeighborConnectDir) {
		case LINK_DIRECTION_RIGHT: // Not used for IIV
		case LINK_DIRECTION_LEFT: // Not used for IIV
			// -> T | S || S | -> T
			//    T | ? || ? |    T
			if (source._gridLineIdx <= target._gridLineIdx) {
				return std::make_tuple(target._gridColIdx, target._gridLineIdx);
			}
			// T    | ? || ? |    T
			// -> T | S || S | -> T
			else {
				return std::make_tuple(target._gridColIdx, target._gridLineIdx + 1);
			}
			break;

		case LINK_DIRECTION_UP:
			//    S
			// -> T
			//    T
			return std::make_tuple(target._gridColIdx, target._gridLineIdx);

			//    T
			// -> T
			//    S
		case LINK_DIRECTION_DOWN:
			return std::make_tuple(target._gridColIdx, target._gridLineIdx + 1);

		default:
			ZHL::Throw<std::runtime_error>("LevelGenerator::ComputeSafeConnection shape %d link %d\n", target._shape, target._originNeighborConnectDir);
			return std::make_tuple(-1, -1);
		}

	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		switch (target._originNeighborConnectDir) {
			// T | -> T | S
		case LINK_DIRECTION_RIGHT:
			return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);

			// S | -> T | T
		case LINK_DIRECTION_LEFT:
			return std::make_tuple(target._gridColIdx, target._gridLineIdx);

		case LINK_DIRECTION_UP: // Not used for IIH
		case LINK_DIRECTION_DOWN: // Not used for IIH
			//    S     // Or under
			// -> T | T
			if (target._gridColIdx <= source._gridColIdx) {
				return std::make_tuple(target._gridColIdx, target._gridLineIdx);
			}
			// ? |    S // Or under
			// T | -> T
			else {
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);
			}

		default:
			ZHL::Throw<std::runtime_error>("LevelGenerator::ComputeSafeConnection shape %d link %d\n", target._shape, target._originNeighborConnectDir);
			return std::make_tuple(-1, -1);
		}

	default:
		switch (target._originNeighborConnectDir) {
		case LINK_DIRECTION_UP:
			switch (target._shape) {
			case ROOMSHAPE_LTL:
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);

			case ROOMSHAPE_LTR:
				return std::make_tuple(target._gridColIdx, target._gridLineIdx);

			default:
				if (target._gridColIdx <= source._gridColIdx) {
					return std::make_tuple(target._gridColIdx, target._gridLineIdx);
				}
				else {
					return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);
				}
			}

		case LINK_DIRECTION_LEFT:
			switch (target._shape) {
			case ROOMSHAPE_LTL:
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);

			case ROOMSHAPE_LBL:
				return std::make_tuple(target._gridColIdx, target._gridLineIdx);

			default:
				if (source._gridLineIdx <= target._gridLineIdx) {
					return std::make_tuple(target._gridColIdx, target._gridLineIdx);
				}
				else {
					return std::make_tuple(target._gridColIdx, target._gridLineIdx + 1);
				}
			}

		case LINK_DIRECTION_RIGHT:
			switch (target._shape) {
			case ROOMSHAPE_LTR:
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx + 1);

			case ROOMSHAPE_LBR:
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);

			default:
				if (source._gridLineIdx <= target._gridLineIdx) {
					return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx);
				}
				else {
					return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx + 1);
				}
			}

		case LINK_DIRECTION_DOWN:
			switch (target._shape) {
			case ROOMSHAPE_LBL:
				return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx + 1);

			case ROOMSHAPE_LBR:
				return std::make_tuple(target._gridColIdx, target._gridLineIdx + 1);

			default:
				if (target._gridColIdx <= source._gridColIdx) {
					return std::make_tuple(target._gridColIdx, target._gridLineIdx + 1);
				}
				else {
					return std::make_tuple(target._gridColIdx + 1, target._gridLineIdx + 1);
				}
			}
		default:
			ZHL::Throw<std::runtime_error>("LevelGenerator::ComputeSafeConnection shape %d link %d\n", target._shape, target._originNeighborConnectDir);
			return std::make_tuple(-1, -1);
		}
	}
}

static std::string s_PlaceRoomError;

static const char* PlaceRoomError(std::string message) {
	s_PlaceRoomError = std::move(message);
	return s_PlaceRoomError.c_str();
}

MOD_EXPORT const char* L_LevelGenerator_PlaceRoom(LevelGenerator* generator, int column, int line, int shape, LevelGenerator_Room* neighbor, int* outIndex) {
	*outIndex = -1;
	eRoomShape eShape = (eRoomShape)shape;

	auto [ok, errors] = ValidateRoomPlacement(generator, column, line, eShape);
	if (!ok) {
		std::ostringstream err;
		err << "Invalid room data (shape = " << shape << ", coordinates (" << line << ", " << column << ")), this would collide with room";
		std::vector<int> const& conflicts = *errors;
		if (conflicts.size() > 1) {
			err << "s";
		}

		err << ": ";
		for (int conflict : conflicts) {
			LevelGenerator_Room const& room = generator->GetAllRooms()->at(conflict);
			err << conflict << " at (" << room._gridLineIdx << ", " << room._gridColIdx << "); ";
		}

		return PlaceRoomError(err.str());
	}

	// Reverse engineering is_placement_valid, the following must be known :
	// gridCol, lineCol, horizontal / vertical size, shape, originNeighborConnect*, col / line link, distance from start

	LevelGenerator_Room room;
	room._gridColIdx = column;
	room._gridLineIdx = line;
	room._shape = eShape;
	std::tie(room._horizontalSize, room._verticalSize) = ShapeToDimensions(eShape);
	room._distanceFromStart = neighbor->_distanceFromStart + 1;

	auto [connects, connection] = Connects(room, *neighbor);
	if (!connects) {
		return PlaceRoomError("Source room placement does not allow a connection with the target room");
	}

	auto [sourceIndex, targetIndex] = connection;
	LinkDirection dir = ComputeLinkDirection(sourceIndex, targetIndex);
	if (dir == LINK_DIRECTION_INVALID) {
		return PlaceRoomError("Unable to compute the basic link direction with the neighbor");
	}
	room._originNeighborConnectDir = dir;

	if (RequiresAdjustment(eShape)) {
		dir = ComputeAdjustedLinkDirection(room, targetIndex);
		if (dir == LINK_DIRECTION_INVALID) {
			return PlaceRoomError("Unable to compute adjusted link direction with the neighbor");
		}
	}

	room._originNeighborConnectDirAdjust = dir;

	try {
		std::tie(room._linkColIdx, room._linkLineIdx) = ComputeSafeConnection(*neighbor, room);
	}
	catch (std::runtime_error& e) {
		return PlaceRoomError(REPENTOGON::StringFormat("[ERROR] Unable to compute safe connection between room at (%d, %d) and tentative room at (%d, %d): %s\n",
			neighbor->_gridColIdx, neighbor->_gridLineIdx,
			room._gridColIdx, room._gridLineIdx, e.what()));
	}

	if (!generator->is_placement_valid(&room._gridColIdx, eShape)) {
		return PlaceRoomError(REPENTOGON::StringFormat("Error while adding room: placement is invalid (%d, %d) from (%d, %d)", room._gridColIdx, room._gridLineIdx, neighbor->_gridColIdx, neighbor->_gridLineIdx));
	}

	if (generator->place_room(&room)) {
		generator->calc_required_doors();
		*outIndex = generator->GetAllRooms()->back()._generationIndex;
	}

	ZHL::Log("Leaving PlaceRoom\n");
	return nullptr;
}
