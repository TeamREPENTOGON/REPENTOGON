#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include <memory>
#include "../../Patches/CustomModManager.h"

/*LUA_FUNCTION(Lua_GameGetMinimap)
{
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Minimap** toLua = (Minimap**)lua_newuserdata(L, sizeof(Minimap*));
	*toLua = game->GetMinimap();
	luaL_setmetatable(L, lua::metatables::MinimapMT);
	return 1;
}
*/

enum MinimapIconPosition {
	ICON_POSITION_BEFORE_MAP_ICONS = 0,
	ICON_POSITION_AFTER_MAP_ICONS = 1,
	ICON_POSITION_BEFORE_CURSE_ICONS = 2,
	ICON_POSITION_AFTER_CURSE_ICONS = 3,
};

struct MinimapIcon {
	ANM2* sprite;
	std_string anim;
	int frame;
	// Used for deleting icons
	int id;
};

static std::vector<std::unique_ptr<MinimapIcon>> BeforeMapIcons;
static std::vector<std::unique_ptr<MinimapIcon>> AfterMapIcons;
static std::vector<std::unique_ptr<MinimapIcon>> BeforeCurseIcons;
static std::vector<std::unique_ptr<MinimapIcon>> AfterCurseIcons;
static std::map<int, bool> TakenIds;

#pragma region Helper functions
void TryAddIcon(MinimapIcon icons[], unsigned int& num_icons, ANM2* sprite, std_string& anim, int frame) {
	if (num_icons < 7) {
		icons[num_icons].sprite = sprite;
		icons[num_icons].anim = anim;
		icons[num_icons].frame = frame;

		num_icons++;
	}
}

void TryAddAllIcons(MinimapIcon icons[], unsigned int& num_icons, std::vector<std::unique_ptr<MinimapIcon>>& toAdd) {
	for (auto& icon : toAdd) {
		if (num_icons >= 7) {
			break;
		}

		TryAddIcon(icons, num_icons, icon->sprite, icon->anim, icon->frame);
	}
}

int GetFreeId(std::vector<std::unique_ptr<MinimapIcon>>& icons) {
	int free_id = 1;
	bool found = false;

	while (TakenIds.count(free_id) > 0) {
		free_id++;
	}

	TakenIds[free_id] = true;

	return free_id;
}

void RemoveIcon(std::vector<std::unique_ptr<MinimapIcon>>& icons, int& id) {
	int idx_to_delete = 0;
	bool found = false;

	for (size_t i = 0; i < icons.size(); i++)
	{
		if (icons[i]->id == id) {
			idx_to_delete = i;
			found = true;

			TakenIds.erase(TakenIds.find(icons[i]->id));
			delete icons[i]->sprite;

			break;
		}
	}

	if (!found) {
		return;
	}

	icons.erase(icons.begin() + idx_to_delete);
}

void RemoveAllIcons(std::vector<std::unique_ptr<MinimapIcon>>& icons) {
	for (auto& icon : icons) {
		delete icon->sprite;
	}

	icons.clear();
}

bool ValidateSprite(ANM2* sprite, const char* anim, int& frame) {
	if (!sprite->_loaded) {
		return false;
	}

	sprite->SetAnimation(anim, true);
	AnimationData* anim_data = sprite->_animData;
	if (anim_data == nullptr) {
		return false;
	}

	if (frame >= anim_data->GetLength()) {
		return false;
	}

	return true;
}
#pragma endregion

HOOK_METHOD(Game, Exit, (bool shouldSave) -> void) {
	RemoveAllIcons(BeforeMapIcons);
	RemoveAllIcons(AfterMapIcons);
	RemoveAllIcons(BeforeCurseIcons);
	RemoveAllIcons(AfterCurseIcons);

	TakenIds.clear();

	super(shouldSave);
}

