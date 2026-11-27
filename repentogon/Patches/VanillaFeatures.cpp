#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"
#include "VanillaFeatures.h"
#include "rapidxml.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// These hooks allow for reverting Repentance+ changes to 1.7.9b behavior.

static bool IsModXmlPath(const char* xmlpath) {
	std::string path = xmlpath ? xmlpath : "";
	for (char& c : path) {
		c = (c == '\\') ? '/' : (char)std::tolower((unsigned char)c);
	}
	return path.find("/mods/") != std::string::npos;
}

// ----- ITEM METADATA OVERRIDE SYSTEM -----
// This allows us to edit items_metadata.xml dynamically while still letting mod values take precedent.
static constexpr uint64_t TAG_OFFENSIVE = 1ull << 20;

struct ItemMetadataOverride {
	bool REPENTOGONOptions::* toggle; // Rep+ behavior while true; the override applies while false
	int id;
	uint64_t addTags = 0;
	uint64_t removeTags = 0;
	int quality = -1; // -1 leaves the quality alone
};

static const ItemMetadataOverride itemMetadataOverrides[] = {
	// 1.9.7.11 removes Glowing Hourglass and Book of Shadows from Tainted Lost's pool.
	{ &REPENTOGONOptions::vfTaintedLostPoolNoHourglassShadows, COLLECTIBLE_BOOK_OF_SHADOWS, TAG_OFFENSIVE },
	{ &REPENTOGONOptions::vfTaintedLostPoolNoHourglassShadows, COLLECTIBLE_GLOWING_HOUR_GLASS, TAG_OFFENSIVE },
};

struct ItemMetadataState {
	uint64_t addedTags = 0;
	uint64_t removedTags = 0;
	bool qualityApplied = false;
	int originalQuality = 0;
	int appliedQuality = 0;
	bool modTags = false;
	bool modQuality = false;
};

static std::unordered_map<int, ItemMetadataState> itemMetadataStates;

static void UndoItemMetadata(ItemConfig_Item* item, ItemMetadataState& state) {
	item->tags = (item->tags & ~state.addedTags) | state.removedTags;
	state.addedTags = state.removedTags = 0;

	if (state.qualityApplied && item->quality == state.appliedQuality) {
		item->quality = state.originalQuality;
	}
	state.qualityApplied = false;
}

static void ApplyItemMetadata(int id, ItemConfig_Item* item, ItemMetadataState& state) {
	uint64_t addTags = 0;
	uint64_t removeTags = 0;
	int quality = -1;
	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		if (entry.id != id || repentogonOptions.*entry.toggle) {
			continue;
		}
		addTags |= entry.addTags;
		removeTags |= entry.removeTags;
		if (entry.quality >= 0) {
			quality = entry.quality;
		}
	}

	if (!state.modTags) {
		state.addedTags = addTags & ~item->tags;
		state.removedTags = removeTags & item->tags;
		item->tags = (item->tags | addTags) & ~removeTags;
	}

	if (!state.modQuality && quality >= 0) {
		state.qualityApplied = true;
		state.originalQuality = item->quality;
		state.appliedQuality = quality;
		item->quality = quality;
	}
}

static void ForEachOverriddenItem(ItemConfig* config, void (*fn)(int, ItemConfig_Item*, ItemMetadataState&)) {
	for (auto& [id, state] : itemMetadataStates) {
		if (ItemConfig_Item* item = config->GetCollectible(id)) {
			fn(id, item, state);
		}
	}
}

void ApplyVanillaItemMetadata() {
	ForEachOverriddenItem(g_Manager->GetItemConfig(), [](int id, ItemConfig_Item* item, ItemMetadataState& state) {
		UndoItemMetadata(item, state);
		ApplyItemMetadata(id, item, state);
	});
}

// ----- ITEM POOL WEIGHT OVERRIDE SYSTEM -----
struct PoolWeights {
	float weight;
	float decreaseBy;
	float removeOn;

	bool Matches(const PoolItem& item) const {
		return item._initialWeight == weight && item._decreaseBy == decreaseBy && item._removeOn == removeOn;
	}
};

struct ItemPoolOverride {
	bool REPENTOGONOptions::* toggle; // Rep+ values while true, Rep values while false
	int pool;
	int id;
	PoolWeights repPlus;
	PoolWeights vanilla;
};

static const ItemPoolOverride itemPoolOverrides[] = {
	// 1.9.7.11 makes the D20 rarer in the Greed Mode shop.
	{ &REPENTOGONOptions::vfGreedShopD20Rarity, POOL_GREED_SHOP, COLLECTIBLE_D20, { 0.1f, 0.1f, 0.01f }, { 1.0f, 1.0f, 0.1f } },
};

static void ApplyPoolWeights() {
	for (const ItemPoolOverride& entry : itemPoolOverrides) {
		const bool repPlus = repentogonOptions.*entry.toggle;
		const PoolWeights& from = repPlus ? entry.vanilla : entry.repPlus;
		const PoolWeights& to = repPlus ? entry.repPlus : entry.vanilla;

		ItemPool_Item& pool = g_Game->_itemPool._pools[entry.pool];
		for (PoolItem& item : pool._poolList) {
			if (item._itemID != (uint32_t)entry.id || !from.Matches(item)) {
				continue;
			}

			item._weight *= to.weight / from.weight;
			item._initialWeight = to.weight;
			item._decreaseBy = to.decreaseBy;
			item._removeOn = to.removeOn;
			pool._totalWeight += to.weight - from.weight;
		}
	}
}

