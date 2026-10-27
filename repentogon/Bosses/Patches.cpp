#include "BossManager.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"

#undef max

#pragma region Definitions

/// @brief returns whether or not the double trouble room can appear, given the current game progress
/// @param room /// the room is expected to be a DoubleTroubleBossRoom
static bool IsDoubleTroubleBossRoomUnlocked(const RoomConfig_Room& room);

/// Records the boss as defeated.
static void AddCustomBoss(PersistentGameData& persistentData, int bossID);
/// @brief resizes the boss bitsets to the proper size
static void BossPool_PostInit(BossPool& pool);
 
/// @brief Reimplementation of RestoreGameState to account for custom boss ids.
///
/// The game hardcodes the size of the removedBosses vector to NUM_BOSSES and only iterates
/// up to NUM_BOSSES, so we need to patch this behavior to account for custom bosses.
///
/// The function has been reimplemented due it's simplicity, and because it makes it
/// easier to maintain compared to using ASM patches.
static void BossPool_RestoreGameState(BossPool& pool, const GameStateBossPool& state);

/// @brief A specific variant range is used to identify DoubleTrouble boss rooms.
/// Vanilla does not fully reserve this range, so modded boss rooms can accidentally become DoubleTrouble due to variant ID conflicts.
///  
/// To fix this we simply add the entire range of values to the variant set.
/// This ensures that modded boss rooms cannot be assigned a DoubleTrouble variant by accident.
///
/// NOTE: This also prevents modded STBs from adding DoubleTrouble rooms.
/// However, the available variant range is very small, making ID conflicts common, which will push the room outside of the variant range
/// and making it behave like a normal boss room.
///
/// Since the mechanics for adding modded DoubleTrouble rooms have never been documented, it is unlikely that many mods rely on it.
/// Even so, the implementation itself is highly unreliable, so I'd consider removing this behavior low risk.
///
/// If we ever want to restore this functionality, we can just check if the originalVariant is in the DoubleTrouble range and mark the room as
/// DoubleTrouble in a more proper way.
static void block_double_trouble_variants();

/// @brief Reimplementation of GetRandomBossRoom, mainly to simplify double trouble selection.
/// @param boss negative values for special room selection
static RoomConfig_Room* GetRandomBossRoom(int boss, uint32_t seed);

/// @brief Implements double trouble room selection.
///
/// Currently still uses the old method (searching for a variant range).
/// We will definitely need to change it when implementing proper modded double trouble support.
static RoomConfig_Room* get_double_trouble_room(RNG& rng, uint32_t minVariant);
/// @brief Implements dads note room selection
static RoomConfig_Room* get_dads_note_room(RNG& rng);

#pragma endregion

#pragma region Implementation

static bool IsDoubleTroubleBossRoomUnlocked(const RoomConfig_Room& room)
{
    uint32_t bossId = room.Subtype;
    if (const auto* bossConfig = BossManager::GetBossConfig(bossId); bossConfig)
    {
        return g_Manager->GetPersistentGameData()->Unlocked(bossConfig->achievement);
    }

    return true;
}

static void block_double_trouble_variants()
{
    auto& roomSet = g_Game->GetRoomConfig()->_stages[eStbType::STB_SPECIAL_ROOMS]._rooms[0];
    auto* variantSet = roomSet.GetVariantSet(eRoomType::ROOM_BOSS);

    for (size_t i = RoomConfig_Room::BOSS_DOUBLE_TROUBLE_START; i < RoomConfig_Room::BOSS_DOUBLE_TROUBLE_END; i++)
    {
        variantSet->Add(i);
    }
}

static RoomConfig_Room* GetRandomBossRoom(int boss, uint32_t seed)
{
    RNG rng(seed, 39);

    // special cases
    if (boss < 0) return get_double_trouble_room(rng, -boss);
    
    bool dadsNoteRoom = (boss == eBossId::BOSS_MOM_MAUSOLEUM) && ((g_Game->_gameStateFlags & ((uint64_t)1 << STATE_BACKWARDS_PATH_INIT)) != 0);
    if (dadsNoteRoom) return get_dads_note_room(rng);
    
    // default logic
    RoomConfig* roomConfig = g_Game->GetRoomConfig();
    RoomConfig_Room* room = nullptr;

    int tries = 50;
    do {
        uint32_t doors = 0;
        room = roomConfig->GetRandomRoom(rng.Next(), true, STB_SPECIAL_ROOMS, ROOM_BOSS, NUM_ROOMSHAPES, 0, -1, 1, 10, &doors, boss, -1);
    } while (tries-- > 0 && room != nullptr && BossManager::IsDoubleTroubleBossRoom(*room));

    return room;
}

