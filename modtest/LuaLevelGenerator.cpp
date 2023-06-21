#include <iterator>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>
#include <vector>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

#ifdef max
#undef max
#endif

struct LuaLevelGeneratorRoom {
	LevelGenerator* context = nullptr;
	LevelGenerator_Room* room = nullptr;
	bool cleanup = false;
};

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

static void ExtractRooms(lua_State* L, bool deadEnds);
static void PushRoom(lua_State* L, int i, LevelGenerator* generator, LevelGenerator_Room& room);
static LevelGenerator* GetLevelGenerator(lua_State* L, int idx = 1);
static LuaLevelGeneratorRoom* GetLGR(lua_State* L, int idx = 1);
static LuaLevelGeneratorRoom* CreateLGR(lua_State* L);
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

LUA_FUNCTION(lua_LGR_gc) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	if (data->cleanup) {
		delete data->room;
	}
	return 0;
}

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

LuaLevelGeneratorRoom* CreateLGR(lua_State* L) {
	LuaLevelGeneratorRoom* data = (LuaLevelGeneratorRoom*)lua_newuserdatauv(L, sizeof(LuaLevelGeneratorRoom), 0);
	luaL_setmetatable(L, lua::metatables::LevelGeneratorRoomMT);
	return data;
}

void ExtractRooms(lua_State* L, bool deadEnds) {
	LevelGenerator* generator = GetLevelGenerator(L); // lua::GetUserdata<LevelGenerator*>(L, 1, lua::metatables::LevelGeneratorMT);
	lua_newtable(L);
	int i = 1;
	std::vector<int>* target;
	if (deadEnds) {
		target = generator->GetDeadEnds();
	}
	else {
		target = generator->GetNonDeadEnds();
	}

	std::vector<LevelGenerator_Room>& rooms = *generator->GetAllRooms();
	for (int roomId : *target) {
		PushRoom(L, i, generator, rooms[roomId]);
		++i;
	}
}

void PushRoom(lua_State* L, int i, LevelGenerator* generator, LevelGenerator_Room& room) {
	lua_pushinteger(L, i);
	LuaLevelGeneratorRoom* data = CreateLGR(L);
	data->context = generator;
	data->room = &room;
	lua_rawset(L, -3);
}

LevelGenerator* GetLevelGenerator(lua_State* L, int idx) {
	return *lua::GetUserdata<LevelGenerator**>(L, idx, lua::metatables::LevelGeneratorMT);
}

LuaLevelGeneratorRoom* GetLGR(lua_State* L, int idx) {
	return lua::GetUserdata<LuaLevelGeneratorRoom*>(L, idx, lua::metatables::LevelGeneratorRoomMT);
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
		return LINK_DIRECTION_DOWN;
	}
	else if (target == source + 13) {
		return LINK_DIRECTION_UP;
	}
	else if (target == source - 1) {
		return LINK_DIRECTION_RIGHT;
	}
	else if (target == source + 1) {
		return LINK_DIRECTION_LEFT;
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
		return LINK_DIRECTION_ADJ_RIGHT;
	}
	else if (index == sourceIndex + 14 || index == sourceIndex + 15) { // 14 : 1x2 or LBR, 15 : 2x2 or other Ls
		return LINK_DIRECTION_ADJ_LEFT;
	}
	else if (index == sourceIndex - 12) {
		return LINK_DIRECTION_ADJ_DOWN;
	}
	else if (index == sourceIndex + 27) {
		return LINK_DIRECTION_ADJ_UP;
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
			// -> T | S || S |    T
			//    T | ? || ? | -> T
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
		}
	}
}

LUA_FUNCTION(lua_LG_GetAllRooms) {
	LevelGenerator* generator = GetLevelGenerator(L);
	lua_newtable(L);
	int i = 1;
	for (LevelGenerator_Room & room : *(generator->GetAllRooms())) {
		PushRoom(L, i, generator, room);
		++i;
	}
	return 1;
}

LUA_FUNCTION(lua_LG_GetNonDeadEnds) {
	ExtractRooms(L, false);
	return 1;
}

LUA_FUNCTION(lua_LG_GetDeadEnds) {
	ExtractRooms(L, true);
	return 1;
}

/* LUA_FUNCTION(lua_LG_NewRoomData) {
	LevelGenerator_Room* room = new LevelGenerator_Room;
	room->_doors = room->_distanceFromStart
		= room->_horizontalSize
		= room->_verticalSize
		= room->_generationIndex 
		= 0;

	// Invalid value because we need to identify if the room has been properly initialized or not
	room->_gridColIdx = room->_gridLineIdx = room->_shape = LevelGeneratorRoom_INVALID;

	room->_linkColIdx = room->_linkLineIdx
		= room->_originNeighborConnectDir
		= room->_originNeighborConnectDirAdjust
		= -1;

	room->_deadEnd = false;

	LuaLevelGeneratorRoom* data = CreateLGR(L);
	data->context = GetLevelGenerator(L);
	data->cleanup = true;
	data->room = room;
	return 1;
} */

