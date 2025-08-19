#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"

const char* __stdcall PrintGFXPath(const char* gfxPath, PlayerHUD* hud, int slot ) {
	printf("printing it %d %s \n", slot, gfxPath);

	const int collectibleID = hud->_activeItem[slot].id;

	static std::string staticCustomGfxPath;
	
	std::string customGfxPath = XMLStuff.ItemData->GetCustomActiveGFX(collectibleID);

	if (!customGfxPath.empty()) {
		staticCustomGfxPath = customGfxPath;

		return staticCustomGfxPath.c_str();
	}

	return gfxPath;
}

void PatchGFXPath() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SetActiveItemGFXPath);

	printf("[REPENTOGON] Patching HUD::PlayerHUD::Update for active item gfx path at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, -0x1024) //push activeSlot idx
		.Push(ASMPatch::Registers::EBP, -0x1028) //push PlayerHUD
		.Push(ASMPatch::Registers::EAX) //push original filepath to Active item sprite
		.AddInternalCall(PrintGFXPath)
		//.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForCustomActiveGFX() {
	PatchGFXPath();
}