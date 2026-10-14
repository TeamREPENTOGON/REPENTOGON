#include "StagePatches.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../CustomModManager.h"
#include "../Stages/StageManager.h"

// TODO: Should probably refactor this to centralize the custom backdrop handling outside of XMLData
extern uint32_t hookedbackdroptype;

namespace StagePatches {
namespace {

// Allow customs stages to disable the knife piece / ascent quests.
// TODO: Consider separating the secret exit stuff
HOOK_METHOD(Level, HasMirrorDimension, () -> bool) {
	return super() && !StageManager::DisableVanillaQuests();
}
HOOK_METHOD(Level, HasAbandonedMineshaft, () -> bool) {
	return super() && !StageManager::DisableVanillaQuests();
}
HOOK_METHOD(Level, HasPhotoDoor, () -> bool) {
	return super() && !StageManager::DisableVanillaQuests();
}
HOOK_METHOD(Level, generate_mirror_world, () -> void) {
	if (!StageManager::DisableVanillaQuests()) {
		super();
	}
}
HOOK_METHOD(Level, generate_mines_dungeon, () -> void) {
	if (!StageManager::DisableVanillaQuests()) {
		super();
	}
}
HOOK_METHOD(Room, TrySpawnSecretExit, (bool animate, bool force) -> bool) {
	if (!force && StageManager::DisableVanillaQuests()) {
		return false;
	}
	return super(animate, force);
}

HOOK_METHOD(Backdrop, render_black_outside_rect, (Vector* param_1, Vector* param_2) -> void) {
	if (hookedbackdroptype != 0 && XMLStuff.BackdropData->GetAttributeById(hookedbackdroptype, "noblackbg") == "true") {
		return;
	}
	super(param_1, param_2);
}

// Seemingly only used to enable the sandstorm effect
HOOK_METHOD(Level, IsAshpitStageID, () -> bool) {
	if (g_Game->GetDimension() != DIMENSION_DEATH_CERTIFICATE) {
		if (auto* stage = StageManager::GetCurrentCustomStage()) {
			return stage->HasDustStorm();
		}
	}
	return super();
}

// Use the actual stage ID of custom stages for the purposes of FXLayers
// Since custom stage IDs and the LevelStage-esque values used by vanilla will never overlap, this is fine.
void __stdcall EnableCustomStageFxLayers(FXLayers* fxlayers) {
	if (StageManager::GetCurrentOverride().HasOverride()) {
		fxlayers->_levelStage = StageManager::GetCurrentOverride().GetCustomStageID() + 4; // to counter dumb math later on in xml parsing
		fxlayers->_stageType = 0;
	}
	if (hookedbackdroptype != 0) {
		fxlayers->_backdropType = hookedbackdroptype;
	}
}
void ASMPatchFXLayersInit() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::FXLayers_Init_PreReadXml);

	printf("[REPENTOGON] Patching FXLayers::Init at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // FXLayers
		.AddInternalCall(EnableCustomStageFxLayers)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall CheckBackdropNoShading() {
	int backdropType = g_Game->_room->_backdrop.backdropId;
	if (g_Game->_room->_backdrop.configurations[backdropType].fullImage) {
		return true;
	}
	if (hookedbackdroptype != 0) {
		return XMLStuff.BackdropData->GetAttributeById(hookedbackdroptype, "noshading") == "true";
	}
	return false;
}
void ASMPatchLoadBackdropGraphicsNoShading() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LoadBackdropGraphics_NoShading);

