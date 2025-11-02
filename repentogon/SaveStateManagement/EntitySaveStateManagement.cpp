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

static void ClearSaveId(uint32_t id)
{
    ClearId(id);
    // clear entity
}

static void ClearVector(const std::vector<EntitySaveState>& saveEntities)
{
    for (const EntitySaveState& saveState : saveEntities)
    {
        assert(IsHijacked(saveState));
        uint32_t id = GetId(saveState);
        ClearId(id);
        // clear entity (either store entity id and deferr it for later or directly clear it)
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

static void check_entity(std::vector<bool>& checks, const EntitySaveState& saveState)
{
    assert(IsHijacked(saveState)); // some entity was not hijacked
    uint32_t id = GetId(saveState);
    assert(id < s_maxId); // this is really messed up

    assert(!checks[id]); // there is an unaccounted duplicate
    checks[id] = true;

    const EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(saveState);
    if (!flipState)
    {
        return;
    }

    assert(IsHijacked(*flipState)); // some entity was not hijacked
    id = GetId(*flipState);
    assert(id < s_maxId); // this is really messed up
    checks[id] = true; // this can be duplicate due to how the game is coded
}

static void check_save_state_vector(std::vector<bool>& checks, const std::vector<EntitySaveState>& saveStates)
{
    for (const EntitySaveState& saveState : saveStates)
    {
        check_entity(checks, saveState);
    }
}

static void SaveEntity(Entity* entity, EntitySaveState& data, uint32_t id)
{
    // save entity
}

static void RestoreEntity(Entity* entity, EntitySaveState& data, uint32_t id)
{
    // restore entity
}

static void CopyEntity(EntitySaveState& data)
{
    // copy entity
}

HOOK_METHOD(Room, save_entity, (Entity* entity, EntitySaveState* data, bool savingMinecartEntity) -> bool)
{
    assert(!IsHijacked(*data)); // these are always newly created so they should not be already hijacked
    bool saved = super(entity, data, savingMinecartEntity);

    if (saved)
    {
        uint32_t id = NewHijack(*data);
        SaveEntity(entity, *data, id);
    }

    return saved;
}

HOOK_METHOD(Room, restore_entity, (Entity* entity, EntitySaveState* data) -> void)
{
    uint32_t id = UnHijack(*data);
    super(entity, data);
    Hijack(*data, id);
    RestoreEntity(entity, *data, id);
}

HOOK_METHOD(Room, SaveState, () -> void)
{
    ClearVector(this->_descriptor->SavedEntities);
    super();
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

static void __stdcall clear_room_vector() noexcept
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
                   .AddInternalCall(clear_room_vector)
                   .RestoreRegisters(savedRegisters)
                   .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
                   .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void EntitySaveStateManagement::ApplyPatches()
{
    Patch_RoomRestoreState_ClearVector();
}

#ifndef NDEBUG

#include "chrono"

static void check_stability()
{
    std::vector<bool> checks;
    checks.resize(s_maxId, false);

    const Game* game = g_Game;
    const RoomDescriptor* gridRooms = game->_gridRooms;
    size_t roomCount = game->_nbRooms;
    
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
    
    const std::vector<EntitySaveState>& myosotisPickups = game->_myosotisPickups;
    check_save_state_vector(checks, myosotisPickups);

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
    constexpr size_t ESAU_JR_PLAYERS = 4;

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

    // TODO: Check GameState Ids

    // check reusable ids
    for (uint32_t reusableIdx : s_reusableIds)
    {
        assert(!checks[reusableIdx]); // something is using a reusable id (Usually caused by either an Incorrect Clear, or a missed Copy)
        checks[reusableIdx] = true;
    }

    for (bool check : checks)
    {
        assert(check); // one of the ids was not accounted for (Usually caused by missed Clear)
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
    KAGE::_LogMessage(0, "check_stability time taken: %llu", duration);
}

#endif