HOOK_METHOD(Minimap, RenderIcons, (Vector* position, float alpha) -> void) {
	// Check if we actually need to do the custom logic.
	if (BeforeMapIcons.size() == 0 && AfterMapIcons.size() == 0
		&& BeforeCurseIcons.size() == 0 && AfterCurseIcons.size() == 0) {
		super(position, alpha);
	}

	PlayerManager* player_manager = g_Game->GetPlayerManager();
	RNG* rng = &player_manager->_rng;
	Vector vector_zero(0, 0);

	unsigned int num_icons = 0;
	MinimapIcon icons_to_render[7];
	Vector render_position(position->x, position->y);

	std_string icons_anim = "icons";
	std_string curses_anim = "curses";

#pragma region Item icons
	TryAddAllIcons(icons_to_render, num_icons, BeforeMapIcons);

	if (player_manager->FirstCollectibleOwner(COLLECTIBLE_MIND, &rng, true)) {
		TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 5);
	} else {
		if (player_manager->FirstCollectibleOwner(COLLECTIBLE_COMPASS, &rng, true)) {
			TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 0);
		}

		if (player_manager->FirstCollectibleOwner(COLLECTIBLE_BLUE_MAP, &rng, true)) {
			TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 1);
		}

		if (player_manager->FirstCollectibleOwner(COLLECTIBLE_TREASURE_MAP, &rng, true)) {
			TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 2);
		}
	}

	TryAddAllIcons(icons_to_render, num_icons, AfterMapIcons);

	if (player_manager->FirstCollectibleOwner(COLLECTIBLE_RESTOCK, &rng, true)) {
		TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 4);
	}
#pragma endregion

#pragma region Curse icons
	TryAddAllIcons(icons_to_render, num_icons, BeforeCurseIcons);

	unsigned int curses = g_Game->GetCurses();

	for (size_t i = 0; i < 8; i++) {
		if ((curses & 1 << i) != 0) {
			TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, curses_anim, i);
		}
	}

	std::map<uint32_t, CurseSpriteData> curseSpriteMap = GetCurseSpriteMap();

	if (!curseSpriteMap.empty() && num_icons < 7) {
		for (const auto& entry : curseSpriteMap) {
			int adjustedCurseId = (int)entry.first;
			ANM2* curseSprite = entry.second.customANM2;

			if (adjustedCurseId >= 9 && curseSprite->_loaded && (curses & (1U << (adjustedCurseId - 1)))) {
				TryAddIcon(icons_to_render, num_icons, curseSprite, curses_anim, entry.second.frameNum);

				if (num_icons >= 7) break;
			}
		}
	}


	TryAddAllIcons(icons_to_render, num_icons, AfterCurseIcons);
#pragma endregion

#pragma region Rendering
	Vector offset(-16, 0);
	if (num_icons >= 4) {
		offset += Vector(num_icons - 3.0f, 0);
	}

	for (size_t i = 0; i < num_icons; i++) {
		MinimapIcon icon = icons_to_render[i];

		icon.sprite->SetFrame(&icon.anim, icon.frame);

		icon.sprite->_color._tint[3] = alpha;
		icon.sprite->Render(&render_position, &vector_zero, &vector_zero);

		render_position += offset;
	}
#pragma endregion
}

LUA_FUNCTION(Lua_MinimapAddIcon)
{
	Minimap* minimap = g_Game->GetMinimap();
	int position = (int)luaL_checkinteger(L, 1);
	const char* anm2 = luaL_checkstring(L, 2);
	std_string anm2_string(anm2);
	const char* anim = luaL_checkstring(L, 3);
	std_string anim_string(anim);
	int frame = (int)luaL_checkinteger(L, 4);

	ANM2* sprite = new ANM2;
	sprite->Load(anm2_string, true);

	if (!ValidateSprite(sprite, anim, frame)) {
		lua_pushnil(L);
		return 1;
	}

	MinimapIcon* icon = new MinimapIcon;
	icon->sprite = sprite;
	icon->anim = anim_string;
	icon->frame = frame;

	if (position == ICON_POSITION_BEFORE_MAP_ICONS) {
		icon->id = GetFreeId(BeforeMapIcons);
		BeforeMapIcons.push_back(std::unique_ptr<MinimapIcon>(icon));
	} else if (position == ICON_POSITION_AFTER_MAP_ICONS) {
		icon->id = GetFreeId(AfterMapIcons);
		AfterMapIcons.push_back(std::unique_ptr<MinimapIcon>(icon));
	} else if (position == ICON_POSITION_BEFORE_CURSE_ICONS) {
		icon->id = GetFreeId(BeforeCurseIcons);
		BeforeCurseIcons.push_back(std::unique_ptr<MinimapIcon>(icon));
	} else if (position == ICON_POSITION_AFTER_CURSE_ICONS) {
		icon->id = GetFreeId(AfterCurseIcons);
		AfterCurseIcons.push_back(std::unique_ptr<MinimapIcon>(icon));
	}

	lua_pushinteger(L, icon->id);

	return 1;
}

