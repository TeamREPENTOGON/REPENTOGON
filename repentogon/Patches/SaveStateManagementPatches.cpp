#include "SaveStateManagementPatches.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"

constexpr size_t NUM_ESAU_JR_STATES = 4;

static void __stdcall myosotis_copy(size_t mirrorRoomIdx, EntitySaveState* entryPtr) noexcept
{
    mirrorRoomIdx = mirrorRoomIdx - 1;
    assert(0 <= mirrorRoomIdx && mirrorRoomIdx < eGridRooms::MAX_GRID_ROOMS);
    size_t listIdx = (eGridRooms::MAX_GRID_ROOMS - 1) - mirrorRoomIdx;
    RoomDescriptor& room = g_Game->_gridRooms[listIdx];

    std::vector<EntitySaveState>& saveStates = room.SavedEntities;
    size_t entryIdx = entryPtr - saveStates.data();
    assert(0 <= entryIdx && entryIdx < saveStates.size());

    // add a copy of [listIdx][entryIdx] into myosotis buffer
}

static void Patch_LevelInit_MyosotisCopy() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Init_MyosotisCopy);
    ZHL::Log("[REPENTOGON] Patching Level::Init for SaveStateManagement at %p\n", addr);
    
    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 7;
    constexpr size_t RESTORED_BYTES = 7;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ESI) // entryPtr
        .Push(ASMPatch::Registers::EBP, -0x5C) // mirrorRoomIdx
        .AddInternalCall(myosotis_copy)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall add_new_dark_closet_collectible_entry() noexcept
{
    // room is the last placed one (this is actually how it's retrieved, pre entry add)
    size_t listIdx = g_Game->_nbRooms - 1;

    // add new save state entry in room [listIdx]
}

