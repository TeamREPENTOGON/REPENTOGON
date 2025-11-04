#include "EntitySaveStateManagement.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"

namespace {
    struct HijackedState {
        int intStorage7 = 0;
        short gridSpawnIdx = 0;
    };
};

static std::vector<uint32_t> s_reusableIds;
static uint32_t s_maxId = 0;
static std::vector<HijackedState> s_hijackedStates; 

constexpr short SIGNATURE = 0x5247;

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
    //KAGE::_LogMessage(0, "[ESM] Cleared ID: %u\n", id);
}

static bool IsHijacked(const EntitySaveState& data)
{
    return data.gridSpawnIdx == SIGNATURE;
}

static uint32_t GetId(const EntitySaveState& data)
{
    return data._intStorage7;
}

static uint32_t SetId(EntitySaveState& data, uint32_t id)
{
    return data._intStorage7 = id;
}

static void ClearSaveState(const EntitySaveState& saveState)
{
    assert(IsHijacked(saveState));
    uint32_t id = GetId(saveState);
    ClearId(id);
    // clear entity (either store entity id and deferr it for later or directly clear it)
}

static void ClearVector(const std::vector<EntitySaveState>& saveEntities)
{
    for (const EntitySaveState& saveState : saveEntities)
    {
        ClearSaveState(saveState);
    }
}

static void Hijack(EntitySaveState& data, uint32_t id)
{
    HijackedState& hijackedState = s_hijackedStates[id];
    hijackedState.gridSpawnIdx = data.gridSpawnIdx;
    data.gridSpawnIdx = SIGNATURE;
    hijackedState.intStorage7 = data._intStorage7;
    data._intStorage7 = id;
}

static uint32_t NewHijack(EntitySaveState& data)
{
    assert(!IsHijacked(data));
    uint32_t id = NewId();
    //KAGE::_LogMessage(0, "[ESM] New ID: %u\n", id);
    //KAGE::_LogMessage(0, "[ESM] Saved %d, %d, %d\n", data.type, data.variant, data.subtype);
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

static void check_save_state(std::vector<uintptr_t>& checks, const EntitySaveState& saveState)
{
    assert(IsHijacked(saveState)); // some entity was not hijacked
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

static void SaveEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // save entity
}

static void RestoreEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // restore entity
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
        // copy entity (either store entity id and deferr it for later or directly clear it)
    }
}

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
    ClearVector(this->_descriptor->SavedEntities);

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
    RoomDescriptor* gridRooms = game._gridRooms;
    size_t roomCount = game._nbRooms;

    for (size_t i = 0; i < roomCount; i++)
    {
        const RoomDescriptor& room = gridRooms[i];
        ClearVector(room.SavedEntities);
    }

    for (size_t i = MAX_GRID_ROOMS; i < MAX_ROOMS; i++)
    {
        if (i == eGridRooms::ROOM_LIL_PORTAL_IDX && !resetLilPortalRoom)
        {
            continue;
        }

        const RoomDescriptor& room = gridRooms[i];
        ClearVector(room.SavedEntities);
    }

    super(resetLilPortalRoom);
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
        //KAGE::_LogMessage(0, "[ESM] Smart pointer Cleared: %u\n", id);
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
    //KAGE::_LogMessage(0, "[ESM] New Flip State: %u\n", id);
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
        // if not added to list
        if (!pickup._valid)
        {
            continue;
        }

        const EntitySaveState* flipState = pickup._flipSaveState.saveState;
        if (flipState)
        {
            check_save_state(checks, *flipState);
        }
    }
}

