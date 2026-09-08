#ifndef XMLDATA_H
#define XMLDATA_H

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <filesystem>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"
#include "EvaluateStats.h"
#include "../MiscFunctions.h"

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

using XMLAttributes = unordered_map<string, string>;
using XMLNodes = unordered_map<int, XMLAttributes>;
using XMLChilds = unordered_map<string, std::vector<XMLAttributes>>;
using XMLRelEnt = unordered_map<int, std::vector<XMLAttributes>>;
using XMLKinder = unordered_map<int, XMLChilds>;
using XMLEntityKinder = unordered_map<tuple<int, int, int>, XMLChilds>;
using XMLNodeIdxLookup = unordered_map<string, int>;
using XMLNodeIdxLookupMultiple = unordered_map<string, vector<int>>;

inline string stringlower(char* str)
{
	string s{ str };
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

inline string stringlower(const char* str)
{
	string s{ str };
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

inline std::string stringlower(std::string_view str)
{
	string result(str);
	for (auto& c : result) {
		c = tolower(c);
	}
	return result;
}

// Returns the ID string of the current mod being processed, or "BaseGame" for vanilla XMLs.
// Only relevant at times where XMLs are actually being loaded by the game.
std::string GetXMLDataLastModId();

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

// Base class for XMLData categories (except XMLEntity, which is special, lol)
class XMLDataHolder {
public:
	XMLNodes nodes;
	XMLKinder childs;
	XMLNodeIdxLookup childbyname;
	XMLNodeIdxLookup byname;
	XMLNodeIdxLookup bynamemod;
	unordered_map<int, int> byorder;
	XMLNodeIdxLookupMultiple bymod;
	XMLNodeIdxLookup byrelativeid;
	XMLNodeTable byfilepathmulti;
	int maxid;
	int defmaxid;
	bool stuffset = false;

	// Initializes a standard XMLAttributes object.
	// Copies attributes from `raw`, with keys converted to all lowercase.
	// If a "daddy" is provided, also inherits attributes from that.
	static XMLAttributes CreateNode(const xml_node<char>& auxnode, xml_node<char>* daddy, const std::string& currpath, const char* modid);

	// Check the ID attributes of the node, updating it as necessary, and returning the final chosen ID integer.
	virtual int AssignId(XMLAttributes& node, const bool isContent);

	// For performing special processing on the attributes of a node, after ID assignment and child node parsing, but before translations/insertions.
	virtual void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& node, int id) {}

	// Returns the category to use for translation strings. Returns nullptr if this data does not have translations.
	virtual const char* GetTranslationStringCategory() const { return nullptr; }
	
	// Inserts the node into the byname/bynamemod lookup tables.
	virtual void AddByNameLookups(XMLAttributes& node, int id, const std::string& modid);

	// Checks if a translation is available for the specified attribute in the specified category.
	// If there is, the current attribute will be copied to "untranslated<attr>", and the original
	// attribute will be replaced with its ENGLISH translated form.
	static void CheckTranslatedAttribute(XMLAttributes& node, const std::string attr, const char* category) {
		if (category) {
			if (node.count(attr) && !node[attr].empty() && node[attr].front() == '#') {
				bool failed = false;
				// Only translating to English here is intended
				if (const char* english = g_Manager->GetStringTable()->GetString(category, 0, node[attr].substr(1, node[attr].length()).c_str(), &failed); !failed && english && strlen(english) > 0) {
					const std::string untranslatedAttr = "untranslated" + attr;
					node[untranslatedAttr] = node[attr];
					node[attr] = english;
				}
			}
		}
	}

	// Checks for translations using the category defined for this particular instance.
	void CheckTranslatedAttribute(XMLAttributes& node, const char* attr) {
		if (const char* stringCategory = GetTranslationStringCategory()) {
			CheckTranslatedAttribute(node, attr, GetTranslationStringCategory());
		}
	}

	// Adds an XMLAttributes node into the data holder, processing attributes/children/translations
	// as appropriate, and populating lookup tables.
	XMLAttributes& AddNode(const xml_node<char>& auxnode, XMLAttributes node, int id) {
		// Process any child nodes.
		ProcessChilds(&auxnode, id);
		
		// Perform any special processing logic.
		ProcessAttributes(auxnode, node, id);
		
		// If this data supports translations, apply them for name/description.
		CheckTranslatedAttribute(node, "name");
		CheckTranslatedAttribute(node, "description");
		
		// Populate lookup tables.
		const std::string& modid = node["sourceid"];
		AddByNameLookups(node, id, modid);
		bymod[modid].push_back(id);
		if (node.count("sourcepath")) {
			byfilepathmulti.tab[node["sourcepath"]].push_back(id);
		}
		if (node.count("relativeid")) {
			byrelativeid[modid + node["relativeid"]] = id;
		}

		// Insert the node.
		const bool isOverwrite = nodes.count(id) > 0;
		nodes[id] = std::move(node);
		if (!isOverwrite) {
			byorder[nodes.size()] = id;
		}

		return nodes[id];
	}

	// Creates a new node from the provided raw data, assigns it an ID and adds it to the data holder.
	XMLAttributes& CreateAndAddNode(const xml_node<char>& auxnode, xml_node<char>* daddy, const std::string& currpath, const bool isContent, const char* modid) {
		XMLAttributes node = CreateNode(auxnode, daddy, currpath, modid);
		int id = AssignId(node, isContent);
		return AddNode(auxnode, std::move(node), id);
	}

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

	std::optional<int> GetIdByName(std::string_view str)
	{
		auto it = this->byname.find(std::string(str));
		if (it == this->byname.end()) { return std::nullopt; }
		return it->second;
	}

	XMLAttributes* GetNodeOrNullById(int id) {
		auto iter = this->nodes.find(id);
		if (iter != this->nodes.end()) {
			return &iter->second;
		}
		return nullptr;
	}

	// Note: This function returns a copy of the entire XMLAttributes, so it can be a little inefficient.
	// Not changing everything right away just in case there are possible side effects, but try GetAttributeById.
	XMLAttributes GetNodeById(int name) {
		if (XMLAttributes* node = GetNodeOrNullById(name)) {
			return *node;
		}
		return XMLAttributes();
	}

	std::string GetAttributeById(int id, const std::string& key) {
		if (XMLAttributes* node = GetNodeOrNullById(id)) {
			auto iter = node->find(key);
			if (iter != node->end()) {
				return iter->second;
			}
		}
		return "";
	}

	int GetNumberAttributeById(int id, const std::string& key, int defaultValue = -1) {
		const std::string attr = GetAttributeById(id, key);
		if (!attr.empty()) {
			try { return std::stoi(attr); } catch (...) {}
		}
		return defaultValue;

	}

	XMLAttributes GetNodeById(const string& name) { //for convenience,lol
		int id = 0;
		if (name.length() > 0) {
			char* endPtr;
			int tentid = strtol(name.c_str(), &endPtr, 0);
			if (endPtr != "\0") {
				id = tentid;
			}
			else {
				return XMLAttributes();
			}
		}

		auto iter = this->nodes.find(id);
		if (iter == this->nodes.end()) { return XMLAttributes(); }
		else { return iter->second; }
	}

	XMLAttributes  GetNodeByOrder(int name) {
		auto iter = this->byorder.find(name);
		if (iter == this->byorder.end()) { return XMLAttributes(); }
		else { return this->GetNodeById(iter->second); }
	}

	XMLAttributes GetNodeByName(const string& name) {
		auto iter = this->byname.find(name);
		if (iter == this->byname.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}
	XMLAttributes GetNodeByNameMod(const string& name) {
		auto iter = this->bynamemod.find(name);
		if (iter == this->bynamemod.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}
	XMLAttributes GetNodesByMod(const string& name) {
		auto iter = this->bynamemod.find(name);
		if (iter == this->bynamemod.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}

	XMLChilds GetChildsById(int name) {
		auto iter = this->childs.find(name);
		if (iter == this->childs.end()) { return XMLChilds(); }
		return iter->second;
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsByName(const string& name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeByName(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[this->byname[name]];
		}
		else {Childs = XMLChilds();}
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsByOrder(int name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeByOrder(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[this->byorder[name]];
		}
		else { Childs = XMLChilds(); }
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsById(int name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeById(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[name];
		}
		else { Childs = XMLChilds(); }
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	void ProcessChilds(const xml_node<char>* parentnode, int id) {
		ProcessChilds(parentnode, id, "");
	}

	void ProcessChilds(const xml_node<char>* parentnode, int id, const string& lastmodid) {

		for (const xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
			XMLAttributes child;
			for (const xml_attribute<>* attr = auxnodebabe->first_attribute(); attr; attr = attr->next_attribute())
			{
				child[stringlower(attr->name())] = string(attr->value());
			}
			if (lastmodid.length() > 0) {
				child["sourceid"] = lastmodid;
			}
			this->childs[id][stringlower(auxnodebabe->name())].push_back(child);
			if (child.find("name") != child.end()) { //this wont be used too much but it's needed for some cases
				this->childbyname[child["name"]] = this->childs[id][stringlower(auxnodebabe->name())].size();
			}
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
	XMLChilds achievlistpermod;

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
		achievlistpermod.clear();
		byorder.clear();
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

	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& sound, int id) override;
};


class XMLAchievement : public XMLDataHolder {};

class XMLRecipe : public XMLDataHolder {
	int AssignId(XMLAttributes& recipe, const bool isContent) override;
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& recipe, int id) override;
};

class XMLWisp : public XMLDataHolder {
	int AssignId(XMLAttributes& wisp, const bool isContent) override;
};
class XMLWispColor : public XMLDataHolder {};

class XMLLocust : public XMLDataHolder {
	int AssignId(XMLAttributes& locust, const bool isContent) override;
};
class XMLLocustColor : public XMLDataHolder {};

class XMLBossPortrait : public XMLDataHolder {
	void AddByNameLookups(XMLAttributes& boss, int id, const std::string& modid) override;
};

class XMLItem : public XMLDataHolder {
public:
	vector<XMLAttributes> customachievitems;

	const char* GetTranslationStringCategory() const override { return "Items"; }

	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& item, int id) override;
};

class XMLCollectible : public XMLItem {};

class XMLNullItem : public XMLItem {
public:
	const char* GetTranslationStringCategory() const override { return nullptr; }
};

class XMLItemPools : public XMLDataHolder {

};

class XMLCutscene : public XMLDataHolder {

};

class XMLBossPools : public XMLDataHolder {
public:
	static void BuildDefaultDoc(rapidxml::xml_document<char>& doc);
	std::string& GetXmlSourceText(char* sourceText);
};

class XMLNightmare : public XMLDataHolder {
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& nightmare, int id) override;
};

class XMLCostume : public XMLDataHolder {
public:
	int AssignId(XMLAttributes& costume, const bool isContent) override;
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& costume, int id) override;
};

class XMLBombCostume : public XMLDataHolder {
public:
	XMLBombCostume(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}

	int AssignId(XMLAttributes& bombcostume, const bool isContent) override;
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& bombcostume, int id) override;
};

class XMLNullCostume : public XMLCostume {
public:
	XMLNullCostume(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}

	int AssignId(XMLAttributes& costume, const bool isContent) override;
};

class XMLGeneric : public XMLDataHolder {
public:
	XMLGeneric(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}

	const char* GetTranslationStringCategory() const override { return "Default"; }
};

class XMLGiantBook : public XMLDataHolder {
public:
	XMLGiantBook(int m) {
		this->maxid = m;
		this->defmaxid = m;
	}

	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& giantbook, int id) override;
};

class XMLChallenge : public XMLDataHolder {
public:
	const char* GetTranslationStringCategory() const override { return "Challenges"; }
};

class XMLBossColor : public XMLDataHolder {
public:
	unordered_map<tuple<int,int>, int> bytypevar;	
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
				stuffset = true; //this is to set the thing as a 2ndpass is going to be made
			}
		}
	}
};

