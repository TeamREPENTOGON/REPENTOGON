#ifndef XMLDATA_H
#define XMLDATA_H

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <cctype>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"
using namespace std;

//hashing thingy for tuples by whoever fed ChatGPT + some edits from me, lol
template<>
struct hash<tuple<int, int, int>> {
	size_t operator()(const tuple<int, int, int>& key) const {
		size_t hash1 = hash<int>{}(get<0>(key));
		size_t hash2 = hash<int>{}(get<1>(key));
		size_t hash3 = hash<int>{}(get<2>(key));
		size_t seed = 0;
		std::hash<int> hasher;
		seed ^= hasher(hash1) + 0x9e3779b9 + (seed << 6) + (seed >> 2); //These magic numbers are apparently a thing
		seed ^= hasher(hash2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(hash3) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};
//hashing thingy for tuples by whoever fed ChatGPT + some edits from me, lol

struct XMLMod {
	unordered_map<int, unordered_map<string, string>> mods; //idx is loadorder
	unordered_map<string, int> modbyname; //tainteds get added a "-Tainted-" at the end as an index
	unordered_map<string, int> modbyid;
	unordered_map<string, int> modbydirectory;
	unordered_map<string, int> modbyfullpath;
	unordered_map<string, int> modbyfolder;

	unordered_map<int, ModEntry*> modentries;
	unordered_map<string, int> modplayers;
	unordered_map<string, int> modentities;
	unordered_map<string, int> modcards;
	unordered_map<string, int> modpills;
	unordered_map<string, int> modmusictracks;
	int nomods;
};

typedef unordered_map<string, string> XMLAttributes;


struct XMLMusic {
	unordered_map<int, XMLAttributes> tracks;
	unordered_map<string, int> musicbyname;
	unordered_map<string, int> musicbynamemod;
	unordered_map<string, int> musicbymod;
	int maxid = 118; //last vanilla music track, for now I need to do this here, may scrap it later and fully automate it
};

struct XMLCard {
	unordered_map<int, XMLAttributes> cards;
	unordered_map<string, int> cardbyname;
	unordered_map<string, int> cardbynamemod;
	unordered_map<string, int> cardbymod;
	unordered_map<string, int> cardbypickup;
	int maxid;
};

struct XMLPill {
	unordered_map<int, XMLAttributes> pills;
	unordered_map<string, int> pillbyname;
	unordered_map<string, int> pillbynamemod;
	unordered_map<string, int> pillbymod;
	unordered_map<string, int> pillbypickup;
	int maxid;
};

struct XMLPlayer {
	unordered_map<int, XMLAttributes> players;
	unordered_map<string, int> playerbyname;
	unordered_map<string, int> playerbynamemod;
	unordered_map<string, int> playerbymod;
	int maxid;
};

struct XMLEntity {
	unordered_map<tuple<int,int,int>, XMLAttributes> entities; //idx is type-var-sub vector
	unordered_map<string, tuple<int, int, int>> entitybyname;
	unordered_map<string, tuple<int, int, int>> entitybynamemod;
	unordered_map<string, tuple<int, int, int>> entitybytype;
	unordered_map<tuple<int, int, int>, tuple<int, int, int>> entitybytypevar;
	int maxid;
};


struct XMLData {
	XMLPlayer PlayerData;
	XMLEntity EntityData;
	XMLMusic MusicData;
	XMLPill PillData;
	XMLCard CardData;
	XMLMod ModData;
};
extern XMLData XMLStuff;

#endif