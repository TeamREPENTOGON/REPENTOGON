#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "EntityPlus.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"


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


void UpdateMaxCoinsKeysBombs(const std::string& customcache, const double newMaxCoins) {
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

	*max = (int)std::clamp(newMaxCoins, 0.0, (double)INT_MAX);;

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
	const bool maxCoinsKeysBombs = customcache == "maxcoins" || customcache == "maxkeys" || customcache == "maxbombs";
	
	float initialValue = 0.0;

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

		if (customcache == "maxcoins" && g_Game->_playerManager.FirstCollectibleOwner(COLLECTIBLE_DEEP_POCKETS, nullptr, true)) {
			initialValue = 999;
		}
		else {
			initialValue = 99;
		}
	} else if (customcache == "familiarmultiplier") {
		// Familiar multiplier has special treatment, and does not go through the usual callback.
		// Still otherwise implemented as a customcache because it is "triggered" in the same ways.
		// When this cache is triggered on the player, wipe the cached multipliers from all their
		// familiars, indirectly triggering MC_EVALUATE_FAMILIAR_MULTIPLIER instead.
		InvalidateCachedFamiliarMultipliers(player);
		return;
	}
	
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (!playerPlus) {
		// Aaah!
		return;
	}

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
			const double resultValue = lua_tonumber(L, -1);

			playerPlus->customCacheResults[customcache] = resultValue;

			if (maxCoinsKeysBombs) {
				UpdateMaxCoinsKeysBombs(customcache, resultValue);
			}

			return;
		}
	}

	// Callback did not run, or did not return a valid value.

	if (maxCoinsKeysBombs) {
		UpdateMaxCoinsKeysBombs(customcache, initialValue);
	}

	playerPlus->customCacheResults.erase(customcache);
}

void TriggerCustomCache(Entity_Player* player, const std::set<std::string>& customcaches, const bool immediate) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (immediate) {
		for (const std::string& customcache : customcaches) {
			playerPlus->customCacheTags.erase(customcache);
			RunEvaluateCustomCacheCallback(player, customcache);
		}
	} else if (playerPlus) {
		for (const std::string& customcache : customcaches) {
			playerPlus->customCacheTags.insert(customcache);
		}
		player->_temporaryeffects._shouldEvaluateCache = true;
	}
}

void TriggerNullCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.NullItemData->GetCustomCache(id), immediate);
}

void TriggerCollectibleCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.ItemData->GetCustomCache(id), immediate);
}

void TriggerTrinketCustomCache(Entity_Player* player, const int id, const bool immediate) {
	TriggerCustomCache(player, XMLStuff.TrinketData->GetCustomCache(id), immediate);
}

void TriggerItemCustomCache(Entity_Player* player, ItemConfig_Item* item, const bool immediate) {
	if (item->type == 0) {  // Null
		TriggerNullCustomCache(player, item->id, immediate);
	}
	else if (item->type == 2) {  // Trinket
		TriggerTrinketCustomCache(player, item->id, immediate);
	}
	else {  // Collectible (Passive/Active/Familiar)
		TriggerCollectibleCustomCache(player, item->id, immediate);
	}
}

// Re-evaluate custom caches that appeared in XML data if EvaluateItems is run for CacheFlag.CACHE_ALL.
HOOK_METHOD_PRIORITY(Entity_Player, EvaluateItems, -1, () -> void) {
	const int cacheFlagAll = (1 << 16) - 1;  // CacheFlag.CACHE_ALL
	const bool evaluateAll = (this->_cacheFlags & cacheFlagAll) == cacheFlagAll;

	super();

	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this);
	if (playerPlus) {
		if (evaluateAll) {
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
	}
}

// Collectibles (both real and via wisps) trigger cache evaluations immediately when added or removed.

HOOK_METHOD_PRIORITY(Entity_Player, AddCollectible, -1, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	super(type, charge, firsttime, slot, vardata);
	TriggerCollectibleCustomCache(this, type, true);
}

