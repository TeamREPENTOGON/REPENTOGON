#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "EntityPlus.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"


// Max coins/keys/bombs is a global thing so no need to store it on any player.
int MAX_COINS = 99;
char HUD_COINS_STR_FORMAT[] = "%002d";

int __stdcall GetMaxCoins() {
	return MAX_COINS;
}

int MAX_KEYS = 99;
char HUD_KEYS_STR_FORMAT[] = "%002d";

int __stdcall GetMaxKeys() {
	return MAX_KEYS;
}

int MAX_BOMBS = 99;
char HUD_BOMBS_STR_FORMAT[] = "%002d";

int __stdcall GetMaxBombs() {
	return MAX_BOMBS;
}

float GetVanillaStatMultiplier(Entity_Player* player) {
	float mult = 1.0f + player->GetTrinketMultiplier(TRINKET_CRACKED_CROWN) * 0.2f;
	if (player->_playerType == PLAYER_BETHANY_B) {
		mult *= 0.75f;
	}
	return mult;
}

double GetDefaultCustomCacheValue(Entity_Player* player, const std::string& customcache) {
	if (customcache == "maxcoins" || customcache == "maxkeys" || customcache == "maxbombs") {
		if (customcache == "maxcoins" && g_Game->_playerManager.FirstCollectibleOwner(COLLECTIBLE_DEEP_POCKETS, nullptr, true)) {
			return 999;
		}
		return 99;
	}
	else if (customcache == "healthtype") {
		EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
		if (playerPlus) {
			playerPlus->disableHealthTypeModification = true;
			double defaultHealthType = (double)player->GetHealthType();
			playerPlus->disableHealthTypeModification = false;
			return defaultHealthType;
		}
	}
	else if (customcache == "tearscap") {
		return 5.0;
	}
	else if (customcache == "statmultiplier") {
		return GetVanillaStatMultiplier(player);
	}

	return 0;
}

double GetCustomCacheValue(Entity_Player* player, const std::string& customcache) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);

	if (playerPlus && playerPlus->customCacheResults.find(customcache) != playerPlus->customCacheResults.end()) {
		return playerPlus->customCacheResults[customcache];
	}
	return GetDefaultCustomCacheValue(player, customcache);
}

void UpdateMaxCoinsKeysBombs(const std::string& customcache, const double newMax) {
	Entity_Player* player = g_Game->_playerManager.GetPlayer(0);

	int* current = nullptr;
	int* max = nullptr;
	char* hudStr = nullptr;
	int consumableCache = -1;

	if (customcache == "maxcoins") {
		current = player ? &player->_numCoins : nullptr;
		max = &MAX_COINS;
		hudStr = HUD_COINS_STR_FORMAT;
		consumableCache = 1;
	} else if (customcache == "maxkeys") {
		current = player ? &player->_numKeys : nullptr;
		max = &MAX_KEYS;
		hudStr = HUD_KEYS_STR_FORMAT;
		consumableCache = 2;
	} else if (customcache == "maxbombs") {
		current = player ? &player->_numBombs : nullptr;
		max = &MAX_BOMBS;
		hudStr = HUD_BOMBS_STR_FORMAT;
		consumableCache = 4;
	} else {
		return;
	}

	*max = (int)std::clamp(newMax, 0.0, (double)INT_MAX);;

	const int numDigits = *max < 1 ? 1 : ((int)std::floor(std::log10(*max)) + 1);
	if (numDigits > 9) {
		hudStr[2] = '1';
		hudStr[3] = '0';
	}
	else {
		hudStr[2] = '0';
		hudStr[3] = '0' + numDigits;;
	}

	// Previously, if you lost Deep Pockets, you could keep your excess coins until the next time 
	// AddCoins gets called (the game only truncated to the limit after adding/removing coins).
	// Feels like a bug/oversight, and is unintuitive for the "maxcoins" customcache.
	// However, maintain it for dailies.
	// Esau Jr also still currently has this quirk but I don't think he's worth messing around with.
	if (g_Game->GetDailyChallenge()._id == 0 && player && *current > *max) {
		*current = *max;
		player->ConsumableCountChanged(consumableCache);
	}

	// Refresh the HUD
	if (g_Game->GetHUD()->_hudRefreshType != 2) {
		g_Game->GetHUD()->_hudRefreshType = 1;
		g_Game->GetHUD()->_hudRefreshCountdown = 2;
	}
}

