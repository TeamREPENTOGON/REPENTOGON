#include "ExtraLives.h"

#include <unordered_set>

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
			if ((player->_trinketsID[0] & TRINKET_ID_MASK) == trinketID) {
				numLives += 1;
			}
			if ((player->_trinketsID[1] & TRINKET_ID_MASK) == trinketID) {
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

// TriggerDeath calls (that aren't `checkOnly`) are done in PlayerManager::Update, where it iterates over the players to look for dead ones no longer playing an animation.
// We may call TriggerDeath earlier than normal on the non-main twin, in order to allow mods to save the main twin with the other twin's revives.
// Since the game is going to iterate to the non-main twin still, we mark the twin's player index here to prevent callbacks from getting excecuted twice.
static std::unordered_set<int> s_EarlyTriggeredTwinDeaths;

HOOK_METHOD(PlayerManager, Update, () -> void) {
    super();
    s_EarlyTriggeredTwinDeaths.clear();
}

void PostReviveChecks(Entity_Player* player) {
    // Sync with twin
    Entity_Player* twin = player->GetTwinPlayer();
    if (twin) {
        const int damageCooldown = std::max(player->_damageCooldown, twin->_damageCooldown);
        if (twin->_dead) {
            twin->Revive();
        }
        player->_damageCooldown = damageCooldown;
        twin->_damageCooldown = damageCooldown;
    }

    // Players can sometimes be invisible after revival.
    if (g_Game->GetRoomTransition()->GetTransitionMode() == 0) {
        player->_visible = true;
        if (twin) {
            twin->_visible = true;
        }
    }
}

// Shared behaviour for MC_PRE_TRIGGER_PLAYER_DEATH (1050) and MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES (1051)
// Returns true if the player was revived during or due to the callback.
bool RunTriggerDeathCallback(Entity_Player* player, const int callbackid) {
	if (player->_exists && CallbackState.test(callbackid - 1000) && !s_EarlyTriggeredTwinDeaths.count(player->_playerIndex)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!player->_exists) {
			// It's possible for this to happen semi-naturally if ChangePlayerType was called on Jacob, which removes Esau.
			return false;
		}

		// Mods can return false to revive the player.
		if (!result && lua_isboolean(L, -1) && !lua_toboolean(L, -1) && player->_dead) {
			// Small convenience - if a mod sets the damage cooldown and returns false, maintain that damage cooldown (calling Revive resets it normally).
			const int damageCooldown = player->_damageCooldown;
			player->Revive();
			player->_damageCooldown = std::max(player->_damageCooldown, damageCooldown);
		}

		// It's possible that someone revived the player manually, without returning false.
		if (!player->_dead) {
            PostReviveChecks(player);
			return true;
		}
	}
	return false;
}

// MC_PRE_TRIGGER_PLAYER_DEATH (id: 1050)
HOOK_METHOD(Entity_Player, TriggerDeath, (bool checkOnly) -> bool) {
	if (checkOnly) {
        if (super(checkOnly) || GetCustomReviveCount(this, /*includeHidden=*/true) > 0) {
            return true;
        }
        if (Entity_Player* twin = this->GetTwinPlayer()) {
            return GetCustomReviveCount(twin, /*includeHidden=*/true) > 0;
        }
        return false;
	}
	this->_damageCooldown = 1;
	if (RunTriggerDeathCallback(this, 1050)) {
		return true;
	}
	if (!this->_exists) {
		return false;
	}
	return super(checkOnly);
}

// MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES (id: 1051)
void __stdcall RunPostTriggerPlayerDeathCheckRevivesCallback(Entity_Player* player) {
	RunTriggerDeathCallback(player, 1051);
    if (player->_exists && player->_dead) {
		if (Entity_Player* twin = player->GetTwinPlayer(); twin && twin->_exists) {
            if (!twin->_dead) {
                player->Revive();
                PostReviveChecks(player);
            } else if (player->_playerIndex < twin->_playerIndex) {
                // If the main twin is about to die, trigger death on the other twin early to fish for modded revives.
                // Otherwise by the time this happens naturally it may be too late for mods to prevent the run from ending!
                twin->TriggerDeath(false);
				if (!player->_dead) {
					PostReviveChecks(player);
				}
                s_EarlyTriggeredTwinDeaths.insert(twin->_playerIndex);
            }
        }
    }
}

// Include custom extra lives in the count, but not hidden ones.
HOOK_METHOD(Entity_Player, GetExtraLives, ()->int) {
	return super() + GetCustomReviveCount(this, /*includeHidden=*/false);
}

// Seems like this call isn't inlined in rep+
/*
int __stdcall GetExtraLivesHook(Entity_Player* player) {
	return player->GetExtraLives();
}

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
	SigScan patchScanner("807d??000f84????????32c0");
	patchScanner.Scan();
	void* patchAddr = patchScanner.GetAddress();

	printf("[REPENTOGON] Patching EntityPlayer::TriggerDeath for custom revives at %p\n", patchAddr);

	SigScan exitScanner("a1????????3b05????????74??8b08");
	exitScanner.Scan();
	void* exitAddr = exitScanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)patchAddr, 4))  // cmp byte ptr [ebp+?], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)patchAddr + 0xA)  // This run is checkOnly, don't run the callback
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EBX)  // Push the Entity_Player
		.AddInternalCall(RunPostTriggerPlayerDeathCheckRevivesCallback)  // Run the callback
		.RestoreRegisters(reg)
		.AddRelativeJump(exitAddr);  // We go to the same place regardless of whether or not the player was revived
	sASMPatcher.PatchAt(patchAddr, &patch);
}

// Returns 1 to prevent save deletion if the player has any revive item, including hidden ones which don't normally get counted.
// The actual number of lives is not needed for where this gets called, >0 prevents save deletion.
int __stdcall PlayerHasAnyReviveItem(Entity_Player* player) {
	if (player->GetExtraLives() > 0 || GetCustomReviveCount(player, /*includeHidden=*/true) > 0) {
		return 1;
	}
	return 0;
}

// Patch for multiple locations to prevent save deletion if the player has a custom revive item.
void ASMPatchPreventSaveDeletion(const char* sig) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching to prevent save deletion for revives at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ECX)  // Push the Entity_Player
		.AddInternalCall(PlayerHasAnyReviveItem)
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForExtraLives() {
	// ASMPatchInlinedGetExtraLives();
	ASMPatchReviveQuestionMark();
	ASMPatchPostTriggerPlayerDeathCheckRevivesCallback();

	// PauseScreen::ProcessInput
	ASMPatchPreventSaveDeletion("e8????????83f8017d??e8????????68000000ff");
	// GameState::Save
	ASMPatchPreventSaveDeletion("e8????????83f8010f8c????????8b3d");
	// GameState::SaveSteamCloud
	ASMPatchPreventSaveDeletion("e8????????83f8010f8c????????8b0d");
}
