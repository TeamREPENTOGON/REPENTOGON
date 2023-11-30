#ifndef SESSIONSTUFF_H
#define SESSIONSTUFF_H

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
#include <support/CPPUtils.cpp>

using namespace std;

extern unordered_map < string, vector<int>> PlayerFormTracker; //playerid<transformationcounts>
extern unordered_map <string, vector<int> > TagPlayerForms;//<tag,transformations>
extern unordered_map <int, vector<string> > TagsForItem;//<itemid,tags>

inline bool CheckPlayerform(Entity_Player* player, int playerformid) {
	string playerid = to_string(player->GetCollectibleRNG(235)->_seed); //235 invalid collectible that still has an RNG object
	return (PlayerFormTracker[playerid][playerformid] > 3);
}


inline void IncreasePlayerFormCounters(Entity_Player* player,string tag,int amount = 1) {
	printf("Checking forms for Tag (%s) \n", tag.c_str());
	string playerid = to_string(player->GetCollectibleRNG(235)->_seed); //235 invalid collectible that still has an RNG object
	bool transformed = false;
	if (TagPlayerForms.find(tag) != TagPlayerForms.end()) {
		for each (int playerformid  in TagPlayerForms[tag]){
			//if (PlayerFormTracker[playerid].find(playerformid) != PlayerFormTracker[playerid].end()) {
			if (PlayerFormTracker.find(playerid) == PlayerFormTracker.end()) { PlayerFormTracker[playerid] = vector<int>(XMLStuff.PlayerFormData->maxid + 1,0); }
				PlayerFormTracker[playerid][playerformid] += amount;
				printf("Form Increased to: %d \n", PlayerFormTracker[playerid][playerformid]);
				if (PlayerFormTracker[playerid][playerformid] < 0) { PlayerFormTracker[playerid][playerformid] = 0; }
				if (((PlayerFormTracker[playerid][playerformid] - amount) < 3) && (PlayerFormTracker[playerid][playerformid] >= 3)) {
					XMLAttributes att = XMLStuff.PlayerFormData->nodes[playerformid];
					g_Game->GetHUD()->ShowItemText(const_cast<char*>(att["name"].c_str()), "", false, false);					
					player->AddCacheFlags(1);
					player->AddNullCostume(1);
					transformed = true;
					//apply transformation
				}
				else if (((PlayerFormTracker[playerid][playerformid] - amount) > 3)) {
					/* string costume = XMLStuff.PlayerFormData->nodes[playerformid]["costume"];
					if (toint(costume) > 0) {
					
					}
					else {
						
					}
					for each (CostumeSpriteDesc desc in player->_costumeSpriteDescs) {
						if (desc._sprite._filename = ) {
						
						}
					}
					*/
				}
			//}
		}
		if (transformed) {
			g_Manager->_sfxManager.Play(32, 1, 1, false, 1, 1);
			player->EvaluateItems();
		}
	}
}

inline void IncreasePlayerFormCountersForItem(Entity_Player* player, int itemid, int amount = 1) {
	if (TagsForItem.find(itemid) == TagsForItem.end()) {
		string tags = XMLStuff.ItemData->nodes[itemid]["tags"];
		if (XMLStuff.ItemData->nodes[itemid].find("customtags") != XMLStuff.ItemData->nodes[itemid].end()) {
			tags += " " + XMLStuff.ItemData->nodes[itemid]["customtags"];
		}
		TagsForItem[itemid] = antlrcpp::split(tags, " ", 0);
	}
	for each (string tag in TagsForItem[itemid]) {
		IncreasePlayerFormCounters(player, tag, amount);
	}
}

inline void BuildFormArrays() {
	for each (auto node in XMLStuff.PlayerFormData->nodes) {
		
			XMLAttributes att = node.second;
			if (node.first > 14) {
				if (att.find("tags") != att.end()) {
					vector<string> tags = antlrcpp::split(att["tags"], " ", 0);
					for each (string tag in tags) {
						printf("playerform tracker added for (%s)\n", tag.c_str());
						TagPlayerForms[tag].push_back(node.first);
					}
				}
			}
			if (att.find("customtags") != att.end()) {
				vector<string> tagsc = antlrcpp::split(att["customtags"], " ", 0);
				for each (string ctag in tagsc) {
					printf("playerform tracker added for (%s)\n", ctag.c_str());
					TagPlayerForms[ctag].push_back(node.first);
				}
			}
	}
}



#endif