LUA_FUNCTION(lua_LG_PlaceRoom) {
	LevelGenerator* generator = GetLevelGenerator(L);

	int column = (int)luaL_checkinteger(L, 2);
	if (column < 0 || column > 12) {
		return luaL_error(L, "Invalid column %lld, value must be between 0 and 12 (inclusive)", column);
	}

	int line = (int)luaL_checkinteger(L, 3);
	if (line < 0 || line > 12) {
		return luaL_error(L, "Invalid line %lld, value must be between 0 and 12 (inclusive)", line);
	}

	int shape = (int)luaL_checkinteger(L, 4);
	if (shape < 0 || shape >= eRoomShape::MAX_ROOMSHAPES) {
		return luaL_error(L, "Invalid room shape %lld, value must be between 0 and %d (inclusive)", shape, eRoomShape::MAX_ROOMSHAPES);
	}

	eRoomShape eShape = (eRoomShape)shape;

	auto [ok, errors] = ValidateRoomPlacement(generator, column, line, eShape);
	if (!ok) {
		std::ostringstream err;
		err << "Invalid room data (shape = " << shape << ", coordinates (" << line << ", " << column << ")), this would collide with room";
		std::vector<int> const& conflicts = *errors;
		if (conflicts.size() > 1) {
			err << "s:";
		}

		err << " ";
		for (int conflict : conflicts) {
			LevelGenerator_Room const& room = generator->GetAllRooms()->at(conflict);
			err << conflict << " at (" << room._gridLineIdx << ", " << room._gridColIdx << "); ";
		}

		return luaL_error(L, err.str().c_str());
	}

	LuaLevelGeneratorRoom* neighborData = GetLGR(L, 5);

	// Reverse engineering is_placement_valid, the following must be known :
	// gridCol, lineCol, horizontal / vertical size, shape, originNeighborConnect*, col / line link, distance from start

	LevelGenerator_Room room;
	room._gridColIdx = column;
	room._gridLineIdx = line;
	room._shape = eShape;
	std::tie(room._horizontalSize, room._verticalSize) = ShapeToDimensions(eShape);
	room._distanceFromStart = neighborData->room->_distanceFromStart + 1;

	auto [connects, connection] = Connects(room, *neighborData->room);
	if (!connects) {
		return luaL_error(L, "Source room placement does not allow a connection with the target room");
	}

	auto [sourceIndex, targetIndex] = connection;
	LinkDirection dir = ComputeLinkDirection(sourceIndex, targetIndex);
	if (dir == LINK_DIRECTION_INVALID) {
		return luaL_error(L, "Unable to compute the basic link direction with the neighbor");
	}

	if (RequiresAdjustment(eShape)) {
		dir = ComputeAdjustedLinkDirection(room, targetIndex);
		if (dir == LINK_DIRECTION_INVALID) {
			return luaL_error(L, "Unable to compute adjusted link direction with the neighbor");
		}
	}

	room._originNeighborConnectDir = dir;
	room._originNeighborConnectDirAdjust = dir;

	std::tie(room._linkColIdx, room._linkLineIdx) = ComputeSafeConnection(*neighborData->room, room);

	if (!generator->is_placement_valid(&room._gridColIdx, eShape)) {
		return luaL_error(L, "Error while adding room: placement is invalid");
	}

	bool result = generator->place_room(&room);

	if (result) {
		generator->calc_required_doors();
		lua_pushinteger(L, generator->GetAllRooms()->back()._generationIndex);
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(lua_LGR_GenerationIndex) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_generationIndex);
	return 1;
}

LUA_FUNCTION(lua_LGR_Column) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_gridColIdx);
	return 1;
}

LUA_FUNCTION(lua_LGR_Line) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_gridLineIdx);
	return 1;
}

LUA_FUNCTION(lua_LGR_Shape) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_shape);
	return 1;
}

LUA_FUNCTION(lua_LGR_DoorMask) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_doors);
	return 1;
}

LUA_FUNCTION(lua_LGR_Neighbors) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_newtable(L);

	int i = 1;
	for (int idx : data->room->_neighbors) {
		lua_pushinteger(L, i);
		lua_pushinteger(L, idx);
		lua_rawset(L, -3);
		++i;
	}

	return 1;
}

static void RegisterLevelGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetAllRooms", lua_LG_GetAllRooms },
		{ "GetDeadEnds", lua_LG_GetDeadEnds },
		{ "GetNonDeadEnds", lua_LG_GetNonDeadEnds },
		{ "PlaceRoom", lua_LG_PlaceRoom },
		{ NULL, NULL }
	};
	
	lua::RegisterNewClass(L, "LevelGenerator", lua::metatables::LevelGeneratorMT, functions);
}

static void RegisterLevelGeneratorRoom(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GenerationIndex", lua_LGR_GenerationIndex },
		{ "DoorMask", lua_LGR_DoorMask },
		{ "Column", lua_LGR_Column },
		{ "Line", lua_LGR_Line },
		{ "Shape", lua_LGR_Shape },
		{ "Neighbors", lua_LGR_Neighbors },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, "LevelGeneratorRoom", lua::metatables::LevelGeneratorRoomMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterLevelGenerator(this->_state);
	RegisterLevelGeneratorRoom(this->_state);
}