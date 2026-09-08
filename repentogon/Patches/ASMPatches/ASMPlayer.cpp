#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMPlayer.h"
#include "../XMLData.h"
#include "../EntityConfigEx.h"
#include "../ItemConfigEx.h"
#include "ASMDefinition.h"

thread_local CheckFamiliarStorage familiarsStorage;

void __stdcall CheckFamiliar_Internal(Entity_Familiar* familiar) {
	familiar->Update();

	if (familiarsStorage.inUse) {
		familiarsStorage.familiars.push_back(familiar);
	}

	return;
}

void ASMPatchCheckFamiliar() {
	SigScan scanner("8b06ff50??8b75"); // this is immediately before the call to Update()
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Push the familiar spawned
		.AddInternalCall(CheckFamiliar_Internal)
		.RestoreRegisters(savedRegisters)
		// this should neatly fit in the 5 bytes used to call Update, and we handle calling it there, nothing to restore
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

int marsDoubleTapWindow = 10;
void __stdcall SetMarsDoubleTapWindow() {
	marsDoubleTapWindow = repentogonOptions.marsDoubleTapWindow;
}

void ASMPatchMarsDoubleTapWindow() {
	SigScan scanner("83bf????????0a7f"); // cmp dword ptr [EDI + XXXXXXXX],0xa
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* frameWindowPtr = &marsDoubleTapWindow;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)  // Store for later
		.AddInternalCall(SetMarsDoubleTapWindow)
		.AddBytes("\xA1").AddBytes(ByteBuffer().AddAny((char*)&frameWindowPtr, 4)) // mov eax, dword ptr ds:[XXXXXXXX]
		// Gets offset of mars framecount directly from the address for update resistance
		.AddBytes("\x39\x87").AddBytes(ByteBuffer().AddAny((char*)addr+0x2, 4)) // cmp dword ptr [EDI + 0xXXXXXXXX,EAX
		.RestoreRegisters(savedRegisters)
		// This code block is otherwise so simple that I don't expect this offset to ever change
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

// When AddActiveCharge is called with a negative value for the ActiveSlot, it iterates over each valid ActiveSlot and recursively calls AddActiveCharge for that slot.
// For some reason, when it does this, it passes the function's boolean params in the wrong order. From the user's perspective, this would cause the function to
// allow overcharge despite the bool for allowing overcharge being to false. This patch puts the params in the correct order.
//
// It is worth noting that there does not appear to be any place in the game's code where -1 is passed as the ActiveSlot while any of the booleans are also set to TRUE,
// so this mistake does not cause any vanilla bugs, and likewise fixing it should not have any effects to the basegame.
void ASMPatchAddActiveCharge() {
	SigScan scanner("ff75??8bcfff75??ff75??56");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	sASMPatcher.FlatPatch((char*)addr + 0x2, "\x18", 1);
	sASMPatcher.FlatPatch((char*)addr + 0x7, "\x14", 1);
	sASMPatcher.FlatPatch((char*)addr + 0xA, "\x10", 1);
}

unsigned int __stdcall GetPlayerHurtSound(Entity_Player* player) {
	int playerType = player->GetPlayerType();
	XMLAttributes playerData = XMLStuff.PlayerData->GetNodeById(playerType);

	const unsigned int defaultHurtSound = 55; // Default hurt grunt sound. Nicalis won't ever change it... right?

	if (playerData.count("hurtsound") == 0) {
		return defaultHurtSound; 
	}

	XMLAttributes soundData = XMLStuff.SoundData->GetNodeById(playerData["hurtsound"]);

	if (soundData.count("id") == 0) {
		return defaultHurtSound;
	}
	else {
		return stoi(soundData["id"]);
	}
}

void ASMPatchPlayerHurtSound() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_HurtSoundOverride);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::ALL & ~ASMPatch::SavedRegisters::Registers::EAX, true);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // EntityPlayer*
		.AddInternalCall(GetPlayerHurtSound)
		.RestoreRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5); // Jump to next instruction (play sound)

	sASMPatcher.PatchAt(addr, &patch);

	RegisterCustomXMLAttr(XMLStuff.PlayerData, "hurtsound", XMLStuff.SoundData); //this is so the parsing for all of the attribute nes is done only once and by the xmldata structure itself in a single place
}

