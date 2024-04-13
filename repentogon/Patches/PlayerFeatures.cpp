#pragma once
#include "CompletionTracker.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "JsonSavedata.h"

#include "../ImGuiFeatures/LogViewer.h"
#include "../MiscFunctions.h"

class PlayerFeatureHandler {
public:
	unordered_map<string, int> GreedDonationCounter;
	string jsonpath;
	int currentModdedPlayerID = 0;

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
		currentModdedPlayerID = playerType;
		string playerIdx = GetPlayerIdx(playerType);

		if (GreedDonationCounter.find(playerIdx) == GreedDonationCounter.end()) {
			GreedDonationCounter.insert({ playerIdx, 0 });
		}
		return GreedDonationCounter.at(playerIdx);
	}

	void IncreaseGreedDonationCoinCount(int amount)
	{
		if (currentModdedPlayerID < 41) {
			return;
		}
		string playerIdx = GetPlayerIdx(currentModdedPlayerID);

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
HOOK_METHOD(Entity_Player, GetGreedDonationBreakChance, ()->float)
{
	float jamChance = super();
	if (this->GetPlayerType() < 41) // vanilla characters
	{
		playerFeatureHandler.currentModdedPlayerID = 0;
		return jamChance;
	}
	int donatedCoins = playerFeatureHandler.GetGreedDonationCoinCount(this->GetPlayerType());
	jamChance = floor(0.2f * min(100.0f, exp(0.023f * donatedCoins) - 1.0f) + 0.5f);
	if (g_Game->IsHardMode()) {
		// Greedier Mode caps percentage at 1%
		jamChance = min(jamChance, 1.0f);
	}
	return jamChance;
}

// Custom handling for Greed donations with modded characters
HOOK_METHOD(PersistentGameData, IncreaseEventCounter, (int eEvent, int val)->void)
{
	super(eEvent, val);
	if (eEvent == 115) // EventCounter.GREED_DONATION_MACHINE_COUNTER
	{
		playerFeatureHandler.IncreaseGreedDonationCoinCount(val);
	}
}