void ApplyVanillaPoolWeights() {
	if (Isaac::IsInGame()) {
		ApplyPoolWeights();
	}
}

HOOK_METHOD(ItemPool, shuffle_pools, () -> void) {
	ApplyPoolWeights();
	super();
}

void RefreshVanillaFeatureStats(const bool* toggle) {
	int cacheFlags = 0;
	if (toggle == &repentogonOptions.vfLudovicoShotSpeed) {
		cacheFlags |= CACHE_SHOTSPEED;
	}

	if (cacheFlags == 0 || !Isaac::IsInGame()) {
		return;
	}

	for (Entity_Player* player : g_Game->GetPlayerManager()->_playerList) {
		player->AddCacheFlags(cacheFlags);
		player->EvaluateItems();
	}
}

HOOK_METHOD(ItemConfig, LoadMetadata, (char* xmlpath, ModEntry* modentry) -> void) {
	const bool fromMod = IsModXmlPath(xmlpath);

	if (fromMod) {
		ForEachOverriddenItem(this, [](int, ItemConfig_Item* item, ItemMetadataState& state) { UndoItemMetadata(item, state); });
	}
	else {
		itemMetadataStates.clear();
	}

	std::unordered_map<int, std::pair<uint64_t, int>> before;
	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		if (ItemConfig_Item* item = this->GetCollectible(entry.id)) {
			before[entry.id] = { item->tags, item->quality };
		}
	}

	super(xmlpath, modentry);

	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		ItemConfig_Item* item = this->GetCollectible(entry.id);
		if (!item) {
			continue;
		}

		ItemMetadataState& state = itemMetadataStates[entry.id];
		if (fromMod) {
			auto previous = before.find(entry.id);
			if (previous == before.end()) {
				state.modTags = state.modQuality = true;
			}
			else {
				state.modTags |= item->tags != previous->second.first;
				state.modQuality |= item->quality != previous->second.second;
			}
		}
	}

	ForEachOverriddenItem(this, ApplyItemMetadata);
}
// ----- END ITEM METADATA OVERRIDE -----

// ----- ABYSS REIMPLEMENTATION -----
// Full reimplementation of Repentance's Abyss, pre 1.9.7.4 rework.

static const BitSet128 kLocustTearHoming(1u << 2, 0, 0, 0);
static const BitSet128 kLocustTearShielded(0, 1u << 2, 0, 0);
static const BitSet128 kLocustTearBone(0, 1u << 19, 0, 0);
static const BitSet128 kLocustTearJacobs(0, 1u << 23, 0, 0);
static const BitSet128 kLocustTearRift(0, 0, 1u << 12, 0);
static const BitSet128 kLocustTearLudovico(0, 0, 0, 1u << 31);

static constexpr unsigned int FAMILIAR_ABYSS_LOCUST = 231;
static constexpr unsigned long long FLAG_LOCUST_FRIENDLY_NO_TARGET = 0x20000010;
static constexpr unsigned long long FLAG_LOCUST_NO_KNOCKBACK = 0x4000000;

static bool HasAnyFlag(BitSet128 flags, const BitSet128& mask) { return flags.HasAny(mask); }

static float AbyssRandomFloat() {
	const unsigned int value = Isaac::genrand_int32();
	return (float)(double)value * 2.3283064365386963e-10f;
}

static float AbyssLength(float x, float y) { return std::sqrt(x * x + y * y); }

static Vector AbyssNormalized(const Vector& v) {
	const float length = AbyssLength(v.x, v.y);
	if (length > 0.0f) {
		const float inverse = 1.0f / length;
		return Vector(v.x * inverse, v.y * inverse);
	}
	return v;
}

static float AbyssDistance(const Vector& a, const Vector& b) { return AbyssLength(a.x - b.x, a.y - b.y); }

static Vector AbyssRandomVector() {
	const float angle = AbyssRandomFloat() * 3.1415927f * 2.0f;
	return Vector((float)std::cos((double)angle), (float)std::sin((double)angle));
}

struct Locust179 {
	float damageMultiplier = 1.0f;
	BitSet128 tearFlags;
	float damageMultiplier2 = 1.0f;
	BitSet128 tearFlags2;
	float speed = 1.0f;
	unsigned int collisionInterval = 7;
	float scale = 1.0f;
	float procChance = 0.25f;
	int count = 1;
	ColorMod overlayColor;
	ColorMod color;
	ColorMod backColor;
	std::string overlayGfx;
	std::string backGfx;
	std::string gfx;
};

static std::unordered_map<int, Locust179> locusts179;
static const Locust179 defaultLocust179;

static const Locust179& GetLocust179(int id) {
	auto it = locusts179.end();
	if (g_Manager->GetItemConfig()->GetCollectible(id)) {
		it = locusts179.find(id);
	}
	if (it == locusts179.end()) {
		it = locusts179.find(COLLECTIBLE_BREAKFAST);
	}
	return it != locusts179.end() ? it->second : defaultLocust179;
}

static const Locust179& FamiliarLocust179(Entity_Familiar* familiar) {
	return GetLocust179(familiar->_variant == FAMILIAR_ABYSS_LOCUST ? familiar->_subtype : COLLECTIBLE_BREAKFAST);
}