static void check_run_stability(std::vector<uintptr_t>& checks)
{
    const Game* game = g_Game;
    const RoomDescriptor* gridRooms = game->_gridRooms;
    size_t roomCount = game->_nbRooms;

    // Check Rooms
    for (size_t i = 0; i < roomCount; i++)
    {
        const RoomDescriptor& room = gridRooms[i];
        check_save_state_vector(checks, room.SavedEntities);
    }

    for (size_t i = MAX_GRID_ROOMS; i < MAX_ROOMS; i++)
    {
        const RoomDescriptor& room = gridRooms[i];
        check_save_state_vector(checks, room.SavedEntities);
    }

    const BackwardsStageDesc* backwardsStages = game->_backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        const BackwardsStageDesc& backwardsStage = backwardsStages[i];

        for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
        {
            const RoomDescriptor& room = backwardsStage._bossRooms[j];
            check_save_state_vector(checks, room.SavedEntities);
        }

        for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
        {
            const RoomDescriptor& room = backwardsStage._treasureRooms[j];
            check_save_state_vector(checks, room.SavedEntities);
        }
    }

    const std::vector<EntitySaveState>& myosotisPickups = game->_myosotisPickups;
    check_save_state_vector(checks, myosotisPickups);

    // Check Players
    const PlayerManager& playerManager = game->_playerManager;
    const std::vector<Entity_Player*>& players =  playerManager._playerList;

    for (const Entity_Player* player: players)
    {
        if (!player)
        {
            return;
        }

        check_save_state_vector(checks, player->_movingBoxContents);

        Entity_Player* backupPlayer = player->_backupPlayer;
        if (backupPlayer)
        {
            check_save_state_vector(checks, backupPlayer->_movingBoxContents);
        }

        if (player->_hasUnlistedRestoreState)
        {
            const GameStatePlayer& unlistedState = player->_unlistedRestoreState;
            check_save_state_vector(checks, unlistedState._movingBoxContents);
        }
    }

    const Entity_Player* const* esauJrStates = playerManager._esauJrState;

    for (size_t i = 0; i < 4; i++)
    {
        const Entity_Player* player = esauJrStates[i];
        if (!player)
        {
            continue;
        }

        check_save_state_vector(checks, player->_movingBoxContents);

        Entity_Player* backupPlayer = player->_backupPlayer;
        if (backupPlayer)
        {
            check_save_state_vector(checks, backupPlayer->_movingBoxContents);
        }

        if (player->_hasUnlistedRestoreState)
        {
            const GameStatePlayer& unlistedState = player->_unlistedRestoreState;
            check_save_state_vector(checks, unlistedState._movingBoxContents);
        }
    }
}

static void check_game_state_stability(const GameState& gameState, std::vector<uintptr_t>& checks)
{
    const RoomDescriptor* rooms = gameState._rooms;
    size_t roomCount = gameState._roomCount;

    // Check Rooms
    for (size_t i = 0; i < roomCount; i++)
    {
        const RoomDescriptor& room = rooms[i];
        check_save_state_vector(checks, room.SavedEntities);
    }

    for (size_t i = MAX_GRID_ROOMS; i < MAX_ROOMS; i++)
    {
        const RoomDescriptor& room = rooms[i];
        check_save_state_vector(checks, room.SavedEntities);
    }

    const BackwardsStageDesc* backwardsStages = gameState._backwardsStages;

    for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
    {
        const BackwardsStageDesc& backwardsStage = backwardsStages[i];

        for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
        {
            const RoomDescriptor& room = backwardsStage._bossRooms[j];
            check_save_state_vector(checks, room.SavedEntities);
        }

        for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
        {
            const RoomDescriptor& room = backwardsStage._treasureRooms[j];
            check_save_state_vector(checks, room.SavedEntities);
        }
    }

    // Check Players
    const GameStatePlayer* players = gameState._players;
    size_t playerCount = gameState._playerCount;

    for (size_t i = 0; i < playerCount; i++)
    {
        const GameStatePlayer& player = players[i];
        check_save_state_vector(checks, player._movingBoxContents);
    }
    
    const GameStatePlayer* esauJrStates = gameState._esauJrStates;

    for (size_t i = 0; i < ESAU_JR_PLAYERS; i++)
    {
        const GameStatePlayer& player = esauJrStates[i];
        if (player._playerType == -1)
        {
            continue;
        }

        check_save_state_vector(checks, player._movingBoxContents);
    }
}

static void check_stability()
{
    std::vector<size_t> checks;
    checks.resize(s_maxId, 0);
    
    // will trigger a duplicate if it's still being used
    for (uint32_t reusableIdx : s_reusableIds)
    {
        checks[reusableIdx] = 1;
    }

    check_entity_list_stability(checks);
    check_run_stability(checks);

    //check_game_state_stability(g_Manager->_gamestate, checks);
    //check_game_state_stability(g_Game->_glowingHourglassStates[0]._gameState, checks);
    //check_game_state_stability(g_Game->_glowingHourglassStates[1]._gameState, checks);

    for (uintptr_t check : checks)
    {
        assert(check != 0); // one of the ids was not accounted for (Usually caused by missed Clear)
    }
}

HOOK_METHOD(Game, Update, () -> void)
{
    super();

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    check_stability();
    auto end = clock::now();

    unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    //KAGE::_LogMessage(0, "check_stability time taken: %llu\n", duration);
}

#endif