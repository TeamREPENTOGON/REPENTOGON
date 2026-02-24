#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "../ASMPatches.h"
#include "../XMLPlayerExtras.h"
#include "../EntityPlus.h"

void ASMPatchSpawnSelectedBaby() {
	SigScan scanner("8b4d??6affe8????????8bf8");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.MoveFromMemory(ASMPatch::Registers::EBP, -0x34, ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EBX, 0x8) //death awaits
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patches into `PlayerManager::init_special_baby_selection` to hide modded characters from the co-op menu if they are still locked or hidden by an achievement.
bool __stdcall ShouldHideModdedCharacterInCoopWheel(EntityConfig_Player* conf) {
	return conf->_hidden || (conf->_achievement > 0 && !g_Manager->GetPersistentGameData()->Unlocked(conf->_achievement)) || !IsCharacterUnlockedRgon(conf->_id) || IsCharacterHiddenByAchievementRgon(conf->_id);
}
void ASMPatchCoopWheelRespectModdedAchievements() {
	SigScan scanner("80b8????????0075??8b47??895d");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Log("[REPENTOGON] Patching PlayerManager::init_special_baby_selection (to respect modded achievements) at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX)  // EntityConfig_Player
		.AddInternalCall(ShouldHideModdedCharacterInCoopWheel)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

/*
* The below patches primarily enable preservation of the ControllerIndex for twins and coplayers spawned by mods, preventing the "Controller Disconnected" popups.
* 
* For the ControllerIndexes, there are two places where this is relevant: PlayerManager::AutoReassignControllers and PauseScreen::ControllerDisconnectHandling.
* AutoReassignControllers runs once after continuing a run, first setting the ControllerIndex of all players to -1 and then re-assigning the available controllers.
* Vanilla twins/subplayers/etc are handled appropriately by the SetControllerIndex function, but ones created by mods are mistaken for separate co-op players.
* If a player cannot be assigned a controller here (or if a controller is disconnected later) the "Controller Disconnected" popup will kick in.
* 
* It is worth noting that GameState remembers and restores the ControllerIndex values for all players, however on continues it is overridden by AutoReassignControllers.
* 
* To generally account for mods, we make the assumption that any players that have the same ControllerIndex should be reassigned to the same ControllerIndex.
* This assumption likely makes no difference for vanilla, where twins/subplayers/etc are all already handled. And in general, it feels reasonable.
* 
* When the game "unassigns" a controller in either location, we'll unassign every player with the same original ControllerIndex.
* Then, when the game "reassigns" a new controller, we'll also reassign every other player that originally shared their ControllerIndex.
* 
* Additionally, the vanilla game's ControllerDisconnectHandling is bugged and will not work in some cases, such as if Jacob & Esau are a co-op player.
* In vanilla Repentance+, attempting to continue a run where Jacob & Esau was player 2+ will cause you to get stuck in the "Controller Disconnected" popup.
* This is because after Jacob's ControllerIndex is reassigned, it will unassign Esau's controller again afterwards, due to referring to outdated information.
* Jacob & Esau's ControllerIndexes are both set at the same time in either case, so this repeats.
* 
* To fix this, we force the the game to break its player loop after reassigning a controller in ControllerDisconnectHandling.
* This is a bit of an ungraceful fix, but it works fine in practice because if there are still players to assign, the game will do it next update.
* If there are no more players to assign, the game will properly realize that (due to properly re-checking the status of all controllers).
*/

// Patches over where the game unassigns a controller (PlayerManager::AutoReassignControllers and PauseScreen::ControllerDisconnectHandling).
// Unassign other stray players with the same ControllerIndex and keep track of all their prior ControllerIndexes (ie, modded twins and coplayers).
void __stdcall UnassignController(Entity_Player* player) {
	int previousControllerIndex = player->_controllerIndex;

	player->SetControllerIndex(-1, true);

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (playerPlus && previousControllerIndex > -1) {
		for (Entity_Player* otherPlayer : g_Game->GetPlayerManager()->_playerList) {
			EntityPlayerPlus* otherPlayerPlus = GetEntityPlayerPlus(otherPlayer);
			if (otherPlayerPlus && otherPlayer->_controllerIndex == previousControllerIndex) {
				ZHL::Log("[ASMPlayerManager::UnassignController] Transitively unassigning ControllerIndex %d from player %d (via player %d)\n", previousControllerIndex, otherPlayer->_playerIndex, player->_playerIndex);
				otherPlayer->SetControllerIndex(-1, false);
				otherPlayerPlus->previousControllerIndex = previousControllerIndex;
			}
		}
		playerPlus->previousControllerIndex = previousControllerIndex;
	}
}
static void PatchUnassignController(char* def) {
	void* addr = sASMDefinitionHolder->GetDefinition(def);

	ZHL::Log("[REPENTOGON] PatchUnassignController @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(UnassignController)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Used to remember that a controller was reassigned in ControllerDisconnectHandling between two patches.
// The second patch is at the end of the for loop and cannot be missed.
bool reassignedDisconnectedController = false;

// Patches over where the game reassigns a controller (PlayerManager::AutoReassignControllers and PauseScreen::ControllerDisconnectHandling).
// Assign the same controller to any other players that previously shared this player's ControllerIndex (ie, modded twins and coplayers).
// For ControllerDisconnectHandling, additionally track that a controller was just reassigned, so that we can force ControllerDisconnectHandling
// to stop and properly re-check everything next update to circumvnet flaws with the vanilla handling.
void __stdcall ReassignController(Entity_Player* player, int newIndex, bool disconnectedController) {
	const bool wasUnset = player->_controllerIndex == -1;

	player->SetControllerIndex(newIndex, true);

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (wasUnset && playerPlus && playerPlus->previousControllerIndex > -1) {
		for (Entity_Player* otherPlayer : g_Game->GetPlayerManager()->_playerList) {
			EntityPlayerPlus* otherPlayerPlus = GetEntityPlayerPlus(otherPlayer);
			if (otherPlayerPlus && otherPlayer->_controllerIndex == -1 && otherPlayerPlus->previousControllerIndex == playerPlus->previousControllerIndex) {
				if (otherPlayer->_controllerIndex != newIndex) {
					ZHL::Log("[ASMPlayerManager::ReassignController] Transitively re-assigning ControllerIndex %d to player %d (via player %d)\n", newIndex, otherPlayer->_playerIndex, player->_controllerIndex);
					otherPlayer->SetControllerIndex(newIndex, false);
				}
				otherPlayerPlus->previousControllerIndex = -1;
			}
		}
		playerPlus->previousControllerIndex = -1;
	}

	if (wasUnset && disconnectedController) {
		reassignedDisconnectedController = true;
	}
}
static void PatchReassignController(char* def, bool disconnectedController) {
	void* addr = sASMDefinitionHolder->GetDefinition(def);

	ZHL::Log("[REPENTOGON] PatchReassignController @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(savedRegisters)
		.Push(disconnectedController)
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(ReassignController)
		.RestoreRegisters(savedRegisters)
		.Pop(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patch at the end of the player loop in ControllerDisconnectHandling.
// Break out of the loop if we just reassigned a controller.
// The game will not handle things properly if we don't, even for completely vanilla cases,
// such as Jacob & Esau as player 2 (this issue exists in vanilla Rep+).
bool __stdcall PostReassignDisconnectedController() {
	if (reassignedDisconnectedController) {
		reassignedDisconnectedController = false;
		return true;
	}
	return false;
}
static void PatchPostReassignDisconnectedController() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::ControllerDisconnected_LoopEnd);

	ZHL::Log("[REPENTOGON] PatchPostReassignDisconnectedController @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(PostReassignDisconnectedController)
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0xC)  // Jump for true
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote.
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchReassignControllers() {
	PatchUnassignController(&AsmDefinitions::AutoReassignControllers_Unassign);
	PatchReassignController(&AsmDefinitions::AutoReassignControllers_Reassign, false);

	PatchUnassignController(&AsmDefinitions::ControllerDisconnected_Unassign);
	PatchReassignController(&AsmDefinitions::ControllerDisconnected_Reassign, true);

	PatchPostReassignDisconnectedController();
}

// Hook after RestoreGameState to restore modded twins.
// We remember twin relationships ourselves (see EntitySaveStateEx.cpp) and restore that relationship here.
// ControllerIndexes are handled fine by the above code.
// restoreTwinID is the index of the main twin when the state was saved.
// We do not rely on the index still being the same - we only look for the pair of players with the same restoreTwinID.
HOOK_METHOD(PlayerManager, RestoreGameState, (GameState* state) -> void) {
	super(state);

	for (int i = 0; i < _playerList.size(); i++) {
		Entity_Player* player = _playerList[i];
		if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player); playerPlus && playerPlus->restoreTwinID > -1) {
			for (int j = i + 1; j < _playerList.size(); j++) {
				Entity_Player* otherPlayer = _playerList[j];
				if (EntityPlayerPlus* otherPlayerPlus = GetEntityPlayerPlus(otherPlayer); otherPlayerPlus && otherPlayerPlus->restoreTwinID == playerPlus->restoreTwinID) {
					ZHL::Log("[ASMPlayerManager] Restoring twin relationship for players %d and %d\n", player->_playerIndex, otherPlayer->_playerIndex);
					player->SetTwinPlayer(otherPlayer);
					playerPlus->restoreTwinID = -1;
					otherPlayer->SetTwinPlayer(player);
					otherPlayerPlus->restoreTwinID = -1;
					break;
				}
			}
		}
	}
}
