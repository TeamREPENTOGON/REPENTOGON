/* FOR MAINTAINERS:
*  - There is no single point where the Level's rooms are reset or partially reset; each case must be found and handled individually.
*  - The main operators (like operator= or the destructor) for EntitySaveState have not been used as hook points, since many temporary states are created, and we do not need to signal their creation, copy or destruction.
*  - Some places manually create and insert EntitySaveStates into a container, without using a constructor; each of these must be found and handled.
*  - Due to the unreliable nature of the EntitySaveState methods, manual work is required when save states are copied or cleared by the game. As such, all points where EntitySaveState is used must be reviewed.
*    - The only exception is SmartPointers, which have a single point of destruction, but their initialization and copy must still be handled manually.
*    - Because the EntitySaveState destructor always attempts to call an unused global Notify function, references to that function's memory address can be used to locate uses of EntitySaveState.
*    - There are unhandled cases where EntitySaveState and GameStatePlayer are used, but all of these are related to online play and have not been addressed.
*  - There is no mechanism that checks if an ID is cleared multiple times. We must ensure this never occurs.
*    - A possible improvement could involve marking cleared EntitySaveStates with a special flag to make detection safer. However, this would not prevent issues caused by duplicate, which are the main cause of multiple clears.
*  - Because of SmartPointers, it is not currently possible to fully clear the ID generator, as some references would persist after reset, causing incorrect clears.
*    - Full handling of SmartPointers would be required to safely enable full resets.
*    - Enabling this would make the system resilient to “memory leaks”, but might still allow incorrect clears.
*  - It is assumed that all Entity saves are handled by Room::save_entity().
*  - It is assumed that every time Room::save_entity() is called with 'isSavingMinecartEntity' set to true, it is immediately followed by another call to Room::save_entity(), with no interruptions.
*  - It is assumed that all Entity restores are handled by Room::restore_entity(), except for Entity_Pickup::TryFlipState.
*  - It is assumed that GameState::SaveState's initial state is cleared.
*  - It is assumed that GameState is in a cleared state when it is read.
*  - It is assumed that every time a GameState is cleared, GameState::Clear() is called.
*  - Confirm that Level::RestoreGameState does not reset the LilPortalRoom when calling reset_room_list, since we currently have to perform that clear manually.
*  - It is assumed that Entity_Player::StoreGameState's initial state is equivalent to an initialized or newly constructed state.
*  - It is assumed that every time a player's state is cleared, GameStatePlayer::Init() is called.
*  - It is assumed that all player and familiar saves are handled by Entity_Player::StoreGameState().
*  - It is assumed that all player restores are handled by Entity_Player::RestoreGameState().
*  - It is assumed that all familiar restores are handled by Entity_Player::RestoreGameState_PostLevelInit().
*  - There is one known case where familiar data is modified manually outside the designated function; ensure all such cases are identified and handled.
*    - This specific case occurs when restoring backup players in PlayerManager::RestoreGameState(), which copies data from the save state into the unlisted save state.
*  - To avoid having to iterate over non "initialized" RoomDescriptors in BackwardsStage, when resetting the game's state, we diverge from the game's behavior by actually clearing the ids.
*    - In contrast, the game keeps the vector around and simply sets the room count to 0, only properly clearing it when saving a new RoomDescriptor.
*/

#include "EntitySaveStateManagement.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include "../MiscFunctions.h"

#define DEBUG_MSG true

namespace {
    struct HijackedState {
        int intStorage7 = 0;
        short gridSpawnIdx = 0;
    };
};

#if DEBUG_MSG
    static void LogDebug(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        std::string message = REPENTOGON::StringFormat(format, args);
        va_end(args);

        KAGE::_LogMessage(0, message.c_str());
    }
#else
    static void LogDebug(const char* format, ...) {};
#endif

static std::vector<uint32_t> s_reusableIds;
static uint32_t s_maxId = 0;
static std::vector<HijackedState> s_hijackedStates; 