static const char vanillaLocustsXml179[] = R"(<locusts gfxroot="gfx/familiar/locusts/">
	<color name="default" />
	<color name="fly_red" r="768" g="512" b="512" or="0" og="-255" ob="-255"/>
	<color name="fly_green" r="512" g="768" b="512" or="-255" og="0" ob="-255"/>
	<color name="fly_blue" r="512" g="512" b="768" or="-255" og="-255" ob="0"/>
	<color name="fly_yellow" r="1024" g="768" b="512" or="0" og="0" ob="-255"/>
	<color name="fly_orange" r="1024" g="512" b="512" or="0" og="0" ob="-255"/>
	<color name="fly_black" r="255" g="255" b="255"/>
	<color name="fly_purple" r="768" g="512" b="1024" or="0" og="-255" ob="0"/>
	<color name="fly_pink" r="1024" g="512" b="768" or="0" og="-255" ob="0"/>
	<color name="fly_cyan" r="680" g="1024" b="1024" or="0" og="0" ob="0"/>
	<locust id="2" count="3" color="fly_red" />
	<locust id="3" tearFlags="2" color="fly_purple" />
	<locust id="4" damageMultiplier="3" scale="1.25" color="fly_red" gfx="big.png" />
	<locust id="6" damageMultiplier="1" speed="1.3" collisionInterval="5" color="fly_yellow" />
	<locust id="7" damageMultiplier="2" color="fly_red" />
	<locust id="10" damageMultiplier="0.5" color="fly_black" count="2" />
	<locust id="13" tearFlags2="4" color="fly_green" />
	<locust id="25" color="fly_red" />
	<locust id="103" tearFlags2="4" color="fly_green" />
	<locust id="118" damageMultiplier="1" speed="1" scale="1.25" color="fly_black" gfx="big.png" />
	<locust id="149" damageMultiplier="1.5" speed="0.8" scale="1.25" color="fly_green" tearFlags="4 12" gfx="big.png" />
	<locust id="153" count="4" color="fly_red" />
	<locust id="257" tearFlags="22" color="fly_orange" backGfx="flame.png" backColor="fly_orange" />
	<locust id="305" tearFlags2="4" color="fly_green" />
	<locust id="374" tearFlags2="39" color="fly_cyan" backGfx="glow.png" backColor="fly_cyan" />
	<locust id="494" tearFlags="55" color="fly_cyan" overlayGfx="spark.png" overlayColor="fly_cyan" />
	<locust id="559" tearFlags="55" color="fly_cyan" overlayGfx="spark.png" overlayColor="fly_cyan" />
</locusts>)";

static const char* LocustXmlValue(rapidxml::xml_attribute<char>* attribute) {
	return attribute && attribute->value() ? attribute->value() : "";
}

static void ParseLocustTearFlags179(const char* text, BitSet128& flags) {
	std::istringstream stream(text);
	std::string token;
	while (stream >> token) {
		const int bit = std::atoi(token.c_str());
		if (bit < 0 || bit >= 128) {
			continue;
		}
		unsigned int* words[] = { &flags.ll, &flags.lh, &flags.hl, &flags.hh };
		*words[bit >> 5] |= 1u << (bit & 0x1f);
	}
}

static ColorMod ParseLocustColorPreset179(rapidxml::xml_node<char>* node) {
	ColorMod color;
	for (rapidxml::xml_attribute<char>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
		const std::string name = attribute->name();
		const float value = (float)std::atof(LocustXmlValue(attribute)) / 255.0f;
		if (name == "r") color._tint[0] = value;
		else if (name == "g") color._tint[1] = value;
		else if (name == "b") color._tint[2] = value;
		else if (name == "a") color._tint[3] = value;
		else if (name == "cr") color._colorize[0] = value;
		else if (name == "cg") color._colorize[1] = value;
		else if (name == "cb") color._colorize[2] = value;
		else if (name == "ca") color._colorize[3] = value;
		else if (name == "or") color._offset[0] = value;
		else if (name == "og") color._offset[1] = value;
		else if (name == "ob") color._offset[2] = value;
	}
	return color;
}