static RoomConfig_Room* get_double_trouble_room(RNG& rng, uint32_t minVariant)
{
    RoomConfig* roomConfig = g_Game->GetRoomConfig();
    RoomConfig_Room* room = nullptr;

    int tries = 50;
    do {
        uint32_t doors = 0;
        room = roomConfig->GetRandomRoom(rng.Next(), true, STB_SPECIAL_ROOMS, ROOM_BOSS, NUM_ROOMSHAPES, minVariant, minVariant + 49, 1, 10, &doors, -1, -1);
        if (room == nullptr) break; // no rooms in that range, it's useless to try again
    } while (tries-- > 0 && !IsDoubleTroubleBossRoomUnlocked(*room));

    return room;
}

static RoomConfig_Room* get_dads_note_room(RNG& rng)
{
    RoomConfig* roomConfig = g_Game->GetRoomConfig();
    uint32_t doors = 0;
    return roomConfig->GetRandomRoomFromOptionalStage(rng.Next(), true, g_Game->GetStageID(0), STB_MAUSOLEUM, ROOM_BOSS, NUM_ROOMSHAPES, 0, -1, 1, 10, &doors, BOSS_MOM_MAUSOLEUM, -1);
}

void AddCustomBoss(PersistentGameData& persistentData, int bossID)
{
    if (persistentData.readonly)
    {
        return;
    }

    // TODO
}

static void BossPool_PostInit(BossPool& pool)
{
    assert(pool._removedBosses.size() == NUM_BOSSES);
    assert(pool._levelBlacklist.size() == NUM_BOSSES);

    size_t numBosses = std::max((size_t)NUM_BOSSES, g_Manager->GetEntityConfig()->GetBosses()->size());
    pool._removedBosses.resize(numBosses, false);
    pool._levelBlacklist.resize(numBosses, false);
}

static void BossPool_RestoreGameState(BossPool& pool, const GameStateBossPool& state)
{
    size_t numBosses = g_Manager->GetEntityConfig()->GetBosses()->size();
    assert(state._removedBosses.size() == numBosses);

    // restore seeds
    for (size_t i = 0; i < NUM_STB; i++)
    {
        pool._pool[i]._rng = RNG(state._poolSeeds[i], 17);
    }

    // restore removed
    pool._removedBosses.resize(numBosses, false);
    pool._levelBlacklist.resize(numBosses, false);

    for (size_t i = 0; i < numBosses; i++)
    {
        pool._removedBosses[i] = state._removedBosses[i];
        pool._levelBlacklist[i] = false;
    }
}

#pragma endregion

#pragma region Patches

HOOK_METHOD(EntityConfig, LoadBosses, (char * xmlpath) -> void)
{
    super(xmlpath);
    BossManager::detail::LoadModBosses(*this);
}

HOOK_METHOD(ModManager, UpdateRooms, (int id, int mode) -> void)
{   
	bool isSpecialRoomsStb = id == eStbType::STB_SPECIAL_ROOMS && mode == 0;
	if (isSpecialRoomsStb)
	{
        block_double_trouble_variants();
	}
    
    super(id, mode);

    if (isSpecialRoomsStb)
    {
        BossManager::detail::LoadCustomBossRooms();
    }
}

HOOK_METHOD(PersistentGameData, AddBoss, (int bossID) -> void)
{
    if (bossID >= NUM_BOSSES)
    {
        AddCustomBoss(*this, bossID);
        return;
    }

    super(bossID);
}

HOOK_STATIC_PRIORITY(RoomConfig, GetRandomBossRoom, 9999, (int bossId, uint32_t seed) -> RoomConfig_Room*, __cdecl)
{
    return ::GetRandomBossRoom(bossId, seed);
}

HOOK_METHOD(BossPool, Init, (uint32_t seed) -> void)
{
    super(seed);
    BossPool_PostInit(*this);
}

HOOK_METHOD_PRIORITY(BossPool, RestoreGameState, 9999, (GameStateBossPool* state) -> void)
{
    BossPool_RestoreGameState(*this, *state);
}

static bool __stdcall asm_is_double_trouble_boss_room(const RoomConfig_Room* room)
{
    return BossManager::IsDoubleTroubleBossRoom(*room);
};