static void Patch_LevelGenerateDarkCloset_AddEntry() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_generate_dark_closet_AddCollectibleToRoom);
    ZHL::Log("[REPENTOGON] Patching Level::generate_dark_closet for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 7;
    constexpr size_t RESTORED_BYTES = 7;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(add_new_dark_closet_collectible_entry)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static bool __fastcall save_room_minecart_entity(Room* room, void* unusedEDX, Entity* entity, EntitySaveState* saveState, bool savingMinecartEntity) noexcept
{
    bool saved = room->save_entity(entity, saveState, savingMinecartEntity);
    if (saved)
    {
        size_t listIdx = room->_descriptor->ListIndex;
        // add entry (for minecart)
        // save entity in room [listIdx]
    }

    return saved;
}

static void Patch_RoomSaveState_SaveMinecartEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_SaveState_SaveMinecartEntity);
    ZHL::Log("[REPENTOGON] Patching Room::SaveState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(save_room_minecart_entity);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static bool __fastcall save_room_entity(Room* room, void* unusedEDX, Entity* entity, EntitySaveState* saveState, bool savingMinecartEntity) noexcept
{
    bool saved = room->save_entity(entity, saveState, savingMinecartEntity);
    if (saved)
    {
        size_t listIdx = room->_descriptor->ListIndex;
        // save entity in room [listIdx]
    }

    return saved;
}

static void Patch_RoomSaveState_SaveEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_SaveState_SaveEntity);
    ZHL::Log("[REPENTOGON] Patching Room::SaveState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(save_room_entity);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __fastcall restore_room_entity(Room* room, void* unusedEDX, Entity* entity, EntitySaveState* saveState) noexcept
{
    RoomDescriptor& currentRoom = *room->_descriptor;
    size_t listIdx = currentRoom.ListIndex;
    std::vector<EntitySaveState>& saveStates = currentRoom.SavedEntities;
    size_t entryIdx = saveState - saveStates.data();
    assert(0 <= entryIdx && entryIdx < saveStates.size());

    room->restore_entity(entity, saveState);
    // restore [listIdx][entryIdx]
}

static void Patch_RoomRestoreState_RestoreMinecartEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_RestoreState_RestoreMinecartEntity);
    ZHL::Log("[REPENTOGON] Patching Room::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_room_entity);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void Patch_RoomRestoreState_RestoreEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_RestoreState_RestoreEntity);
    ZHL::Log("[REPENTOGON] Patching Room::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_room_entity);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __fastcall restore_myosotis_entity(Room* room, void* unusedEDX, Entity* entity, EntitySaveState* saveState) noexcept
{
    RoomDescriptor& currentRoom = *room->_descriptor;
    size_t listIdx = currentRoom.ListIndex;
    std::vector<EntitySaveState>& saveStates = g_Game->_myosotisPickups;
    size_t entryIdx = saveState - saveStates.data();
    assert(0 <= entryIdx && entryIdx < saveStates.size());

    room->restore_entity(entity, saveState);
    // restore myosotis buffer [entryIdx]
}

static void Patch_RoomUpdate_RestoreMyosotisEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_RestoreMyosotisEntity);
    ZHL::Log("[REPENTOGON] Patching Room::Update for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_myosotis_entity);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __fastcall store_game_state_player(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState, bool saveTemporaryFamiliars) noexcept
{
    player->StoreGameState(saveState, saveTemporaryFamiliars);
    // store in currently saved game state at player[saveStateIdx]
}

static void Patch_PlayerManagerStoreGameState_StorePlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_StoreGameState_StorePlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    constexpr size_t GAME_STATE_PLAYER_COUNT_OFFSET = offsetof(GameState, _playerCount);
    patch.Push(ASMPatch::Registers::ESI, GAME_STATE_PLAYER_COUNT_OFFSET) // saveStateIdx
        .AddInternalCall(store_game_state_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall store_game_state_backup_player(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState, bool saveTemporaryFamiliars) noexcept
{
    player->StoreGameState(saveState, saveTemporaryFamiliars);
    // may need some additional operations depending on how we store backup players
    // store in currently saved game state at player[saveStateIdx]
}

static void Patch_PlayerManagerStoreGameState_StoreBackupPlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_StoreGameState_StoreBackupPlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    constexpr size_t GAME_STATE_PLAYER_COUNT_OFFSET = offsetof(GameState, _playerCount);
    patch.Push(ASMPatch::Registers::ESI, GAME_STATE_PLAYER_COUNT_OFFSET) // saveStateIdx
        .AddInternalCall(store_game_state_backup_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall store_game_state_esau_jr_player(Entity_Player* player, void* unusedEDX, size_t mirroredSaveStateIdx, GameStatePlayer* saveState, bool saveTemporaryFamiliars) noexcept
{
    player->StoreGameState(saveState, saveTemporaryFamiliars);
    size_t saveStateIdx = (mirroredSaveStateIdx - NUM_ESAU_JR_STATES);
    // may need some additional operations depending on how we store esau jr players
    // store in currently saved game state at esauJr[saveStateIdx]
}

static void Patch_PlayerManagerStoreGameState_StoreEsauJrState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_StoreGameState_StoreEsauJrState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    constexpr int NUM_STACK_ARGS = 2;
    patch.Push(ASMPatch::Registers::ESP, -0x14 + (NUM_STACK_ARGS * -4)) // mirroredSaveStateIdx
        .AddInternalCall(store_game_state_esau_jr_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_game_state_player(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState(saveState);
    // restore from currently saved game state at player[saveStateIdx]
}

static void Patch_PlayerManagerRestoreGameState_RestorePlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameState_RestorePlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::ESI) // saveStateIdx
        .AddInternalCall(restore_game_state_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_game_state_backup_player(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState(saveState);
    // may need some additional operations depending on how we store backup players
    // restore from currently saved game state at player[saveStateIdx]
}

static void Patch_PlayerManagerRestoreGameState_RestoreBackupPlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameState_RestoreBackupPlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBP, -0x14) // saveStateIdx
        .AddInternalCall(restore_game_state_backup_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall store_game_state_backup_player_post_restore(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState, bool saveTemporaryFamiliars) noexcept
{
    player->StoreGameState(saveState, saveTemporaryFamiliars);
    // may need some additional operations depending on how we store backup players
    // store into unlisted save state of this player
    // override familiar data with that of current saved game state at player[saveStateIdx]
}

static void Patch_PlayerManagerRestoreGameState_StoreBackupPlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameState_StoreBackupPlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBP, -0x14) // saveStateIdx
        .AddInternalCall(store_game_state_backup_player_post_restore)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_game_state_esau_jr_player(Entity_Player* player, void* unusedEDX, size_t mirroredSaveStateIdx, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState(saveState);
    size_t saveStateIdx = (mirroredSaveStateIdx - NUM_ESAU_JR_STATES);
    // may need some additional operations depending on how we store esau jr players
    // restore from currently saved game state at esauJr[saveStateIdx]
}

static void Patch_PlayerManagerRestoreGameState_RestoreEsauJrState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameState_RestoreEsauJrState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBX) // mirroredSaveStateIdx
        .AddInternalCall(restore_game_state_esau_jr_player)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_game_state_player_post_level_init(Entity_Player* player, void* unusedEDX, size_t saveStateIdx, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState_PostLevelInit(saveState);
    // restore extra data
    // setup context for restoring Familiars
}

static void Patch_PlayerManagerRestoreGameStatePostLevelInit_RestorePlayerState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameStatePostLevelInit_RestorePlayerState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState_PostLevelInit for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::ESI) // saveStateIdx
        .AddInternalCall(restore_game_state_player_post_level_init)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall store_player_unlisted_state(Entity_Player* player, void* unusedEDX, GameStatePlayer* saveState, bool saveTemporaryFamiliars) noexcept
{
    player->StoreGameState(saveState, saveTemporaryFamiliars);
    // store into unlisted state for this player
}

static void Patch_PlayerManagerReplacePlayer_StoreUnlistedState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_ReplacePlayer_StoreUnlistedState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::ReplacePlayer for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(store_player_unlisted_state);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __fastcall restore_unlisted_state(Entity_Player* player, void* unusedEDX, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState(saveState);
    // restore from unlisted state for this player
}

static void __fastcall restore_unlisted_state_post_level_init(Entity_Player* player, void* unusedEDX, GameStatePlayer* saveState) noexcept
{
    player->RestoreGameState_PostLevelInit(saveState);
    // restore from unlisted state for this player
    // setup context for familiar restore
}

static void Patch_PlayerManagerReplacePlayer_RestoreUnlistedState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_ReplacePlayer_RestoreUnlistedState);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::ReplacePlayer for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_unlisted_state);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void Patch_PlayerManagerReplacePlayer_RestoreUnlistedStatePostLevelInit() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_ReplacePlayer_RestoreUnlistedStatePostLevelInit);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::ReplacePlayer for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_unlisted_state_post_level_init);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void Patch_EntityPlayerUpdate_LazarusB_RestoreUnlistedState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_Update_LazarusB_RestoreUnlistedState);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::Update for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_unlisted_state);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void Patch_EntityPlayerUpdate_LazarusB_RestorePostLevelInitUnlistedState() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_Update_LazarusB_RestorePostLevelInitUnlistedState);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::Update for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(restore_unlisted_state_post_level_init);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static bool __fastcall save_moving_box_entity(Room* room, void* unusedEDX, Entity_Player* player, Entity* entity, EntitySaveState* saveState, bool savingMinecartEntity) noexcept
{
    bool saved = room->save_entity(entity, saveState, savingMinecartEntity);
    if (saved)
    {
        // save entity in moving box for this player
    }

    return saved;
}

