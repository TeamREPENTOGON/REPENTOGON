#include "HookSystem.h"
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

	return playerType == 4 || playerType == 16 || playerType == 17 || playerType == 14 || playerType == 25 || playerType == 35 || playerType == 33;
}

void ASMPatchNightmareSceneNoShake() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan signature("83ff0474??83ff10");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching NightmareScene::Show for noshake tag patch at %p\n", addr);

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
	printf("[REPENTOGON] Patching UnnamedPlayerPortraitsHandler? for noshake tag patch at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // playerType
		.AddInternalCall(PlayerTypeNoShake)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x26) // jump for true
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

	return itemID != 488 && itemID != 475 && itemID != 422 && itemID != 326 && itemID != 482 && itemID != 636;
}

void ASMPatchPlayerItemNoMetronome() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan signature("81ffe80100000f84????????");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching Player::UseActiveItem for nometronome tag at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // playerType
		.AddInternalCall(PlayerItemNoMetronome)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x42) // jump for true
		.AddRelativeJump((char*)addr + 0x11E);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall PlayerItemNoExpansionPack(int itemID) {
	if (XMLStuff.ItemData->HasCustomTag(itemID, ITEM_NO_EXPANSION_PACK)) {
		return false;
	}

	return itemID != 489 && itemID != 488 && itemID != 422 && itemID != 703;
}

void ASMPatchPlayerItemNoExpansionPack() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan signature("81f9e9010000");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching Player::TriggerActiveItemUsed for noexpansionpack tag at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // itemId
		.AddInternalCall(PlayerItemNoExpansionPack)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x20) // jump for true
		.AddRelativeJump((char*)addr + 0x7b);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForPlayerCustomTags() {
	ASMPatchPlayerNoShake();
	ASMPatchPlayerItemNoMetronome();
	ASMPatchPlayerItemNoExpansionPack();
}