static void Patch_RoomTransitionStartBossIntro_IsDoubleTroubleBossRoom()
{
    // the original IsDoubleTroubleBossRoom is inlined and it contains multiple jumps
    // as such it's impossible to derive both destinations from jnz 
    constexpr size_t JNZ_OFFSET = 4;
    constexpr size_t JNZ_LENGTH = 6;
    constexpr size_t FINAL_JA_OFFSET = 23;
    constexpr size_t FINAL_JA_LENGTH = 6;

    uintptr_t addr = (uintptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::RoomTransition_StartBossInto_IsDoubleTroubleBossRoom);
    ZHL::Log("[REPENTOGON] Patching RoomTransition::StartBossIntro (IsDoubleTroubleBossRoom) at %p\n", (void*)addr);

    int32_t jnz_rel32; memcpy(&jnz_rel32, (void*)(addr + JNZ_OFFSET + 2), sizeof(jnz_rel32));
    uintptr_t jnzNext = addr + JNZ_OFFSET + JNZ_LENGTH;
    uintptr_t finalJaNext  = addr + FINAL_JA_OFFSET + FINAL_JA_LENGTH;

    uintptr_t trueDestination = finalJaNext; // fallthrough
    uintptr_t falseDestination = jnzNext + jnz_rel32;

    // confirm that final JA is a JA rel32 and that it points to falseDestination
    const uint8_t* ja = (const uint8_t*)(addr + FINAL_JA_OFFSET);
    int32_t ja_rel32; memcpy(&ja_rel32,  ja + 2, sizeof(ja_rel32));
    bool valid = ja[0] == 0x0F && ja[1] == 0x87 && finalJaNext + ja_rel32 == falseDestination; // the op code leads to false destination
    if (!valid)
    {
        ZHL::Log("[FATAL] checks for RoomTransition::StartBossIntro (IsDoubleTroubleBossRoom) patch failed");
        std::abort();
    }

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
    ASMPatch patch;
    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EAX) // push room
        .AddInternalCall(asm_is_double_trouble_boss_room)
        .RestoreRegisters(savedRegisters)
        .AddBytes("\x84\xC0") // TEST AL, AL
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)trueDestination)
        .AddRelativeJump((void*)falseDestination);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_NightmareSceneShow_IsDoubleTroubleBossRoom()
{
    // the original IsDoubleTroubleBossRoom is inlined and it contains multiple jumps
    // as such it's impossible to derive both destinations from jnz 
    constexpr size_t JNZ_OFFSET = 4;
    constexpr size_t JNZ_LENGTH = 2;
    constexpr size_t FINAL_JA_OFFSET = 19;
    constexpr size_t FINAL_JA_LENGTH = 2;

    uintptr_t addr = (uintptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::NightmareScene_Show_IsDoubleTroubleBossRoom);
    ZHL::Log("[REPENTOGON] Patching NightmareScene::Show (IsDoubleTroubleBossRoom) at %p\n", (void*)addr);
    
    int8_t jnz_rel8 = *(int8_t*)(addr + JNZ_OFFSET + 1);
    uintptr_t jnzNext = addr + JNZ_OFFSET + JNZ_LENGTH;
    uintptr_t finalJaNext  = addr + FINAL_JA_OFFSET + FINAL_JA_LENGTH;

    uintptr_t trueDestination = finalJaNext; // fallthrough
    uintptr_t falseDestination = jnzNext + jnz_rel8;

    // confirm that final JA is a JA rel8 and that it points to falseDestination
    const uint8_t* ja = (const uint8_t*)(addr + FINAL_JA_OFFSET);
    bool valid = ja[0] == 0x77 && finalJaNext + ja[1] == falseDestination; // the op code leads to false destination
    if (!valid)
    {
        ZHL::Log("[FATAL] checks for NightmareScene::Show (IsDoubleTroubleBossRoom) patch failed");
        std::abort();
    }

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
    ASMPatch patch;
    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EAX) // push room
        .AddInternalCall(asm_is_double_trouble_boss_room)
        .RestoreRegisters(savedRegisters)
        .AddBytes("\x84\xC0") // TEST AL, AL
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)trueDestination)
        .AddRelativeJump((void*)falseDestination);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void BossManager::detail::ApplyPatches()
{
    Patch_RoomTransitionStartBossIntro_IsDoubleTroubleBossRoom();
    Patch_NightmareSceneShow_IsDoubleTroubleBossRoom();
}

#pragma endregion