static void Patch_EntityPlayerUseActiveItem_MovingBox_SaveEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_SaveEntity);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBP, -0x5c4) // player
        .AddInternalCall(save_moving_box_entity)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_moving_box_entity(Room* room, void* unusedEDX, Entity_Player* player, size_t entryIdx, Entity* entity, EntitySaveState* saveState) noexcept
{
    room->restore_entity(entity, saveState);
    // restore entity in moving box[entryIdx] for this player
}

static void Patch_EntityPlayerUseActiveItem_MovingBox_RestoreEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_RestoreEntity);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBP, -0x5cc) // entryIdx
        .Push(ASMPatch::Registers::EBP, -0x5c4) // player
        .AddInternalCall(restore_moving_box_entity)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall store_familiar(Entity_Player* player, Entity_Familiar** entityList, size_t listIdx) noexcept
{
    Entity_Familiar* familiar = entityList[listIdx];
    //store familiar within player
}

static void Patch_EntityPlayerStoreGameState_PostStoreFamiliar() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_StoreGameState_PostStoreFamiliar);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EBP, -0x28) // listIdx
        .Push(ASMPatch::Registers::EBP, -0x18) // entityList
        .Push(ASMPatch::Registers::ESI) // player
        .AddInternalCall(store_familiar)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall restore_familiar(Entity_Familiar* familiar, size_t saveIdx, FamiliarData* saveState) noexcept
{
    familiar->RestoreState(saveState);
    // based on the context given by Restore_PostLevelInit restore the familiar at saveIdx
}

