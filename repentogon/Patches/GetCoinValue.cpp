#include "GetCoinValue.h"

#include "ASMPatches.h"
#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"

const int vanillaCoinValues[8] = { 1, 1, 5, 10, 2, 1, 5, 1 };

// Re-implementation of GetCoinValue that returns 1 for unknown (ie, modded) subtypes instead of random huge numbers.
// Can also run our MC_PICKUP_GET_COIN_VALUE callback when desired.
int __stdcall GetCoinValueReimplementation(Entity_Pickup* pickup, const bool runCallback) {
	if (pickup->_variant != 20) return 0;

	const unsigned int subtype = pickup->_subtype;

	// MC_PICKUP_GET_COIN_VALUE
	const int callbackid = 1250;
	if (runCallback && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(subtype)
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.call(1);

		if (!result && lua_isinteger(L, -1)) {
			return (int)lua_tointeger(L, -1);
		}
	}

	if (subtype < 8) {
		return vanillaCoinValues[subtype];
	}

	XMLAttributes xmlData = XMLStuff.EntityData->GetNodesByTypeVarSub(5, 20, subtype, true);
	const std::string xmlCoinValue = xmlData["coinvalue"];

	if (isdigit(xmlCoinValue[0])) {
		return std::stoi(xmlCoinValue);
	}
	return 1;
}

HOOK_METHOD(Entity_Pickup, GetCoinValue, () -> int) {
	return GetCoinValueReimplementation(this, /*runCallback=*/true);
}

// Patches overtop a bunch of inlined(?) duplicates of EntityPickup::GetCoinValue(), so that our reimplementation is called instead.
void PatchSingleInlinedGetCoinValue(const char* sig, const ASMPatch::Registers pickupReg, const ASMPatch::Registers valueOutReg, const ASMPatch::SavedRegisters::Registers valueOutSavedReg, const int jmp, const bool runCallback) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching inlined GetCoinValue at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~valueOutSavedReg, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(runCallback)
		.Push(pickupReg)  // Pushes the Entity_Pickup
		.AddInternalCall(GetCoinValueReimplementation)
		.CopyRegister(valueOutReg, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jmp);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForGetCoinValue() {
	// Entity_NPC::ai_ultra_greed()
	// These seem to only be used for pathfinding to coins, or to determine the total value of coins in the room, and probably don't matter that much, since the pickup collision code is used to determine how many coins he actually receives.
	// Therefore, for the moment, I've decided not to run the callback here, and allow modded coins to just count as 1 penny's worth. It is niche enough that I don't think running the callback constantly is worthwhile (xml tag is still supported anyway).
	// REP+: Ultra greed doesn't freak out anymore, might not need these, but the sigs were updated anyway.
	// Commented out for now, might still want to patch so that ultra greed doesn't ignore modded coins, but not high priority.
	//PatchSingleInlinedGetCoinValue("8b48??8d41??83f80677??8b048d????????eb??33c003f8", ASMPatch::Registers::EAX, ASMPatch::Registers::EAX, ASMPatch::SavedRegisters::Registers::EAX, 0xA, /*runCallback=*/false);
	//PatchSingleInlinedGetCoinValue("8b50??8d4a", ASMPatch::Registers::EAX, ASMPatch::Registers::ECX, ASMPatch::SavedRegisters::Registers::ECX, 0x7, /*runCallback=*/false);

	// Entity_Pickup::handle_collision() - This is where the familiars and bosses will actually receive the appropriate coin value, so definately run the callback here.
	// Familiars
	PatchSingleInlinedGetCoinValue("8b56??8d4a??83f906", ASMPatch::Registers::ESI, ASMPatch::Registers::ECX, ASMPatch::SavedRegisters::Registers::ECX, 0xB, /*runCallback=*/true);
	// Ultra Greed
	PatchSingleInlinedGetCoinValue("8b48??8d41??83f80677??8b048d????????eb??33c08b8d", ASMPatch::Registers::EAX, ASMPatch::Registers::EAX, ASMPatch::SavedRegisters::Registers::EAX, 0x16, /*runCallback=*/true);
	// Bumbino
	PatchSingleInlinedGetCoinValue("8b95????????8d42", ASMPatch::Registers::ESI, ASMPatch::Registers::EAX, ASMPatch::SavedRegisters::Registers::EAX, 0x19, /*runCallback=*/true);
}