static void ParseLocustXml179(char* text) {
	rapidxml::xml_document<char> document;
	try {
		document.parse<0>(text);
	}
	catch (...) {
		return;
	}

	rapidxml::xml_node<char>* root = document.first_node("locusts");
	if (!root) {
		return;
	}

	const std::string gfxRoot = LocustXmlValue(root->first_attribute("gfxroot"));
	std::unordered_map<std::string, ColorMod> colors;
	ItemConfig* itemConfig = g_Manager->GetItemConfig();

	for (rapidxml::xml_node<char>* node = root->first_node(); node; node = node->next_sibling()) {
		const std::string tag = node->name();
		if (tag == "color") {
			const std::string name = LocustXmlValue(node->first_attribute("name"));
			if (!name.empty()) {
				colors[name] = ParseLocustColorPreset179(node);
			}
			continue;
		}

		rapidxml::xml_attribute<char>* idAttribute = node->first_attribute("id");
		if (tag != "locust" || !idAttribute) {
			continue;
		}

		const int id = std::atoi(LocustXmlValue(idAttribute));
		if (id < 0 || !itemConfig->GetCollectible(id)) {
			continue;
		}

		Locust179 locust;
		bool keepsProcChance = false;
		for (rapidxml::xml_attribute<char>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
			const std::string name = attribute->name();
			const char* value = LocustXmlValue(attribute);
			if (name == "speed") locust.speed = (float)std::atof(value);
			else if (name == "scale") locust.scale = (float)std::atof(value);
			else if (name == "procChance") { locust.procChance = (float)std::atof(value); keepsProcChance = true; }
			else if (name == "tearFlags") ParseLocustTearFlags179(value, locust.tearFlags);
			else if (name == "tearFlags2") { ParseLocustTearFlags179(value, locust.tearFlags2); keepsProcChance = true; }
			else if (name == "damageMultiplier") locust.damageMultiplier = (float)std::atof(value);
			else if (name == "damageMultiplier2") { locust.damageMultiplier2 = (float)std::atof(value); keepsProcChance = true; }
			else if (name == "color" || name == "backColor" || name == "overlayColor") {
				auto preset = colors.find(value);
				const ColorMod color = preset != colors.end() ? preset->second : ColorMod();
				(name == "color" ? locust.color : name == "backColor" ? locust.backColor : locust.overlayColor) = color;
			}
			else if (name == "gfx") locust.gfx = gfxRoot + value;
			else if (name == "backGfx") locust.backGfx = gfxRoot + value;
			else if (name == "overlayGfx") locust.overlayGfx = gfxRoot + value;
			else if (name == "count") locust.count = (short)std::atoi(value);
		}

		if (!keepsProcChance) {
			locust.procChance = 0.0f;
		}

		locusts179[id] = locust;
	}
}

HOOK_METHOD(ItemConfig, LoadLocustConfig, (char* xmlpath, ModEntry* modentry) -> void) {
	super(xmlpath, modentry);

	if (!IsModXmlPath(xmlpath)) {
		locusts179.clear();
		std::vector<char> text(vanillaLocustsXml179, vanillaLocustsXml179 + sizeof(vanillaLocustsXml179));
		ParseLocustXml179(text.data());
		return;
	}

	std::ifstream file(xmlpath, std::ios::binary);
	if (!file) {
		return;
	}
	std::vector<char> text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	text.push_back('\0');
	ParseLocustXml179(text.data());
}

static float AbyssFamiliarMultiplier(Entity_Familiar* familiar) {
	familiar->GetMultiplier();
	float multiplier;
	__asm {
		movss multiplier, xmm0
	}
	return multiplier;
}

static float AbyssCollisionMass(Entity* collider, Entity* other) {
	collider->GetCollisionMass(other);
	float mass;
	__asm {
		movss mass, xmm0
	}
	return mass;
}

static void AbyssSpawnLocusts(Entity_Player* player, int collectible, const Vector& position) {
	const Locust179& locust = GetLocust179(collectible);
	for (int i = 0; i < locust.count; ++i) {
		const float speed = AbyssRandomFloat() * 4.0f + 6.0f;
		float angle = AbyssRandomFloat() * 3.1415927f;
		angle += angle;
		const Vector velocity((float)std::cos((double)angle) * speed, (float)std::sin((double)angle) * speed);
		const unsigned int seed = Isaac::genrand_int32();
		Entity_Familiar* familiar = (Entity_Familiar*)g_Game->Spawn(ENTITY_FAMILIAR, FAMILIAR_ABYSS_LOCUST, position, velocity, player, collectible, seed, 0);
		familiar->_state = (Isaac::genrand_int32() & 1) + 4;
		familiar->Update();
	}

	const unsigned int seed = Isaac::genrand_int32();
	Entity* poof = g_Game->Spawn(ENTITY_EFFECT, 15, Vector(position.x + 0.0f, position.y + 10.0f), Vector(0.0f, 0.0f), nullptr, 0, seed, 0);
	ColorMod color(0.1f, 0.1f, 0.1f, 1.0f);
	poof->SetColor(&color, -1, 255, false, true);
	poof->Update();
}

static void AbyssLocustAppearance(Entity_Familiar* familiar) {
	const Locust179& locust = FamiliarLocust179(familiar);
	ANM2& sprite = familiar->_sprite;

	bool replaced = false;
	const std::string* sheets[] = { &locust.gfx, &locust.backGfx, &locust.overlayGfx };
	for (int layer = 0; layer < 3; ++layer) {
		if (!sheets[layer]->empty()) {
			std::string sheet = *sheets[layer];
			sprite.ReplaceSpritesheet(layer, sheet);
			replaced = true;
		}
	}
	if (replaced) {
		sprite.LoadGraphics(false);
	}

	const ColorMod* colors[] = { &locust.color, &locust.backColor, &locust.overlayColor };
	for (int layer = 0; layer < 3; ++layer) {
		if (LayerState* state = sprite.GetLayer(layer)) {
			state->_color = *colors[layer];
		}
	}

	familiar->_size = locust.scale * familiar->_entityConfig->collisionRadius;
}