static void Patch_EntityPlayerRestoreGameStatePostLevelInit_RestoreFamiliar() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_RestoreGameStatePostLevelInit_RestoreFamiliarState);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::RestoreGameState_PostLevelInit for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    constexpr int NUM_STACK_ARGS = 1;
    patch.Push(ASMPatch::Registers::ESP, 0x18 + (NUM_STACK_ARGS * -4)) // saveIdx
        .AddInternalCall(restore_familiar)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static bool __fastcall save_golden_battery(Room* room, void* unusedEDX, RoomDescriptor* roomDesc, Entity* entity, EntitySaveState* saveState, bool savingMinecartEntity) noexcept
{
    bool saved = room->save_entity(entity, saveState, savingMinecartEntity);
    if (saved)
    {
        size_t listIdx = roomDesc->ListIndex;
        // save entity in room [listIdx]
    }

    return saved;
}

static void Patch_EntityPickupHandleCollision_SaveEntity() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_handle_collision_GoldenBattery_SaveEntity);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::handle_collision for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    intptr_t resumeAddr = addr + 5;

    patch.Push(ASMPatch::Registers::EBP, -0x84) // room
        .AddInternalCall(save_golden_battery)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall change_mineshaft_room(size_t listIdx) noexcept
{
    // only clear anything related to entity contents
}

static void Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_mothers_shadow_ChangeMineshaftRoom);
    ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_mothers_shadow for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 5;
    constexpr size_t RESTORED_BYTES = 5;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EBP, -0x48) // listIdx
        .AddInternalCall(change_mineshaft_room)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void SaveStateManagement::ApplyPatches() noexcept
{
    /*
    Patch_LevelInit_MyosotisCopy();
    Patch_LevelGenerateDarkCloset_AddEntry();
    Patch_RoomSaveState_SaveMinecartEntity();
    Patch_RoomSaveState_SaveEntity();
    Patch_RoomRestoreState_RestoreMinecartEntity();
    Patch_RoomRestoreState_RestoreEntity();
    Patch_RoomUpdate_RestoreMyosotisEntity();
    Patch_PlayerManagerStoreGameState_StorePlayerState();
    Patch_PlayerManagerStoreGameState_StoreBackupPlayerState();
    Patch_PlayerManagerStoreGameState_StoreEsauJrState();
    Patch_PlayerManagerRestoreGameState_RestorePlayerState();
    Patch_PlayerManagerRestoreGameState_RestoreBackupPlayerState();
    Patch_PlayerManagerRestoreGameState_StoreBackupPlayerState();
    Patch_PlayerManagerRestoreGameState_RestoreEsauJrState();
    Patch_PlayerManagerRestoreGameStatePostLevelInit_RestorePlayerState();
    Patch_PlayerManagerReplacePlayer_StoreUnlistedState();
    Patch_PlayerManagerReplacePlayer_RestoreUnlistedState();
    Patch_PlayerManagerReplacePlayer_RestoreUnlistedStatePostLevelInit();
    Patch_EntityPlayerUpdate_LazarusB_RestoreUnlistedState();
    Patch_EntityPlayerUpdate_LazarusB_RestorePostLevelInitUnlistedState();
    Patch_EntityPlayerUseActiveItem_MovingBox_SaveEntity();
    Patch_EntityPlayerUseActiveItem_MovingBox_RestoreEntity();
    Patch_EntityPlayerStoreGameState_PostStoreFamiliar();
    Patch_EntityPlayerRestoreGameStatePostLevelInit_RestoreFamiliar();
    Patch_EntityPickupHandleCollision_SaveEntity();
    Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom();
    */
}