unsigned int __stdcall GetPlayerDeathSound(Entity_Player* player) {
	int playerType = player->GetPlayerType();
	XMLAttributes playerData = XMLStuff.PlayerData->GetNodeById(playerType);

	const unsigned int defaultDeathSound = 217; // Default death grunt sound.

	if (playerData.count("deathsound") == 0) {
		return defaultDeathSound;
	}

	XMLAttributes soundData = XMLStuff.SoundData->GetNodeById(playerData["deathsound"]); 

	if (soundData.count("id") == 0) {
		return defaultDeathSound;
	}
	else {
		return stoi(soundData["id"]);
	}
}

void ASMPatchPlayerDeathSound() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_DeathSoundOverride);

	patch.Push(ASMPatch::Registers::EDI) // EntityPlayer*
		.AddInternalCall(GetPlayerDeathSound)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5); // Jump to next instruction (play sound)

	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPlayerDeathSoundSoulOfLazarus() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_SoulOfLazarusDeathSoundOverride);

	patch.Push(ASMPatch::Registers::EBX) // EntityPlayer*
		.AddInternalCall(GetPlayerDeathSound)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5); // Jump to next instruction (play sound)

	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPlayerDeathSoundLost() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_SoulDeathSoundOverride);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::ALL & ~ASMPatch::SavedRegisters::Registers::EAX, true);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // EntityPlayer*
		.AddInternalCall(GetPlayerDeathSound)
		.RestoreRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPlayerDeathSoundAstralProjection() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_AstralProjectionDeathSoundOverride);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::ALL & ~ASMPatch::SavedRegisters::Registers::EAX, true);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // EntityPlayer*
		.AddInternalCall(GetPlayerDeathSound)
		.RestoreRegisters(savedRegisters)
		.MoveToMemory(ASMPatch::Registers::EAX, 0, ASMPatch::Registers::ECX)
		.AddRelativeJump((char*)addr + 0x8); 

	sASMPatcher.PatchAt(addr, &patch);

	RegisterCustomXMLAttr(XMLStuff.PlayerData, "deathsound", XMLStuff.SoundData); //this is so the parsing for all of the attribute nes is done only once and by the xmldata structure itself in a single place
}

bool __stdcall PlayerIsNotLostHeartType(Entity_Player* player) {
	return player->GetHealthType() != 2;
}

void ASMPatchPlayerLostSoulSkipPeePuddle() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LostSoulHeartTypeSkipSpawnPuddle);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	const int skipJumpOffset = 0xD + *(int*)((char*)addr + 0x9);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // push Player
		.AddInternalCall(PlayerIsNotLostHeartType)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + skipJumpOffset) // Skipping hearts gain
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore instructions that we overwrote
		.AddRelativeJump((char*)addr + 0x7);

	sASMPatcher.PatchAt(addr, &patch);

}

bool __stdcall ShouldAddBloodyTears(Entity_Player* player) {
	if (auto* ex = EntityConfigEx::GetPlayerEx(player->GetPlayerType())) {
		if (ex->HasBloodyTears()) {
			return true;
		}
	}
	return ItemConfigEx::HasItemWithCustomTag(player, CustomTags::BLOODY_TEARS, false)
		|| ItemConfigEx::HasEffectWithCustomTag(player, CustomTags::BLOODY_TEARS_EFFECT);
}
void ASMPatchGetTearParamsAddBloodyTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GetTearHitParams_BloodyTears);
	const int bloodyJumpOffset = *(int*)((char*)addr + 0xB) + 0xF;

	ZHL::Log("[REPENTOGON] Patching GetTearHitParams for adding bloody tears at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	patch//.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore instructions that we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(ShouldAddBloodyTears)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + bloodyJumpOffset)
		.AddRelativeJump((char*)addr + 0x2A);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall ShouldDisableBloodyTears(Entity_Player* player) {
	// The chocolate milk check was overwritten by the patch.
	return player->HasCollectible(COLLECTIBLE_CHOCOLATE_MILK, false)
		|| ItemConfigEx::HasItemWithCustomTag(player, CustomTags::DISABLE_BLOODY_TEARS, false)
		|| ItemConfigEx::HasEffectWithCustomTag(player, CustomTags::DISABLE_BLOODY_TEARS_EFFECT);
}
void ASMPatchGetTearParamsDisableBloodyTears() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GetTearHitParams_BloodyTearsDisabled);

	ZHL::Log("[REPENTOGON] Patching GetTearHitParams for overriding bloody tears at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(ShouldDisableBloodyTears)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}