constexpr short HIJACK_MARKER = 0x5247;
constexpr short WRITTEN_MARKER = 0x5246;

template <typename Func, typename... Args>
static void ForEachSaveState_AllRooms(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    RoomDescriptor* roomList = game->_gridRooms;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        func(roomList[i].SavedEntities, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_BackwardsStage(BackwardsStageDesc& backwardsStage, Func&& func, Args&&... args)
{
    for (size_t i = 0; i < backwardsStage._bossRoomsCount; i++)
    {
        RoomDescriptor& room = backwardsStage._bossRooms[i];
        func(room.SavedEntities, std::forward<Args>(args)...);
    }

    for (size_t i = 0; i < backwardsStage._treasureRoomsCount; i++)
    {
        RoomDescriptor& room = backwardsStage._treasureRooms[i];
        func(room.SavedEntities, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_AllBackwardsStages(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    BackwardsStageDesc* backwardsStages = game->_backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        BackwardsStageDesc& backwardsStage = backwardsStages[i];
        ForEachSaveState_BackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_AllPlayers(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    // Player data
    const PlayerManager& playerManager = game->_playerManager;
    const auto& players = playerManager._playerList;

    auto handlePlayer = [&](const Entity_Player* player)
    {
        if (!player)
        {
            return;
        }

        func(player->_movingBoxContents, std::forward<Args>(args)...);

        if (player->_backupPlayer)
        {
            func(player->_backupPlayer->_movingBoxContents, std::forward<Args>(args)...);
        }

        if (player->_hasUnlistedRestoreState)
        {
            func(player->_unlistedRestoreState._movingBoxContents, std::forward<Args>(args)...);
        }
    };

    for (const Entity_Player* player : players)
    {
        handlePlayer(player);
    }

    for (size_t i = 0; i < 4; i++)
    {
        handlePlayer(playerManager._esauJrState[i]);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_GameState_AllRooms(GameState& gameState, Func&& func, Args&&... args)
{
    RoomDescriptor* rooms = gameState._rooms;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        RoomDescriptor& room = rooms[i];
        func(room.SavedEntities, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_GameState_AllBackwardsStages(GameState& gameState, Func&& func, Args&&... args)
{
    BackwardsStageDesc* backwardsStages = gameState._backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        BackwardsStageDesc& backwardsStage = backwardsStages[i];
        ForEachSaveState_BackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_GameState_AllPlayers(GameState& gameState, Func&& func, Args&&... args)
{
    GameStatePlayer* players = gameState._players;
    size_t playerCount = gameState._playerCount;

    for (size_t i = 0; i < playerCount; i++)
    {
        GameStatePlayer& player = players[i];
        func(player._movingBoxContents, std::forward<Args>(args)...);
    }

    GameStatePlayer* esauJrStates = gameState._esauJrStates;

    for (size_t i = 0; i < ESAU_JR_PLAYERS; i++)
    {
        GameStatePlayer& player = esauJrStates[i];
        if (player._playerType == -1)
        {
            continue;
        }

        func(player._movingBoxContents, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachSaveState_Game(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    ForEachSaveState_AllRooms(std::forward<Func>(func), std::forward<Args>(args)...);
    func(game->_myosotisPickups, std::forward<Args>(args)...);
    ForEachSaveState_AllBackwardsStages(std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachSaveState_AllPlayers(std::forward<Func>(func), std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
static void ForEachSaveState_GameState(GameState& gameState, Func&& func, Args&&... args)
{
    ForEachSaveState_GameState_AllRooms(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachSaveState_GameState_AllBackwardsStages(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachSaveState_GameState_AllPlayers(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
}

static uint32_t NewId()
{
    if (s_reusableIds.size() > 0)
    {
        uint32_t id = s_reusableIds.back();
        s_reusableIds.pop_back();
        return id;
    }

    uint32_t id = s_maxId;
    s_maxId++;
    s_hijackedStates.emplace_back();
    return id;
}

static void ClearId(uint32_t id)
{
    s_reusableIds.push_back(id);
    LogDebug("[ESM] Cleared ID: %u\n", id);
}

static bool IsHijacked(const EntitySaveState& data)
{
    return data.gridSpawnIdx == HIJACK_MARKER;
}

static bool IsSaved(const EntitySaveState& data)
{
    return data.gridSpawnIdx == WRITTEN_MARKER;
}

static uint32_t GetId(const EntitySaveState& data)
{
    return data._intStorage7;
}

static uint32_t SetId(EntitySaveState& data, uint32_t id)
{
    return data._intStorage7 = id;
}

static void Hijack(EntitySaveState& data, uint32_t id)
{
    HijackedState& hijackedState = s_hijackedStates[id];
    hijackedState.gridSpawnIdx = data.gridSpawnIdx;
    data.gridSpawnIdx = HIJACK_MARKER;
    hijackedState.intStorage7 = data._intStorage7;
    data._intStorage7 = id;
}

static uint32_t NewHijack(EntitySaveState& data)
{
    assert(!IsHijacked(data));
    uint32_t id = NewId();
    LogDebug("[ESM] New ID: %u\n", id);
    LogDebug("[ESM] Saved %d, %d, %d\n", data.type, data.variant, data.subtype);
    Hijack(data, id);
    return id;
}

static uint32_t UnHijack(EntitySaveState& data)
{
    assert(IsHijacked(data));
    uint32_t id = GetId(data);
    HijackedState& hijackedState = s_hijackedStates[id];
    data.gridSpawnIdx = hijackedState.gridSpawnIdx;
    data._intStorage7 = hijackedState.intStorage7;
    return id;
}

static uint32_t MarkAsWritten(EntitySaveState& data)
{
    assert(IsHijacked(data));
    uint32_t id = GetId(data);
    ClearId(id);
    data.gridSpawnIdx = WRITTEN_MARKER;
    return id;
}

static void check_save_state(std::vector<uintptr_t>& checks, const EntitySaveState& saveState)
{
    bool isSaved = IsSaved(saveState);
    assert(IsHijacked(saveState) || isSaved); // some entity is neither hijacked or saved
    if (isSaved)
    {
        return;
    }

    uint32_t id = GetId(saveState);
    assert(id < s_maxId); // this is really messed up

    uintptr_t checkPtr = checks[id];
    uintptr_t saveStatePtr = (uintptr_t)&saveState;
    if (checkPtr == saveStatePtr)
    {
        return;
    }

    assert(checkPtr == 0); // there is an unaccounted duplicate
    checks[id] = saveStatePtr;

    const EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(saveState);
    if (flipState)
    {
        check_save_state(checks, *flipState);
    }
}

static void check_save_state_vector(std::vector<uintptr_t>& checks, const std::vector<EntitySaveState>& saveStates)
{
    for (const EntitySaveState& saveState : saveStates)
    {
        check_save_state(checks, saveState);
    }
}

static void ClearSaveState(const EntitySaveState& saveState)
{
    assert(IsHijacked(saveState));
    uint32_t id = GetId(saveState);
    ClearId(id);
    // clear entity (either store entity id and defer it for later or directly clear it)
}

static void ClearVector(const std::vector<EntitySaveState>& saveEntities)
{
    for (const EntitySaveState& saveState : saveEntities)
    {
        ClearSaveState(saveState);
    }
}

static void ClearSaveVector(const std::vector<EntitySaveState>& saveEntities)
{
    for (const EntitySaveState& saveState : saveEntities)
    {
        if (IsHijacked(saveState))
        {
            ClearSaveState(saveState);
        }
    }
}

static void CopySaveState(EntitySaveState& saveState)
{
    assert(IsHijacked(saveState));
    uint32_t sourceId = GetId(saveState);
    uint32_t targetId = NewId();
    SetId(saveState, targetId);
    s_hijackedStates[targetId] = s_hijackedStates[sourceId];
    LogDebug("[ESM] New ID: %u\n", targetId);
    LogDebug("[ESM] Copied %d -> %d\n", sourceId, targetId);
}

static void CopyVector(std::vector<EntitySaveState>& saveEntities)
{
    for (EntitySaveState& saveState : saveEntities)
    {
        CopySaveState(saveState);
        // copy entity (either store entity id and defer it for later or directly clear it)
    }
}

static void WriteVector(std::vector<EntitySaveState>& saveEntities)
{
    for (EntitySaveState& saveState : saveEntities)
    {
        uint32_t id = MarkAsWritten(saveState);
        // serialize the entity
    }
}

static void SaveEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // save entity
}

static void RestoreEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // restore entity
}

static auto check_vector_lambda = [](const std::vector<EntitySaveState>& vec, std::vector<uintptr_t>& checks) { check_save_state_vector(checks, vec); };
static auto ClearVectorLambda = [](const std::vector<EntitySaveState>& vec) { ClearVector(vec); };
static auto ClearSaveVectorLambda = [](const std::vector<EntitySaveState>& vec) { ClearSaveVector(vec); };
static auto CopyVectorLambda = [](std::vector<EntitySaveState>& vec) { CopyVector(vec); };
static auto WriteVectorLambda = [](std::vector<EntitySaveState>& vec) { WriteVector(vec); };

static std::pair<Entity*, EntitySaveState*> s_minecartEntity = {nullptr, nullptr};

HOOK_METHOD(Room, save_entity, (Entity* entity, EntitySaveState* data, bool savingMinecartEntity) -> bool)
{
    std::pair<Entity*, EntitySaveState*> minecartEntity = s_minecartEntity;
    s_minecartEntity.first = nullptr; s_minecartEntity.second = nullptr;

    assert(!IsHijacked(*data)); // these are always newly created so they should not be already hijacked
    bool saved = super(entity, data, savingMinecartEntity);

    if (!saved)
    {
        if (minecartEntity.first)
        {
            assert(entity->_type == eEntityType::ENTITY_MINECART); // the next thing after a minecart entity save must always be a minecart
            EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*minecartEntity.second);
            if (flipState)
            {
                UnHijack(*flipState);
            }
        }

        return false;
    }

    if (savingMinecartEntity)
    {
        s_minecartEntity.first = entity;
        s_minecartEntity.second = data;
        return true;
    }

    if (minecartEntity.first)
    {
        assert(entity->_type == eEntityType::ENTITY_MINECART); // the next thing after a minecart entity save must always be a minecart
        EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*minecartEntity.second);
        if (flipState)
        {
            CopySaveState(*flipState);
        }

        uint32_t id = NewHijack(*minecartEntity.second);
        SaveEntity(*minecartEntity.first, *minecartEntity.second, id);
    }

    EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*data);
    if (flipState)
    {
        CopySaveState(*flipState);
    }

    uint32_t id = NewHijack(*data);
    SaveEntity(*entity, *data, id);
    return saved;
}

HOOK_METHOD(Room, restore_entity, (Entity* entity, EntitySaveState* data) -> void)
{
    uint32_t id = UnHijack(*data);

    super(entity, data);

    Hijack(*data, id);
    RestoreEntity(*entity, *data, id);
}

HOOK_METHOD(Room, SaveState, () -> void)
{
    RoomDescriptor* desc = this->_descriptor;
    if (desc->GridIndex != eGridRooms::ROOM_DEBUG_IDX)
    {
        ClearVector(this->_descriptor->SavedEntities);
    }

    super();
}

HOOK_METHOD(Level, Init, (bool resetLilPortalRoom) -> void)
{
    ClearVector(g_Game->_myosotisPickups);
    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void)
{
    Game& game = *g_Game;
    RoomDescriptor* roomList = game._gridRooms;
    constexpr size_t LIL_PORTAL_IDX = MAX_GRID_ROOMS + (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        if (i == LIL_PORTAL_IDX && !(resetLilPortalRoom))
        {
            continue;
        }

        const RoomDescriptor& room = roomList[i];
        ClearVector(room.SavedEntities);
    }

    super(resetLilPortalRoom);
}

HOOK_METHOD(Game, SaveBackwardsStage, (int stage) -> void)
{
    BackwardsStageDesc& backwardsStage = g_Game->_backwardsStages[stage - 1];
    ForEachSaveState_BackwardsStage(backwardsStage, ClearVectorLambda);

	super(stage);

    ForEachSaveState_BackwardsStage(backwardsStage, CopyVectorLambda);
}

// Clear backwards stage save state, even though the game simply sets room count to 0, to avoid having to iterate "uninitialized" RoomDescriptors in the BackwardsStage. 
HOOK_METHOD(Game, ResetState, () -> void)
{
    ForEachSaveState_AllBackwardsStages(ClearVectorLambda);
	super();
}

static void WriteGameState()
{
    Manager* manager = g_Manager;
    GameState& gameState = manager->_gamestate;

    ForEachSaveState_GameState(gameState, WriteVectorLambda);
}

HOOK_METHOD(GameState, Clear, () -> void)
{
    LogDebug("[ESM] Start GameState::Clear\n");

    ForEachSaveState_GameState_AllRooms(*this, ClearSaveVectorLambda);
    ForEachSaveState_GameState_AllBackwardsStages(*this, ClearSaveVectorLambda);
    // player methods are handled by GameStatePlayer::Init
    super();

    LogDebug("[ESM] End GameState::Clear\n");
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
    LogDebug("[ESM] Start Game::SaveState\n");

    // there is no need to clear the state since SaveState always calls Clear before saving
	super(state);
    ForEachSaveState_GameState_AllRooms(*state, CopyVectorLambda);
    ForEachSaveState_GameState_AllBackwardsStages(*state, CopyVectorLambda);
    // players are handled in their appropriate methods, since we would be clearing twice otherwise

    LogDebug("[ESM] End Game::SaveState\n");
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
    LogDebug("[ESM] Start Game::RestoreState\n");

    ForEachSaveState_AllBackwardsStages(ClearVectorLambda);
    super(state, startGame);
    // copy is in a separate patch as copying it here might cause problems due to callbacks running in the mean time.

    LogDebug("[ESM] End Game::RestoreState\n");
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void)
{
    // Manual clear the lil portal room
    constexpr size_t LIL_PORTAL_IDX = (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;
    ClearVector(g_Game->_negativeGridRooms[LIL_PORTAL_IDX].SavedEntities);
    super(state);
}

HOOK_METHOD(GameStatePlayer, Init, () -> void)
{
    ClearSaveVector(this->_movingBoxContents);
    super();
}

HOOK_METHOD(Entity_Player, StoreGameState, (GameStatePlayer* saveState, bool saveTemporaryFamiliars) -> void)
{
    super(saveState, saveTemporaryFamiliars);
    CopyVector(saveState->_movingBoxContents);
}

HOOK_METHOD(Entity_Player, RestoreGameState, (GameStatePlayer * saveState) -> void)
{
    ClearVector(this->_movingBoxContents);
    super(saveState);
    CopyVector(this->_movingBoxContents);
}

HOOK_METHOD(GameState, write, (GameStateIO** gameStateIO) -> bool)
{
    WriteGameState();
    return super(gameStateIO);
}

HOOK_METHOD(GameState, write_rerun, (GameStateIO** gameStateIO) -> bool)
{
    WriteGameState();
    return super(gameStateIO);
}

HOOK_METHOD(GameState, read, (GameStateIO** gameStateIO, bool isLocalRun) -> bool)
{
    // disable for now
    if (true)
    {
        return false;
    }

    return super(gameStateIO, isLocalRun);
}

HOOK_METHOD(GameState, read_rerun, (GameStateIO** gameStateIO) -> bool)
{
    // disable for now
    if (true)
    {
        return false;
    }

    return super(gameStateIO);
}

static void __stdcall restore_game_state_backwards_rooms() noexcept
{
    ForEachSaveState_AllBackwardsStages(CopyVectorLambda);
}

static void Patch_GameRestoreState_PostBackwardsStageDescRestore() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_RestoreState_PostBackwardsStageDescRestore);
    ZHL::Log("[REPENTOGON] Patching Game::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(restore_game_state_backwards_rooms)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_room_vector() noexcept
{
    RoomDescriptor& room = *g_Game->_room->_descriptor;
    ClearVector(room.SavedEntities);
}

static void Patch_RoomRestoreState_ClearVector() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_RestoreState_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Room::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_clear_room_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_copy_myosotis_pickups() noexcept
{
    CopyVector(g_Game->_myosotisPickups);
}

static void Patch_LevelInit_PostMyosotisEffect() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Init_PostMyosotisEffect);
    ZHL::Log("[REPENTOGON] Patching Level::Init for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_copy_myosotis_pickups)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall restore_level_game_state() noexcept
{
    ForEachSaveState_AllRooms(CopyVectorLambda);
}

static void Patch_LevelRestoreGameState_PreRoomLoad() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_RestoreGameState_PreRoomLoad);
    ZHL::Log("[REPENTOGON] Patching Level::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(restore_level_game_state)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_vector_pre_backwards_assign(std::vector<EntitySaveState>* saveStateVector) noexcept
{
    ClearVector(*saveStateVector);
}

static void __stdcall asm_copy_vector_post_backwards_assign(std::vector<EntitySaveState>* saveStateVector) noexcept
{
    CopyVector(*saveStateVector);
}

static void Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_place_rooms_backwards_Boss_AssignEntitySaveStateVector);
    ZHL::Log("[REPENTOGON] Patching Level::place_rooms_backwards for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 8;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 8;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ECX) // saveStateVector
        .AddInternalCall(asm_clear_vector_pre_backwards_assign)
        .RestoreRegisters(savedRegisters) 
        .Push(ASMPatch::Registers::ECX) // saveStateVector (for asm_copy_vector_post_backwards_assign)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_vector_post_backwards_assign)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_place_rooms_backwards_Treasure_AssignEntitySaveStateVector);
    ZHL::Log("[REPENTOGON] Patching Level::place_rooms_backwards for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 10;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 10;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDX) // saveStateVector
        .AddInternalCall(asm_clear_vector_pre_backwards_assign)
        .RestoreRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDX) // saveStateVector (for asm_copy_vector_post_backwards_assign)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_vector_post_backwards_assign)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_moving_box_vector(Entity_Player& player) noexcept
{
    ClearVector(player._movingBoxContents);
}

static void Patch_PlayerUseActiveItem_MovingBoxClearVector() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for  at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDI) // player
        .AddInternalCall(asm_clear_moving_box_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

// use fastcall to emulate thiscall
static void __fastcall asm_clear_smart_pointer(EntitySaveState* saveState) noexcept
{
    if (IsHijacked(*saveState))
    {
        uint32_t id = GetId(*saveState);
        ClearSaveState(*saveState);
        LogDebug("[ESM] Smart pointer Cleared: %u\n", id);
    }

    saveState->destructor();
}

static void Patch_ReferenceCount_EntitySaveStateDestructor() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::KAGE_ReferenceCount_EntitySaveState_DecrementReference_SaveStateDestructor);
    ZHL::Log("[REPENTOGON] Patching KAGE::ReferenceCount<EntitySaveState>::DecrementReference for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(asm_clear_smart_pointer);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __stdcall asm_hijack_new_flip_state(EntitySaveState& saveState) noexcept
{
    uint32_t id = NewHijack(saveState);
    LogDebug("[ESM] New Flip State: %u\n", id);
}

static void Patch_PickupInitFlipState_CreateSaveState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_InitFlipState_PostSaveStateConstructor);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::InitFlipState for SaveStateManagent at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EAX) // arg
        .AddInternalCall(asm_hijack_new_flip_state)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_flip_restore(Entity& entity, EntitySaveState& saveState) noexcept
{
    RestoreEntity(entity, saveState, GetId(saveState));
}

static void Patch_PickupTryFlip_RestoreFlipState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_TryFlip_PostRestoreFlipState);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::TryFlip for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 4;
    constexpr size_t JNZ_OP_CODE_SIZE = 2;
    constexpr intptr_t JNZ_OFFSET = 4;
    intptr_t relativeJmpAddr = addr + (intptr_t)RESTORED_BYTES + (intptr_t)JNZ_OP_CODE_SIZE + JNZ_OFFSET;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDI) // saveState
        .Push(ASMPatch::Registers::ESI) // entity
        .AddInternalCall(asm_flip_restore)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)relativeJmpAddr)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void EntitySaveStateManagement::ApplyPatches()
{
    Patch_ReferenceCount_EntitySaveStateDestructor();
    Patch_RoomRestoreState_ClearVector();
    Patch_LevelInit_PostMyosotisEffect();
    Patch_LevelRestoreGameState_PreRoomLoad();
    Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector();
    Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector();
    Patch_GameRestoreState_PostBackwardsStageDescRestore();
    Patch_PlayerUseActiveItem_MovingBoxClearVector();
    Patch_PickupInitFlipState_CreateSaveState();
    Patch_PickupTryFlip_RestoreFlipState();
}

#ifndef NDEBUG

#include "chrono"

constexpr size_t MAX_PICKUPS = 512;
constexpr size_t ESAU_JR_PLAYERS = 4;

static void check_entity_list_stability(std::vector<size_t>& checks)
{
    const EntityFactory* factory = g_Game->_entityFactory;
    const Entity_Pickup* pickups = factory->_pickup;
    
    for (size_t i = 0; i < MAX_PICKUPS; i++)
    {
        const Entity_Pickup& pickup = pickups[i];
        const EntitySaveState* flipState = pickup._flipSaveState.saveState;
        if (flipState)
        {
            check_save_state(checks, *flipState);
        }
    }
}

static void check_run_stability(std::vector<uintptr_t>& checks)
{
    ForEachSaveState_Game(check_vector_lambda, checks);
}

static void check_game_state_stability(GameState& gameState, std::vector<uintptr_t>& checks)
{
    ForEachSaveState_GameState(gameState, check_vector_lambda, checks);
}

static void check_stability()
{
    std::vector<uintptr_t> checks;
    checks.resize(s_maxId, 0);
    
    // will trigger a duplicate if it's still being used
    for (uint32_t reusableIdx : s_reusableIds)
    {
        assert(checks[reusableIdx] == 0); // An id was cleared twice
        checks[reusableIdx] = 1;
    }

    Manager* manager = g_Manager;
    Game* game = g_Game;

    check_entity_list_stability(checks);
    check_run_stability(checks);

    check_game_state_stability(manager->_gamestate, checks);
    check_game_state_stability(game->_glowingHourglassStates[0]._gameState, checks);
    check_game_state_stability(game->_glowingHourglassStates[1]._gameState, checks);

    for (uintptr_t check : checks)
    {
        assert(check != 0); // one of the ids was not accounted for (Usually caused by missed Clear)
    }
}

HOOK_STATIC(Manager, Update, (bool unk) -> void, __stdcall)
{
    super(unk);

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    check_stability();
    auto end = clock::now();

    unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    //KAGE::_LogMessage(0, "check_stability time taken: %llu\n", duration);
}

#endif