static void AbyssLocustTriggerCollision(Entity_Familiar* familiar, const BitSet128& flags, Entity* collider) {
	Entity_Player* player = familiar->_player;
	Room* room = g_Game->GetCurrentRoom();

	if (HasAnyFlag(flags, kLocustTearBone)) {
		for (int i = 0; i < 2; ++i) {
			float angle = AbyssRandomFloat() * 3.1415927f;
			angle += angle;
			const float sine = (float)std::sin((double)angle);
			const Vector velocity((float)std::cos((double)angle) * 10.0f, sine * 10.0f);
			const unsigned int seed = Isaac::genrand_int32();
			const Vector position(familiar->_pos.x - familiar->_velocity.x, familiar->_pos.y - familiar->_velocity.y);
			Entity_Tear* tear = (Entity_Tear*)g_Game->Spawn(ENTITY_TEAR, 29, position, velocity, player, 0, seed, 0);

			tear->SetCollisionDamage(familiar->_collisionDamage * 0.5f);
			BitSet128 tearFlags = flags;
			tearFlags.RemoveFlags(kLocustTearLudovico);
			tearFlags.RemoveFlags(kLocustTearBone);
			tear->SetTearFlags(tearFlags);
			tear->SetColor(&familiar->_sprite._color, -1, 255, false, true);
			tear->_parent.SetReference(familiar->_parent.GetReference());
			tear->Update();
		}
	}

	if (HasAnyFlag(flags, kLocustTearJacobs)) {
		Entity_Effect* lightning = Game::ChainLightning(&familiar->_pos, familiar->_collisionDamage * 0.5f, flags, player);
		if (collider) {
			lightning->_target.SetReference(collider);
		}
		lightning->Update();
	}

	if (HasAnyFlag(flags, kLocustTearRift) && room->IsPositionInRoom(&familiar->_pos, -10.0f)) {
		const float damage = familiar->_collisionDamage * 0.5f;
		Vector position;
		room->GetClampedPosition(&position, &familiar->_pos, 0.0f, 0.0f, 0.0f, 0.0f);
		const unsigned int seed = Isaac::genrand_int32();
		Entity_Effect* rift = (Entity_Effect*)g_Game->Spawn(ENTITY_EFFECT, 180, position, Vector(0.0f, 0.0f), player, 0, seed, 0); // EFFECT_RIFT
		rift->_sprite._scale = Vector(familiar->_sprite._scale.x * 1.0f, familiar->_sprite._scale.y * 1.0f);
		rift->SetCollisionDamage(damage);
		rift->_timeout = 60;
		rift->_lifespan = 60;
		rift->Update();
	}
}

static void AbyssLocustHit(Entity_Familiar* familiar, Entity* collider) {
	const unsigned int type = collider->_type;
	if (type - 10 > 0x3dd) {
		return;
	}
	if (!collider->IsVulnerableEnemy(nullptr) && type != ENTITY_FIREPLACE && type != ENTITY_MOVABLE_TNT && type != ENTITY_POOP) {
		return;
	}
	if (familiar->_hitList.IsEntityInHitList(collider)) {
		return;
	}
	familiar->_hitList.AddEntityToHitList(collider);

	const Locust179& locust = FamiliarLocust179(familiar);
	BitSet128 flags = locust.tearFlags;
	float multiplier = locust.damageMultiplier;
	if (locust.procChance > 0.0f && locust.procChance > AbyssRandomFloat()) {
		flags.AddFlags(locust.tearFlags2);
		multiplier *= locust.damageMultiplier2;
	}

	Entity_Player* player = familiar->_player;
	const float damage = (AbyssFamiliarMultiplier(familiar) * 0.25f + 0.75f) * multiplier * player->_tearPoisonDamage;

	unsigned long long damageFlags = 0;
	Entity_Tear::ApplyTearFlagModifiers(0, &damageFlags, 0, flags, 0.0f, familiar);
	EntityRef source(familiar);
	collider->TakeDamage(damage, damageFlags, &source, 30);
	Entity_Tear::ApplyTearFlagEffects(collider, &familiar->_pos, flags, familiar, damage);
	AbyssLocustTriggerCollision(familiar, flags, collider);

	if (familiar->_fireCooldown == -1) {
		familiar->_fireCooldown = -2;
	}

	if (player->_playerType == PLAYER_APOLLYON_B && player->HasCollectible(COLLECTIBLE_BIRTHRIGHT, false) && collider->IsVulnerableEnemy(nullptr) &&
		((familiar->_targetPos.x == 0.0f && familiar->_targetPos.y == 0.0f) || !familiar->_target.GetReference())) {
		familiar->SetTarget(collider);
		familiar->_targetPos = familiar->_pos;
	}

	if (!(collider->_flags & FLAG_LOCUST_NO_KNOCKBACK)) {
		Vector push = AbyssNormalized(familiar->_velocity);
		const float mass = AbyssCollisionMass(collider, familiar);
		if (mass > 0.0f) {
			push.x /= mass;
			push.y /= mass;
		}
		const float strength = familiar->_mass * 1.8000001f;
		push.x *= strength;
		push.y *= strength;
		collider->AddVelocity(&push, false);
	}
}