std::vector<Entity_Familiar*> InvalidateCachedFamiliarMultipliers(Entity_Player* player) {
	Room* room = g_Game->GetCurrentRoom();
	EntityList* entityList = room->GetEntityList();
	EntityList_EL queryResult(*entityList->GetUpdateEL());
	entityList->QueryType(&queryResult, 3, -1, -1, false, false);

	unsigned int size = queryResult._size;

	std::vector<Entity_Familiar*> out;

	if (size) {
		Entity** data = queryResult._data;
		while (size) {
			Entity_Familiar* familiar = (*data)->ToFamiliar();
			if (familiar && familiar->_player == player) {
				EntityFamiliarPlus* familiarPlus = GetEntityFamiliarPlus(familiar);
				if (familiarPlus) {
					familiarPlus->cachedMultiplier = std::nullopt;
				}
			}
			++data;
			--size;
		}
		queryResult.Destroy();
	}

	return out;
}

float RunEvaluateFamiliarMultiplierCallback(Entity_Familiar* familiar, const float baseMult) {
	// MC_EVALUATE_FAMILIAR_MULTIPLIER
	const int callbackid = 1225;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(familiar->_variant)
			.push(familiar, lua::Metatables::ENTITY_FAMILIAR)
			.push(baseMult)
			.push(familiar->_player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!result && lua_isnumber(L, -1)) {
			const float newMult = (float)lua_tonumber(L, -1);
			if (newMult > 0) {
				return newMult;
			}
		}
	}
	return baseMult;
}

// Take the customcache string as a copy to be extra safe that it can't get destroyed while this is running.
void RunEvaluateCustomCacheCallback(Entity_Player* player, const std::string customcache) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (!playerPlus) {
		// Aaah!
		return;
	}

	const bool maxCoinsKeysBombs = customcache == "maxcoins" || customcache == "maxkeys" || customcache == "maxbombs";

	double initialValue = GetDefaultCustomCacheValue(player, customcache);

	if (maxCoinsKeysBombs) {
		// Only run evaluations as player 1 for global stats like this.
		player = g_Game->_playerManager.GetPlayer(0);
		if (!player) return;  // ???

		// Remove the customcache from the other players to avoid any unnecessary evaluations.
		for (Entity_Player* otherPlayer : g_Game->_playerManager._playerList) {
			if (otherPlayer && otherPlayer->_exists) {
				EntityPlayerPlus* otherPlayerPlus = GetEntityPlayerPlus(otherPlayer);
				if (otherPlayerPlus) {
					otherPlayerPlus->customCacheTags.erase(customcache);
				}
			}
		}
	} else if (customcache == "familiarmultiplier") {
		// Familiar multiplier has special treatment, and does not go through the usual callback.
		// Still otherwise implemented as a customcache because it is "triggered" in the same ways.
		// When this cache is triggered on the player, wipe the cached multipliers from all their
		// familiars, indirectly triggering MC_EVALUATE_FAMILIAR_MULTIPLIER instead.
		InvalidateCachedFamiliarMultipliers(player);
		playerPlus->customCacheResults[customcache] = 0;
		return;
	}

	double previousValue = initialValue;
	if (playerPlus->customCacheResults.find(customcache) != playerPlus->customCacheResults.end()) {
		previousValue = playerPlus->customCacheResults[customcache];
	}
	double newValue = initialValue;

	// MC_EVALUATE_CUSTOM_CACHE
	const int callbackid = 1224;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(customcache.c_str())
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(customcache.c_str())
			.push(initialValue)
			.call(1);

		if (!result && lua_isnumber(L, -1)) {
			newValue = lua_tonumber(L, -1);
		}
	}

	playerPlus->customCacheResults[customcache] = newValue;

	if (maxCoinsKeysBombs) {
		UpdateMaxCoinsKeysBombs(customcache, newValue);
	} else if (customcache == "healthtype") {
		player->GetHealthType();  // Trigger enforcement of the health type
	} else if (customcache == "tearscap" && newValue != previousValue) {
		player->_cacheFlags |= CACHE_FIREDELAY;
		playerPlus->customCacheRequiresEvaluateItemsCall = true;
	} else if (customcache == "statmultiplier" && newValue != previousValue) {
		player->_cacheFlags |= CACHE_DAMAGE | CACHE_FIREDELAY | CACHE_SHOTSPEED | CACHE_RANGE | CACHE_SPEED;
		playerPlus->customCacheRequiresEvaluateItemsCall = true;
	}
}

