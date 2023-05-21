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
	LINK_DIRECTION_LEFT,
	LINK_DIRECTION_DOWN,
	LINK_DIRECTION_RIGHT,
	LINK_DIRECTION_UP,
	LINK_DIRECTION_ADJ_LEFT,
	LINK_DIRECTION_ADJ_DOWN,
	LINK_DIRECTION_ADJ_RIGHT,
	LINK_DIRECTION_ADJ_UP
};

// Used to indicate if 
static constexpr uint32_t LevelGeneratorRoom_INVALID = std::numeric_limits<uint32_t>::max();

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
static bool IsInitialized(LevelGenerator_Room const& room);
static LinkDirection ComputeLinkDirection(int source, int target);
static bool RequiresAdjustment(eRoomShape shape);
static LinkDirection ComputeAdjustedLinkDirection(LevelGenerator_Room const& source, int index);

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
	return lua::UserdataToData<LevelGenerator*>(luaL_checkudata(L, idx, lua::metatables::LevelGeneratorMT));
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

bool IsInitialized(LevelGenerator_Room const& room) {
	return room._gridColIdx != LevelGeneratorRoom_INVALID &&
		room._gridLineIdx != LevelGeneratorRoom_INVALID &&
		room._shape != LevelGeneratorRoom_INVALID;
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

LUA_FUNCTION(lua_LG_NewRoomData) {
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
}

LUA_FUNCTION(lua_LG_PlaceRoom) {
	LevelGenerator* generator = GetLevelGenerator(L);
	LuaLevelGeneratorRoom* data = GetLGR(L, 2);
	bool result = generator->place_room(data->room);
	if (result) {
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

LUA_FUNCTION(lua_LGR_SetCoordsAndShape) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_Integer col = luaL_checkinteger(L, 2);
	lua_Integer line = luaL_checkinteger(L, 3);
	lua_Integer shape = luaL_checkinteger(L, 4);

	if (col < 0 || col > 12) {
		return luaL_argerror(L, 2, "Column parameter must be between 0 and 12 (inclusive)");
	}

	if (line < 0 || line > 12) {
		return luaL_argerror(L, 3, "Line parameter must be between 0 and 12 (inclusive)");
	}

	if (shape < 0 || shape >= eRoomShape::MAX_ROOMSHAPES) {
		char buffer[4096];
		sprintf(buffer, "Shape parameter must be between 0 and %d\n", eRoomShape::MAX_ROOMSHAPES);
		return luaL_argerror(L, 4, buffer);
	}

	auto [ok, errors] = ValidateRoomPlacement(data->context, col, line, (eRoomShape)shape);
	if (!ok) {
		std::ostringstream err;
		err << "Invalid room data (shape = " << shape << ", coordinates (" << line << ", " << col << ")), this would collide with room";
		std::vector<int> const& conflicts = *errors;
		if (conflicts.size() > 1) {
			err << "s:";
		}

		err << " ";
		for (int conflict : conflicts) {
			LevelGenerator_Room const& room = data->context->GetAllRooms()->at(conflict);
			err << conflict << " at (" << room._gridLineIdx << ", " << room._gridColIdx << "); ";
		}

		return luaL_error(L, err.str().c_str());
	}

	LevelGenerator_Room* room = data->room;
	room->_gridColIdx = col;
	room->_linkColIdx = line;
	room->_shape = shape;
	std::tie(room->_horizontalSize, room->_verticalSize) = ShapeToDimensions((eRoomShape)shape);

	return 0;
}

LUA_FUNCTION(lua_LGR_SetGenerationNeighbor) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	LevelGenerator_Room* room;

	if (lua_type(L, 2) == LUA_TNUMBER) {
		lua_Integer value = luaL_checkinteger(L, 2);
		if (int n = data->context->GetAllRooms()->size(); value >= n || n < 0) {
			std::ostringstream err;
			err << "Invalid generation neighbor room " << value << ". Highest available value " << n;
			return luaL_argerror(L, 2, err.str().c_str());
		}

		room = &(data->context->GetAllRooms()->at(value));
	}
	else if (lua_type(L, 2) == LUA_TUSERDATA) {
		LuaLevelGeneratorRoom* other = GetLGR(L, 2);
		room = other->room;
	}
	else {
		return luaL_argerror(L, 2, "Expected number of LevelGeneratorRoom");
	}

	if (!IsInitialized(*data->room)) {
		return luaL_error(L, "Room missing position on the grid and/or shape, no connection can be established");
	}

	auto [connects, connection] = Connects(*room, *data->room);
	if (!connects) {
		return luaL_error(L, "Source room placement does not allow a connection with the target room");
	}

	auto [sourceIndex, targetIndex] = connection;
	LinkDirection dir = ComputeLinkDirection(sourceIndex, targetIndex);
	if (dir == LINK_DIRECTION_INVALID) {
		return luaL_error(L, "Unable to compute the basic link direction with the neighbor");
	}

	if (RequiresAdjustment((eRoomShape)room->_shape)) {
		dir = ComputeAdjustedLinkDirection(*room, targetIndex);
		if (dir == LINK_DIRECTION_INVALID) {
			return luaL_error(L, "Unable to compute adjusted link direction with the neighbor");
		}
	}

	data->room->_originNeighborConnectDir = dir;
	data->room->_originNeighborConnectDirAdjust = dir;

	return 0;
}

LUA_FUNCTION(lua_LGR_Connect) {
	return 0;
}

static void RegisterLevelGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetAllRooms", lua_LG_GetAllRooms },
		{ "GetDeadEnds", lua_LG_GetDeadEnds },
		{ "GetNonDeadEnds", lua_LG_GetNonDeadEnds },
		{ "NewRoomData", lua_LG_NewRoomData },
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
		{ "SetCoordsAndShape", lua_LGR_SetCoordsAndShape },
		{ "SetGenerationNeighbor", lua_LGR_SetGenerationNeighbor },
		{ "Connect", lua_LGR_Connect },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, "LevelGeneratorRoom", lua::metatables::LevelGeneratorRoomMT, functions, lua_LGR_gc);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterLevelGenerator(this->_state);
	RegisterLevelGeneratorRoom(this->_state);
}