class XMLTrinket : public XMLItem {
public:
	// XMLNodeIdxLookup bypickup;
};

class XMLPocketItem : public XMLDataHolder {
public:
	// XMLNodeIdxLookup bypickup;
	vector<XMLAttributes> customachievitems;

	const char* GetTranslationStringCategory() const override { return "PocketItems"; }
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& node, int id) override;
};

class XMLCard : public XMLPocketItem {};
class XMLPill : public XMLPocketItem {};

class XMLStage : public XMLDataHolder {
public:
	unordered_map<tuple<int,int>, int> bystagealt;	
	unordered_map<int, int> bybasestage;
};

class XMLPlayer : public XMLDataHolder {
	const char* GetTranslationStringCategory() const override { return "Players"; }

	void AddByNameLookups(XMLAttributes& player, int id, const std::string& modid) override;
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& player, int id) override;
};

class XMLBackdrop : public XMLDataHolder {
public:
	XMLRelEnt relfxlayers; //<backdripid, vector<fxlayerid>>
	XMLRelEnt relfxrays;	//<backdripid, vector<fxrayid>>
	XMLRelEnt relfxparams; //<backdripid, vector<fxparamid>>
	vector<XMLAttributes> GetRelatedFXLayers(int backdropid) {
		return relfxlayers[backdropid];
	}
	vector<XMLAttributes> GetRelatedFXRays(int backdropid) {
		return relfxrays[backdropid];
	}
	vector<XMLAttributes> GetRelatedFXParams(int backdropid) {
		return relfxparams[backdropid];
	}

