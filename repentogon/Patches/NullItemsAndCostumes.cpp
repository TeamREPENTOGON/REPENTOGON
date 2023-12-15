#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "XMLData.h"
#include <Log.h>

/*
* This file contains fixes/support for Null Items and Null Costumes, primarily via ASM patches.
* 
* 1. Fixes the game not properly loading null items from mod's XMLs in the first place.
* 2. Allows for mods to assign costumes to their null items by given them the same ID in their XMLs.
*/

// Track the latest mod to be loading XML data, same as the XMLData code.
// Equivalent to the lastmodid used in XMLData.cpp, except empty for basegame.
std::string lastModIdButCooler = "";

void UpdateLastModId(ModEntry* modentry) {
	lastModIdButCooler = "";
	if (modentry != nullptr) {
		if (std::string(modentry->GetId()).length() > 0) {
			lastModIdButCooler = modentry->GetId();
		}
		else if (modentry->GetDir().length() > 0) {
			lastModIdButCooler = modentry->GetDir();
		}
	}
}

HOOK_METHOD(ItemConfig, Load, (char* xmlpath, ModEntry* modentry)->void) {
	UpdateLastModId(modentry);
	super(xmlpath, modentry);
}

HOOK_METHOD(ItemConfig, LoadCostumes, (char* xmlpath, ModEntry* modentry)->void) {
	UpdateLastModId(modentry);
	super(xmlpath, modentry);
}

/*
* Patch that fixes Null Items loaded from modded item.xmls
*
* The game currently does not handle nullitems from modded xmls any differently than
* vanilla ones. That means it will just attempt to write the item's ItemConfig entry
* directly into the ItemConfig's nullitems std::vector, at the index of the ID specified
* in the xml (or 0, if not specified). This means that the game will either overwrite
* a vanilla nullitem at best, or write into memory outside of the bounds of the std::vector
* at worst, because the vector is only pre-sized to 132 to fit the vanilla nullitems.
*
* This patch makes it so that any nullitem loaded from a mod is assigned the next available
* id and properly pushed to the std::vector.
*/
bool __stdcall FixLoadingNullItemFromXml(ItemConfig_Item* newNullItem) {
	if (!lastModIdButCooler.empty()) {
		// We are loading a Null Item from a mod's items.xml. The game will not
		// handle it properly, so assign it the next available ID ourselves, and
		// push it to the ItemConfig's nullitems std::vector properly.
		ItemConfig* itemConfig = g_Manager->GetItemConfig();
		newNullItem->id = itemConfig->GetNullItems()->size();
		itemConfig->GetNullItems()->push_back(newNullItem);
		return true;
	}
	return false;
}

void ASMPatchFixLoadingNullItemsFromXml() {
	// This is the spot in "parse_items_xml()" where a Null Item is going to be written into the ItemConfig's Null Items vector.
	SigScan scanner("8b34??85f60f84????????8bbe????????85ff74??8d8f????????e8????????8d8f????????e8????????689801000057e8????????83c408c786????????000000008d8e????????e8????????8d4e??e8????????8d4e??e8????????8d4e??e8????????68e000000056e8????????8bbd");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the commands we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the newly created ItemConfig_Item
		.AddInternalCall(FixLoadingNullItemFromXml)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x5) // Jump for false (no new entry was added, continue as normal)
		.AddRelativeJump((char*)addr + 0x89);  // Jump for true (new entry was added, skip some internal code)
	sASMPatcher.PatchAt(addr, &patch);
}

/*
* Patch that allows modded null items to be assigned a costume from the same mod, if both were given the same "relative ID" in their respective XMLs.
* 
* This mirrors the existing behaviour for "normal" items, where mods can assign the same ID to an item and a costume to have the game associate them.
*/

// Called right before the game would normally auto-generate a null item to correspond with a mod's null costume.
// If the costume was assigned an ID in the mod's xmls, and there also exists a null item defined in the mod's xml that was given the same ID,
// finds the actual ID that was assigned to that null item by the game and gives the costume that same ID, so that null item grants this costume.
bool __stdcall TieModdedCostumesToModdedNullItems(char* ebp) {
	// Yes, this is accurate to how the assembly near the patch refers to the ItemConfig_Costume.
	ItemConfig_Costume* costume = (ItemConfig_Costume*)(ebp - 0x100DC);

	if (lastModIdButCooler.empty() || costume == nullptr || costume->id == 0) {
		// This costume was not assigned a "relative" ID in a mod's xml.
		return false;
	}

	// The "id" of the costume is currently the ID assigned to it in the mod's xml.
	// If the game generates a null item for it, it will assign that new null item's ID to the costume as well.

	// Thanks to XMLData, we can look up if there was a null item defined within the same mod's xmls with the same relative ID.
	const std::string modRelativeKey = lastModIdButCooler + std::to_string(costume->id);

	if (XMLStuff.NullItemData->byrelativeid.count(modRelativeKey) > 0) {
		const unsigned int nullItemID = XMLStuff.NullItemData->byrelativeid[modRelativeKey];
		const XMLAttributes& nullItemXml = XMLStuff.NullItemData->nodes[nullItemID];
		const ItemConfig_Item* nullItem = g_Manager->GetItemConfig()->GetNullItem(nullItemID);

		if (!nullItem || nullItem->name != nullItemXml.at("name")) {
			const std::string err = "WARNING: Trying to match costume [" + std::string(costume->anm2Path) + "] "
				+ "to a NullItem with same mod-relative ID [" + std::to_string(costume->id) + "] from mod [" + lastModIdButCooler + "], "
				+ "but NullItem XMLData [" + nullItemXml.at("name") + "] with ID [" + std::to_string(nullItemID) + "] "
				+ "does not match the corresponding ItemConfig entry [" + (nullItem ? nullItem->name : "<NULL>") + "]!\n";
			ZHL::Log(err.c_str());
			if (!nullItem) return false;
		}

		// This mod defined a null costume and a null item with the same relative ID.
		// Assign the ID of that null item to this costume and return true to skip the game generating a new null item.
		costume->id = nullItemID;
		return true;
	}

	return false;
}

void ASMPatchTieModdedCostumesToModdedNullItems() {
	// This is the spot in parse_costumes_xml() where a new null item would be automatically generated for a MODDED null costume.
	SigScan scanner("8b47??2b47??c1f80268e0000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP)  // Push EBP
		.AddInternalCall(TieModdedCostumesToModdedNullItems)  // See if we can match this costume to a null item from the same mod.
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		// If the function returned true, we assigned the costume to a null item, so we'll jump further and skip the code that would normally generate a new one.
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x5E)
		// If the function returned false, we didn't find a null item to attach the costume to, so go ahead with the original code to generate a new one.
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6)) // Restore the overridden commands
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchNullItemAndNullCostumeSupport() {
	ASMPatchFixLoadingNullItemsFromXml();
	ASMPatchTieModdedCostumesToModdedNullItems();
}