static void AbyssLocustAI(Entity_Familiar* familiar) {
	Entity_Player* player = familiar->_player;
	Room* room = g_Game->GetCurrentRoom();
	Vector& position = familiar->_pos;
	Vector& velocity = familiar->_velocity;

	*(unsigned int*)&familiar->_flags |= 0x44000000;

	const int state = familiar->_state;
	if (state > 0) {
		familiar->_friction *= 0.96f;
		familiar->_state = state - 1;
		familiar->_entityCollisionClass = 0;
		if (state - 1 == 0) {
			familiar->_entityCollisionClass = 4;
		}
		return;
	}

	const Locust179& locust = FamiliarLocust179(familiar);

	if (familiar->_fireCooldown >= 0) {
		bool wander = state == 0;
		if (state == -1 && 100.0f > AbyssDistance(player->_pos, position)) {
			familiar->_state = 0;
			wander = true;
		}

		if (wander) {
			velocity.x *= 0.86f;
			velocity.y *= 0.86f;
			const float speed = AbyssRandomFloat() * 1.5f + 0.5f;
			const float toPlayerX = (player->_pos.x - position.x) * 0.02f;
			const float toPlayerY = (player->_pos.y - position.y) * 0.02f;
			float angle = AbyssRandomFloat() * 3.1415927f;
			angle += angle;
			const float sine = (float)std::sin((double)angle);
			const float cosine = (float)std::cos((double)angle);
			Vector direction(cosine + toPlayerX, toPlayerY + sine);
			const float length = AbyssLength(direction.x, direction.y);
			if (length > 0.0f) {
				const float inverse = 1.0f / length;
				direction.x *= inverse;
				direction.y *= inverse;
			}
			velocity.x += direction.x * speed * familiar->_speedMultiplier;
			velocity.y += direction.y * speed * familiar->_speedMultiplier;
		}
		else {
			const float dx = player->_pos.x - position.x;
			const float dy = player->_pos.y - position.y;
			const float length = AbyssLength(dx, dy);
			float step = 0.0f;
			if (length >= 0.1f) {
				step = std::min(length * 0.15f, 6.0f) / length;
			}
			velocity.x += dx * step;
			velocity.y += dy * step;
			familiar->_friction *= 0.8f;
		}

		int cooldown = familiar->_fireCooldown;
		if (cooldown > 0) {
			if (familiar->_state != 0) {
				familiar->_entityGridCollisionClass = 0;
				familiar->_entityCollisionClass = 0;
				return;
			}
			familiar->_fireCooldown = --cooldown;
		}

		if (cooldown == 0 && player->_fireDirection != -1) {
			Vector direction = Isaac::GetAxisAlignedUnitVectorFromDir(player->_fireDirection);
			int aimDirection = 0;
			const bool aimed = familiar->TryAimAtMarkedTarget(&direction, &aimDirection, nullptr);
			const float speed = locust.speed * 10.0f;

			Vector clamped;
			room->GetClampedPosition(&clamped, &position, 5.0f, 5.0f, 5.0f, 5.0f);
			familiar->_friction = 1.0f;
			position = clamped;
			const float launchX = direction.x * speed;
			const float launchY = direction.y * speed;
			velocity.x = launchX;
			velocity.y = launchY;

			if (!aimed) {
				float playerX = player->_velocity.x * 0.6f;
				float playerY = player->_velocity.y * 0.6f;
				playerX += playerX;
				playerY += playerY;
				const Vector normal = AbyssNormalized(direction);
				const float dot = playerY * normal.y + playerX * normal.x;
				const float projectionX = dot * normal.x;
				const float projectionY = dot * normal.y;
				if (0.0f > direction.y * projectionY + direction.x * projectionX) {
					playerX -= projectionX;
					playerY -= projectionY;
				}
				velocity.x = launchX + playerX;
				velocity.y = launchY + playerY;
			}

			familiar->_state = -1;
			familiar->_fireCooldown = -1;

			std::vector<Entity_Familiar*> locusts;
			EntityList_EL* entities = room->GetEntityList()->GetUpdateEL();
			for (unsigned int i = 0; i < entities->_size; ++i) {
				Entity* entity = entities->_data[i];
				if (entity && entity->_type == ENTITY_FAMILIAR && ((Entity_Familiar*)entity)->_player == player && entity->_variant == familiar->_variant) {
					locusts.push_back((Entity_Familiar*)entity);
				}
			}
			if (!locusts.empty()) {
				const int count = (int)locusts.size();
				const int delay = std::max(3, 10 - count / 2);
				for (Entity_Familiar* other : locusts) {
					if (other != familiar && other->_fireCooldown >= 0 && other->_fireCooldown < delay) {
						other->_fireCooldown = delay;
					}
				}
			}
		}

		familiar->_entityGridCollisionClass = 0;
		familiar->_entityCollisionClass = 0;
		return;
	}

	familiar->_state = -1;
	float bestHuntScore = 1000000000.0f;

	if (familiar->_fireCooldown == -1 && HasAnyFlag(locust.tearFlags, kLocustTearHoming)) {
		if (!familiar->_target.GetReference() && familiar->IsFrame(4, familiar->_index)) {
			float bestScore = 1000000000.0f;
			Entity* best = nullptr;
			Vector ahead(velocity.x * 5.0f + position.x, velocity.y * 5.0f + position.y);
			EntityList_EL found = room->GetEntityList()->QueryRadius(&ahead, 128.0f, 8);
			for (unsigned int i = 0; i < found._size; ++i) {
				Entity* enemy = found._data[i];
				if (!enemy->IsVulnerableEnemy(nullptr) || (enemy->_flags & FLAG_LOCUST_FRIENDLY_NO_TARGET) || familiar->_hitList.IsEntityInHitList(enemy) || (Isaac::genrand_int32() & 1) == 0) {
					continue;
				}
				if (familiar->_entityGridCollisionClass == 4) {
					Vector enemyPosition = enemy->_pos;
					if (!room->CheckLine(&position, &enemyPosition, 3, 0, false, false, nullptr)) {
						continue;
					}
				}
				const Vector from(velocity.x * 5.0f + position.x, velocity.y * 5.0f + position.y);
				float score = AbyssDistance(from, enemy->_pos) + AbyssRandomFloat() * 128.0f;
				if (familiar->_hitList.IsEntityInHitList(enemy)) {
					score += 1000.0f;
				}
				if (bestScore > score) {
					bestScore = score;
					best = enemy;
				}
			}
			familiar->_target.SetReference(best);
			if (found._size) {
				found.Destroy();
			}
		}

		if (Entity* target = familiar->_target.GetReference()) {
			const Vector away(position.x - target->_pos.x, position.y - target->_pos.y);
			const float speed = AbyssLength(velocity.x, velocity.y);
			const Vector normal = AbyssNormalized(away);
			const float stepX = familiar->_speedMultiplier * normal.x;
			const float stepY = familiar->_speedMultiplier * normal.y;
			float newX = velocity.x * 0.8f - (stepX + stepX);
			float newY = velocity.y * 0.8f - (stepY + stepY);
			const float length = AbyssLength(newX, newY);
			if (length > 0.0f) {
				newX *= speed / length;
				newY *= speed / length;
			}
			velocity.x = newX;
			velocity.y = newY;
		}
	}

	bool returning = false;
	if (familiar->_collidesWithGrid) {
		const Vector wall = AbyssNormalized(familiar->_velocityOnCollidingWithWall);
		const Vector& wallDirection = familiar->_directionOfWallCollide;
		returning = wall.y * wallDirection.y + wall.x * wallDirection.x > 0.3f;
	}
	if (!returning && familiar->_subtype == COLLECTIBLE_NUMBER_ONE && AbyssDistance(position, player->_pos) > 100.0f) {
		returning = true;
	}
	if (!returning && familiar->_fireCooldown < -1) {
		returning = true;
	}

	if (returning) {
		familiar->_entityGridCollisionClass = 0;

		const int gridX = (int)((position.x - 40.0f) / 40.0f + 0.5f);
		const int gridY = (int)((position.y - 120.0f) / 40.0f + 0.5f);
		if (gridX >= 0 && gridX < (int)room->_gridWidth && gridY >= 0 && gridY < (int)room->_gridHeight) {
			const unsigned int index = gridY * room->_gridWidth + gridX;
			if (index <= 0x1bf) {
				if (GridEntity* grid = (&room->_gridEntities[0])[index]) {
					EntityRef noSource(nullptr);
					grid->Hurt(1, &noSource);
				}
			}
		}

		if (familiar->_targetPos.x == 0.0f && familiar->_targetPos.y == 0.0f) {
			velocity.x *= 0.8f;
			velocity.y *= 0.8f;
			if (--familiar->_fireCooldown < -10) {
				familiar->_fireCooldown = 30;
				familiar->_targetPos = Vector(0.0f, 0.0f);
			}
		}
		else {
			if (Entity* target = familiar->_target.GetReference()) {
				familiar->_targetPos = target->_pos;
			}

			if (player->_fireDirection == -1 || familiar->_fireCooldown < -15) {
				familiar->_targetPos = Vector(0.0f, 0.0f);
			}
			else {
				velocity.x *= 0.86f;
				velocity.y *= 0.86f;
				if (familiar->_fireCooldown >= -15) {
					--familiar->_fireCooldown;
				}

				Entity* target = familiar->_target.GetReference();
				if (target && target->_dead) {
					familiar->_target.SetReference(nullptr);
					target = nullptr;
				}
				if (!target && (Isaac::genrand_int32() & 1) == 0) {
					Entity* best = nullptr;
					EntityList_EL found = room->GetEntityList()->QueryRadius(&position, 50.0f, 8);
					for (unsigned int i = 0; i < found._size; ++i) {
						Entity* enemy = found._data[i];
						if (!enemy->IsVulnerableEnemy(nullptr) || (enemy->_flags & FLAG_LOCUST_FRIENDLY_NO_TARGET)) {
							continue;
						}
						const float score = AbyssDistance(position, enemy->_pos) + AbyssRandomFloat() * 128.0f;
						if (bestHuntScore > score) {
							bestHuntScore = score;
							best = enemy;
						}
					}
					if (best) {
						familiar->_target.SetReference(best);
					}
					if (found._size) {
						found.Destroy();
					}
				}

				if (familiar->_target.GetReference()) {
					familiar->_fireCooldown = -2;
				}

				const float speed = AbyssRandomFloat() * 1.5f + 0.5f;
				const Vector jitter = AbyssRandomVector();
				const Vector toTarget((familiar->_targetPos.x - position.x) * 0.02f + jitter.x, (familiar->_targetPos.y - position.y) * 0.02f + jitter.y);
				const Vector direction = AbyssNormalized(toTarget);
				velocity.x += speed * direction.x * familiar->_speedMultiplier;
				velocity.y += direction.y * speed * familiar->_speedMultiplier;
			}
		}
	}

	familiar->_entityGridCollisionClass = 3;
	familiar->_entityCollisionClass = 4;
}

