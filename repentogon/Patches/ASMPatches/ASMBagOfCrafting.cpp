#include "IsaacRepentance.h"

#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../XMLData.h"
#include "../ASMPatches.h"
#include "LuaCore.h"
#include "HookSystem.h"

bool __stdcall CanPickupBePickedUp(Entity_Pickup* pickup, const bool ignorePrice, const bool boomerang, const int meleeVariant) {
	const std::vector<int>& xmlCraftingPickups = XMLStuff.EntityData->GetBagOfCraftingPickups(pickup->_variant, pickup->_subtype);
	const bool hasCraftingPickups = !xmlCraftingPickups.empty() && xmlCraftingPickups.at(0) > 0;
	const std::string nodeValue = XMLStuff.EntityData->GetNodesByTypeVarSub(pickup->_type, pickup->_variant, pickup->_subtype, false)[boomerang ? "boomerang" : "meleecollide"];
	if ((!boomerang && meleeVariant == 4 && hasCraftingPickups) || nodeValue == "true") {
		return ignorePrice || pickup->_price == 0;
	} else if (nodeValue == "false") {
		return false;
	}
	return pickup->CanBePickedUp(ignorePrice);
}
void PatchCanPickupBePickedUp(const bool boomerang) {
	void* addr = sASMDefinitionHolder->GetDefinition(boomerang ? &AsmDefinitions::CanPickupBoomerang : &AsmDefinitions::CanPickupMelee);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(savedRegisters);
	if (boomerang) {
		patch.Push(0);
	} else {
		patch.Push(ASMPatch::Registers::ESI);  // meleeVariant
	}
	patch.Push(boomerang)
		.Push(ASMPatch::Registers::EAX)  // ignorePrice
		.Push(ASMPatch::Registers::ECX)  // Entity_Pickup*
		.AddInternalCall(CanPickupBePickedUp)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchCanPickupBePickedUpJawBone() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CanPickupJawboneInlinedStart);
	void* exitTrue = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CanPickupJawboneInlinedTrue);
	void* exitFalse = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CanPickupJawboneInlinedFalse);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(0)
		.Push(true)  // boomerang
		.Push(false)  // ignorePrice
		.Push(ASMPatch::Registers::ESI)  // Entity_Pickup*
		.AddInternalCall(CanPickupBePickedUp)
		.AddBytes("\x84\xC0")  // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, exitTrue)
		.AddRelativeJump(exitFalse);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall TryAddToBagOfCraftingTrampoline(Entity_Player* player, Entity_Pickup* pickup, int* bagOfCraftingPickups) {
	if (pickup->_price != 0 && !pickup->CanPickupShopItem(player)) {
		return false;
	}

	int8_t initialPickups[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	const std::vector<int>& xmlPickups = XMLStuff.EntityData->GetBagOfCraftingPickups(pickup->_variant, pickup->_subtype);

	if (!xmlPickups.empty()) {
		for (uint8_t i = 0; i < 8 && i < xmlPickups.size(); i++) {
			initialPickups[i] = xmlPickups[i];
		}
	} else {
		initialPickups[0] = bagOfCraftingPickups[0];
		initialPickups[1] = bagOfCraftingPickups[1];
	}

	int8_t callbackResultPickups[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bool useCallbackResult = false;

	// MC_TRY_ADD_TO_BAG_OF_CRAFTING
	const int callbackid = 1015;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(pickup->_variant)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(pickup, lua::Metatables::ENTITY_PICKUP);
		for (int i = 0; i < 8; i++) {
			if (initialPickups[i] > 0) {
				caller.push(initialPickups[i]);
			} else {
				break;
			}
		}
		lua::LuaResults result = caller.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				useCallbackResult = true;
				int tablesize = (int)lua_rawlen(L, -1);
				for (int i = 1; i <= 8 && i <= tablesize; i++) {
					int value = lua::callbacks::ToInteger(L, i);
					if (value > 0 && value < 30) {
						callbackResultPickups[i - 1] = value;
					} else {
						break;
					}
				}
			} else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1)) {
				return false;
			}
		}
	}

	memcpy(bagOfCraftingPickups, useCallbackResult ? callbackResultPickups : initialPickups, 8);  // forgive me
	return useCallbackResult ? callbackResultPickups[0] > 0 : initialPickups[0] > 0;
}

