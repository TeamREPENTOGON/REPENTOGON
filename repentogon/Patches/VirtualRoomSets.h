#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../RoomConfigUtility.h"
#include "../LogUtility.h"
#include "LuaCore.h"

#include <string>
#include <vector>
#include <list>
#include <array>
#include <optional>

class VirtualRoomSetManager;
class VirtualRoomSet;

class VirtualRoomSetManager
{
public:
	class detail
	{
	public:
		using _VirtualRoomSet = std::vector<RoomConfig_Room*>;
	public:
		/// @brief Clears the restore rooms DB on rerun, as there are no rooms.
		static void ClearDB(uint32_t slot);
		/// @brief Hijacks the GameState structure and prepares it for Save
		static void PreWriteGameState(GameState& gameState, uint32_t slot);
		/// @brief Restores the GameState structure to it's "normal" form after Save.
		static void PostWriteGameState(GameState& gameState, uint32_t slot);
		/// @brief Checks if the game state has at least one structure that has been hijacked by
		/// the manager, which requires normalizing.
		static bool GameStateNeedsHandling(GameState& gameState);

		/// @brief Writes the restored rooms DB to permanent storage.
		static bool WriteSave(const std::string& fileName, uint32_t slot, uint32_t gameChecksum, bool isRerun);
		/// @brief Reads the restore rooms DB for the save slot, if it hasn't been loaded yet, then
		/// restores the game state to it's "normal" form.
		static bool ReadSave(GameState& gameState, const std::string& fileName, uint32_t slot, bool isRerun);
		/// @brief Deletes the restored rooms DB file, also clears the DB itself.
		static void DeleteSave(const std::string fileName, uint32_t slot, bool isRerun);

		// The lua functions are placed here since they are just details for the LUA API
		// rather than functionality of a VirtualRoomSet

		static int Lua_AddLuaRooms(lua_State* L, VirtualRoomSet& virtualSet, int tableIdx);
		static int Lua_AddStbRooms(lua_State* L, VirtualRoomSet& virtualSet, const std::string& fileName);
	};

public:
	/// @brief Gets the vanilla set extension, using the vanilla RoomSet id.
	static VirtualRoomSet GetVanillaSet(uint32_t stageId, int mode);
	/// @brief Creates a new room set.
	static VirtualRoomSet CreateSet();
};

class VirtualRoomSet
{
	friend class VirtualRoomSetManager;
	using _VirtualRoomSet = VirtualRoomSetManager::detail::_VirtualRoomSet;
	using Iterator = _VirtualRoomSet::const_iterator;

private:
	const size_t m_id;

private:
	VirtualRoomSet(size_t id);
public:
	RoomConfig_Room* operator[](size_t index);
	size_t size();
	Iterator begin();
	Iterator end();
	/// @brief Adds the rooms that match the given filter to the buffer
	void GetRooms(std::vector<RoomConfig_Room*>buffer, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType);
	void ResetRoomWeights();
};

inline VirtualRoomSet::VirtualRoomSet(size_t id)
	: m_id(id)
{}