static void AbyssUse(Entity_Player* player) {
	ItemConfig_Item* queued = player->_queuedItem._item;
	if (queued && (queued->type == 1 || queued->type == 3 || queued->type == 4) && !(queued->tags & 0x8000)) { // !TAG_QUEST
		AbyssSpawnLocusts(player, queued->id, player->_pos);
		player->DestroyQueueItem();
	}

	Room* room = g_Game->GetCurrentRoom();
	EntityList_EL* entities = room->GetEntityList()->GetUpdateEL();
	bool absorbed = false;
	for (unsigned int i = 0; i < entities->_size; ++i) {
		Entity* entity = entities->_data[i];
		if (entity->_type != ENTITY_PICKUP || entity->_dead) {
			continue;
		}

		Entity_Pickup* pickup = entity->ToPickup();
		if (!pickup || !pickup->CanReroll() || pickup->_price != 0 || pickup->_variant != 100 || pickup->_subtype == 0 || pickup->_timeout > 0) {
			continue;
		}

		const Vector position = pickup->_pos;
		AbyssSpawnLocusts(player, pickup->_subtype, position);
		pickup->_timeout = 4;
		pickup->TriggerTheresOptionsPickup();
		absorbed = true;
	}

	if (!absorbed) {
		return;
	}

	Manager::PlaySound(834, 1.0f, 2, false, 1.0f);

	if (room->_roomType == 11 || room->_roomType == 17) { // ROOM_CHALLENGE, ROOM_BOSSRUSH
		g_Game->GetAmbush()->StartChallenge();
	}
}

