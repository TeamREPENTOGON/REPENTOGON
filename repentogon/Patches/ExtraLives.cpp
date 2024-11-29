#include "ExtraLives.h"

#include "ASMPatches.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "SigScan.h"
#include "XMLData.h"
#include "../LuaInit.h"

#include "ASMPatcher.hpp"

// Returns the # of extra life items/effects that the player has via REPENTOGON customtags.
int GetCustomReviveCount(Entity_Player* player, const bool includeHidden) {
	int numLives = 0;
	TemporaryEffects& effects = player->_temporaryeffects;
	for (const auto& [nullID, reviveInfo] : XMLStuff.NullItemData->customreviveitems) {
		if ((includeHidden || !reviveInfo.hidden) && (reviveInfo.item || reviveInfo.effect)) {
			numLives += effects.GetNullEffectNum(nullID);
		}
	}
	for (const auto& [itemID, reviveInfo] : XMLStuff.ItemData->customreviveitems) {
		if (!includeHidden && reviveInfo.hidden) continue;
		if (reviveInfo.item) {
			numLives += player->GetCollectibleNum(itemID, !includeHidden);
		}
		if (reviveInfo.effect) {
			numLives += effects.GetCollectibleEffectNum(itemID);
		}
	}
	for (const auto& [trinketID, reviveInfo] : XMLStuff.TrinketData->customreviveitems) {
		if (!includeHidden && reviveInfo.hidden) continue;
		if (reviveInfo.item) {
			if ((player->_trinketsID[0] & 32767) == trinketID) {
				numLives += 1;
			}
			if ((player->_trinketsID[1] & 32767) == trinketID) {
				numLives += 1;
			}
			numLives += player->GetSmeltedTrinket()->at(trinketID)._trinketNum + player->GetSmeltedTrinket()->at(trinketID)._goldenTrinketNum;
		}
		if (reviveInfo.effect) {
			numLives += effects.GetTrinketEffectNum(trinketID);
		}
	}
	return numLives;
}

// Returns true if a "?" should be displayed at the end of the player's extra lives count.
bool __stdcall PlayerHasChanceRevive(Entity_Player* player) {
	if (player->HasCollectible(212, false)) {
		// Guppy's Collar
		return true;
	}
	TemporaryEffects& effects = player->_temporaryeffects;
	for (const auto& [nullID, reviveInfo] : XMLStuff.NullItemData->customreviveitems) {
		if (reviveInfo.chance && !reviveInfo.hidden && (reviveInfo.item || reviveInfo.effect) && effects.HasNullEffect(nullID)) {
			return true;
		}
	}
	for (const auto& [itemID, reviveInfo] : XMLStuff.ItemData->customreviveitems) {
		if (reviveInfo.chance && !reviveInfo.hidden) {
			if (reviveInfo.item && player->HasCollectible(itemID, false)) {
				return true;
			}
			if (reviveInfo.effect && effects.HasCollectibleEffect(itemID)) {
				return true;
			}
		}
	}
	for (const auto& [trinketID, reviveInfo] : XMLStuff.TrinketData->customreviveitems) {
		if (reviveInfo.chance && !reviveInfo.hidden) {
			if (reviveInfo.item && player->HasTrinket(trinketID, false)) {
				return true;
			}
			if (reviveInfo.effect && effects.HasTrinketEffect(trinketID)) {
				return true;
			}
		}
	}
	return false;
}

// Shared behaviour for MC_PRE_TRIGGER_PLAYER_DEATH (1050) and MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES (1051)
// Returns true if the player was revived during or due to the callback.
bool RunTriggerDeathCallback(Entity_Player* player, const int callbackid) {
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		// Mods can return false to revive the player.
		if (!result && lua_isboolean(L, -1) && !lua_toboolean(L, -1)) {
			// Small convenience - if a mod sets the damage cooldown and returns false, maintain that damage cooldown (calling Revive resets it normally).
			const int damageCooldown = player->_damageCooldown;
			player->Revive();
			player->_damageCooldown = std::max(player->_damageCooldown, damageCooldown);
		}

		// It's possible that someone revived the player manually, without returning false.
		// Just make sure the player is visible.
		if (!player->_dead) {
			if (g_Game->GetRoomTransition()->GetTransitionMode() == 0) {
				player->_visible = true;
			}
			return true;
		}
	}
	return false;
}