void TryAddToBagOfCraftingPatches() {
	const int8_t chosenTypeEspOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingChosenTypeEspOffset) + 0x3);
	const int8_t secondChosenTypeEspOffset = chosenTypeEspOffset + 0x4;
	const int8_t pickupEspOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingPickupEspOffset) + 0x3);
	const int8_t belialEspOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingBelialEffectEspOffset) + 0x3);

	void* mainPatchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingPostPickType);
	void* invalidPickupAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingInvalidType);
	printf("[REPENTOGON] Patching TryAddToBagOfCrafting ingredient decisions at %p and %p\n", mainPatchAddr, invalidPickupAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch mainPatch;
	mainPatch.AddBytes("\x8D\x44\x24").AddBytes(ByteBuffer().AddAny((char*)(&chosenTypeEspOffset), 0x1))  // LEA EAX, DWORD PTR [ESP+?]
		.AddBytes("\x8B\x4C\x24").AddBytes(ByteBuffer().AddAny((char*)(&pickupEspOffset), 0x1))  // MOV ECX, DWORD PTR [ESP+?]
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // eBagOfCraftingPickup* (array of 2)
		.Push(ASMPatch::Registers::ECX)  // Entity_Pickup*
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.AddInternalCall(TryAddToBagOfCraftingTrampoline)
		.AddBytes("\x84\xC0")  // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)invalidPickupAddr, 0x7))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)invalidPickupAddr + 0x7)
		.AddBytes("\x31\xC0")  // XOR EAX, EAX
		.AddBytes("\x31\xC9")  // XOR ECX, ECX
		.AddRelativeJump((char*)mainPatchAddr + 0x7);
	sASMPatcher.PatchAt(mainPatchAddr, &mainPatch);

	ASMPatch invalidPatch;
	invalidPatch.AddBytes("\xC7\x44\x24").AddBytes(ByteBuffer().AddAny((char*)(&chosenTypeEspOffset), 0x1)).AddZeroes(4)  // MOV DWORD PTR [ESP+?], 0x0
		.AddBytes("\xC7\x44\x24").AddBytes(ByteBuffer().AddAny((char*)(&secondChosenTypeEspOffset), 0x1)).AddZeroes(4)  // MOV DWORD PTR [ESP+?], 0x0
		.AddBytes("\x8B\x5C\x24").AddBytes(ByteBuffer().AddAny((char*)(&pickupEspOffset), 0x1))  // MOV EBX, DWORD PTR [ESP+?]
		.AddBytes("\xC6\x44\x24").AddBytes(ByteBuffer().AddAny((char*)(&belialEspOffset), 0x1)).AddZeroes(1)  // MOV BYTE PTR [ESP+?], 0x0
		.AddRelativeJump(mainPatchAddr);
	sASMPatcher.PatchAt(invalidPickupAddr, &invalidPatch);

	void* arrayAccessPatchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingAccessArray);
	void* arrayLengthAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryAddToBagOfCraftingArrayLength);
	printf("[REPENTOGON] Patching TryAddToBagOfCrafting array handling at %p and %p\n", arrayAccessPatchAddr, arrayLengthAddr);

	ASMPatch arrayAccessPatch;
	arrayAccessPatch.AddBytes("\x0F\xB6\x54\x0C\x10")  // MOVZX EDX, BYTE PTR [ESP+ECX+0x10]
		.AddBytes("\x85\xD2")  // TEST EDX, EDX
		.AddRelativeJump((char*)arrayAccessPatchAddr + 0x6);
	sASMPatcher.PatchAt(arrayAccessPatchAddr, &arrayAccessPatch);

	sASMPatcher.FlatPatch((char*)arrayLengthAddr + 0x2, "\x08", 1);
}

// MC_POST_ADD_TO_BAG_OF_CRAFTING
HOOK_METHOD(Entity_Player, TryAddToBagOfCrafting, (Entity_Pickup* pickup) -> bool) {
	if (!pickup) {
		return false;
	}
	const bool added = super(pickup);
	if (added) {
		const int callbackid = 1016;
		if (CallbackState.test(callbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaCaller(L).push(callbackid)
				.push(pickup->_variant)
				.push(this, lua::Metatables::ENTITY_PLAYER)
				.push(pickup, lua::Metatables::ENTITY_PICKUP)
				.call(1);
		}
	}
	return added;
}

void ASMPatchesForBagOfCrafting() {
	TryAddToBagOfCraftingPatches();
	PatchCanPickupBePickedUp(true);
	PatchCanPickupBePickedUp(false);
	PatchCanPickupBePickedUpJawBone();
}