	int AssignId(XMLAttributes& backdrop, const bool isContent) override;
	void ProcessAttributes(const xml_node<char>& auxnode, XMLAttributes& backdrop, int id) override;
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

	void Clear() {
		nodes.clear();
		childs.clear();
		byname.clear();
		bynamemod.clear();
		bymod.clear();
		bytypevar.clear();
		maxid = 0;
	}

	XMLAttributes GetNodeById(const tuple<int, int, int> &name) {
		auto iter = this->nodes.find(name);
		if (iter == this->nodes.end()) { return XMLAttributes(); }
		else { return iter->second; }
	}

	XMLAttributes GetNodeByName(const string &name) {
		auto iter = this->byname.find(name);
		if (iter == this->byname.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}

	XMLAttributes GetNodeByOrder(int name) {
		auto iter = this->byorder.find(name);
		if (iter == this->byorder.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}
	XMLAttributes GetNodeByNameMod(const string &name) {
		auto iter = this->bynamemod.find(name);
		if (iter == this->bynamemod.end()) { return XMLAttributes(); }
		return this->GetNodeById(iter->second);
	}

	XMLChilds GetChildsById(const tuple<int, int, int>& name) {
		auto iter = this->childs.find(name);
		if (iter == this->childs.end()) { return XMLChilds(); }
		return iter->second;
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsByOrder(int name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeByOrder(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[this->byorder[name]];
		}
		else { Childs = XMLChilds(); }
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsByName(const std::string& name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeByName(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[this->byname[name]];
		}
		else { Childs = XMLChilds(); }
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	tuple<XMLAttributes, XMLChilds> GetXMLNodeNChildsById(const tuple<int, int, int>& name) {
		XMLAttributes Node;
		XMLChilds Childs;
		Node = this->GetNodeById(name);
		if (Node.end() != Node.begin()) {
			Childs = this->childs[name];
		}
		else { Childs = XMLChilds(); }
		return tuple<XMLAttributes, XMLChilds>(Node, Childs);
	}

	//XMLAttributes GetNodesByMod(const string &name) { //not set up for now (unused anyway)
		//auto iter = this->bymod.find(name);
		//if (iter == this->bymod.end()) { return XMLAttributes(); }
		//return this->GetNodeById(iter->second);
	//}

	XMLAttributes* GetNodesOrNullByTypeVarSub(int type, int var, int sub, bool strict) {
		auto iter = this->nodes.find({ type, var, sub });
		if (iter != this->nodes.end()) {
			return &iter->second;
		}
		if (strict) {
			return nullptr;
		}
		iter = this->nodes.find({ type, var, 0 });
		if (iter != this->nodes.end()) {
			return &iter->second;
		}
		iter = this->nodes.find({ type, 0, 0 });
		if (iter != this->nodes.end()) {
			return &iter->second;
		}
		return nullptr;
	}

	// Note: This function returns a copy of the entire XMLAttributes, so it can be a little inefficient.
	// Not changing everything right away just in case there are possible side effects, but try GetAttributeByTypeVarSub.
	XMLAttributes GetNodesByTypeVarSub(int type, int var, int sub, bool strict) {
		if (XMLAttributes* node = GetNodesOrNullByTypeVarSub(type, var, sub, strict)) {
			return *node;
		}
		return XMLAttributes();
	}

	std::string GetAttributeByTypeVarSub(int type, int var, int sub, bool strict, const std::string& key) {
		if (XMLAttributes* node = GetNodesOrNullByTypeVarSub(type, var, sub, strict)) {
			auto iter = node->find(key);
			if (iter != node->end()) {
				return iter->second;
			}
		}
		return "";
	}

	void ProcessChilds(const xml_node<char>* parentnode, tuple<int, int, int> id) {

		for (const xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
			XMLAttributes child;
			for (const xml_attribute<>* attr = auxnodebabe->first_attribute(); attr; attr = attr->next_attribute())
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
	XMLCollectible* ItemData = new XMLCollectible();
	XMLNullItem* NullItemData = new XMLNullItem();
	XMLItemPools* PoolData = new XMLItemPools();
	XMLBossPools* BossPoolData = new XMLBossPools();
	XMLBossPortrait* BossPortraitData = new XMLBossPortrait();
	XMLTrinket* TrinketData = new XMLTrinket();
	XMLMusic* MusicData = new XMLMusic(119);
	XMLSound* SoundData = new XMLSound(914);
	XMLPill* PillData = new XMLPill();
	XMLCard* CardData = new XMLCard();
	XMLChallenge* ChallengeData = new XMLChallenge();
	XMLNightmare* NightmareData = new XMLNightmare();
	XMLCostume* CostumeData = new XMLCostume();
	XMLWisp* WispData = new XMLWisp();
	XMLWispColor* WispColorData = new XMLWispColor();
	XMLLocust* LocustData = new XMLLocust();
	XMLLocustColor* LocustColorData = new XMLLocustColor();
	XMLNullCostume* NullCostumeData = new XMLNullCostume(130);
	XMLBombCostume* BombCostumeData = new XMLBombCostume(19);
	XMLCurse* CurseData = new XMLCurse(1);
	XMLRecipe* RecipeData = new XMLRecipe();
	XMLCutscene* CutsceneData = new XMLCutscene();
	XMLStage* StageData = new XMLStage();
	XMLBackdrop* BackdropData = new XMLBackdrop();
	XMLAchievement* AchievementData = new XMLAchievement();
	XMLGiantBook* GiantBookData = new XMLGiantBook(46);
	XMLGeneric* BossRushData = new XMLGeneric(0);
	XMLGeneric* PlayerFormData = new XMLGeneric(14);
	XMLGeneric* FxLayerData = new XMLGeneric(0);
	XMLGeneric* FxParamData = new XMLGeneric(0);
	XMLGeneric* FxRayData = new XMLGeneric(0);
	XMLBossColor* BossColorData = new XMLBossColor();
	XMLGeneric* BabyData = new XMLGeneric(0);

	unordered_map<string, XMLGeneric*> CustomXMLData;

	XMLMod* ModData = new XMLMod();

	// Converts a string of space-separated tags to lowercase, parses each individual tag, and inserts them into the provided set.
	// Ex: "tag1 tag2 tag3"
	// For tag-like attributes like 'customtags' and 'customcache'.
	static void ParseTagsString(const string& str, set<string>& out) {
		const string tagsstr = stringlower(str.c_str());
		if (!tagsstr.empty()) {
			stringstream tagstream(tagsstr);
			string tag;
			while (getline(tagstream, tag, ' ')) {
				if (!tag.empty()) {
					out.insert(tag);
				}
			}
		}
	}
};

extern unordered_map<string, int> getxmlnodeidbyname;
extern vector<XMLDataHolder*> xmlnodetypetodata;
extern XMLData XMLStuff;

inline int getLineNumber(const char* data, const char* errorOffset) {
	if (strlen(errorOffset) <= 0) { return 0; }
	int lineNumber = 1;
	const char* current = data;
	while (current < errorOffset) {
		if (*current == '\n') {
			lineNumber++;
		}
		current++;
	}
	return lineNumber;
}

namespace XML
{
	constexpr const char* BASE_GAME_ID = "BaseGame";

	struct Document
	{
	private:
		std::unique_ptr<rapidxml::xml_document<char>> doc;
		std::string sourceText;

	public:
		Document()
		{
			this->doc = std::make_unique<rapidxml::xml_document<char>>();
		}

		rapidxml::xml_document<char>* GetDocument() { return doc.get(); }
		std::string& LoadSourceText(const std::filesystem::path& path);
	};

	struct ParsedAttributes
	{
		XMLAttributes all;
		std::vector<rapidxml::xml_attribute<char>*> attributes;
	};

	struct ContentDocument
	{
		Document document;
		ModEntry* mod;

		rapidxml::xml_document<char>* GetDocument() { return this->document.GetDocument(); }
	};

	namespace detail
	{
		class ContentIterator
		{
			using ModIterator = decltype(std::declval<ModManager*>()->_mods.begin());
	
		private:
			ModIterator m_current;
			ModIterator m_end;
			std::string m_filePath;
			ContentDocument m_currentDocument;
	
		public:
			ContentIterator(ModIterator current, ModIterator end, const char* filePath)
				: m_current(current), m_end(end), m_filePath(filePath)
			{
				advance();
			}
	
			ContentDocument& operator*() { return m_currentDocument; }
			ContentDocument* operator->() { return &m_currentDocument; }
			bool operator!=(const ContentIterator& other) const { return m_current != other.m_current; }
	
			ContentIterator& operator++()
			{
				++m_current;
				advance();
				return *this;
			}
	
		private:
			void advance();
		};

		class ContentRange
		{
		private:
			ModManager* m_manager;
			std::string m_filePath;

		public:
			ContentRange(ModManager* manager, string_view filePath)
				: m_manager(manager), m_filePath(filePath)
			{}

			ContentIterator begin() { return ContentIterator( m_manager->_mods.begin(), m_manager->_mods.end(), m_filePath.c_str()); }
			ContentIterator end() { return ContentIterator( m_manager->_mods.end(), m_manager->_mods.end(), m_filePath.c_str()); }
		};

		static int get_error_line(const char* sourceText, const char* errorPoint, std::size_t sourceTextSize);
	}

	/// @brief Returns the node's name as a string_view, ensuring that string operations are safe even when parsing using rapidxml::parse_no_destructive
	inline std::string_view GetName(const rapidxml::xml_node<char>* node) { return std::string_view(node->name(), node->name_size()); }
	/// @brief Returns the attribute's name as a string_view, ensuring that string operations are safe even when parsing using rapidxml::parse_no_destructive
	inline std::string_view GetName(const rapidxml::xml_attribute<char>* attribute) { return std::string_view(attribute->name(), attribute->name_size()); }
	/// @brief Returns the attribute's value as a string_view, ensuring that string operations are safe even when parsing using rapidxml::parse_no_destructive
	inline std::string_view GetValue(const rapidxml::xml_attribute<char>* attribute) { return std::string_view(attribute->value(), attribute->value_size()); }
	inline std::string_view GetModId(const ModEntry* mod);
	
	/// @brief Sets the attribute's value.
	///
	/// WARNING: Only use this if the value outlives the xml_document the attribute belongs to.
	/// Otherwise use SetValueCopy.
	inline void SetValue(rapidxml::xml_attribute<char>* attribute, std::string_view value) { attribute->value(value.data(), value.size()); }
	/// @brief Copies the value into the documents' memory pool and sets the attribute's value.
	/// @return returns a pointer to the copy in the document's memory_pool.
	const char* SetValueCopy(rapidxml::xml_document<char>& doc, rapidxml::xml_attribute<char>* attribute, std::string_view value);
	/// @brief Appends a new node to the specified node
	///
	/// WARNING: the name must outline the xml_document, however this is fairly usually not an issue
	// as names are static strings.
	rapidxml::xml_node<char>* AppendNewNode(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, std::string_view name);
	
	/// @brief Appends a new attribute to the specified node
	///
	/// WARNING: both name and value must outline the xml_document.
	/// If this is not the case, then it's suggested to call AppendNewAttribute with no value,
	/// then follow this with a SetValueCopy on the attribute.
	rapidxml::xml_attribute<char>* AppendNewAttribute(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, std::string_view name, std::string_view value = "");

	bool ParseDoc(rapidxml::xml_document<char>& doc, char* sourceText, std::string_view filePath);
	/// @brief Attempts resources redirection and parses the document.
	/// @param originalSourceText must remain alive as long as the document.
	XML::Document GetResourcesDoc(std::string_view filePath, char* originalSourceText);
	/// @brief Returns a range to iterate content documents
	XML::detail::ContentRange GetContentDocs(std::string_view fileName);
	/// @param destParent This node must belong to destDoc
	/// @return the cloned node
	rapidxml::xml_node<char>* CloneNode(rapidxml::xml_document<char>& destDoc, rapidxml::xml_node<char>* destParent, rapidxml::xml_node<char>* node);

	/// @brief Returns all attributes in the node, alongside the xml_attributes specified in the map.
	///
	/// Additionally, every duplicate attribute name specified in the map is removed from the node.
	/// This ensures that the game's parsers behave deterministically in this otherwise unhandled case.
	ParsedAttributes ParseAttributes(rapidxml::xml_node<char>* node, const std::unordered_map<std::string_view, size_t>& attributeMap);
	/// @brief Returns all attributes in the node, alongside the xml_attributes specified in the map.
	XMLAttributes ParseAttributes(rapidxml::xml_node<char>* node);
}

int XML::detail::get_error_line(const char* sourceText, const char* errorPoint, std::size_t sourceTextSize)
{
	const char* start = sourceText;
	const char* end = sourceText + sourceTextSize;

	bool inBounds = start <= errorPoint && errorPoint <= end;
	if (!inBounds)
	{
		return 0;
	}

    return std::count(start, errorPoint, '\n') + 1;
}

inline std::string& XML::Document::LoadSourceText(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streampos end = file.tellg();
	file.seekg(0);

	std::size_t fileSize = end > 0 ? (size_t)end : 0;
	this->sourceText.resize(fileSize);
	file.read(this->sourceText.data(), this->sourceText.size());

	return this->sourceText;
}

inline void XML::detail::ContentIterator::advance()
{
	auto try_advance = [](ContentIterator& it) -> bool
	{
		ModEntry* mod = *it.m_current;
		if (!mod->_loaded)
		{
			return false;
		}

		std::string contentPath;
		mod->GetContentPath(&contentPath, &it.m_filePath);
		auto expandedPath = g_ContentManager.GetMountedFilePath(contentPath.c_str());
		if (!expandedPath)
		{
			return false;
		}

		XML::Document document;
		std::string& sourceText = document.LoadSourceText(*expandedPath);
		if (!XML::ParseDoc(*document.GetDocument(), sourceText.data(), *expandedPath))
		{
			return false;
		}

		it.m_currentDocument = ContentDocument{ std::move(document), mod };
		return true;
	};

	while (m_current != m_end)
	{
		if (try_advance(*this)) { return; }
		m_current++;
	}
}

inline std::string_view XML::GetModId(const ModEntry* mod)
{
	std::string_view modId = mod->_metadataWorkshopID;
	if (modId.size() == 0) {
		modId = mod->_directory;
	}

	return modId;
}

inline bool XML::ParseDoc(rapidxml::xml_document<char>& doc, char* sourceText, std::string_view filePath)
{
	std::string_view sourceTextView = sourceText;
	try {
		doc.parse<0>(sourceText);
		return true;
	}
	catch (rapidxml::parse_error err) {
		int lineNumber = detail::get_error_line(sourceTextView.data(), err.where<char>(), sourceTextView.size());
		std::string error = REPENTOGON::StringConcat("[XMLError] ", err.what(), " at line ", std::to_string(lineNumber), " in ", filePath, "\n");
		if(g_Game && g_Game->GetConsole())
		{
			// console does not need newline
			error.back() = '\0';
			g_Game->GetConsole()->PrintError(error);
			error.back() = '\n';
		}
	
		KAGE::SafeLogMessage(3, error.c_str());
		doc.clear();
	}
	return false;
} 

inline XML::Document XML::GetResourcesDoc(std::string_view filePath, char* originalSourceText)
{
	std::optional<std::string> redirect = REPENTOGON::GetResourcesRedirect(filePath);
	XML::Document document;
	rapidxml::xml_document<char>& doc = *document.GetDocument();

	// initialize resources doc.
	if (redirect)
	{
		auto& redirectSourceText = document.LoadSourceText(redirect.value());
		XML::ParseDoc(doc, redirectSourceText.data(), redirect.value());
	}
	else
	{
		XML::ParseDoc(doc, originalSourceText, filePath);
	}

    return document;
}

inline XML::detail::ContentRange XML::GetContentDocs(std::string_view filePath)
{
    return XML::detail::ContentRange(g_Manager->GetModManager(), filePath);
}

inline rapidxml::xml_node<char>* XML::CloneNode(rapidxml::xml_document<char>& destDoc, rapidxml::xml_node<char>* destParent, rapidxml::xml_node<char>* node)
{
	rapidxml::xml_node<char>* clonedNode = destDoc.clone_node(node);
	destParent->append_node(clonedNode);

	return clonedNode;
}

inline XML::ParsedAttributes XML::ParseAttributes(rapidxml::xml_node<char>* node, const std::unordered_map<std::string_view, size_t>& attributeMap)
{
	XML::ParsedAttributes results;
	size_t mapSize = attributeMap.size();
	results.attributes.resize(mapSize, nullptr);

	for (auto* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
	{
		std::string_view name = XML::GetName(attribute);
		std::string_view value = XML::GetValue(attribute);

		results.all[stringlower(name)] = value;

		auto it = attributeMap.find(name);
		if (it != attributeMap.end())
		{
			size_t attributeId = it->second;
			assert(attributeId < mapSize); // something's wrong with the input map

			if (auto* previous = results.attributes[attributeId]; previous)
			{
				node->remove_attribute(previous);
			}

			results.attributes[attributeId] = attribute;
		}
	}

	return results;
}

inline XMLAttributes XML::ParseAttributes(rapidxml::xml_node<char>* node)
{
	XMLAttributes attributes;

	for (auto* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
	{
		std::string_view name = XML::GetName(attribute);
		std::string_view value = XML::GetValue(attribute);

		attributes[stringlower(name)] = value;
	}

	return attributes;
}

inline const char* XML::SetValueCopy(rapidxml::xml_document<char>& doc, rapidxml::xml_attribute<char>* attribute, std::string_view value)
{
	char* copy = doc.allocate_string(value.data(), value.size());
	attribute->value(copy, value.size());
	return copy;
}

inline rapidxml::xml_node<char>* XML::AppendNewNode(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, std::string_view name)
{
	auto* newNode = doc.allocate_node(rapidxml::node_element, name.data(), nullptr, name.size());
	node->append_node(newNode);
    return newNode;
}

inline rapidxml::xml_attribute<char>* XML::AppendNewAttribute(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, std::string_view name, std::string_view value)
{
	auto* attribute = doc.allocate_attribute(name.data(), value.data(), name.size(), value.size());
	node->append_attribute(attribute);
	return attribute;
}

inline bool isvalidid(const std::string& str) {
	if (!str.empty()) {
		char* endPtr;
		int returnval = std::strtol(str.c_str(), &endPtr, 0);
		if (*endPtr == '\0') {
			return (returnval != 0 || str == "0");
		}
	}
	return false;
}

inline string ComaSeparatedNamesToIds(const string& names, XMLDataHolder* xmldata, const string& mod = "BaseGame") {
	size_t start = 0;
	size_t pos = names.find(',');
	string item;
	string parsedlist = "";
	while (pos != std::string::npos) {
		item = names.substr(start, pos - start);
		if (!isvalidid(item)) {
			if (xmldata->bynamemod.find(item + mod) != xmldata->byname.end()) { //priority for stuff from its own mod
				parsedlist += to_string(xmldata->byname[item]) + ",";
			}
			else if (xmldata->byname.find(item) != xmldata->byname.end()) {
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
	//ZHL::Log("itemlist: %s (%s) \n", parsedlist.c_str(),names.c_str());
	return parsedlist;
}

inline bool MultiValXMLParamParse(xml_node<char>* auxnode, xml_document<char>* xmldoc, XMLDataHolder* xmldata, const char* attrname) {
	xml_attribute<char>* attr = auxnode->first_attribute(attrname);
	if (attr) {
		string parseditemlist = ComaSeparatedNamesToIds(string(auxnode->first_attribute(attrname)->value()), xmldata);
		if (parseditemlist.length() > 0) { // didnt wanna do this, but its sadly needed to prevent a vanilla fuckup where _Kilburn used spaces instead of commas to separate elements (which, for the record, doesnt even work well in vanilla)
			xml_attribute<char>* newAttr = xmldoc->allocate_attribute(attrname, xmldoc->allocate_string(parseditemlist.c_str()));
			auxnode->remove_attribute(attr);
			auxnode->append_attribute(newAttr);
		}
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


using XMLattrparse = unordered_map<string, XMLDataHolder *>;
extern unordered_map<XMLDataHolder*, XMLattrparse> xmllatepatches; //xmlname, list of attrs to parse


inline void RegisterCustomXMLAttr(XMLDataHolder* XMLDataToUpdate, const string& AttributeName, XMLDataHolder* XMLDataForIds) { // this makes it so Name attributes in XMLData are parsed to their Id counterparts after Manager::LoadConfig finishes (as in, after all xmls are loaded, which means no XML load order shenanigans)
	xmllatepatches[XMLDataToUpdate][AttributeName] = XMLDataForIds;
}


inline bool MultiValXMLParamParseLATE() {
	bool did = false;
	for each (auto patch in xmllatepatches) {
		XMLDataHolder* xmldatasource = patch.first;
		for each (auto node in xmldatasource->nodes) {
			XMLAttributes auxnode = node.second;
			for each (auto toparse in xmllatepatches[xmldatasource]) {
				XMLDataHolder* xmldata = toparse.second;
				string attrname = toparse.first;
				if (auxnode.find(attrname) != auxnode.end()) {
					string parseditemlist = ComaSeparatedNamesToIds(auxnode[attrname], xmldata, auxnode["sourceid"]);
					//printf("DINGUS: %s BINGUS: %s \n", auxnode[attrname].c_str(), parseditemlist.c_str());
					if (parseditemlist.length() > 0) {
						auxnode["raw-" + attrname] = auxnode[attrname];
						auxnode[attrname] = parseditemlist;
						xmldatasource->nodes[node.first] = auxnode;
						did = true;
					}
				}
			}
		}
	}
	return did;
}


struct CustomXML {
	string filename;
	string rootnodename;
	string entrynodename;
};
extern vector<CustomXML> pendingcustomxmls;

inline int xmltoint(const string& str) {
	if (str.length() > 0) {
		char* endPtr;
		int returnval = strtol(str.c_str(), &endPtr, 0);
		if (endPtr != "\0") {
			return returnval;
		}
	}
	return 0;
}

inline bool XMLParse(xml_document<char>* xmldoc, char* xml, const string& dir) {
	try {
		if (strlen(xml) == strlen(xml + 1)) {
			xmldoc->parse<0>(xml);
		}
		else {
			char* zeroTerminatedStr = new char[strlen(xml) + 1];
			strcpy(zeroTerminatedStr, xml);
			xml = zeroTerminatedStr; // for exception handler
			xmldoc->parse<0>(zeroTerminatedStr);
		}
		return true;
	}
	catch (rapidxml::parse_error err) {
		int lineNumber = getLineNumber(xml, err.where<char>());
		string a = stringlower((char*)string(xml).substr(0, 60).c_str());
		string reason = err.what() + string(" at line ") + to_string(lineNumber);
		string error = "[XMLError] " + reason + " in " + dir;
		if(g_Game && g_Game->GetConsole())
			g_Game->GetConsole()->PrintError(error);
		KAGE::SafeLogMessage(3, (error + "\n").c_str());
		//printf("%s \n", error.c_str());
		//mclear(xmldoc);
	}
	return false;
}

inline char* GetResources(const string& dir, const string& filename) {
	vector<string> paths = { dir + "\\resources-repentogon\\", dir + "\\resources-dlc3\\" + filename, dir + "\\resources\\" + filename };
	for (const string& path : paths) {
		ifstream file(path.c_str());
		if (file.is_open()) {
			std::stringstream sbuffer;
			sbuffer << file.rdbuf();
			string filedata = sbuffer.str();
			char* buffer = new char[filedata.length()];
			strcpy(buffer, filedata.c_str());
			return buffer;
		}
	}
	return "";
}

inline bool GetContent(const string& dir, xml_document<char>* xmldoc) {
	ifstream file(dir.c_str());
	if (file.is_open()) {
		//		printf("path: %s \n", dir.c_str());
		std::stringstream sbuffer;
		sbuffer << file.rdbuf();
		string filedata = sbuffer.str();
		char* buffer = new char[filedata.length() + 1];
		strcpy(buffer, filedata.c_str());
		if (XMLParse(xmldoc, buffer, dir)) {
			delete[] buffer;
			return true;
		}
	}
	return false;
}

inline void UpdateRelEntTracker(XMLDataHolder* data, XMLRelEnt* target, const char* trgtattr) {
	for each (auto node in data->nodes) {
		XMLAttributes attrs = node.second;
		if (attrs.find(trgtattr) != attrs.end()) {
			int entid = xmltoint(attrs[trgtattr]);
			if (target->find(entid) == target->end()) {
				target->insert(pair<int, vector<XMLAttributes>>(entid, vector<XMLAttributes >()));
			}
			target->at(entid).push_back(attrs);
		}
	}
}
inline void inheritdaddyatts(xml_node<char>* daddy, XMLAttributes* atts) {
	for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
	{
		if (atts->find(attr->name()) == atts->end()) {
			atts->insert(pair<string, string>(stringlower(attr->name()), string(attr->value())));
		}
	}
}

inline void LoadGenericXMLData(XMLDataHolder* data, xml_node<char>* daddy, bool iscontent,string currpath, string lastmodid) {
	int id = 1;
	xml_node<char>* babee = daddy->first_node();
	for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
		XMLAttributes attributes;
		for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
		{
			attributes[stringlower(attr->name())] = string(attr->value());
		}
		inheritdaddyatts(daddy, &attributes);
		//string oldid = attributes["id"];
		if ((attributes.find("id") != attributes.end()) && ((attributes.find("sourceid") == attributes.end()) || !iscontent)) {
			id = xmltoint(attributes["id"]);
		}

		else {
			if (attributes.find("id") != attributes.end()) { attributes["relativeid"] = attributes["id"]; }
			data->maxid = data->maxid + 1;
			attributes["id"] = to_string(data->maxid);
			id = data->maxid;
		}
		if (id > data->maxid) {
			data->maxid = id;
		}

		if (attributes.find("sourceid") == attributes.end()) {
			attributes["sourceid"] = lastmodid;
		}
		data->ProcessChilds(auxnode, id);

		XMLDataHolder::CheckTranslatedAttribute(attributes, "name", "Default");

		if (attributes.find("relativeid") != attributes.end()) { data->byrelativeid[attributes["sourceid"] + attributes["relativeid"]] = id; }
		data->bynamemod[attributes["name"] + attributes["sourceid"]] = id;
		data->bynamemod[attributes["untranslatedname"] + attributes["sourceid"]] = id;
		data->bymod[attributes["sourceid"]].push_back(id);
		data->byfilepathmulti.tab[currpath].push_back(id);
		if (attributes.find("name") != attributes.end()) {
			data->byname[attributes["name"]] = id;
			data->byname[attributes["untranslatedname"]] = id;
		}
		data->nodes[id] = attributes;
		data->byorder[data->nodes.size()] = id;
		//printf("gen: %s id: %d // %d \n", attributes["name"].c_str(), id, data->maxid);
	}
}

inline void LoadCustomXML(CustomXML xml) {
	XMLStuff.CustomXMLData[xml.filename] = new XMLGeneric(0);
	vector<pair<string,string>> dirstoload;
	//check for resources
	string targetresource = "";	
	string lastmodid;
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (mod->IsEnabled()) {
			string dir = filesystem::current_path().parent_path().string() + "\\mods\\" + mod->GetDir();
			vector<string> paths = { dir + "\\resources-repentogon\\" + xml.filename, dir + "\\resources-dlc3\\" + xml.filename, dir + "\\resources\\" + xml.filename };
			for (const string& path : paths) {
				if (filesystem::exists(path)) {
					targetresource = path;
					break;
				}
			}
		}
	}
	if (!targetresource.empty()) {
		dirstoload.push_back({ targetresource, "BaseGame"});
	}
	//end check for resources
	//check for content 
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (mod->IsEnabled()) {
			string dir = filesystem::current_path().parent_path().string() + "\\mods\\" + mod->GetDir();
			vector<string> paths = { dir + "\\content-repentogon\\" + xml.filename, dir + "\\content-dlc3\\" + xml.filename, dir + "\\content\\" + xml.filename };
			for (const string& path : paths) {
				if (filesystem::exists(path)) {
					lastmodid = string(mod->GetId());
					if (string(lastmodid).length() == 0) {
						lastmodid = string(mod->GetDir());
					} 
					dirstoload.push_back({path, lastmodid});
					break;
				}
			}
		}
	}
	//end check for content
	//load everything
	int count = 0;
	for (const pair<string,string>& path : dirstoload) {
		xml_document<char>* xmldoc = new xml_document<char>();
		if (GetContent(path.first, xmldoc)) {
			xml_node<char>* root = xmldoc->first_node(xml.rootnodename.c_str());
			LoadGenericXMLData(XMLStuff.CustomXMLData[xml.filename], root, count > 0, path.first,path.second); //first element in the vector is the resrouce
			//UpdateRelEntTracker(XMLStuff.FxLayerData, &XMLStuff.BackdropData->relfxlayers, "backdrop");
			count++;
		}
	}
		
	//end load everything
}

inline bool LoadCustomXMLs() {
	bool did = false;
	while (!pendingcustomxmls.empty()) {
		LoadCustomXML(pendingcustomxmls.back());
		pendingcustomxmls.pop_back();
		did = true;
	}
	for (auto entry : XMLStuff.CustomXMLData) {
		getxmlnodeidbyname[entry.first] = xmlnodetypetodata.size();
		xmlnodetypetodata.push_back(entry.second);
	}
	return did;
}

inline void RegisterGenericCustomXML(string filename, string rootname,string entrynodename, bool loaditNOW = false) {
	CustomXML xml;
	xml.filename = filename;
	xml.rootnodename = rootname;
	xml.entrynodename = entrynodename;
	if (!loaditNOW) {
		pendingcustomxmls.push_back(xml);
	}
	else {
		LoadCustomXML(xml);
	}
}



inline int GetMaxIdFromChilds(const xml_node<char>* parentnode,const char* attrname = "id") {
	int maxid = -1;
	for (const xml_node<char>* auxnodebabe = parentnode->first_node(); auxnodebabe; auxnodebabe = auxnodebabe->next_sibling()) {
		const xml_attribute<char>* attr = auxnodebabe->first_attribute(attrname);
		if (attr && (stoi(attr->value()) > maxid)) {
			maxid = stoi(attr->value());
		}
	}
	return maxid;
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
	xmlnodeenum["bosscolors"] = 25;
	xmlnodeenum["fxlayers"] = 26;
	xmlnodeenum["fxparams"] = 27;
	xmlnodeenum["fxrays"] = 28;
	xmlnodeenum["babies"] = 29;
	xmlnodeenum["name"] = 99; //for mod metadata
}

extern unordered_map<string, int> xmlmaxnode;
inline void initxmlmaxnodeenum() {
	xmlmaxnode["giantbook.xml"] = 46;
	xmlmaxnode["nightmares.xml"] = 16;
	xmlmaxnode["playerforms.xml"] = 14;
	xmlmaxnode["cutscenes.xml"] = 26;
	xmlmaxnode["backdrops.xml"] = 62;
}

extern unordered_map<string, int> xmlfullmerge;
inline void initxmlfullmergelist() {
	xmlfullmerge["bosscolors.xml"] = 1;	
}


#endif