// MC_PRE_TRIGGER_PLAYER_DEATH (id: 1050)
HOOK_METHOD(Entity_Player, TriggerDeath, (bool checkOnly) -> bool) {
	if (checkOnly) {
		return super(checkOnly) || GetCustomReviveCount(this, /*includeHidden=*/true) > 0;
	}
	this->_damageCooldown = 1;
	return RunTriggerDeathCallback(this, 1050) || super(checkOnly);
}

// MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES (id: 1051)
void __stdcall RunPostTriggerPlayerDeathCheckRevivesCallback(Entity_Player* player) {
	RunTriggerDeathCallback(player, 1051);
}

// Include custom extra lives in the count, but not hidden ones.
HOOK_METHOD(Entity_Player, GetExtraLives, ()->int) {
	return super() + GetCustomReviveCount(this, /*includeHidden=*/false);
}

int __stdcall GetExtraLivesHook(Entity_Player* player) {
	return player->GetExtraLives();
}

// Seems like this call isn't inlined in rep+
/*
// Patch over what seems to be an inlined or simply duplicate instance of Entity_Player::GetExtraLives in PlayerHUD::RenderHearts.
void ASMPatchInlinedGetExtraLives() {
	SigScan scanner("8b82????????0fbf8a");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching inline Entity_Player::GetExtraLives in PlayerHUD::RenderHearts at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::ESI, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDX)  // Push the Entity_Player
		.AddInternalCall(GetExtraLivesHook)
		.AddBytes("\x89\xC6") // mov esi,eax
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x4A);
	sASMPatcher.PatchAt(addr, &patch);
}
*/

// Patch over the decision of whether or not to display the "?" after the extra lives count.
void ASMPatchReviveQuestionMark() {
	SigScan scanner("e8????????84c0ba????????b9????????8d45");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching PlayerHUD::RenderHearts for revives question mark at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)  // Pop the inputs for the overridden call to HasCollectible
		.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ECX)  // Push the Entity_Player
		.AddInternalCall(PlayerHasChanceRevive)
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patch into EntityPlayer::TriggerDeath, right after all of the vanilla revive effects have been evaluated but none revived the player
void ASMPatchPostTriggerPlayerDeathCheckRevivesCallback() {
	SigScan scanner("807d??000f84????????32c0");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching EntityPlayer::TriggerDeath for custom revives at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes("\x80\x7d\x08").AddZeroes(1)  // cmp byte ptr [ebp+8], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0xA)  // This run is checkOnly, don't run the callback
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EBX)  // Push the Entity_Player
		.AddInternalCall(RunPostTriggerPlayerDeathCheckRevivesCallback)  // Run the callback
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr - 0x895);  // We go to the same place regardless of whether or not the player was revived
	sASMPatcher.PatchAt(addr, &patch);
}

// Returns true to prevent save deletion if the player has any revive item, including hidden ones.
bool __stdcall PlayerHasAnyReviveItem(Entity_Player* player) {
	return player->GetExtraLives() > 0 || GetCustomReviveCount(player, /*includeHidden=*/true) > 0;
}

// Patch for multiple locations to prevent save deletion if the player has a custom revive item.
void ASMPatchPreventSaveDeletion(const char* sig, const int reviveJump, const int deleteJump) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching to prevent save deletion for revives at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ECX)  // Push the Entity_Player
		.AddInternalCall(PlayerHasAnyReviveItem)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + reviveJump)  // Jump for true (prevent save from being deleted)
		.AddRelativeJump((char*)addr + deleteJump);  // Jump for false (allow save to be deleted)
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForExtraLives() {
	// ASMPatchInlinedGetExtraLives();
	ASMPatchReviveQuestionMark();
	ASMPatchPostTriggerPlayerDeathCheckRevivesCallback();

	// PauseScreen::ProcessInput
	ASMPatchPreventSaveDeletion("e8????????83f8017d??e8????????68000000ff", 0xF, 0xA);
	// GameState::Save
	ASMPatchPreventSaveDeletion("e8????????83f8010f8c????????8b3d", 0xE, 0x2B5);
	// GameState::SaveSteamCloud
	ASMPatchPreventSaveDeletion("e8????????83f8010f8c????????8b0d", 0xE, 0x2B7);
}
