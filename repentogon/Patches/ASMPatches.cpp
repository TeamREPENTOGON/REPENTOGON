
#include "ASMPatches.h"

#include "../LuaInterfaces/LuaRender.h"
#include "NullItemsAndCostumes.h"
#include "CustomCache.h"
#include "FamiliarTags.h"
#include "PlayerTags.h"
#include "GetCoinValue.h"
#include "PocketItems.h"
#include "Anm2Extras.h"
#include "ExtraLives.h"
#include "EntityPlus.h"
#include "CustomItemPools.h"
#include "CardsExtras.h"

#include "ASMPatches/ASMCallbacks.h"
#include "ASMPatches/ASMDelirium.h"
#include "ASMPatches/ASMEntityNPC.h"
#include "ASMPatches/ASMGridEntityCollision.h"
#include "ASMPatches/ASMGridEntitySpawn.h"
#include "ASMPatches/ASMLevel.h"
#include "ASMPatches/ASMMenu.h"
#include "ASMPatches/ASMPlayer.h"
#include "ASMPatches/ASMPlayerManager.h"
#include "ASMPatches/ASMRender.h"
#include "ASMPatches/ASMRoom.h"
#include "ASMPatches/ASMStatusEffects.h"
#include "ASMPatches/ASMTweaks.h"
#include "ASMPatches/ASMTweaks.h"
#include "ASMPatches/ASMXMLItem.h"

#include "ASMPatcher.hpp"

/* This patch hooks KAGE_LogMessage by hand. LibZHL can't properly hook functions with varargs, and we need varargs to properly get log messages.
*  So, we'll just do it manually, not a big deal.
*  We manually call a trampoline function that takes a const char* as an input, and prints it to our ImGui log.
*  At the point we hook, KAGE_LogMessage has already fully processed its varargs, and is storing the result in the ECX register.
*  We pass ECX to our trampoline as a result.
   */
void ASMPatchLogMessage() {
	SigScan scanner("8d51??8a014184c075??2bca80b9????????0a");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Adding LogMessage callback for ImGui at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x51") // Push ECX to the stack, so we can restore it as the function expects later
		.AddBytes("\x51") // Push ECX one more time, for use on our function (this will be consumed).
		.AddInternalCall(LogMessageCallback)
		.AddBytes("\x59") // Pop ECX

		// Reintroduce overwritten assembly + some extra to work around some broken jumps
		.AddBytes("\x8D\x51\x01") // lea edx,dword pointer ds:[ecx+1]
		.AddBytes("\x8A\x01") // mov al,byte ptr ds:[ecx]
		.AddBytes("\x41") // inc ecx
		.AddBytes("\x84\xC0") // test al, al
		.AddBytes("\x75\xF9") // jne xxxxxxxx
		.AddRelativeJump((char*)addr + 10);
	sASMPatcher.PatchAt(addr, &patch);
}

/* Console::RunCommand takes a Player param for commands that need to run on a player.
*  RunCommand will attempt to derive this from PlayerManager if not set.
*  This prevents commands from being run on the main menu.
*  We will handle this on our end in two parts- one hook, and one patch. This is the patch side of that.
*  We just skip over the check by replacing the conditional JMP with an unconditional one.
*/
void ASMPatchConsoleRunCommand() {
	SigScan scanner("75??8b0d????????5781c1a8ba0100");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Console::RunCommand Player requirement at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\xEB");
	sASMPatcher.FlatPatch(addr, &patch);
}

