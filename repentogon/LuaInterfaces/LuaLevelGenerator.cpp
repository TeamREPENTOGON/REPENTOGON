#include <iterator>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>
#include <vector>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaLevelGenerator.h"

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
		data->cleanup = false;
		ZHL::Log("About to cleanup %p\n", data->room);
		delete data->room;
		ZHL::Log("Done cleaning up %p\n", data->room);
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
		return luaL_error(L, "Invalid column %d, value must be between 0 and 12 (inclusive)", column);
	}

	int line = (int)luaL_checkinteger(L, 3);
	if (line < 0 || line > 12) {
		return luaL_error(L, "Invalid line %d, value must be between 0 and 12 (inclusive)", line);
	}

	int shape = (int)luaL_checkinteger(L, 4);
	if (shape < 0 || shape >= eRoomShape::MAX_ROOMSHAPES) {
		return luaL_error(L, "Invalid room shape %d, value must be between 0 and %d (inclusive)", shape, eRoomShape::MAX_ROOMSHAPES);
	}
	
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
	room._originNeighborConnectDir = dir;

	if (RequiresAdjustment(eShape)) {
		dir = ComputeAdjustedLinkDirection(room, targetIndex);
		if (dir == LINK_DIRECTION_INVALID) {
			return luaL_error(L, "Unable to compute adjusted link direction with the neighbor");
		}
	}

	room._originNeighborConnectDirAdjust = dir;

	try {
		std::tie(room._linkColIdx, room._linkLineIdx) = ComputeSafeConnection(*neighborData->room, room);
	}
	catch (std::runtime_error& e) {
		return luaL_error(L, "[ERROR] Unable to compute safe connection between room at (%d, %d) and tentative room at (%d, %d): %s\n",
			neighborData->room->_gridColIdx, neighborData->room->_gridLineIdx,
			room._gridColIdx, room._gridLineIdx, e.what());
	}

	if (!generator->is_placement_valid(&room._gridColIdx, eShape)) {
		return luaL_error(L, "Error while adding room: placement is invalid (%d, %d) from (%d, %d)", room._gridColIdx, room._gridLineIdx, neighborData->room->_gridColIdx, neighborData->room->_gridLineIdx);
	}

	void* rooms = generator->GetAllRooms();
	auto get_ptr = [rooms]() -> void* {
		char* first = *(char**)rooms;
		char* last = *(char**)((char*)rooms + 8);

		if (last - first > 0x1000) {
			return *(char**)((char*)first - 4);
		}
		else {
			return first;
		}
	};
	//void* unk = ((char*)*(void**)rooms - 4);
	//ZHL::Log("pre place_room unk = %p\n", *(void**)unk);
	HANDLE heap = GetProcessHeap();
	// ZHL::Log("room ptr: %p, heap ptr: %p\n", get_ptr(), heap);

	auto check_stack = [heap, get_ptr](const char* text) {
		bool ok_array = HeapValidate(heap, 0, get_ptr());
		bool ok_all = HeapValidate(heap, 0, 0);

		ZHL::Log("%s: array = %hhd (%p), all = %hhd\n", text, ok_array, get_ptr(), ok_all);

		return std::make_tuple(ok_array, ok_all);
	};

	auto heap_iterate = [heap](const char* s) {
		PROCESS_HEAP_ENTRY entry;
		entry.lpData = NULL;

		ZHL::Log("Validating heap because a corruption has been detected (%s)\n", s);
		HeapLock(heap);
		while (HeapWalk(heap, &entry)) {
			if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) {
				BOOL ok = HeapValidate(heap, 0, entry.lpData);
				if (!ok) {
					ZHL::Log("Invalid heap block at %p\n", entry.lpData);
				}
				else {
					ZHL::Log("Valid heap block at %p\n", entry.lpData);
				}
			}
		}

		DWORD err = GetLastError();
		if (GetLastError() != ERROR_NO_MORE_ITEMS) {
			ZHL::Log("Error while validating heap: %d\n", err);
		}

		HeapUnlock(heap);
	};

	auto dump_layout = [generator](const char* s) {
		ZHL::Logger logger;
		logger.Log(s);

		signed char rooms[13][13] = { -1 } ;
		for (LevelGenerator_Room const& room : *generator->GetAllRooms()) {
			int col = room._gridColIdx;
			int line = room._gridLineIdx;
			int idx = room._generationIndex;
			switch (room._shape) {
			case 1:
			case 2:
			case 3:
				rooms[line][col] = idx;
				break;

			case 4:
			case 5:
				rooms[line][col] = idx;
				rooms[line + 1][col] = idx;
				break;

			case 6:
			case 7:
				rooms[line][col] = idx;
				rooms[line][col + 1] = idx;
				break;

			case 8:
				rooms[line][col] = idx;
				rooms[line + 1][col] = idx;
				rooms[line][col + 1] = idx;
				rooms[line + 1][col + 1] = idx;
				break;

			case 9:
				rooms[line + 1][col] = idx;
				rooms[line][col + 1] = idx;
				rooms[line + 1][col + 1] = idx;
				break;

			case 10:
				rooms[line][col] = idx;
				rooms[line + 1][col] = idx;
				rooms[line + 1][col + 1] = idx;
				break;

			case 11:
				rooms[line][col] = idx;
				rooms[line][col + 1] = idx;
				rooms[line + 1][col + 1] = idx;
				break;

			case 12:
				rooms[line][col] = idx;
				rooms[line + 1][col] = idx;
				rooms[line][col + 1] = idx;
				break;

			default:
				break;
			}
		}

		for (int i = 0; i < 13; ++i) {
			for (int j = 0; j < 13; ++j) {
				char c = rooms[i][j];
				if (c >= 10) {
					logger.Log("%hhd ", c);
				}
				else {
					logger.Log("%hhd  ", c);
				}
			}
			logger.Log("\n");
		}

		logger.Log("\n");
	};

	// bool ok_array_pre_place, ok_all_pre_place, ok_array_post_place, ok_all_post_place, ok_array_post_calculate, ok_all_post_calculate, ok_array_post_mark, ok_all_post_mark; 
	// std::tie(ok_array_pre_place, ok_all_pre_place) = check_stack("pre place_room");
	// dump_layout("pre place_room\n");
	bool result = generator->place_room(&room);
	// dump_layout("post place_room\n");
	// std::tie(ok_array_post_place, ok_all_post_place) = check_stack("post place_room");

	// generator->mark_dead_ends();
	// std::tie(ok_array_post_mark, ok_all_post_mark) = check_stack("post mark_dead_ends");

	/* if (ok_array_post_place && ok_array_post_mark != ok_array_post_place) {
		ZHL::Log("Local heap block corruption during mark_dead_ends");
		abort();
	} */

	/* if (ok_all_post_place && ok_all_post_mark != ok_all_post_place) {
		ZHL::Log("Global heap corruption during mark_dead_ends");
		abort();
	} */

	if (result) {
		// unk = ((char*)*(void**)rooms - 4);
		// ZHL::Log("pre calc_required_doors unk = %p\n", *(void**)unk);
		generator->calc_required_doors();
		// dump_layout("post calc_required_doors\n");
		// std::tie(ok_array_post_calculate, ok_all_post_calculate) = check_stack("post calc_required_doors");

		/* if (ok_array_post_mark && ok_array_post_calculate != ok_array_post_mark) {
			// heap_iterate("array invalidated during calc_required_doors\n");
			abort();
		} */

		/* if (ok_all_post_mark && ok_all_post_calculate != ok_all_post_mark) {
			// heap_iterate("heap invalidated during calc_required_doors\n");
			abort();
		} */
		// unk2 = ((char*)*(void**)rooms - 4);
		// ZHL::Log("post calc_required_doors unk = %p\n", *(void**)unk2);
		lua_pushinteger(L, generator->GetAllRooms()->back()._generationIndex);
	}
	else {
		lua_pushnil(L);
	}

	ZHL::Log("Leaving PlaceRoom\n");
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

LUA_FUNCTION(lua_LGR_IsDeadEnd) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushboolean(L, data->room->_deadEnd);
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
	
	lua::RegisterNewClass(L, lua::metatables::LevelGeneratorMT, lua::metatables::LevelGeneratorMT, functions);
}

static void RegisterLevelGeneratorRoom(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GenerationIndex", lua_LGR_GenerationIndex },
		{ "DoorMask", lua_LGR_DoorMask },
		{ "Column", lua_LGR_Column },
		{ "Line", lua_LGR_Line },
		{ "Shape", lua_LGR_Shape },
		{ "Neighbors", lua_LGR_Neighbors },
		{ "IsDeadEnd", lua_LGR_IsDeadEnd },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::LevelGeneratorRoomMT, lua::metatables::LevelGeneratorRoomMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterLevelGenerator(_state);
	RegisterLevelGeneratorRoom(_state);
}