LUA_FUNCTION(Lua_MinimapRemoveIcon)
{
	Minimap* minimap = g_Game->GetMinimap();
	int priority = (int)luaL_checkinteger(L, 1);
	int id = (int)luaL_checkinteger(L, 2);

	if (priority == ICON_POSITION_BEFORE_MAP_ICONS) {
		RemoveIcon(BeforeMapIcons, id);
	}
	else if (priority == ICON_POSITION_AFTER_MAP_ICONS) {
		RemoveIcon(AfterMapIcons, id);
	}
	else if (priority == ICON_POSITION_BEFORE_CURSE_ICONS) {
		RemoveIcon(BeforeCurseIcons, id);
	}
	else if (priority == ICON_POSITION_AFTER_CURSE_ICONS) {
		RemoveIcon(AfterCurseIcons, id);
	}

	return 0;
}

LUA_FUNCTION(Lua_MinimapGetState)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_state);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetState)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_state = (int)luaL_checkinteger(L, 1);
	return 0;
}

LUA_FUNCTION(Lua_MinimapGetHoldTime)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_holdTime);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetHoldTime)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_holdTime = (int)luaL_checkinteger(L, 1);
	return 0;
}

LUA_FUNCTION(Lua_MinimapGetDisplayedSize)
{
	Minimap* minimap = g_Game->GetMinimap();
	Vector buffer;
	buffer = *minimap->GetDisplayedSize(buffer);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), buffer);

	return 1;
}

LUA_FUNCTION(Lua_MinimapGetItemIconsSprite)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataPtr::push(L, &minimap->_itemIconsSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetIconsSprite)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataPtr::push(L, &minimap->_iconsSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));;
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetShakeDuration)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_shakeDuration);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetShakeDuration)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_shakeDuration = (int)luaL_checkinteger(L, 1);
	return 0;
}

LUA_FUNCTION(Lua_MinimapGetShakeOffset)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), minimap->_shakeOffset);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetShakeOffset)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_shakeOffset = *lua::GetLuabridgeUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_MinimapRefresh)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->Refresh();
	return 0;
}

static void RegisterMinimap(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetMinimap", Lua_GameGetMinimap);
	lua_newtable(L);
	//luaL_Reg functions[] = {
	lua::TableAssoc(L, "GetState", Lua_MinimapGetState);
	lua::TableAssoc(L, "SetState", Lua_MinimapSetState);
	lua::TableAssoc(L, "GetHoldTime", Lua_MinimapGetHoldTime);
	lua::TableAssoc(L, "SetHoldTime", Lua_MinimapSetHoldTime);
	lua::TableAssoc(L, "GetDisplayedSize", Lua_MinimapGetDisplayedSize);
	lua::TableAssoc(L, "GetItemIconsSprite", Lua_MinimapGetItemIconsSprite);
	lua::TableAssoc(L, "GetIconsSprite", Lua_MinimapGetIconsSprite);
	lua::TableAssoc(L, "GetShakeDuration", Lua_MinimapGetShakeDuration);
	lua::TableAssoc(L, "SetShakeDuration", Lua_MinimapSetShakeDuration);
	lua::TableAssoc(L, "GetShakeOffset", Lua_MinimapGetShakeOffset);
	lua::TableAssoc(L, "SetShakeOffset", Lua_MinimapSetShakeOffset);
	lua::TableAssoc(L, "Refresh", Lua_MinimapRefresh);
	lua::TableAssoc(L, "AddIcon", Lua_MinimapAddIcon);
	lua::TableAssoc(L, "RemoveIcon", Lua_MinimapRemoveIcon);
	//};
	lua_setglobal(L, "Minimap");
	//lua::RegisterNewClass(L, lua::metatables::MinimapMT, lua::metatables::MinimapMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMinimap(_state);
}