void PerformASMPatches() {
	ASMPatchLogMessage();
	ASMPatchConsoleRunCommand();

	// Callbacks
	PatchPreSampleLaserCollision();
	PatchPreLaserCollision();
	PatchPreEntityTakeDamageCallbacks();
	PatchPostEntityTakeDamageCallbacks();
	ASMPatchPrePlayerUseBomb();
	ASMPatchPostPlayerUseBomb();
	ASMPatchPreMMorphActiveCallback();
	ASMPatchTrySplit();
	ASMPatchInputAction();
	ASMPatchPostNightmareSceneCallback();
	ASMPatchPrePickupVoided();
	ASMPatchPrePickupVoidedBlackRune();
	ASMPatchPrePickupVoidedAbyss();
	ASMPatchPrePickupComposted();
	ASMPatchPostChampionRegenCallback();
	ASMPatchTrinketRender();
	ASMPatchPickupUpdatePickupGhosts();
	ASMPatchProjectileDeath();
	ASMPatchTearDeath();
	ASMPatchPrePlayerGiveBirth();
	ASMPatchesBedCallbacks();
	ASMPatchMainMenuCallback();
	
	ASMPatchPrePlayerPocketItemSwap();

	// Delirium
	delirium::AddTransformationCallback();
	delirium::AddPostTransformationCallback();

	// EntityNPC
	ASMPatchHushBug();
	ASMPatchFireProjectiles();
	ASMPatchFireBossProjectiles();
	//ASMPatchApplyFrozenEnemyDeathEffects();  // This was disabled prior to rep+, ignore it!

	// GridEntity
	PatchGridCollisionCallback();
	PatchGridSpawnCallback();

	// Level
	ASMPatchBlueWombCurse();
	ASMPatchVoidGeneration();
	PatchSpecialQuest();
	ASMPatchDealRoomVariants();
	//PatchOverrideDataHandling();  // This was disabled prior to rep+, ignore it!
	PatchLevelGeneratorTryResizeEndroom();

	// Menu
	ASMPatchModsMenu();
	ASMPatchMenuOptionsLanguageChange();

	// Room
	ASMPatchAmbushWaveCount();
	PatchBossWaveDifficulty();
	ASMPatchMegaSatanEnding();
	ASMPatchWaterDisabler();
	PatchRoomClearDelay();
	ASMPatchTrySpawnBlueWombDoor();

	// Player
	ASMPatchCheckFamiliar();
	ASMPatchPlayerStats();
	ASMPatchesForPlayerCustomTags();
	ASMPatchesForExtraLives();
	ASMPatchMarsDoubleTapWindow();
	ASMPatchAddActiveCharge();

	// Status Effects
	PatchInlinedGetStatusEffectTarget();
	ASMPatchAddWeakness();

	// Render
	//LuaRender::PatchglDrawElements();  // Related to unfinished features, not required for rep+ update
	PatchStatHudPlanetariumChance();

	//PlayerManager
	ASMPatchSpawnSelectedBaby();
	ASMPatchCoopWheelRespectModdedAchievements();

	// External
	ASMPatchesForFamiliarCustomTags();
	PatchNullItemAndNullCostumeSupport();
	ASMPatchesForGetCoinValue();
	ASMPatchesForAddRemovePocketItemCallbacks();
	ASMPatchesForEntityPlus();
	ASMPatchesForCustomCache();
	//ASMPatchesForCustomItemPools();
	//ExtraASMPatchesForCustomItemPools();
	ASMPatchesForCardsExtras();
	ASMPatchesForXMLItem();
	HookImGui();

	// Sprite
	ASMPatchesForANM2Extras();

	// Tweaks (bug crashes)
	if (!ASMPatches::FixGodheadEntityPartition()) {
		ZHL::Log("[ERROR] Unable to find signature for Godhead EntityPartition patch\n");
	}

	if (!ASMPatches::FixTearDetonatorEntityList()) {
		ZHL::Log("[ERROR] Unable to find signature for Tear Detonator EntityList_EL in UseActiveItem\n");
	}

	if (!ASMPatches::SkipArchiveChecksums()) {
		ZHL::Log("[ERROR] Error while applying an archive checksum skip\n");
	};

	if (!ASMPatches::LeaderboarEntryCheckerUpdate()) {
		ZHL::Log("[ERROR] Error while applying the leaderboard entry checker\n");
	};

//	the patch is disabled because it does not do the actual heavylifting and results in a desync ;p
//
//	ASMPatches::AllowConsoleInOnline();

	// This patch needs to be remade to include a toggle setting and fix glowing hourglass and the day before a release isn't the time for that

	//if (!ASMPatches::FixHushFXVeins()) {
	//	ZHL::Log("[ERROR] Error while restoring Hush boss room veins FX\n");
	//}

	ASMPatches::NativeRepentogonResources();
}