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

template<>
struct hash<tuple<int, int>> {
	size_t operator()(const tuple<int, int>& key) const {
		const auto& [x, y] = key;
		return std::hash<int>()(x) ^ std::hash<int>()(y);
	}
};
//hashing thingy for tuples by whoever fed ChatGPT + some edits from me, lol

typedef unordered_map<string, string> XMLAttributes;
typedef unordered_map<int, XMLAttributes> XMLNodes;
typedef unordered_map<string, std::vector <XMLAttributes>> XMLChilds;
typedef unordered_map<int, XMLChilds> XMLKinder;
typedef unordered_map<tuple<int, int, int>, XMLChilds> XMLEntityKinder;
typedef unordered_map<string, int> XMLNodeIdxLookup;
typedef unordered_map<string, vector<int>> XMLNodeIdxLookupMultiple;


inline string stringlower(char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

inline string stringlower(const char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

class XMLNodeTable {
public:
	XMLNodeIdxLookupMultiple tab;
	vector<int> Get(const string &index) {
		if (tab.find(index) != tab.end()) {
			return tab[index];
		}
	}
	void Set(const string &index,int id) {
		if (tab.find(index) != tab.end()) {
			vector<int> v;
			v.push_back(id);
			tab[index] = v;
		}else{
			tab[index].push_back(id);
		}
	}
};


class XMLDataHolder {
public:
	XMLNodes nodes;
	XMLKinder childs;
	XMLNodeIdxLookup byname;
	XMLNodeIdxLookup bynamemod;
	XMLNodeIdxLookupMultiple bymod;
	XMLNodeIdxLookup byrelativeid;
	XMLNodeTable byfilepathmulti;
	int maxid;
	int defmaxid;

	void Clear() {
		nodes.clear();
		childs.clear();
		byname.clear();
		bynamemod.clear();
		bymod.clear();
		byrelativeid.clear();
		byfilepathmulti.tab.clear();
		maxid = defmaxid;
	}

	void ClearByPath(const string &path) {
		if (byfilepathmulti.tab.find(path) != byfilepathmulti.tab.end()) {
			for each (int idx in byfilepathmulti.Get(path)) {
				XMLAttributes node = nodes[idx];
				if (byname[node["name"]] == idx) { byname.erase(node["name"]); }
				if (bynamemod[node["name"] + node["sourceid"]] == idx) { bynamemod.erase(node["name"] + node["sourceid"]); }
				bymod.erase(node["sourceid"]);
				byrelativeid.erase(node["sourceid"] + node["name"]);
				nodes.erase(idx);
				childs.erase(idx);
				maxid -= 1;
			}
			byfilepathmulti.tab[path].clear();
		}
	}

	XMLAttributes GetNodeByName(const string &name) {
		if (this->byname.find(name) == this->byname.end()) { return XMLAttributes(); }
		return this->nodes[this->byname[name]];
	}
	XMLAttributes GetNodeByNameMod(const string &name) {
		if (this->bynamemod.find(name) == this->bynamemod.end()) { return XMLAttributes(); }
		return this->nodes[this->bynamemod[name]];
	}
	XMLAttributes GetNodesByMod(const string &name) {
		if (this->bynamemod.find(name) == this->bynamemod.end()) { return XMLAttributes(); }
		return this->nodes[this->bynamemod[name]];
	}

	void ProcessChilds(xml_node<char>* parentnode, int id) {
		ProcessChilds(parentnode, id, "");
	}

	void ProcessChilds(xml_node<char>* parentnode, int id, string lastmodid) {

		for (xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
			XMLAttributes child;
			for (xml_attribute<>* attr = auxnodebabe->first_attribute(); attr; attr = attr->next_attribute())
			{
				child[stringlower(attr->name())] = string(attr->value());
			}
			if (lastmodid.length() > 0) {
				child["sourceid"] = lastmodid;
			}
			this->childs[id][stringlower(auxnodebabe->name())].push_back(child);
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
	XMLNodeIdxLookup nullitems;
	XMLNodeIdxLookup itempools;
	XMLNodeIdxLookup trinkets;
	XMLNodeIdxLookup cards;
	XMLNodeIdxLookup pills;
	XMLNodeIdxLookup musictracks;
	XMLNodeIdxLookup sounds;
	XMLNodeIdxLookup challenges;
	XMLNodeIdxLookup nightmares;
	XMLNodeIdxLookup costumes;
	XMLNodeIdxLookup nullcostumes;
	XMLNodeIdxLookup bombcostumes;
	XMLNodeIdxLookup wisps;
	XMLNodeIdxLookup wispcolors;
	XMLNodeIdxLookup locusts;
	XMLNodeIdxLookup locustcolors;
	XMLNodeIdxLookup curses;
	XMLNodeIdxLookup recipes;
	XMLNodeIdxLookup bossportraits;
	XMLNodeIdxLookup cutscenes;
	XMLNodeIdxLookup stages;
	XMLNodeIdxLookup backdrops;
	XMLNodeIdxLookup achievements;

	void Clear() {
		nodes.clear();
		modentries.clear();
		byid.clear();
		byname.clear();
		byfolder.clear();
		bydirectory.clear();
		byfullpath.clear();
		players.clear();
		entities.clear();
		items.clear();
		itempools.clear();
		trinkets.clear();
		cards.clear();
		pills.clear();
		musictracks.clear();
		sounds.clear();
		challenges.clear();
		nightmares.clear();
		costumes.clear();
		nullcostumes.clear();
		bombcostumes.clear();
		wisps.clear();
		wispcolors.clear();
		locusts.clear();
		locustcolors.clear();
		curses.clear();
		recipes.clear();
		cutscenes.clear();
		stages.clear();
		backdrops.clear();
		achievements.clear();
		maxid = 0;
	
	}

};




class XMLMusic: public XMLDataHolder {
public:
	XMLMusic(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
};

class XMLSound : public XMLDataHolder {
public:
	XMLSound(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
};


class XMLAchievement : public XMLDataHolder {
};

class XMLRecipe : public XMLDataHolder {

};

class XMLWisp : public XMLDataHolder {

};

class XMLWispColor : public XMLDataHolder {

};


class XMLBossPortrait : public XMLDataHolder {

};

class XMLLocust : public XMLDataHolder {

};

class XMLLocustColor : public XMLDataHolder {

};

class XMLItem : public XMLDataHolder {
	
};

class XMLItemPools : public XMLDataHolder {

};

class XMLCutscene : public XMLDataHolder {

};

class XMLBossPools : public XMLDataHolder {

};

class XMLNightmare : public XMLDataHolder {

};

class XMLCostume : public XMLDataHolder {
	
};

class XMLBombCostume : public XMLDataHolder {
public:
	XMLBombCostume(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
};

class XMLNullCostume : public XMLDataHolder {
public:
	XMLNullCostume(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
};

class XMLGeneric : public XMLDataHolder {
public:
	XMLGeneric(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
};

class XMLChallenge : public XMLDataHolder {

};

class XMLCurse : public XMLDataHolder {
public:
	XMLCurse(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}
	void Clear() {
			for each (auto& n in nodes) {;
				XMLAttributes node = n.second;
				if (strcmp(node["sourceid"].c_str(), "BaseGame") != 0) {
					int idx = n.first;
					if (byname[node["name"]] == idx) { byname.erase(node["name"]); }
					if (bynamemod[node["name"] + node["sourceid"]] == idx) { bynamemod.erase(node["name"] + node["sourceid"]); }
					bymod.erase(node["sourceid"]);
					byrelativeid.erase(node["sourceid"] + node["name"]);
					nodes.erase(idx);
					childs.erase(idx);
					maxid = maxid / 2 ;
				}
			}
		}
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

class XMLStage : public XMLDataHolder {
public:
	unordered_map<tuple<int,int>, int> bystagealt;	
	unordered_map<int, int> bybasestage;
};

class XMLPlayer : public XMLDataHolder {
};

class XMLBackdrop : public XMLDataHolder {
};

class XMLEntity {
public:
	unordered_map<tuple<int, int, int>, XMLAttributes> nodes; //idx is type-var-sub vector
	XMLEntityKinder childs;
	unordered_map<int, tuple<int, int, int>> byorder;
	unordered_map<string, tuple<int, int, int>> byname;
	unordered_map<string, tuple<int, int, int>> bynamemod;
	unordered_map<string, tuple<int, int, int>> bytype;
	unordered_map<string, tuple<int, int, int>> bybossid;
	XMLNodeIdxLookup bymod;
	unordered_map<tuple<int, int, int>, tuple<int, int, int>> bytypevar;
	// Holds the contents of an entity's "customtags" attribute, converted to lowercase and parsed into a set.
	unordered_map<tuple<int, int, int>, set<string>> customtags;

	void Clear() {
		nodes.clear();
		childs.clear();
		byname.clear();
		bynamemod.clear();
		bymod.clear();
		bytypevar.clear();
		customtags.clear();
		maxid = 0;
	}

	XMLAttributes GetNodeByName(const string &name) {
		return this->nodes[this->byname[name]];
	}
	XMLAttributes GetNodeByNameMod(const string &name) {
		return this->nodes[this->bynamemod[name]];
	}
	XMLAttributes GetNodesByMod(const string &name) {
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
		else if (this->nodes.count({ type, var, 0 }) > 0) {
			return this->nodes[{ type, var, 0 }];
		}
		else if (this->nodes.count({ type, 0, 0 }) > 0) {
			return this->nodes[{ type, 0, 0 }];
		}
		return none;
	}

	const set<string>& GetCustomTags(int type, int var, int sub) {
		if (this->nodes.count({ type, var, sub }) > 0) {
			return this->customtags[{ type, var, sub }];
		}
		else if (this->customtags.count({ type, var, 0 }) > 0) {
			return this->customtags[{ type, var, 0 }];
		}
		return this->customtags[{ type, 0, 0 }];
	}

	const set<string>& GetCustomTags(const EntityConfig_Entity& entity) {
		return GetCustomTags(entity.id, entity.variant, entity.subtype);
	}

	bool HasCustomTag(int type, int var, int sub, const std::string tag) {
		const set<string>& customtags = GetCustomTags(type, var, sub);
		return customtags.find(stringlower(tag.c_str())) != customtags.end();
	}

	bool HasCustomTag(const EntityConfig_Entity& entity, const std::string tag) {
		return HasCustomTag(entity.id, entity.variant, entity.subtype, tag);
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
	XMLItem* NullItemData = new XMLItem();
	XMLItemPools* PoolData = new XMLItemPools();
	XMLBossPools* BossPoolData = new XMLBossPools();
	XMLBossPortrait* BossPortraitData = new XMLBossPortrait();
	XMLTrinket* TrinketData = new XMLTrinket();
	XMLMusic* MusicData = new XMLMusic(118);
	XMLSound* SoundData = new XMLSound(832);
	XMLPill* PillData = new XMLPill();
	XMLCard* CardData = new XMLCard();
	XMLChallenge* ChallengeData = new XMLChallenge();
	XMLNightmare* NightmareData = new XMLNightmare();
	XMLCostume* CostumeData = new XMLCostume();
	XMLWisp* WispData = new XMLWisp();
	XMLWispColor* WispColorData = new XMLWispColor();
	XMLWisp* LocustData = new XMLWisp();
	XMLWispColor* LocustColorData = new XMLWispColor();
	XMLNullCostume* NullCostumeData = new XMLNullCostume(130);
	XMLBombCostume* BombCostumeData = new XMLBombCostume(19);
	XMLCurse* CurseData = new XMLCurse(1);
	XMLRecipe* RecipeData = new XMLRecipe();
	XMLCutscene* CutsceneData = new XMLCutscene();
	XMLStage* StageData = new XMLStage();
	XMLBackdrop* BackdropData = new XMLBackdrop();
	XMLAchievement* AchievementData = new XMLAchievement();
	XMLGeneric* GiantBookData = new XMLGeneric(46);
	XMLGeneric* BossRushData = new XMLGeneric(0);
	XMLGeneric* PlayerFormData = new XMLGeneric(14);

	XMLMod* ModData = new XMLMod();

	
};



inline bool isvalidid(const string& str) {
	if (str.length() > 0) {
		char* endPtr;
		int returnval = strtol(str.c_str(), &endPtr, 0);
		if (endPtr != "\0") {
			return returnval > 0;
		}
	}
	return false;
}

inline string ComaSeparatedNamesToIds(const string& names, XMLDataHolder* xmldata) {
	size_t start = 0;
	size_t pos = names.find(',');
	string item;
	string parsedlist = "";
	while (pos != std::string::npos) {
		item = names.substr(start, pos - start);
		if (!isvalidid(item)) {
			if (xmldata->byname.find(item) != xmldata->byname.end()) {
				parsedlist += to_string(xmldata->byname[item]) + ",";
			}
		}
		else {
			parsedlist += item + ",";
		}
		start = pos + 1;
		pos = names.find(',', start);
	}
	std::string lastItem = names.substr(start);
	if (!isvalidid(lastItem)) {
		if (xmldata->byname.find(lastItem) != xmldata->byname.end()) {
			parsedlist += to_string(xmldata->byname[lastItem]);
		}
	}
	else {
		parsedlist += lastItem;
	}
	//printf("itemlist: %s (%s) \n", parsedlist.c_str(),names.c_str());
	return parsedlist;
}

inline bool MultiValXMLParamParse(xml_node<char>* auxnode, xml_document<char>* xmldoc, XMLDataHolder* xmldata, const char* attrname) {
	xml_attribute<char>* attr = auxnode->first_attribute(attrname);
	if (attr) {
		string parseditemlist = ComaSeparatedNamesToIds(string(auxnode->first_attribute(attrname)->value()), xmldata);
		xml_attribute<char>* newAttr = xmldoc->allocate_attribute(attrname, xmldoc->allocate_string(parseditemlist.c_str()));
		auxnode->remove_attribute(attr);
		auxnode->append_attribute(newAttr);
		return true;
	}
	return false;
}

inline bool SingleValXMLParamParse(xml_node<char>* auxnode, xml_document<char>* xmldoc, XMLDataHolder* xmldata, const char* attrname) {
	xml_attribute<char>* attr = auxnode->first_attribute(attrname);
	if (attr && (!isvalidid(attr->value()))) {
		string val = string(attr->value());
		if (xmldata->byname.find(val) != xmldata->byname.end()) {
			string parseditemlist = to_string(xmldata->byname[val]);
			xml_attribute<char>* newAttr = xmldoc->allocate_attribute(attrname, xmldoc->allocate_string(parseditemlist.c_str()));
			auxnode->remove_attribute(attr);
			auxnode->append_attribute(newAttr);
			return true;
		}
	}
	return false;
}

extern unordered_map<string, int> xmlnodeenum;
inline void initxmlnodeenum() {
	xmlnodeenum["entity"] = 1;
	xmlnodeenum["player"] = 2;
	xmlnodeenum["pocketitems"] = 3;
	xmlnodeenum["items"] = 4;
	xmlnodeenum["bombcostumes"] = 5;
	xmlnodeenum["music"] = 6;
	xmlnodeenum["sounds"] = 7;
	xmlnodeenum["achievements"] = 8;
	xmlnodeenum["challenges"] = 9;
	xmlnodeenum["backdrops"] = 10;
	xmlnodeenum["cutscenes"] = 11;
	xmlnodeenum["stages"] = 12;
	xmlnodeenum["recipes"] = 13;
	xmlnodeenum["wisps"] = 14;
	xmlnodeenum["locusts"] = 15;
	xmlnodeenum["nightmares"] = 16;
	xmlnodeenum["curses"] = 17;
	xmlnodeenum["bosses"] = 18;
	xmlnodeenum["costumes"] = 19;
	xmlnodeenum["itempools"] = 20;
	xmlnodeenum["bosspools"] = 21;
	xmlnodeenum["giantbook"] = 22;
	xmlnodeenum["bossrush"] = 23;
	xmlnodeenum["playerforms"] = 24;
	xmlnodeenum["name"] = 99; //for mod metadata
}

extern unordered_map<string, int> xmlmaxnode;
inline void initxmlmaxnodeenum() {
	xmlmaxnode["giantbook.xml"] = 46;
	xmlmaxnode["nightmares.xml"] = 16;
	xmlmaxnode["playerforms.xml"] = 14;
	xmlmaxnode["cutscenes.xml"] = 26;
}

extern unordered_map<string, int> xmlfullmerge;
inline void initxmlfullmergelist() {

}

extern XMLData XMLStuff;

#endif