void TriggerCustomCache(Entity_Player* player, const std::set<std::string>& customcaches, const bool immediate) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (!playerPlus) return;  // Aaah!

	if (immediate) {
		for (const std::string& customcache : customcaches) {
			playerPlus->customCacheTags.erase(customcache);
			RunEvaluateCustomCacheCallback(player, customcache);
		}
		if (playerPlus->customCacheRequiresEvaluateItemsCall) {
			player->EvaluateItems();
		}
	} else {
		for (const std::string& customcache : customcaches) {
			playerPlus->customCacheTags.insert(customcache);
		}
		player->_temporaryeffects._shouldEvaluateCache = true;
	}
}

void TriggerCustomCache(Entity_Player* player, XMLItem* xmlData, const int id, const bool immediate) {
	if (xmlData->HasAnyCustomCache(id)) {
		TriggerCustomCache(player, xmlData->GetCustomCache(id), immediate);
	}
}

void TriggerNullCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.NullItemData, id, immediate);
}

void TriggerCollectibleCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.ItemData, id, immediate);
}

void TriggerTrinketCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.TrinketData, id, immediate);
}

void TriggerItemCustomCache(Entity_Player* player, ItemConfig_Item* item, const bool immediate) {
	if (item->type == 0) {  // Null
		TriggerNullCustomCache(player, item->id, immediate);
	} else if (item->type == 2) {  // Trinket
		TriggerTrinketCustomCache(player, item->id, immediate);
	} else {  // Collectible (Passive/Active/Familiar)
		TriggerCollectibleCustomCache(player, item->id, immediate);
	}
}

// Re-evaluate custom caches that appeared in XML data if EvaluateItems is run for CacheFlag.CACHE_ALL.
HOOK_METHOD_PRIORITY(Entity_Player, EvaluateItems, -1, () -> void) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this);
	if (playerPlus) {
		if ((this->_cacheFlags & CACHE_ALL) == CACHE_ALL) {  // If visual studio tries to tell you that this equality check is not needed do NOT listen
			for (const std::string& customcache : XMLStuff.AllCustomCaches) {
				playerPlus->customCacheTags.insert(customcache);
			}
		}
		if (!playerPlus->customCacheTags.empty()) {
			// Make a copy of the customCacheTags and clear before iterating,
			// to avoid any funny business with inserts/removals from the set.
			const std::set<std::string> customcaches = playerPlus->customCacheTags;
			playerPlus->customCacheTags.clear();
			for (const std::string& customcache : customcaches) {
				RunEvaluateCustomCacheCallback(this, customcache);
			}
		}
		playerPlus->customCacheRequiresEvaluateItemsCall = false;
	}

	super();
}

// Collectibles (both real and via wisps) trigger cache evaluations immediately when added or removed.

HOOK_METHOD_PRIORITY(Entity_Player, AddCollectible, -1, (int type, int charge, bool firsttime, int slot, int vardata, int unk) -> void) {
	super(type, charge, firsttime, slot, vardata, unk);
	TriggerCollectibleCustomCache(this, type, true);
}

HOOK_METHOD_PRIORITY(Entity_Familiar, WispInit, -1, () -> void) {
	super();

	if (this->_variant == 237 && this->_subtype > 0 && this->_player) {
		TriggerCollectibleCustomCache(this->_player, this->_subtype, true);
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerCollectibleRemoved, -1, (unsigned int collectibleType) -> void) {
	super(collectibleType);
	TriggerCollectibleCustomCache(this, collectibleType, true);
}

// Trinkets & TemporaryEffects being added/removed don't trigger EvaluateItems immediately, instead waiting until the next EntityPlayer::Update.

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketAdded, -1, (unsigned int trinketType, bool firstTime) -> void) {
	super(trinketType, firstTime);
	TriggerTrinketCustomCache(this, trinketType, false);
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketRemoved, -1, (unsigned int trinketType) -> void) {
	super(trinketType);
	TriggerTrinketCustomCache(this, trinketType, false);
}