	printf("[REPENTOGON] Patching Room::LoadbackdropGraphics at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(CheckBackdropNoShading)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RenderCustomStageIcon(Vector* pos, int index) {
	int frame = g_Manager->_nightmareScene.GetProgressBarMap()[index + 1];
	if (frame >= NUM_STB) {
		if (auto* stage = StageManager::GetStage(frame)) {
			if (ModEntry* mod = stage->GetMod()) {
				if (ModEntryEx* ex = CustomModManager::GetInstance().GetEx(mod)) {
					if (ex->_customStageIcons._loaded && ex->_customStageIcons.SetAnimation(stage->GetName().c_str(), true)) {
						ex->_customStageIcons.GetAnimationState()->Rewind();
						Vector zeroVector(0, 0);
						ex->_customStageIcons.Render(pos, &zeroVector, &zeroVector);
						return false;
					}
				}
			}
		}
	}
	return g_Manager->_nightmareScene._ProgressBarANM2._loaded;
}
void ASMPatchNightmareSceneStageIcon() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::NightmareScene_Render_StageIcon);
	int8_t posOffset = *(int8_t*)((char*)addr + 0x4);

	printf("[REPENTOGON] Patching NightmareScene::Render at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Index
		.LoadEffectiveAddress(ASMPatch::Registers::EBP, posOffset, ASMPatch::Registers::EAX).Push(ASMPatch::Registers::EAX)  // Vector*
		.AddInternalCall(RenderCustomStageIcon)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

RoomConfig_Room* __stdcall GetMirrorRoom(RoomConfig_Room* room, RNG* rng) {
	if (!room) {
		unsigned int doors = 0;
		return VirtualRoomSetManager::GetRandomRoom_IgnoreStageOverride(rng->_seed, false, STB_DOWNPOUR, ROOM_DEFAULT, NUM_ROOMSHAPES, 0, -1, 0, 0, &doors, 34, -1);
	}
	return room;
}
void ASMPatchLevelGeneratorPlaceRoomsGetMirrorRoom() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LevelGenerator_PlaceRooms_GetMirrorRoom);

	printf("[REPENTOGON] Patching LevelGenerator::PLaceRooms for mirror room at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBX)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(GetMirrorRoom)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall DisablePhotoDoor() {
	return StageManager::DisableVanillaQuests();
}
void ASMPatchGenerateDungeonHasPhotoDoor() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_GenerateDungeon_HasPhotoDoor);

	printf("[REPENTOGON] Patching Level::generate_dungeon for photo door at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore overwritten bytes
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x8)
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(DisablePhotoDoor)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall IsFloodedStage(uint32_t stageid) {
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		return stage->IsFlooded();
	}
	return stageid == STB_DOWNPOUR || stageid == STB_DROSS;
}
void ASMPatchFloodedStage() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_Flooded);

	printf("[REPENTOGON] Patching Room::Init for flooded stage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(IsFloodedStage)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x1A)
		.AddRelativeJump((char*)addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall IsRainyCustomStage() {
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		return stage->IsRainy();
	}
	return false;
}
int __stdcall IsRainyStage(uint32_t stageid) {
	return stageid == STB_DOWNPOUR || stageid == STB_DROSS || IsRainyCustomStage();
}
void ASMPatchRoomInitRain() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_InitRain_StageCheck);

	printf("[REPENTOGON] Patching Room::InitRain for rainy stage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(IsRainyStage)
		.AddBytes("\x83\xF8\x01") // cmp eax,0x1
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x14);
	sASMPatcher.PatchAt(addr, &patch);
}
void ASMPatchRoomInitRain2() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_InitRain_LightingCheck);

	printf("[REPENTOGON] Patching Room::InitRain for rainy stage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(IsRainyCustomStage)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x15)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall CheckNoBugs() {
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		return stage->NoBugs();
	}
	return false;
}
void ASMPatchRoomInitBugs() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_PreBugs);
	void* skipAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_PostBugs);
	void* planetariumCheckAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_PreBugs_CheckPlanetarium);

	printf("[REPENTOGON] Patching Room::Init for bugs at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(CheckNoBugs)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, skipAddr)
		.AddBytes(ByteBuffer().AddAny((char*)planetariumCheckAddr, 0x7))
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

int __stdcall CheckThunder(uint32_t stageid) {
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		return stage->HasThunder();
	}
	return stageid == STB_DOWNPOUR;
}
void ASMPatchRoomUpdateThunder() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_Thunder);

	printf("[REPENTOGON] Patching Room::Update for thunder at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x3))
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(CheckThunder)
		.AddBytes("\x83\xF8\x01") // cmp eax,0x1
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

enum class DripType : int8_t {
	VANILLA,
	NONE,
	WATER,
	EMBERS,
};
int8_t __stdcall GetStageDripType(bool roominit) {
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		if (roominit) {
			if (stage->GetEmberChance() > 0) {
				return (int8_t)DripType::EMBERS;
			}
			return (int8_t)DripType::NONE;
		}
		if (stage->GetDripChance() > 0 || stage->GetEmberChance() > 0) {
			RNG rng;
			rng.SetSeed(std::max(Random(), 1u), 35);
			if (rng.RandomFloat() <= stage->GetEmberChance()) {
				return (int8_t)DripType::EMBERS;
			}
			if (rng.RandomFloat() <= stage->GetDripChance()) {
				return (int8_t)DripType::WATER;
			}
		}
		return (int8_t)DripType::NONE;
	}
	return (int8_t)DripType::VANILLA;
}
int __stdcall GetNumInitialEmbers() {
	int num = 10;
	if (auto* stage = StageManager::GetCurrentCustomStage()) {
		num = (int)std::round(stage->GetEmberChance() * 200.0f);
	}
	return std::clamp(num, 1, 100);
}
void ASMPatchRoomInitEmbers() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_CheckEmbers);
	void* emberAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_DoEmbers);
	void* skipAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_SkipEmbers);

	printf("[REPENTOGON] Patching Room::Init for embers at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch::SavedRegisters savedRegisters2(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(true)
		.AddInternalCall(GetStageDripType)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x3C").AddBytes(ByteBuffer().AddInteger((int8_t)DripType::NONE))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipAddr)
		.AddBytes("\x3C").AddBytes(ByteBuffer().AddInteger((int8_t)DripType::EMBERS))
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x6)
		.PreserveRegisters(savedRegisters2)
		.AddInternalCall(GetNumInitialEmbers)
		.AddBytes("\x89\x85").AddBytes(ByteBuffer().AddAny((char*)emberAddr + 0x2, 0x4))
		.RestoreRegisters(savedRegisters2)
		.AddRelativeJump((char*)emberAddr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}
