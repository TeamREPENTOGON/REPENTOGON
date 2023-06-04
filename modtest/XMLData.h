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

typedef unordered_map<string, string> XMLAttributes;

class XMLDataHolder {
public:
	unordered_map<int, XMLAttributes> nodes;
	unordered_map<string, int> byname;
	unordered_map<string, int> bynamemod;
	unordered_map<string, int> bymod;
	int maxid;

	XMLAttributes GetNodeByName(string name) {
		return this->nodes[this->byname[name]];
	}
	XMLAttributes GetNodeByNameMod(string name) {
		return this->nodes[this->bynamemod[name]];
	}
	XMLAttributes GetNodesByMod(string name) {
		return this->nodes[this->bynamemod[name]];
	}
};


class XMLMod: public XMLDataHolder {
public:
	unordered_map<string, int> byid;
	unordered_map<string, int> bydirectory;
	unordered_map<string, int> byfullpath;
	unordered_map<string, int> byfolder;

	unordered_map<int, ModEntry*> modentries;
	unordered_map<string, int> players;
	unordered_map<string, int> entities;
	unordered_map<string, int> items;
	unordered_map<string, int> trinkets;
	unordered_map<string, int> cards;
	unordered_map<string, int> pills;
	unordered_map<string, int> musictracks;
};




class XMLMusic: public XMLDataHolder {
public:
	XMLMusic(){
		this->maxid = 118;//last vanilla music track, for now I need to do this here, may scrap it later and fully automate it
	}	
};

class XMLItem : public XMLDataHolder {

};

class XMLTrinket : public XMLDataHolder {
public:
	unordered_map<string, int> bypickup;
};

class XMLCard : public XMLDataHolder {
public:
	unordered_map<string, int> bypickup;
};

class XMLPill : public XMLDataHolder {
public:
	unordered_map<string, int> bypickup;
};

class XMLPlayer : public XMLDataHolder {
};

class XMLEntity {
public:
	unordered_map<tuple<int, int, int>, XMLAttributes> nodes; //idx is type-var-sub vector
	unordered_map<string, tuple<int, int, int>> byname;
	unordered_map<string, tuple<int, int, int>> bynamemod;
	unordered_map<string, tuple<int, int, int>> bytype;
	unordered_map<tuple<int, int, int>, tuple<int, int, int>> bytypevar;

	XMLAttributes GetNodeByName(string name) {
		return this->nodes[this->byname[name]];
	}
	XMLAttributes GetNodeByNameMod(string name) {
		return this->nodes[this->bynamemod[name]];
	}
	XMLAttributes GetNodesByMod(string name) {
		return this->nodes[this->bynamemod[name]];
	}
	XMLAttributes GetNodesByTypeVarSub(int type,int var, int sub,bool strict ) {
		tuple idx = { type, var, sub };
		XMLAttributes none;
		if (this->nodes.count({ type, var, sub }) > 0) {
			return this->nodes[{ type, var, sub }];
		}
		else if (strict) {
			return none;
		}
		else if (this->nodes.count({ type, var, 0 })) {
			return this->nodes[{ type, var, 0 }];
		}
		else if (this->nodes.count({ type, 0, 0 })) {
			return this->nodes[{ type, 0, 0 }];
		}
		return none;
	}
	int maxid;
};


struct XMLData {
	XMLPlayer* PlayerData = new XMLPlayer();
	XMLEntity* EntityData = new XMLEntity();
	XMLItem* ItemData = new XMLItem();
	XMLTrinket* TrinketData = new XMLTrinket();
	XMLMusic* MusicData = new XMLMusic();
	XMLPill* PillData = new XMLPill();
	XMLCard* CardData = new XMLCard();
	XMLMod* ModData = new XMLMod();
};
extern XMLData XMLStuff;

#endif