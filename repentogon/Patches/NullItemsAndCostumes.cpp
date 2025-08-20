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
* 2. Allow mods to assign costumes to their null items by giving them the same relative ID in their respective XMLs.
* 3. Allow mods to assign costumes to heir characters in players.xml via a new "modcostume" attribute (must match the costume's relative ID).
*/

// Track the latest mod to be loading XML data, same as the XMLData code.
// Equivalent to the lastmodid used in XMLData.cpp, except empty for basegame.
std::string lastModIdButCooler = "";

//this is now basically just a copypaste of void ProcessModEntry(char* xmlpath,ModEntry* mod) from XMLData.cpp with minor changes
void UpdateLastModId( ModEntry* mod, char* xmlpath) {
	bool iscontent = false;
	if (mod != NULL) { //it is null when its loading vanilla stuff
		lastModIdButCooler = mod->GetId();
	}
	else {
		lastModIdButCooler = "BaseGame";
	}
	if ((stringlower(xmlpath).find("/content/") != string::npos) || (stringlower(xmlpath).find("/content-dlc3/") != string::npos)) {
		iscontent = true;
	}
	else {
		iscontent = false;
	}
	if ((string(lastModIdButCooler).length() == 0) || ((lastModIdButCooler == "BaseGame") && iscontent)) {
		string path = string(xmlpath);
		int first = path.find("/mods/") + 6;
		int last = path.find("/content");
		if (!iscontent) {
			last = path.find("/resources");
		}
		else if (last <= 0) {
			last = path.find("/content-dlc3");
		}
		path = path.substr(first, last - first); //when the id is null(which it can fucking be) just use the folder name as ID...
		lastModIdButCooler = path;
	}
	//printf("path: %s (mod:%s iscontent:%d) \n", xmlpath,lastmodid,iscontent);
	//logViewer.AddLog("[REPENTOGON]", "Mod ID: %s \n", lastmodid);
	if (lastModIdButCooler == "BaseGame") {
		lastModIdButCooler = "";
	}
}

HOOK_METHOD(ItemConfig, Load, (char* xmlpath, ModEntry* modentry)->void) {
	UpdateLastModId(modentry, xmlpath);
	super(xmlpath, modentry);
}

HOOK_METHOD(ItemConfig, LoadCostumes, (char* xmlpath, ModEntry* modentry)->void) {
	UpdateLastModId(modentry, xmlpath);
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
	SigScan scanner("8b34??85f674??8bcee8????????68e800000056e8????????8b4f");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching parse_items_xml to properly handle modded null items at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the commands we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the newly created ItemConfig_Item
		.AddInternalCall(FixLoadingNullItemFromXml)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x5) // Jump for false (no new entry was added, continue as normal)
		.AddRelativeJump((char*)addr + 0x2B);  // Jump for true (new entry was added, skip some internal code)
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
		const XMLAttributes& nullItemXml = XMLStuff.NullItemData->GetNodeById(nullItemID);
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
	SigScan scanner("8b47??2b47??c1f80268e8000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching parse_costumes_xml to tie modded costumes to modded null items at %p\n", addr);

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

/*
* Hook that allows mods to assign one of their costumes to a player, if the mod-relative "id" of the costume and the "modcostume" attribute of the player matches.
* This allows the costume to be permanantly applied to the player, including during the mineshaft sequence, like the hair of vanilla characters.
*/
HOOK_METHOD(EntityConfig, LoadPlayers, (char* xmlpath, ModEntry* modentry)->void) {
	UpdateLastModId(modentry, xmlpath);

	super(xmlpath, modentry);

	if (lastModIdButCooler.empty()) return;

	// Check the XMLData of all players added by this mod.
	for (const int id : XMLStuff.PlayerData->bymod[lastModIdButCooler]) {

		if (id == 0 || XMLStuff.PlayerData->nodes.count(id) == 0) continue;

		const XMLAttributes& playerXML = XMLStuff.PlayerData->GetNodeById(id);
		EntityConfig_Player* playerConfig = g_Manager->GetEntityConfig()->GetPlayer(id);

		if (!playerConfig || playerConfig->_name != playerXML.at("name")) {
			ZHL::Log("WARNING: Player ID [%d] used in XMLData for player [%s] does not match corresponding EntityConfig_Player [%s]!\n", id, playerXML.at("name").c_str(), (playerConfig ? playerConfig->_name : "<NULL>").c_str());
			if (!playerConfig) continue;
		}

		if (playerConfig->_costumeID == -1 && playerXML.count("modcostume") > 0) {
			const std::string modRelativeKey = lastModIdButCooler + playerXML.at("modcostume");
			if (XMLStuff.NullCostumeData->byrelativeid.count(modRelativeKey) > 0) {
				// Found a null costume from the same mod that matches the player's "modcostume" attribute.
				playerConfig->_costumeID = XMLStuff.NullCostumeData->byrelativeid[modRelativeKey];
			}
		}
	}
}

/*
* AddCostume makes no effort to check whether Item* is nullptr, and will gladly attempt to access it regardless.
* In fact, in some situations it will turn Item* null, then go ahead and access it later anyways!
* Thus, fixing this requires multiple patches. The easy one is hooking the function to immediately reject nullptrs,
* but the cases of turning Item* into a nullptr will need to be manually patched to bail out instead.
*/

// Immediately return if Item* is nullptr
HOOK_METHOD(Entity_Player, AddCostume, (ItemConfig_Item* item, bool itemStateOnly)->void) {
	if (item == nullptr)
		return;
	super(item, itemStateOnly);
}

void ASMPatchAddCostumeNullptrs() {
	// Patch #1
	SigScan scanner("c1f80283f82d");
	scanner.Scan();

	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch AddCostume nullptr crash #1!\n");
		return;
	}

	void* addr = (char*)scanner.GetAddress() + 6;

	ASMPatch patch;
	patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JG, (char*)addr + 0x6)
		.AddRelativeJump((char*)addr + 0x569);

	sASMPatcher.PatchAt(addr, &patch);


	// Patch #2
	SigScan scanner2("c1f80283f87d");
	scanner2.Scan();

	if (!scanner2.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch AddCostume nullptr crash #2!\n");
		return;
	}

	addr = (char*)scanner2.GetAddress() + 6;

	ASMPatch patch2;
	patch2.AddConditionalRelativeJump(ASMPatcher::CondJumps::JG, (char*)addr + 0x6)
		.AddRelativeJump((char*)addr + 0x519);

	sASMPatcher.PatchAt(addr, &patch2);
}


// Function called in ASMPatches.cpp to run patches at the appropriate time.
void PatchNullItemAndNullCostumeSupport() {
	ASMPatchFixLoadingNullItemsFromXml();
	ASMPatchTieModdedCostumesToModdedNullItems();
	ASMPatchAddCostumeNullptrs();
}