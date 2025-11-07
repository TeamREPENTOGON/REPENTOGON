/* FOR MAINTAINERS:
*  - There is no single place in which the Level's rooms are reset or partially reset; each must be found and handled accordingly.
*  - The main operators (like operator= or the destructor) for EntitySaveState have not been used as hook points, as there are a lot of temporary states created, and we do not need to signal their creation, copy or destruction.
*  - There are places where EntitySaveStates are manually created and inserted into a EntitySaveState container, without the use of a constructor; each must be found and handled accordingly.
*  - Due to the unreliable nature of the EntitySaveState methods, manual work is required when save states are copied or cleared by the game; as such all points in which EntitySaveState is used must be found.
*     - The only exception is SmartPointers which have a single point of destruction, but their initialization and copy needs to be handled manually.
*     - Because the destructor for EntitySaveState always attempts to call an unused global Notify function, you can use references to that memory address to find places in which EntitySaveState is used.
*     - There are unhandled places in which EntitySaveState and GameStatePlayer are used, but they are all related to online play, as such they have not been handled.
*  - To make clears fast, there is no mechanism that checks if an id is potentially cleared multiple times, as such we must ensure that this never occurs.
      - We could implment this by potentially having a different marker when an EntitySaveState is cleared, so it's safer, but it wouldn't solve the problem for when there are accidental duplicates, due to missed Copies.
*  - Because of SmartPointers it is not currently possible to fully clear the id generator, as some of them might still be around when resetting, and causing incorrect clears.
*     - We might need to fully handle SmartPointers manually, if we want to allow this.
*     - Enabling this feature will make the system able to resist "memory leaks" as the clear would fully reset all ids, but it might be subject to incorrect clears so we would still need to be careful.
*  - It is assumed that every Entity Save is handled by the Room::save_entity function.
*  - It is assumed that all places in which 'isSavingMinecartEntity' in Room::save_entity is set to true, are, when successful, immediately followed by another Room::save_entity and that there are no possible interruptions to this flow.
*  - It is assumed that all Entity Restores are handled by the Room::restore_entity function, with Entity_Pickup::TryFlipState being the only exception.
*  - It is assumed that GameState::SaveState's initial state is a cleared state.
*  - It is assumed that when the GameState is read, it's in a cleared state.
*  - It is assumed that everytime a game state is cleared GameState::Clear is called.
*  - We are diverging in behavior compared to regular GameState store, when restoring grid rooms (the game copies every grid room, instead we only copy up till room count and off-grid), this shouldn't cause problems, but it's something to keep in mind.
*  - Confirm that Level::RestoreGameState does not reset the LilPortalRoom in reset_room_list, as we are performing that clear manually, and it would cause a double clear otherwise.
*  - It is assumed that Entity_Player::StoreGameState's initial state is akin to that of an initialized state or newly constructed state.
*  - It is assumed that everytime a player's state is cleared GameStatePlayer::Init is called.
*  - It is assumed that every player save and familiar save is handled by Entity_Player::StoreGameState.
*  - It is assumed that every player restore is handled by Entity_Player::RestoreGameState.
*  - It is assumed that every familiar restore is handled by Entity_Player::RestoreGameState_PostLevelInit.
*  - A single place where familiar data was modified manually outside of the designated funtion; make sure all are found and handled properly.
*     - This single occurrence happens when restoring the backup players in PlayerManager::RestoreGameState, which performs a copy of the data from the save state into the unlisted save state.
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
static void ForEachRoomSaveState(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    RoomDescriptor* roomList = game->_gridRooms;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        func(roomList[i].SavedEntities, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachBackwardsStageSaveState(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    BackwardsStageDesc* backwardsStages = game->_backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        BackwardsStageDesc& backwardsStage = backwardsStages[i];
        for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
        {
            func(backwardsStage._bossRooms[j].SavedEntities, std::forward<Args>(args)...);
        }

        for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
        {
            func(backwardsStage._treasureRooms[j].SavedEntities, std::forward<Args>(args)...);
        }
    }
}

template <typename Func, typename... Args>
static void ForEachPlayerSaveState(Func&& func, Args&&... args)
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
static void ForEachGameStateRoomSaveState(GameState& gameState, Func&& func, Args&&... args)
{
    RoomDescriptor* rooms = gameState._rooms;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        RoomDescriptor& room = rooms[i];
        func(room.SavedEntities, std::forward<Args>(args)...);
    }
}

template <typename Func, typename... Args>
static void ForEachGameStateBackwardsStageSaveState(GameState& gameState, Func&& func, Args&&... args)
{
    BackwardsStageDesc* backwardsStages = gameState._backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        BackwardsStageDesc& backwardsStage = backwardsStages[i];

        for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
        {
            RoomDescriptor& room = backwardsStage._bossRooms[j];
            func(room.SavedEntities, std::forward<Args>(args)...);
        }

        for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
        {
            RoomDescriptor& room = backwardsStage._treasureRooms[j];
            func(room.SavedEntities, std::forward<Args>(args)...);
        }
    }
}

template <typename Func, typename... Args>
static void ForEachGameStatePlayerSaveState(GameState& gameState, Func&& func, Args&&... args)
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
static void ForEachRunSaveState(Func&& func, Args&&... args)
{
    Game* game = g_Game;
    ForEachRoomSaveState(std::forward<Func>(func), std::forward<Args>(args)...);
    func(game->_myosotisPickups, std::forward<Args>(args)...);
    ForEachBackwardsStageSaveState(std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachPlayerSaveState(std::forward<Func>(func), std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
static void ForEachGameStateSaveState(GameState& gameState, Func&& func, Args&&... args)
{
    ForEachGameStateRoomSaveState(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachGameStateBackwardsStageSaveState(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
    ForEachGameStatePlayerSaveState(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
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

static void WriteGameState()
{
    Manager* manager = g_Manager;
    GameState& gameState = manager->_gamestate;

    ForEachGameStateSaveState(gameState, WriteVectorLambda);
}

HOOK_METHOD(GameState, Clear, () -> void)
{
    LogDebug("Start GameState::Clear\n");

    ForEachGameStateRoomSaveState(*this, ClearSaveVectorLambda);
    ForEachGameStateBackwardsStageSaveState(*this, ClearSaveVectorLambda);
    // player methods are handled by GameStatePlayer::Init
    super();

    LogDebug("End GameState::Clear\n");
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
    LogDebug("Start Game::SaveState\n");

    // there is no need to clear the state since SaveState always calls Clear before saving
	super(state);
    ForEachGameStateRoomSaveState(*state, CopyVectorLambda);
    ForEachGameStateBackwardsStageSaveState(*state, CopyVectorLambda);
    // players are handled in their appropriate methods, since we would be clearing twice otherwise

    LogDebug("End Game::SaveState\n");
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
    LogDebug("Start Game::RestoreState\n");

    ForEachBackwardsStageSaveState(ClearVectorLambda);
    super(state, startGame);
    // copy is in a separate patch as copying it here might cause problems due to callbacks running in the mean time.

    LogDebug("End Game::RestoreState\n");
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
    ForEachBackwardsStageSaveState(CopyVectorLambda);
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
    ForEachRoomSaveState(CopyVectorLambda);
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
    ForEachRunSaveState(check_vector_lambda, checks);
}

static void check_game_state_stability(GameState& gameState, std::vector<uintptr_t>& checks)
{
    ForEachGameStateSaveState(gameState, check_vector_lambda, checks);
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