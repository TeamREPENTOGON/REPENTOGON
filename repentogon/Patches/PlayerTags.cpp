#include "HookSystem.h"
#include "Log.h"
#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "SigScan.h"
#include "XMLData.h"

// Contains the code for some tag strings that can be added to the new "customtags" XML attribute in players.xml.
constexpr char ITEM_NO_METRONOME[] = "nometronome";  // Prevent items from being picked by Metronome.
constexpr char ITEM_NO_EXPANSION_PACK[] = "noexpansionpack";  // Prevent active items from being picked by Expansion Pack.

bool __stdcall PlayerTypeNoShake(int playerType) {
	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(playerType);

	if (!playerXML["noshake"].empty()) {
		std::string noShake = playerXML["noshake"];
		return noShake == "true" ? true : noShake == "false" ? false : false;
	}

	return playerType == PLAYER_BLUEBABY || playerType == PLAYER_THEFORGOTTEN || playerType == PLAYER_THESOUL || playerType == PLAYER_KEEPER || playerType == PLAYER_BLUEBABY_B || playerType == PLAYER_THEFORGOTTEN_B || playerType == PLAYER_KEEPER_B;
}

void ASMPatchNightmareSceneNoShake() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan signature("83ff0474??83ff10");
	signature.Scan();

	void* addr = signature.GetAddress();
	ZHL::Log("[REPENTOGON] Patching NightmareScene::Show for noshake tag patch at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // playerType
		.AddInternalCall(PlayerTypeNoShake)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x3F) // jump for true
		.AddRelativeJump((char*)addr + 0x23);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchBossIntroNoShake() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan signature("83f80474??83f81074");
	signature.Scan();

	void* addr = signature.GetAddress();
	ZHL::Log("[REPENTOGON] Patching RoomTransition:StartBossIntro for noshake tag patch at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // playerType
		.AddInternalCall(PlayerTypeNoShake)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x26) // jump for true
		.AddBytes("\x8B\x4D\x0C") // mov ecx, dword ptr ss : [ebp+C]
		.AddRelativeJump((char*)addr + 0x29);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPlayerNoShake() {
	ASMPatchNightmareSceneNoShake();
	ASMPatchBossIntroNoShake();
}

bool __stdcall PlayerItemNoMetronome(int itemID) {
	if (XMLStuff.ItemData->HasCustomTag(itemID, ITEM_NO_METRONOME)) {
		return false;
	}

	return itemID != COLLECTIBLE_METRONOME && itemID != COLLECTIBLE_PLAN_C && itemID != COLLECTIBLE_GLOWING_HOUR_GLASS && itemID != COLLECTIBLE_BREATH_OF_LIFE && itemID != COLLECTIBLE_CLICKER && itemID != COLLECTIBLE_R_KEY && itemID != COLLECTIBLE_URN_OF_SOULS;
}

void ASMPatchPlayerItemNoMetronome() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan patchSignature("81ffe80100000f84????????");
	patchSignature.Scan();

	void* patchAddr = patchSignature.GetAddress();
	ZHL::Log("[REPENTOGON] Patching Player::UseActiveItem for nometronome tag at %p\n", patchAddr);

	SigScan exitSignature("8bbd????????85ff0f8f");
	exitSignature.Scan();
	void* exitAddr = exitSignature.GetAddress();

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // playerType
		.AddInternalCall(PlayerItemNoMetronome)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)patchAddr + 0x42) // jump for true
		.AddRelativeJump(exitAddr);
	sASMPatcher.PatchAt(patchAddr, &patch);
}

bool __stdcall PlayerItemNoExpansionPack(int itemID) {
	if (XMLStuff.ItemData->HasCustomTag(itemID, ITEM_NO_EXPANSION_PACK)) {
		return false;
	}

	return itemID != COLLECTIBLE_D_INFINITY && itemID != COLLECTIBLE_METRONOME && itemID != COLLECTIBLE_GLOWING_HOUR_GLASS && itemID != COLLECTIBLE_ESAU_JR;
}

void ASMPatchPlayerItemNoExpansionPack() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan patchSignature("81f9e9010000");
	patchSignature.Scan();

	void* patchAddr = patchSignature.GetAddress();
	ZHL::Log("[REPENTOGON] Patching Player::TriggerActiveItemUsed for noexpansionpack tag at %p\n", patchAddr);

	SigScan exitSignature("8b45??ff45??8b55??8b88????????8b80????????2bc1c1f8023bd00f82????????8b8f????????81c1500b0000");
	exitSignature.Scan();
	void* exitAddr = exitSignature.GetAddress();

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // itemId
		.AddInternalCall(PlayerItemNoExpansionPack)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)patchAddr + 0x20) // jump for true
		.AddRelativeJump(exitAddr);
	sASMPatcher.PatchAt(patchAddr, &patch);
}

void ASMPatchesForPlayerCustomTags() {
	ASMPatchPlayerNoShake();
	ASMPatchPlayerItemNoMetronome();
	ASMPatchPlayerItemNoExpansionPack();
}