HOOK_METHOD_PRIORITY(TemporaryEffects, AddEffect, -1, (TemporaryEffect* effect, bool addCostume, int count) -> void) {
	super(effect, addCostume, count);
	if (!this->_disabled && count != 0 && this->_player && effect->_item) {
		TriggerItemCustomCache(this->_player, effect->_item, false);
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerEffectRemoved, -1, (ItemConfig_Item* item, int unused) -> void) {
	super(item, unused);
	if (item) {
		TriggerItemCustomCache(this, item, false);
	}
}


// Familiar Multiplier!
// Patched into the end of Entity_Familiar::GetMultiplier().
void __stdcall FamiliarGetMultiplierTrampoline(Entity_Familiar* familiar, float baseMult) {
	EntityFamiliarPlus* familiarPlus = GetEntityFamiliarPlus(familiar);

	if (!familiarPlus) {
		// Aaah!
		return;
	}

	if (!familiarPlus->cachedMultiplier.has_value() || familiarPlus->cachedMultiplierPlayer != (uintptr_t)familiar->_player) {
		familiarPlus->cachedMultiplier = RunEvaluateFamiliarMultiplierCallback(familiar, baseMult);
		familiarPlus->cachedMultiplierPlayer = (uintptr_t)familiar->_player;
	}

	float mult = *familiarPlus->cachedMultiplier;

	__asm {
		movd xmm0, mult
	}
}
void PatchFamiliarGetMultiplierCallback() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::FamiliarGetMultiplier);

	printf("[REPENTOGON] Patching end of Entity_Familiar::GetMultiplier for callback at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | (ASMPatch::SavedRegisters::Registers::XMM_REGISTERS & ~ASMPatch::SavedRegisters::Registers::XMM0), true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x66\x0F\x7E\xC0")  // movd eax, xmm0
		.Push(ASMPatch::Registers::EAX) // float baseMult
		.Push(ASMPatch::Registers::ESI) // Entity_Familiar*
		.AddInternalCall(FamiliarGetMultiplierTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}


// MAX COINS
const char* __stdcall GetHudCoinsStringFormat() {
	return HUD_COINS_STR_FORMAT;
}
void PatchHudRenderCoins() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxCoins_HudRender);

	printf("[REPENTOGON] Patching HUD::Render for max coins at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~(ASMPatch::SavedRegisters::Registers::EAX | ASMPatch::SavedRegisters::Registers::ECX), true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)  // Pop inputs to overridden FirstCollectibleOwner
		.Pop(ASMPatch::Registers::EAX)
		.Pop(ASMPatch::Registers::EAX)
		.AddBytes(ByteBuffer().AddAny((char*)addr + 0x5, 3))  // Restore a thing
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetHudCoinsStringFormat)
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX)
		.MoveImmediate(ASMPatch::Registers::EAX, 0)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x17);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchAddCoins() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxCoins_AddCoins);

	printf("[REPENTOGON] Patching EntityPlayer::AddCoins for max coins at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)  // Pop inputs to overridden FirstCollectibleOwner
		.Pop(ASMPatch::Registers::EAX)
		.Pop(ASMPatch::Registers::EAX)
		.AddBytes(ByteBuffer().AddAny((char*)addr + 0x7, 0xF))  // Restore a things
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxCoins)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x1E);
	sASMPatcher.PatchAt(addr, &patch);
}


// MAX KEYS
const char* __stdcall GetHudKeysStringFormat() {
	return HUD_KEYS_STR_FORMAT;
}
void PatchAddKeys() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxKeys_AddKeys);

	printf("[REPENTOGON] Patching EntityPlayer::AddKeys for max keys at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr + 0x3, 0x7))  // Restore a thigns
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxKeys)
		.AddBytes("\x39\x45").AddBytes(ByteBuffer().AddAny((char*)addr + 0x5, 0x1))  // cmp DWORD PTR [ebp+?],eax
		.AddBytes("\x89\x45").AddBytes(ByteBuffer().AddAny((char*)addr + 0x9, 0x1))  // mov DWORD PTR [ebp+?],eax
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x11);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchHudRenderKeys() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxKeys_HudRender);

	printf("[REPENTOGON] Patching HUD::Render for max keys at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetHudKeysStringFormat)
		.RestoreRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}


// MAX BOMBS
const char* __stdcall GetHudBombsStringFormat() {
	return HUD_BOMBS_STR_FORMAT;
}
void PatchAddBombs() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxBombs_AddBombs);

	printf("[REPENTOGON] Patching EntityPlayer::AddBombs for max bombs at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EDX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxBombs)
		.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x89\x54\x24").AddBytes(ByteBuffer().AddAny((char*)addr + 0x3, 0x1))  // mov dword ptr [esp+?], edx
		.AddBytes("\x39\xD0")  // cmp eax,edx
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchHudRenderBombs() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxBombs_HudRender);

	printf("[REPENTOGON] Patching HUD::Render for max bombs at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetHudBombsStringFormat)
		.RestoreRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchControlBombs() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxBombs_ControlBombs);

	printf("[REPENTOGON] Patching EntityPlayer::control_bombs for max bombs at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::ECX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxBombs)
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}


