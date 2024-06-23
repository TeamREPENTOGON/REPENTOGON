#include "PocketItems.h"

#include "ASMPatches.h"
#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"

// ----------------------------------------------------------------------------------------------------
// -- CALLBACKS
// ----------------------------------------------------------------------------------------------------

// MC_PRE_PLAYER_ADD_CARD (1350)
// MC_PRE_PLAYER_ADD_PILL (1352)
std::optional<int> RunPreAddPocketItemCallback(Entity_Player* player, const uint32_t pocketItemType, const int id, const int slot) {
	if (id > 0 && (pocketItemType == 0 || pocketItemType == 1)) {
		const int precallbackid = pocketItemType == 1 ? 1350 : 1352;

		if (CallbackState.test(precallbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults results = lua::LuaCaller(L).push(precallbackid)
				.push(id)
				.push(player, lua::Metatables::ENTITY_PLAYER)
				.push(id)
				.push(slot)
				.call(1);

			if (!results) {
				if (lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == false) {
					// Cancel adding the pocket item.
					return std::nullopt;
				}
				else if (lua_isinteger(L, -1)) {
					const int newid = (const int)lua_tointeger(L, -1);
					if (newid > 0) {
						return newid;
					}
				}
			}
		}
	}
	return id;
}

// MC_POST_PLAYER_ADD_CARD (1351)
// MC_POST_PLAYER_ADD_PILL (1353)
void RunPostAddPocketItemCallback(Entity_Player* player, const uint32_t pocketItemType, const int id, const int slot) {
	if (id > 0 && (pocketItemType == 0 || pocketItemType == 1)) {
		const int postcallbackid = pocketItemType == 1 ? 1351 : 1353;

		if (CallbackState.test(postcallbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaCaller(L).push(postcallbackid)
				.push(id)
				.push(player, lua::Metatables::ENTITY_PLAYER)
				.push(id)
				.push(slot)
				.call(1);
		}
	}
}

// MC_POST_PLAYER_REMOVE_CARD (1354)
// MC_POST_PLAYER_REMOVE_PILL (1355)
void RunPostRemovePocketItemCallback(Entity_Player* player, const uint32_t pocketItemType, const int id, const int slot) {
	if (id > 0 && (pocketItemType == 0 || pocketItemType == 1)) {
		const int callbackid = pocketItemType == 1 ? 1354 : 1355;

		if (CallbackState.test(callbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaCaller(L).push(callbackid)
				.push(id)
				.push(player, lua::Metatables::ENTITY_PLAYER)
				.push(id)
				.push(slot)
				.call(1);
		}
	}
}

// MC_PRE_PLAYER_COLLECT_CARD (1356)
// MC_PRE_PLAYER_COLLECT_PILL (1358)
// This is called from patches where pickup will always be a valid card or pill.
bool __stdcall RunPreCollectPocketItemCallback(Entity_Pickup* pickup, Entity_Player* player) {
	const bool isCard = pickup->_variant == 300;

	const int callbackid = isCard ? 1356 : 1358;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackid)
			.push(pickup->_subtype)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.call(1);

		if (!results && lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == false) {
			// Cancel the collection.
			return true;
		}
	}

	if (pickup->_type != 5 || (!isCard && pickup->_variant != 70) || (isCard && pickup->_variant != 300) || pickup->_subtype == 0) {
		// Verify that the entity is still a valid card/pill.
		return true;
	}

	player->AddPocketItem(pickup->_subtype, isCard ? 1 : 0);  // Use pickup->_subtype directly in case it was changed during the callback.
	return false;
}

// MC_POST_PLAYER_COLLECT_CARD (1357)
// MC_POST_PLAYER_COLLECT_PILL (1359)
void __stdcall RunPostCollectPocketItemCallback(Entity_Pickup* pickup, Entity_Player* player) {
	if (pickup->_type == 5 && (pickup->_variant == 300 || pickup->_variant == 70)) {
		const int callbackid = pickup->_variant == 300 ? 1357 : 1359;
		if (CallbackState.test(callbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaCaller(L).push(callbackid)
				.push(pickup->_subtype)
				.push(player, lua::Metatables::ENTITY_PLAYER)
				.push(pickup, lua::Metatables::ENTITY_PICKUP)
				.call(1);
		}
	}
}

// MC_POST_PLAYER_DROP_CARD (1360)
// MC_POST_PLAYER_DROP_PILL (1361)
void RunPostDropPocketItemCallback(Entity_Player* player, const uint32_t pocketItemType, const int id, const int slot, Entity_Pickup* pickup) {
	if (id > 0 && (pocketItemType == 0 || pocketItemType == 1)) {
		RunPostRemovePocketItemCallback(player, pocketItemType, id, slot);  // Trigger MC_POST_PLAYER_REMOVE_CARD/PILL first.

		if (pickup != nullptr && pickup->_type == 5 && (pickup->_variant == 300 || pickup->_variant == 70)) {
			const int callbackid = pickup->_variant == 300 ? 1360 : 1361;
			if (CallbackState.test(callbackid - 1000)) {
				lua_State* L = g_LuaEngine->_state;
				lua::LuaStackProtector protector(L);
				lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

				lua::LuaCaller(L).push(callbackid)
					.push(pickup->_subtype)
					.push(player, lua::Metatables::ENTITY_PLAYER)
					.push(pickup, lua::Metatables::ENTITY_PICKUP)
					.push(slot)
					.call(1);
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------
// -- "SIMPLE" HOOKS
// ----------------------------------------------------------------------------------------------------

HOOK_METHOD(Entity_Player, AddPocketItem, (int id, unsigned int pocketItemType) -> void) {
	std::optional<int> precallbackresult = RunPreAddPocketItemCallback(this, pocketItemType, id, 0);

	if (precallbackresult) {
		id = *precallbackresult;
		super(id, pocketItemType);
		RunPostAddPocketItemCallback(this, pocketItemType, id, 0);
	}
}

HOOK_METHOD(Entity_Player, RemovePocketItem, (int slot) -> void) {
	const int id = this->GetPocketItem(slot)->_id;
	const uint32_t pocketItemType = this->GetPocketItem(slot)->_type;

	super(slot);

	RunPostRemovePocketItemCallback(this, pocketItemType, id, slot);
}

HOOK_METHOD(Entity_Player, SetCard, (int slot, int card) -> void) {
	if (slot < 0 || slot > 3) return;

	std::optional<int> precallbackresult = RunPreAddPocketItemCallback(this, 1, card, slot);

	if (!precallbackresult) return;

	card = *precallbackresult;

	const int existingID = this->GetPocketItem(slot)->_id;
	const uint32_t existingPocketItemType = this->GetPocketItem(slot)->_type;

	super(slot, card);

	RunPostRemovePocketItemCallback(this, existingPocketItemType, existingID, slot);
	RunPostAddPocketItemCallback(this, 1, card, slot);
}

HOOK_METHOD(Entity_Player, SetPill, (int slot, int pillColor) -> void) {
	if (slot < 0 || slot > 3) return;

	std::optional<int> precallbackresult = RunPreAddPocketItemCallback(this, 0, pillColor, slot);

	if (!precallbackresult) return;

	pillColor = *precallbackresult;

	const int existingID = this->GetPocketItem(slot)->_id;
	const uint32_t existingPocketItemType = this->GetPocketItem(slot)->_type;

	super(slot, pillColor);

	RunPostRemovePocketItemCallback(this, existingPocketItemType, existingID, slot);
	RunPostAddPocketItemCallback(this, 0, pillColor, slot);
}

// ----------------------------------------------------------------------------------------------------
// -- EntityPickup::HandleCollision() Patches (picking up stuff off the floor)
// ----------------------------------------------------------------------------------------------------

void PreCollectPocketItemPatch(const char* sig, void* exitAddr) {
	SigScan signature(sig);
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.Pop(ASMPatch::Registers::ECX)  // Pop the existing function inputs from the stack
		.Pop(ASMPatch::Registers::ECX)
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EDI)  // Copy the Entity_Player* to ECX where the game expects it to be afterwards.
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.Push(ASMPatch::Registers::ESI)  // Entity_Pickup*
		.AddInternalCall(RunPreCollectPocketItemCallback)
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)exitAddr)  // Jump for true, disallow pickup.
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void PostCollectPocketItemPatch(const char* sig) {
	SigScan signature(sig);
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the overridden bytes first.
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.AddBytes("\xFF\x75\x84")  // push dword ptr [EBP - 0x7C] (Entity_Pickup*)
		.AddInternalCall(RunPostCollectPocketItemCallback)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchCollectPocketItem() {
	SigScan signature("81bd????????18fcffff");
	signature.Scan();
	void* exitAddr = signature.GetAddress();

	PreCollectPocketItemPatch("e8????????68????????ff76??8bcfe8????????8b0d", exitAddr);  // Card
	PreCollectPocketItemPatch("e8????????68????????ff76??8bcfe8????????b001", exitAddr);  // Pill

	PostCollectPocketItemPatch("b0018845??e9????????6a0051");  // Card
	PostCollectPocketItemPatch("b0018845??e9????????8b46");  // Pill
}

// ----------------------------------------------------------------------------------------------------
// -- Entity_Player::DropPocketItem() Patch/Hook
// ----------------------------------------------------------------------------------------------------

// Place to store the Entity_Pickup* that gets spawned in the middle of DropPocketItem. I promise it's used carefully.
// - It is set to nullptr again at the beginning AND end of the DropPocketItem hook.
// - The ASM patch that sets this only runs when the pointer is non-null, and again, it is fetched, used and cleared before the DropPocketItem call ends.
Entity_Pickup* dropPocketItemPickup = nullptr;

void __stdcall DropPocketItemCatchSpawnedPickup(Entity_Pickup* pickup) {
	dropPocketItemPickup = pickup;
}

// Patch that catches the spawned Entity_Pickup
void PatchDropPocketItem() {
	SigScan signature("c780????????0400000083fe03");
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0xA))  // Restore the overridden bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Entity_Pickup*
		.AddInternalCall(DropPocketItemCatchSpawnedPickup)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD(Entity_Player, DropPocketItem, (int slot, Vector* pos) -> void) {
	int droppedSlot = slot;

	// If -1 was passed, find the slot that the game will try to remove something from.
	if (droppedSlot == -1) {
		droppedSlot = 0;
		while (this->GetPocketItem(droppedSlot)->_id == 0 || this->GetPocketItem(droppedSlot)->_type == 2) {
			droppedSlot++;
			if (droppedSlot > 3) {
				return;
			}
		}
	}

	// Get the info on the pocket item before the game removes it.
	const int droppedID = this->GetPocketItem(droppedSlot)->_id;
	const uint32_t droppedPocketItemType = this->GetPocketItem(droppedSlot)->_type;

	// Carefully catch the spawned pickup entity while calling super.
	dropPocketItemPickup = nullptr;
	super(slot, pos);
	Entity_Pickup* pickup = dropPocketItemPickup;
	dropPocketItemPickup = nullptr;

	RunPostDropPocketItemCallback(this, droppedPocketItemType, droppedID, droppedSlot, pickup);
}

// ----------------------------------------------------------------------------------------------------
// -- Entity_Player::TriggerCollectibleRemoved() Patches (when pocket slots are lost)
// ----------------------------------------------------------------------------------------------------

// Stores some stuff in one patch to use it in another after it normally might no longer be in memory.
// The two patches will always run in order (no funny conditional jumps between them) so this is safe I PROMISE.
Entity_Pickup* triggerCollectibleRemovedPickup = nullptr;
std::optional<uint32_t> triggerCollectibleRemovedPocketItemType = std::nullopt;
std::optional<int> triggerCollectibleRemovedPocketItemID = std::nullopt;
std::optional<int> triggerCollectibleRemovedPocketItemSlot = std::nullopt;

void __stdcall TriggerCollectibleRemovedCatchSpawnedPickup(Entity_Pickup* pickup, Entity_Player* player, const int slot) {
	triggerCollectibleRemovedPickup = pickup;
	triggerCollectibleRemovedPocketItemType = player->GetPocketItem(slot)->_type;
	triggerCollectibleRemovedPocketItemID = player->GetPocketItem(slot)->_id;
	triggerCollectibleRemovedPocketItemSlot = slot;
}

void FirstPatchTriggerCollectibleRemoved() {
	SigScan signature("8b5424??8b4424??83fa03");
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::ESP)
		.PreserveRegisters(savedRegisters)
		.AddBytes("\xFF\x72\x18")  // push dword ptr [EDX + 0x18] (slot)
		.Push(ASMPatch::Registers::EBX)  // Entity_Player*
		.Push(ASMPatch::Registers::EAX)  // Entity_Pickup*
		.AddInternalCall(TriggerCollectibleRemovedCatchSpawnedPickup)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore the overridden bytes
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall TriggerCollectibleRemovedTrampoline(Entity_Player* player) {
	// ASM patch was overtop slot 3 being cleared, so restore that.
	player->GetPocketItem(3)->_id = 0;
	player->GetPocketItem(3)->_type = 1;

	if (triggerCollectibleRemovedPocketItemType && triggerCollectibleRemovedPocketItemID && triggerCollectibleRemovedPocketItemSlot) {
		RunPostDropPocketItemCallback(player, *triggerCollectibleRemovedPocketItemType, *triggerCollectibleRemovedPocketItemID, *triggerCollectibleRemovedPocketItemSlot, triggerCollectibleRemovedPickup);
	}

	triggerCollectibleRemovedPickup = nullptr;
	triggerCollectibleRemovedPocketItemType = std::nullopt;
	triggerCollectibleRemovedPocketItemID = std::nullopt;
	triggerCollectibleRemovedPocketItemSlot = std::nullopt;
}

void SecondPatchTriggerCollectibleRemoved() {
	SigScan signature("c783????????00000000c783????????01000000897424");
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBX)  // Entity_Player*
		.AddInternalCall(TriggerCollectibleRemovedTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x14);
	sASMPatcher.PatchAt(addr, &patch);
}

// ----------------------------------------------------------------------------------------------------
// -- Entity_Player::control_pocket_item() Patch (using cards/pills)
// ----------------------------------------------------------------------------------------------------

void __stdcall ControlCardTrampoline(Entity_Player* player, const int id, const uint32_t pocketItemType) {
	RunPostRemovePocketItemCallback(player, pocketItemType, id, 0);
	player->TriggerEchoChamber(pocketItemType, id);  // Restores the call we ASM patched overtop of.
}
void PatchControlPocketItem(const char* sig, const uint32_t pocketItemType) {
	SigScan signature(sig);
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.Pop(ASMPatch::Registers::ECX)  // Pop the original inputs to TriggerEchoChamber
		.Pop(ASMPatch::Registers::ECX)
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::ESI)  // Copy the Entity_Player* back to ECX
		.PreserveRegisters(savedRegisters)
		.Push((int)pocketItemType)  // PocketItemType
		.Push(ASMPatch::Registers::EDI)  // ID
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(ControlCardTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// ----------------------------------------------------------------------------------------------------
// -- Entity_Player::TriggerDeath() Patch (Soul of Lazarus)
// ----------------------------------------------------------------------------------------------------

// Trigger MC_POST_PLAYER_REMOVE_CARD when Soul of Lazarus is consumed.
void __stdcall ConsumeSoulOfLazarusTrampoline(Entity_Player* player, const int slot) {
	// ASM patch was overtop slot 3 being cleared, so restore that.
	player->GetPocketItem(3)->_id = 0;
	player->GetPocketItem(3)->_type = 1;

	RunPostRemovePocketItemCallback(player, 1, 89, slot);
}
void PatchConsumeSoulOfLazarus() {
	SigScan signature("c783????????00000000c783????????010000008b35");
	signature.Scan();
	void* addr = signature.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDX)  // Slot
		.Push(ASMPatch::Registers::EBX)  // Entity_Player*
		.AddInternalCall(ConsumeSoulOfLazarusTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x14);
	sASMPatcher.PatchAt(addr, &patch);
}

// ----------------------------------------------------------------------------------------------------
// -- Public function to apply all patches
// ----------------------------------------------------------------------------------------------------

void ASMPatchesForAddRemovePocketItemCallbacks() {
	PatchCollectPocketItem();

	PatchDropPocketItem();

	FirstPatchTriggerCollectibleRemoved();
	SecondPatchTriggerCollectibleRemoved();

	PatchControlPocketItem("e8????????6a04578bce", 1);  // Card
	PatchControlPocketItem("e8????????6a0057ff75", 0);  // Pill

	PatchConsumeSoulOfLazarus();
}