bool VanillaAbyssRework() {
	return repentogonOptions.vfAbyssRework;
}

bool VanillaAbyssLocustHit(Entity_Familiar* familiar, Entity* collider) {
	if (repentogonOptions.vfAbyssRework) {
		return false;
	}
	AbyssLocustHit(familiar, collider);
	return true;
}

bool VanillaAbyssUse(Entity_Player* player) {
	if (repentogonOptions.vfAbyssRework) {
		return false;
	}
	AbyssUse(player);
	return true;
}

HOOK_METHOD(Entity_Familiar, ai_abyss_locust, () -> void) {
	if (repentogonOptions.vfAbyssRework) {
		super();
		return;
	}
	AbyssLocustAI(this);
}

HOOK_STATIC(Isaac, SpawnLocust, (Entity_Player* player, int collectibleType, Vector* pos) -> void, __cdecl) {
	if (repentogonOptions.vfAbyssRework) {
		super(player, collectibleType, pos);
		return;
	}
	AbyssSpawnLocusts(player, collectibleType, *pos);
}

HOOK_METHOD(Entity_Familiar, UpdateLocustAppearance, () -> void) {
	if (repentogonOptions.vfAbyssRework) {
		super();
		return;
	}
	AbyssLocustAppearance(this);
}

HOOK_METHOD(Entity_Familiar, SetLocustConfigIndex, (int index) -> void) {
	if (repentogonOptions.vfAbyssRework) {
		super(index);
		return;
	}
	this->_coins = index;
	this->_size = FamiliarLocust179(this).scale * this->_entityConfig->collisionRadius;
}

HOOK_METHOD(Entity_Familiar, GetCollisionInterval, () -> int) {
	if (repentogonOptions.vfAbyssRework || this->_variant != FAMILIAR_ABYSS_LOCUST) {
		return super();
	}
	return (int)FamiliarLocust179(this).collisionInterval;
}

HOOK_METHOD(Entity_Familiar, CanBlockProjectiles, () -> bool) {
	if (repentogonOptions.vfAbyssRework || this->_variant != FAMILIAR_ABYSS_LOCUST) {
		return super();
	}
	BitSet128 flags = FamiliarLocust179(this).tearFlags;
	return flags.HasAny(kLocustTearShielded);
}
// ----- END ABYSS (1.7.9b) -----

// 1.9.7.11 makes The Siren unable to charm Isaac's Heart, and Lilith's Incubus.
HOOK_METHOD_PRIORITY(Entity_Familiar, CanCharm, 1000, () -> bool) {
	if (!repentogonOptions.vfSirenCharmHeartIncubus && (_variant == 62 || _variant == 80)) {
		return true;
	}
	
	return super();
}

// 1.9.7.11 spawns enemies in Boss Rush, Challenge Rooms, Great Gideon and Greed Mode waves through a spawn
// indicator effect. Repentance spawned them directly.
HOOK_STATIC(Entity_Effect, SpawnEnemyIndicator, (unsigned int type, unsigned int variant, const Vector* position, void* unused1, Entity* spawner, unsigned int subtype, unsigned int seed, void* unused2, int spawnGridIdx) -> Entity*, __stdcall) {
	if (repentogonOptions.vfEnemySpawnIndicators) {
		return super(type, variant, position, unused1, spawner, subtype, seed, unused2, spawnGridIdx);
	}

	Entity* entity = g_Game->Spawn(type, variant, *position, Vector(0, 0), spawner, subtype, seed, 0);

	if (spawnGridIdx != -1) {
		entity->_spawnGridIdx = spawnGridIdx;
	}

	if (spawner && spawner->_type == ENTITY_GIDEON) {
		entity->Update();
	}
	return entity;
}

// 1.9.7.11 gives most bosses a 1% chance to drop a thematic item or trinket.
HOOK_METHOD_PRIORITY(Room, GetBossThematicItem, -1000, (int* retCollectibleType, int* retTrinketType, unsigned int* seed) -> bool) {
	if (!repentogonOptions.vfBossThematicDrops) {
		*retCollectibleType = 0;
		*retTrinketType = 0;
		return false;
	}

	return super(retCollectibleType, retTrinketType, seed);
}