// Prevent RemoveCurseMistEffect from enforcing the coin/key/bomb limits when restoring consumables.
// The patches overwrite cmov's that would conditionally write the pickup limit instead of the actual value, and makes the actual value always written.
// If a higher amount than should normally be allowed is restored, we'll fix it on customcache evaluation anyway.
void PatchRemoveCurseMistEffect() {
	// Coins
	void* coinPatchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxCoins_CurseMist);
	void* coinExitAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxCoins_CurseMist_Exit);

	printf("[REPENTOGON] Patching RemoveCurseMistEffect for max coins at %p (exit @ %p)\n", coinPatchAddr, coinExitAddr);
	ASMPatch coinPatch;
	coinPatch.AddBytes(ByteBuffer().AddAny((char*)coinPatchAddr + 13, 0x6)) // ECX,dword ptr [EDI+?] (restores something that gets skipped)
		.AddBytes("\x89\xF0")  // MOV EAX,ESI
		.AddRelativeJump((char*)coinExitAddr);
	sASMPatcher.PatchAt((char*)coinPatchAddr, &coinPatch);

	// Keys
	void* keyPatchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxKeys_CurseMist);
	printf("[REPENTOGON] Patching RemoveCurseMistEffect for max keys at %p\n", keyPatchAddr);
	sASMPatcher.FlatPatch((char*)keyPatchAddr, "\x89\xC8\x90\x90\x90", 5);  // MOV EAX, ECX

	// Bombs
	void* bombPatchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::MaxBombs_CurseMist);
	printf("[REPENTOGON] Patching RemoveCurseMistEffect for max bombs at %p\n", bombPatchAddr);
	sASMPatcher.FlatPatch((char*)bombPatchAddr, "\x89\xC2\x90\x90\x90", 5);  // MOV EDX, EAX
}


// Tears cap (real)
void __stdcall TearsCapHook(Entity_Player* player, float* minimumFiredDelayOut) {
	*minimumFiredDelayOut = 5;  // Default value.

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (playerPlus && playerPlus->customCacheResults.find("tearscap") != playerPlus->customCacheResults.end()) {
		const double tearscap = playerPlus->customCacheResults["tearscap"];
		if (tearscap > 0) {
			// Convert to firedelay
			*minimumFiredDelayOut = (float)std::max((30.0 / tearscap) - 1.0, -0.75);
		}
	}
}
void PatchTearsCap() {
	void* patchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TearsCap);
	void* ebpOffsetAddr = (char*)patchAddr + 0x2;
	void* movEsiPlayerAddr = (char*)patchAddr + 0x23;

	printf("[REPENTOGON] Patching EvaluateItems for tears cap at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)ebpOffsetAddr, 0x4))  // lea ebx, [ebp+?]
		.Push(ASMPatch::Registers::EBX)  // float* minimumFiredDelay
		.AddBytes(ByteBuffer().AddAny((char*)movEsiPlayerAddr, 0x6))  // mov ESI,dword ptr [ebp+?]
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(TearsCapHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)patchAddr + 0xA);
	sASMPatcher.PatchAt(patchAddr, &patch);
}


// Stats Multiplier (ie Cracked Crown, Tainted Bethany)
void __stdcall StatMultiplierHook(Entity_Player* player, float* statMultiplierOut) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (playerPlus && playerPlus->customCacheResults.find("statmultiplier") != playerPlus->customCacheResults.end() && playerPlus->customCacheResults["statmultiplier"] > 0) {
		*statMultiplierOut = (float)playerPlus->customCacheResults["statmultiplier"];
	} else {
		*statMultiplierOut = GetVanillaStatMultiplier(player);
	}
}
void PatchStatMultiplier() {
	void* patchAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::StatMultiplier);
	void* ebpOffsetAddr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::StatMultiplier_Jump) + 0x4;
	void* jumpAddr = (char*)ebpOffsetAddr + 0x4;

	printf("[REPENTOGON] Patching EvaluateItems for stat multiplier at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)ebpOffsetAddr, 0x4))  // lea ebx, [ebp+?]
		.Push(ASMPatch::Registers::EBX)  // float* minimumFiredDelay
		.Push(ASMPatch::Registers::ECX)  // Entity_Player*
		.AddInternalCall(StatMultiplierHook)
		.RestoreRegisters(savedRegisters)
		.CopyRegister(ASMPatch::Registers::EAX, ASMPatch::Registers::ECX)
		.AddRelativeJump(jumpAddr);
	sASMPatcher.PatchAt(patchAddr, &patch);
}


void ASMPatchesForCustomCache() {
	PatchFamiliarGetMultiplierCallback();
	PatchAddCoins();
	PatchAddKeys();
	PatchAddBombs();
	PatchHudRenderCoins();
	PatchHudRenderKeys();
	PatchHudRenderBombs();
	PatchControlBombs();
	PatchRemoveCurseMistEffect();
	PatchTearsCap();
	PatchStatMultiplier();
}