HOOK_METHOD(Level, Init, (bool resetLilPortalRoom) -> void)
{
    // clear myosotis buffer
    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void)
{
    // clear rooms
    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, swap_rooms, (int leftIdx, int rightIdx) -> void)
{
    super(leftIdx, rightIdx);
    // swap rooms
}

HOOK_METHOD(Level, DEBUG_goto_room, (RoomConfig_Room * roomConfig) -> void)
{
    // clear eGridRooms::ROOM_DEBUG_IDX
    super(roomConfig);
}

HOOK_METHOD(Level, InitializeGenesisRoom, () -> void)
{
    // clear eGridRooms::ROOM_GENESIS_IDX
    super();
}

HOOK_METHOD(Level, TryInitializeBlueRoom, (int currentIdx, int destinationIdx, int direction) -> bool)
{
    constexpr uint32_t FLAG_BLUE_REDIRECT = 1 << 18;
    if ((direction != -1 && currentIdx >= 0) && (g_Game->GetRoomByIdx(destinationIdx, -1)->Flags & FLAG_BLUE_REDIRECT) != 0)
    {
        // clear eGridRooms::ROOM_BLUE_ROOM_IDX
    }

    return super(currentIdx, destinationIdx, direction);
}

HOOK_METHOD(Room, SaveState, () -> void)
{
    // clear current room entity save state
    super();
}

HOOK_METHOD(Entity_Pickup, TryFlip, (Entity_Player * unusedPlayer, unsigned int unusedSeed) -> bool)
{
    EntitySaveState* flipState = this->_flipSaveState.saveState;
    bool flipOccurred = flipState && flipState->variant == PICKUP_COLLECTIBLE;

    if (flipOccurred)
    {
        // save flip state
    }

    bool result = super(unusedPlayer,  unusedSeed);

    if (flipOccurred)
    {
        // restore flip state
    }

    return result;
}

HOOK_STATIC(Entity_NPC, moms_heart_mausoleum_death, () -> void, __cdecl)
{
    constexpr size_t STATE_MAUSOLEUM_HEART_KILLED_FLAG_IDX = 46;
    constexpr size_t STATE_MAUSOLEUM_HEART_KILLED_WORD_OFFSET = offsetof(Game, _gameStateFlags) + (STATE_MAUSOLEUM_HEART_KILLED_FLAG_IDX / 32) * sizeof(uint32_t);
    constexpr uint32_t STATE_MAUSOLEUM_HEART_KILLED_FLAG = 1 << (STATE_MAUSOLEUM_HEART_KILLED_FLAG_IDX % 32);

    Game* game = g_Game;
    uint32_t bitset = *(uint32_t*)((uintptr_t)game + STATE_MAUSOLEUM_HEART_KILLED_WORD_OFFSET);

    if (bitset & STATE_MAUSOLEUM_HEART_KILLED_FLAG)
    {
        const RoomDescriptor* rooms = game->_gridRooms;
        const RoomDescriptor& lastBossRoom = rooms[game->_lastBossRoomListIdx];
        const RoomDescriptor& currentRoom = *game->_room->_descriptor;

        for (size_t i = 0; i < MAX_ROOMS; i++)
        {
            const RoomDescriptor& room = rooms[i];
            if (!room.Data || &room == &currentRoom || room.Data->Type == eRoomType::ROOM_ULTRASECRET || room.GridIndex == eGridRooms::ROOM_LIL_PORTAL_IDX)
            {
                continue;
            }

            const std::vector<EntitySaveState>& entityStates = room.SavedEntities;
            for (int i = entityStates.size(); i >= 0; i--)
            {
                const EntitySaveState& entityState = entityStates[i];
                if (entityState.type == eEntityType::ENTITY_PICKUP)
                {
                    // clear save state i
                }
            }
        }

        constexpr size_t ADDED_FLIES = 5;
        constexpr size_t ADDED_BUG_EFFECTS = 50;
        constexpr uint32_t FLAG_CLEAR = 1 << 0;

        if (lastBossRoom.Data && (lastBossRoom.Flags & FLAG_CLEAR))
        {
            // add ADDED_FLIES + ADDED_BUG_EFFECTS entries to the save state vector
        }
    }

    super();
}