HOOK_METHOD_PRIORITY(Entity_Familiar, WispInit, -1, () -> void) {
	super();

	if (this->_variant == 237 && this->_player) {
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
	SigScan scanner("5e5b8be55dc3????????????????????????????????????????????????????????558bec6aff68????????64a1????????5081ec28030000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
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
	SigScan scanner("e8????????8bb5????????85c0ba");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~(ASMPatch::SavedRegisters::Registers::EAX | ASMPatch::SavedRegisters::Registers::ECX), true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)  // Pop inputs to overridden FirstCollectibleOwner
		.Pop(ASMPatch::Registers::EAX)
		.Pop(ASMPatch::Registers::EAX)
		.AddBytes(ByteBuffer().AddAny((char*)addr + 0x5, 0x6))  // Restore a thing
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetHudCoinsStringFormat)
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX)
		.MoveImmediate(ASMPatch::Registers::EAX, 0)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x1A);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchAddCoins() {
	SigScan scanner("e8????????f7d8c745??00000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

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
	SigScan scanner("83f8638945??56");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::ESI, true);
	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ESI)
		.AddBytes("\x8D\x75\xFC")  // lea esi, [ebp - 0x4]
		.Push(ASMPatch::Registers::ESI)
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxKeys)
		.CopyRegister(ASMPatch::Registers::ESI, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x39\xF0")  // cmp eax,esi
		.AddBytes("\x89\x45\x08")  // mov dword ptr [ebp + 0x8], eax
		.AddBytes("\x89\x75\xFC")  // mov dword ptr [ebp - 0x4], esi
		.Pop(ASMPatch::Registers::ESI)
		.AddRelativeJump((char*)addr + 0x11);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchHudRenderKeys() {
	// wow
	SigScan scanner("68????????6a10f30f1145??f30f1085????????f30f5885????????68????????f30f1145??e8????????83c4106a006a0083ec108bcc6affe8????????f30f1045??83ec10f30f5805????????8b8d????????c74424??0000803fc74424??0000803ff30f114424??f30f1045??f30f5805????????f30f11042468????????e8????????807d??00");
	scanner.Scan();
	void* addr = scanner.GetAddress();

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
	SigScan scanner("c74424??6300000083f863");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EDX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetMaxBombs)
		.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x89\x54\x24\x08")  // mov dword ptr [esp + 0x8], edx
		.AddBytes("\x39\xD0")  // cmp eax,edx
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchHudRenderBombs() {
	// wow
	SigScan scanner("68????????6a10f30f1145??f30f1085????????f30f5885????????68????????f30f1145??e8????????83c4106a006a0083ec108bcc6affe8????????f30f1045??83ec10f30f5805????????8b8d????????c74424??0000803fc74424??0000803ff30f114424??f30f1045??f30f5805????????f30f11042468????????e8????????f30f1045??f30f5c85????????f30f1185");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(GetHudBombsStringFormat)
		.RestoreRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}


// Prevent RemoveCurseMistEffect from enforcing the coin/key/bomb limits when restoring consumables.
// The patches overwrite cmov's that would conditionally write the pickup limit instead of the actual value, and makes the actual value always written.
// If a higher amount than should normally be allowed is restored, we'll fix it on customcache evaluation anyway.
void PatchRemoveCurseMistEffect() {
	SigScan coinScanner("6a015168a0010000");
	coinScanner.Scan();
	void* coinAddr = coinScanner.GetAddress();
	ASMPatch coinPatch;
	coinPatch.AddBytes("\x8B\x8F\xAC\x12").AddZeroes(2)  // ECX,dword ptr [EDI + 0x12ac] (restores something that gets skipped)
		.AddBytes("\x89\xF0")  // MOV EAX,ESI
		.AddRelativeJump((char*)coinAddr + 0x24);
	sASMPatcher.PatchAt((char*)coinAddr, &coinPatch);

	SigScan keyScanner("3bc80f4cc18987????????8b87????????0387????????3bc6");
	keyScanner.Scan();
	void* keyAddr = keyScanner.GetAddress();
	sASMPatcher.FlatPatch((char*)keyAddr, "\x89\xC8\x90\x90\x90", 5);  // MOV EAX, ECX

	SigScan bombScanner("3bc60f4cd080bf????????00");
	bombScanner.Scan();
	void* bombAddr = bombScanner.GetAddress();
	sASMPatcher.FlatPatch((char*)bombAddr, "\x89\xC2\x90\x90\x90", 5);  // MOV EDX, EAX
}


void ASMPatchesForCustomCache() {
	PatchFamiliarGetMultiplierCallback();
	PatchAddCoins();
	PatchAddKeys();
	PatchAddBombs();
	PatchHudRenderCoins();
	PatchHudRenderKeys();
	PatchHudRenderBombs();
	PatchRemoveCurseMistEffect();
}
