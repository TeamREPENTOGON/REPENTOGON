#pragma once

#include "PLayerFeatures.h"

#include "CompletionTracker.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "JsonSavedata.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"

#include "../ImGuiFeatures/LogViewer.h"
#include "../MiscFunctions.h"

class PlayerFeatureHandler {
public:
	unordered_map<string, int> GreedDonationCounter;
	string jsonpath;

	void SavePlayerDataToJson()
	{
		rapidjson::Document doc;
		doc.SetObject();

		// Save Greed donation Counter
		ArrayToJson(&doc, "GreedDonationCounter", GreedDonationCounter);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		ofstream ofs(jsonpath);
		ofs << buffer.GetString() << std::endl;
		logViewer.AddLog("[REPENTOGON]", "Player Data saved to : % s \n", jsonpath.c_str());
	}

	void LoadPlayerDataFromJson()
	{
		GreedDonationCounter.clear();
		rapidjson::Document doc = GetJsonDoc(&jsonpath);
		if (!doc.IsObject()) {
			logViewer.AddLog("[REPENTOGON]", "No Player Data for saveslot in: %s \n", jsonpath.c_str());
			return;
		}
		JsonToArray(doc["GreedDonationCounter"], GreedDonationCounter);

		logViewer.AddLog("[REPENTOGON]", "Player Data loaded from: %s \n", jsonpath.c_str());
	}

	void LoadJson(const char* filePrefix, int slot)
	{
		int saveslot = 1;
		if (slot > 0) {
			saveslot = slot;
		}
		jsonpath = std::string(REPENTOGON::GetRepentogonDataPath());
		jsonpath.append("playerdata").append(to_string(saveslot)).append(".json");

		LoadPlayerDataFromJson();
	}

	string GetPlayerIdx(int playerid)
	{
		XMLAttributes playerdata = XMLStuff.PlayerData->GetNodeById(playerid);
		string idx = playerdata["sourceid"] + "-" + playerdata["name"];
		if (playerdata.count("bskinparent") > 0) {
			idx = idx + "-Tainted-";
		}
		return idx;
	}

	int GetGreedDonationCoinCount(int playerType)
	{
		if (playerType < NUM_PLAYER_TYPES) {
			return 0;
		}
		string playerIdx = GetPlayerIdx(playerType);

		if (GreedDonationCounter.find(playerIdx) == GreedDonationCounter.end()) {
			GreedDonationCounter.insert({ playerIdx, 0 });
		}
		return GreedDonationCounter.at(playerIdx);
	}

	void IncreaseGreedDonationCoinCount(int playerType, int amount)
	{
		if (playerType < NUM_PLAYER_TYPES) {
			return;
		}
		string playerIdx = GetPlayerIdx(playerType);

		if (GreedDonationCounter.find(playerIdx) == GreedDonationCounter.end()) {
			GreedDonationCounter.insert({ playerIdx, 0 });
		}
		GreedDonationCounter[playerIdx] += amount;
		SavePlayerDataToJson();
	}
};

static PlayerFeatureHandler playerFeatureHandler{};

HOOK_METHOD(Manager, SetSaveSlot, (unsigned int slot)->void)
{
	super(slot);
	playerFeatureHandler.LoadJson("playerdata", slot);
}

