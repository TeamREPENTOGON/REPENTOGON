#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../RoomConfigUtility.h"
#include "../LogUtility.h"
#include "LuaCore.h"
#include "../SaveStateManagement/GameStateManagement.h"
#include "../Utils/Array.h"

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
		static void ClearDB(const GameStateSaveInfo& saveInfo);
		/// @brief Hijacks the GameState structure and prepares it for Save
		static void PreWriteGameState(GameState& gameState, const GameStateSaveInfo& saveInfo);
		/// @brief Restores the GameState structure to it's "normal" form after Save.
		static void PostWriteGameState(GameState& gameState, const GameStateSaveInfo& saveInfo);
		/// @brief Checks if the game state has at least one structure that has been hijacked by
		/// the manager, which requires normalizing.
		static bool GameStateNeedsHandling(GameState& gameState);

		/// @brief Writes the restored rooms DB to permanent storage.
		static bool WriteSave(const GameState& gameState, const GameStateSaveInfo& saveInfo, bool isRerun);
		/// @brief Reads the restore rooms DB for the save slot, if it hasn't been loaded yet, then
		/// restores the game state to it's "normal" form.
		static bool ReadSave(GameState& gameState, const GameStateSaveInfo& saveInfo, bool isRerun);
		/// @brief Deletes the restored rooms DB file, also clears the DB itself.
		static void DeleteSave(const GameStateSaveInfo& saveInfo, bool isRerun);

        /// @brief Loads the vanilla set if needed. Returns the set index the added rooms start at.
        static size_t BeginAddRooms(VirtualRoomSet& virtualSet);
        /// @brief Adds a copy of the room to the room holder and the set.
        static RoomConfig_Room* AddRoom(VirtualRoomSet& virtualSet, const RoomConfig_Room& room);
        /// @brief Commits the rooms added since BeginAddRooms to the vanilla RoomConfig system.
        static void EndAddRooms(VirtualRoomSet& virtualSet, size_t begin);
        /// @brief Adds the rooms stored in the .stb to the specified set.
        /// @param fileName is expected to correspond to file(s) found @ `<mod root>/content/rooms/<filename>`
        /// @return the set index the added rooms start at; they run to the end of the set.
        static size_t AddStbRooms(VirtualRoomSet& virtualSet, const std::string& fileName);
        /// @brief Adds the rooms stored in the .stb to the specified set.
        /// @param fileName is expected to correspond to file(s) found @ `<mod root>/content/rooms/<filename>`
        static void AddStbRooms(const uint32_t stageId, const uint32_t mode, const std::string& fileName);

		// The lua functions are placed here since they are just details for the LUA API
		// rather than functionality of a VirtualRoomSet

		/// @brief Adds the rooms stored in the passed table to the specified set.
		/// Places the return table on the Lua stack.
		/// @return number of return Lua values the function has pushed on the stack.
		static int Lua_AddLuaRooms(lua_State* L, VirtualRoomSet& virtualSet, int tableIdx);
		/// @brief Adds the rooms stored in the .stb to the specified set.
		/// Places the return table on the Lua stack.
		/// @param fileName is expected to correspond to file(s) found @ `<mod root>/content/rooms/<filename>`
		/// @return number of return Lua values the function has pushed on the stack.
		static int Lua_AddStbRooms(lua_State* L, VirtualRoomSet& virtualSet, const std::string& fileName);

        static size_t GetId(const VirtualRoomSet& virtualSet);
	};

public:
	/// @brief Gets the roomset extension corresponding to the given stage ID and mode (vanilla or custom).
	static VirtualRoomSet GetSet(uint32_t stageId, int mode);
    /// @brief Gets the roomset extension corresponding to the given id.
    static VirtualRoomSet GetSetById(size_t id);
    /// @brief Ensures that the virtual roomset for the given stage id is initialized.
    static void TryInitializeSet(uint32_t stageId);
    /// @brief Wrapper for RoomConfig::GetRoomByStageTypeAndVariant that ignores the current custom stage override.
    static RoomConfig_Room* VirtualRoomSetManager::GetRoomByStageTypeAndVariant_IgnoreStageOverride(uint32_t stage, uint32_t type, uint32_t variant, int mode);
    /// @brief Wrapper for RoomConfig::GetRandomRoom that ignores the current custom stage override.
    static RoomConfig_Room* VirtualRoomSetManager::GetRandomRoom_IgnoreStageOverride(uint32_t seed, bool reduceWeight, int stage, int roomType, int roomShape, uint32_t minVariant, int maxVariant, int minDifficulty, int maxDifficulty, uint32_t* requiredDoors, uint32_t roomSubtype, int mode);
};

class VirtualRoomSet
{
private:
	friend class VirtualRoomSetManager;

    using _VirtualRoomSet = VirtualRoomSetManager::detail::_VirtualRoomSet;
public:
    class ScopedRoomLoader;
	using Iterator = _VirtualRoomSet::const_iterator;

private:
	const size_t m_id;

private:
	VirtualRoomSet(size_t id);
    _VirtualRoomSet& get_data();
public:
	RoomConfig_Room* operator[](size_t index);
	size_t size();
	Iterator begin();
	Iterator end();
	/// @brief Adds the rooms that match the given filter to the buffer
	void GetRooms(std::vector<RoomConfig_Room*>buffer, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType);
	void ResetRoomWeights();
    ScopedRoomLoader LoadRooms();
};

class VirtualRoomSet::ScopedRoomLoader
{
    friend class VirtualRoomSet;
private:
    VirtualRoomSet m_roomSet;
    size_t m_begin = 0;

private:
    ScopedRoomLoader(VirtualRoomSet& roomSet);
public:
    ScopedRoomLoader(const ScopedRoomLoader&) = delete;
    ScopedRoomLoader& operator=(const ScopedRoomLoader&) = delete;
    ScopedRoomLoader(ScopedRoomLoader&& o) = delete;
    ScopedRoomLoader& operator=(ScopedRoomLoader&&) = delete;

    ~ScopedRoomLoader();
    /// @brief Adds the room to the set.
    void AddRoom(const RoomConfig_Room& room);
    /// @brief Adds the rooms to the set
    void AddRooms(REPENTOGON::Array<RoomConfig_Room> rooms);
};

inline VirtualRoomSet::VirtualRoomSet(size_t id)
	: m_id(id)
{}

inline VirtualRoomSet::ScopedRoomLoader::ScopedRoomLoader(VirtualRoomSet& roomSet)
    : m_roomSet(roomSet)
{
    this->m_begin = VirtualRoomSetManager::detail::BeginAddRooms(roomSet);
}

inline VirtualRoomSet::ScopedRoomLoader::~ScopedRoomLoader()
{
    VirtualRoomSetManager::detail::EndAddRooms(this->m_roomSet, this->m_begin);
}

inline void VirtualRoomSet::ScopedRoomLoader::AddRoom(const RoomConfig_Room& room)
{
    VirtualRoomSetManager::detail::AddRoom(this->m_roomSet, room);
}

inline void VirtualRoomSetManager::detail::AddStbRooms(const uint32_t stageId, const uint32_t mode, const std::string& fileName)
{
    VirtualRoomSet virtualSet = VirtualRoomSetManager::GetSet(stageId, mode);
    VirtualRoomSetManager::detail::AddStbRooms(virtualSet, fileName);
}