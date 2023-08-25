#ifndef ACHIEVSTUFF_H
#define ACHIEVSTUFF_H

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>
#include <Vector>

#include <lua.hpp>
#include "LuaCore.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "XMLData.h"
#include "CompletionTracker.h"

using namespace std;

extern unordered_map<string, int > Achievements; //1 unlocked != 1 locked
extern unordered_map<int, unordered_map<int,vector<int>> > CompletionMarkListeners;
extern unordered_map<int, unordered_map<int, vector<int>> > EventCounterListeners;
extern unordered_map<tuple<int, int, int>, vector<int>> BossDeathListeners;


extern int toint(string str);

inline void IncreaseAchievementCounter(int achievementid) {
	string* achievement = &XMLStuff.AchievementData->nodes[achievementid]["name"];
	if (Achievements[*achievement] != 1) {
		Achievements[*achievement] += 1;
		if (Achievements[*achievement] == 1) {
			g_Manager->GetPersistentGameData()->TryUnlock(achievementid);
		}
	}
}

inline void AddMarkTracker(int achievementid, int marktype, int charaid = 0) {
	CompletionMarkListeners[marktype][charaid].push_back(achievementid);
}

inline void RunTrackersForMark(int marktype, int charaid = 0) {
	for each (int achievementid in CompletionMarkListeners[marktype][charaid]) {
		IncreaseAchievementCounter(achievementid);
	}
	if (charaid > 0) {
		for each (int achievementid in CompletionMarkListeners[marktype][0]) {
			IncreaseAchievementCounter(achievementid);
		}
	}
}

inline void AddBossDeathTracker(int achievementid, tuple<int, int, int>* boss, int charaid = 0) {
	BossDeathListeners[*boss].push_back(achievementid);
}

inline void RunTrackersForBossDeath(tuple<int, int, int>* boss) {
	for each (int achievementid in BossDeathListeners[*boss]) {
		IncreaseAchievementCounter(achievementid);
	}
}

inline void AddEventCounterTracker(int eventid, int achievementid,int charaid = 0) {
	EventCounterListeners[eventid][charaid].push_back(achievementid);
}

inline void RunTrackersForEventCounter(int eventid, int charaid = 0) {
	for each (int achievementid in EventCounterListeners[eventid][charaid]) {
		IncreaseAchievementCounter(achievementid);
	}
}


inline void InitAchievs() {
	for each (auto & att in XMLStuff.AchievementData->nodes)
	{
		if (att.first > 638) {
			XMLAttributes node = att.second;
			Achievements[node["name"]] = 0;
			if (node.count("count") > 0) {
				Achievements[node["name"]] = -(toint(node["count"]) - 1);
			}

			int completionchara = 0;
			if (node.count("characondition") > 0) {
				completionchara = XMLStuff.PlayerData->byname[node["characondition"]];
			}
			if (node.count("markcondition") > 0) {
				printf("[Achiev] Mark tracker for '%s' with condition %s \n", node["characondition"].c_str(), node["markcondition"].c_str());
				AddMarkTracker(att.first, reversemarksenum[node["markcondition"]], completionchara);
			}


		}
	}
}

#endif