void ASMPatchRoomUpdateDrips() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_DripCheck);
	void* vanillaAddr = (char*)addr + 0x8;
	void* dripAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_DoDrip);
	void* emberAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_DoEmber);
	void* skipAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Update_EndDrips);

	printf("[REPENTOGON] Patching Room::Update for drips at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(false)
		.AddInternalCall(GetStageDripType)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x3C").AddBytes(ByteBuffer().AddInteger((int8_t)DripType::NONE))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipAddr)
		.AddBytes("\x3C").AddBytes(ByteBuffer().AddInteger((int8_t)DripType::WATER))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, dripAddr)
		.AddBytes("\x3C").AddBytes(ByteBuffer().AddInteger((int8_t)DripType::EMBERS))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, emberAddr)
		.AddBytes("\x83\xfe\x03")  // CMP ESI,0x3
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, vanillaAddr)
		.AddBytes("\x83\xfe\x04")  // CMP ESI,0x4
		.AddRelativeJump(vanillaAddr);
	sASMPatcher.PatchAt(addr, &patch);
}

/*
void ASMPatchSetStageAlt() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::NightmareScene_SetStageAlt_VoidThing);

	printf("[REPENTOGON] Patching NightmareScene::SetStageAlt at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddByte('\x90', 6));
	sASMPatcher.FlatPatch(addr, &patch, true);
}
*/

}  // namespace

void ApplyPatches() {
	ASMPatchFXLayersInit();
	ASMPatchLoadBackdropGraphicsNoShading();
	ASMPatchNightmareSceneStageIcon();
	ASMPatchLevelGeneratorPlaceRoomsGetMirrorRoom();
	ASMPatchGenerateDungeonHasPhotoDoor();
	ASMPatchFloodedStage();
	ASMPatchRoomInitRain();
	ASMPatchRoomInitRain2();
	ASMPatchRoomInitBugs();
	ASMPatchRoomUpdateThunder();
	ASMPatchRoomInitEmbers();
	ASMPatchRoomUpdateDrips();
	//ASMPatchSetStageAlt();
}

}  // namespace StagePatches