// Fix modded characters having 100% greed donation jam chance
void __stdcall GetGreedDonationBreakChanceForModdedCharacterTrampoline(const int playerType) {
	// This function will only get called for modded characters due to the placement of the patch.
	const int donatedCoins = playerFeatureHandler.GetGreedDonationCoinCount(playerType);
	float jamChance = floor(0.2f * min(100.0f, exp(0.023f * donatedCoins) - 1.0f) + 0.5f);
	if (g_Game->IsHardMode()) {
		// Greedier Mode caps percentage at 1%
		jamChance = min(jamChance, 1.0f);
	}
	__asm {
		movd xmm0, jamChance
	}
}
void PatchGetGreedDonationBreakChanceForModdedCharacters() {
	// Patches over where 99.9f would normally be placed in XMM0 for a modded character.
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GreedDonationBreakChanceForModdedCharacters);

	printf("[REPENTOGON] Patching GetGreedDonationBreakChance for modded characters at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | (ASMPatch::SavedRegisters::Registers::XMM_REGISTERS & ~ASMPatch::SavedRegisters::Registers::XMM0), true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX)  // PlayerType
		.AddInternalCall(GetGreedDonationBreakChanceForModdedCharacterTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

// Custom handling for Greed donations with modded characters
void __stdcall IncreaseGreedDonationCoinCountForModdedCharactersTrampoline(const int playerType) {
	playerFeatureHandler.IncreaseGreedDonationCoinCount(playerType, 1);
}
void PatchIncreaseGreedDonationCoinCountForModdedCharacters() {
	// Where the game checks if the PlayerType in EAX is a vanilla one.
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GreedDonationCoinCountForModdedCharacters);

	// Patch overrides the jump normally taken by modded characters that skips the per-player greed coin event counter.
	const int jumpOffset = 0x5 + *(int8_t*)((char*)addr + 0x4);

	printf("[REPENTOGON] Patching Greed Donation Machine coin count for modded characters at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes("\x83\xf8\x29")  // CMP EAX, 41
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JL, (char*)addr + 0x5)  // Continue normally for vanilla characters.
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // PlayerType
		.AddInternalCall(IncreaseGreedDonationCoinCountForModdedCharactersTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);  // Continue on for modded characters (skip vanilla character logic, as usual).
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_PRE_ADD_TRINKET (1014)
// This is here to hook into our AddSmeltedTrinketToPlayer function, as well as the vanilla AddTrinket.
// Btw we don't need a MC_POST_ADD_TRINKET, we have MC_POST_TRIGGER_TRINKET_ADDED bro
struct PreAddTrinketResult {
	int trinketID;
	bool firstTime;
	bool skip = false;
};

PreAddTrinketResult RunPreAddTrinket(Entity_Player* player, int trinketID, bool firstTime) {
	PreAddTrinketResult result{ trinketID, firstTime };

	const int callbackid = 1014;
	if (ItemConfig::IsValidTrinket(trinketID) && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua::LuaCaller caller(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults luaResult = caller.push(callbackid)
			.push(trinketID)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(trinketID)
			.push(firstTime)
			.call(1);

		if (!luaResult) {
			if (lua_isboolean(L, -1)) {
				result.skip = !lua_toboolean(L, -1);
			} else if (lua_isinteger(L, -1)) {
				result.trinketID = (int)lua_tointeger(L, -1);
			} else if (lua_istable(L, -1)) {
				lua_pushinteger(L, 1);
				lua_gettable(L, -2);
				if (lua_isinteger(L, -1))
					result.trinketID = (int)lua_tointeger(L, -1);
				lua_pop(L, 1);

				lua_pushinteger(L, 2);
				lua_gettable(L, -2);
				if (lua_isboolean(L, -1))
					result.firstTime = lua_toboolean(L, -1);
				lua_pop(L, 1);
			}
		}
	}

	return result;
}

HOOK_METHOD(Entity_Player, AddTrinket, (int trinketID, bool firstTime) -> void) {
	PreAddTrinketResult preCallbackResult = RunPreAddTrinket(this, trinketID, firstTime);
	if (!preCallbackResult.skip) {
		super(preCallbackResult.trinketID, preCallbackResult.firstTime);
	}
}

bool AddSmeltedTrinketToPlayer(Entity_Player* player, int trinketID, bool firstTime) {
	if (ItemConfig::IsValidTrinket(trinketID)) {
		PreAddTrinketResult preCallbackResult = RunPreAddTrinket(player, trinketID, firstTime);
		if (preCallbackResult.skip) {
			return false;
		} else {
			trinketID = preCallbackResult.trinketID;
			firstTime = preCallbackResult.firstTime;
		}
		
		const int actualTrinketID = trinketID & TRINKET_ID_MASK;
		if (trinketID & TRINKET_GOLDEN_FLAG) {
			player->_smeltedTrinkets[actualTrinketID]._goldenTrinketNum++;
		}
		else {
			player->_smeltedTrinkets[actualTrinketID]._trinketNum++;
		}

		player->TriggerTrinketAdded(trinketID, firstTime);

		History_HistoryItem* historyItem = new History_HistoryItem((TrinketType)trinketID, g_Game->_stage, g_Game->_stageType, g_Game->_room->_roomType, 0);
		player->GetHistory()->AddHistoryItem(historyItem);

		delete(historyItem);

		player->InvalidateCoPlayerItems();

		ItemConfig_Item* config = g_Manager->GetItemConfig()->GetTrinket(actualTrinketID);
		if (config && config->addCostumeOnPickup) {
			player->AddCostume(config, false);
		}

		return true;
	}
	return false;
}
