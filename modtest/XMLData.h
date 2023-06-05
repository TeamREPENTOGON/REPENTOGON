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
typedef unordered_map<int, XMLAttributes> XMLNodes;
typedef unordered_map<string, std::vector <XMLAttributes>> XMLChilds;
typedef unordered_map<int, XMLChilds> XMLKinder;
typedef unordered_map<tuple<int, int, int>, XMLChilds> XMLEntityKinder;
typedef unordered_map<string, int> XMLNodeIdxLookup;


inline string stringlower(char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}


class XMLDataHolder {
public:
	XMLNodes nodes;
	XMLKinder childs;
	XMLNodeIdxLookup byname;
	XMLNodeIdxLookup bynamemod;
	XMLNodeIdxLookup bymod;
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

	void ProcessChilds(xml_node<char>* parentnode, int id) {

		for (xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
			XMLAttributes child;
			for (xml_attribute<>* attr = auxnodebabe->first_attribute(); attr; attr = attr->next_attribute())
			{
				child[stringlower(attr->name())] = string(attr->value());
			}
			this->childs[id][auxnodebabe->name()].push_back(child);
		}
	}

};


class XMLMod: public XMLDataHolder {
public:
	XMLNodeIdxLookup byid;
	XMLNodeIdxLookup bydirectory;
	XMLNodeIdxLookup byfullpath;
	XMLNodeIdxLookup byfolder;

	unordered_map<int, ModEntry*> modentries;
	XMLNodeIdxLookup players;
	XMLNodeIdxLookup entities;
	XMLNodeIdxLookup items;
	XMLNodeIdxLookup trinkets;
	XMLNodeIdxLookup cards;
	XMLNodeIdxLookup pills;
	XMLNodeIdxLookup musictracks;
	XMLNodeIdxLookup sounds;
};




class XMLMusic: public XMLDataHolder {
public:
	XMLMusic(){
		this->maxid = 118;//last vanilla music track, for now I need to do this here, may scrap it later and fully automate it
	}	
};

class XMLSound : public XMLDataHolder {
public:
	XMLSound() {
		this->maxid = 832;//last vanilla music track, for now I need to do this here, may scrap it later and fully automate it		
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
	XMLEntityKinder childs;
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

	void ProcessChilds(xml_node<char>* parentnode, tuple<int, int, int> id) {

		for (xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
			XMLAttributes child;
			for (xml_attribute<>* attr = auxnodebabe->first_attribute(); attr; attr = attr->next_attribute())
			{
				child[stringlower(attr->name())] = string(attr->value());
			}
			this->childs[id][auxnodebabe->name()].push_back(child);
		}
	}

	int maxid;
};


struct XMLData {
	XMLPlayer* PlayerData = new XMLPlayer();
	XMLEntity* EntityData = new XMLEntity();
	XMLItem* ItemData = new XMLItem();
	XMLTrinket* TrinketData = new XMLTrinket();
	XMLMusic* MusicData = new XMLMusic();
	XMLSound* SoundData = new XMLSound();
	XMLPill* PillData = new XMLPill();
	XMLCard* CardData = new XMLCard();
	XMLMod* ModData = new XMLMod();
};
extern XMLData XMLStuff;

#endif