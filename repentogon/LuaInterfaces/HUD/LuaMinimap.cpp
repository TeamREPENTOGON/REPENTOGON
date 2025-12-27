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

const int MAX_MINIMAP_ICONS = 7;
const int BASE_ICON_X_OFFSET = 16;
const int MAX_CURSES = 8;

struct MinimapIcon {
	ANM2* sprite;
	std_string anim;
	int frame;
};

struct RegisteredMinimapIcon {
	ANM2 sprite;
	int position;
	// Unique id that's passed to the lua side
	int id;
};

static std::vector<RegisteredMinimapIcon*> RegisteredCustomIcons;
// Set containing the indexes in RegisteredCustomIcons containing active icons
static std::set<int> ActiveCustomIcons;

#pragma region Helper functions
void TryAddIcon(MinimapIcon icons[], unsigned int& num_icons, ANM2* sprite, std_string& anim, int frame) {
	if (num_icons < MAX_MINIMAP_ICONS) {
		icons[num_icons].sprite = sprite;
		icons[num_icons].anim = anim;
		icons[num_icons].frame = frame;

		num_icons++;
	}
}

void TryAddAllIcons(MinimapIcon icons[], unsigned int& num_icons, std::set<int>::iterator& it, int position) {
	if (num_icons >= MAX_MINIMAP_ICONS) {
		return;
	}

	for (it; it != ActiveCustomIcons.end(); ++it) {
		int icon_idx = *it;
		RegisteredMinimapIcon* icon = RegisteredCustomIcons.at(icon_idx);

		if (icon->position <= position) {
			TryAddIcon(icons, num_icons, &icon->sprite, icon->sprite._animData->_name, icon->sprite.GetFrame());
		} else {
			break;
		}

		if (num_icons >= MAX_MINIMAP_ICONS) {
			break;
		}
	}
}

bool ValidateSprite(ANM2& sprite, const char* anim, int& frame) {
	if (!sprite._loaded) {
		return false;
	}

	sprite.SetAnimation(anim, true);
	AnimationData* anim_data = sprite._animData;
	if (anim_data == nullptr) {
		return false;
	}

	if (frame >= anim_data->GetLength()) {
		return false;
	}

	return true;
}

bool CompareIcons(RegisteredMinimapIcon* a, RegisteredMinimapIcon* b) {
	return a->position < b->position;
}
#pragma endregion

HOOK_METHOD(Game, Exit, (bool shouldSave) -> void) {
	ActiveCustomIcons.clear();

	super(shouldSave);
}

HOOK_METHOD(Minimap, RenderIcons, (Vector* position, float alpha) -> void) {
	// Check if we actually need to do the custom logic.
	if (ActiveCustomIcons.size() <= 0) {
		super(position, alpha);
		return;
	}

	PlayerManager* player_manager = g_Game->GetPlayerManager();
	RNG* rng = &player_manager->_rng;
	Vector vector_zero(0, 0);

	unsigned int num_icons = 0;
	MinimapIcon icons_to_render[MAX_MINIMAP_ICONS];
	Vector render_position(position->x, position->y);

	std_string icons_anim = "icons";
	std_string curses_anim = "curses";

	std::set<int>::iterator active_custom_icons_it = ActiveCustomIcons.begin();

#pragma region Item icons
	TryAddAllIcons(icons_to_render, num_icons, active_custom_icons_it, ICON_POSITION_BEFORE_MAP_ICONS);

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

	TryAddAllIcons(icons_to_render, num_icons, active_custom_icons_it, ICON_POSITION_AFTER_MAP_ICONS);

	if (player_manager->FirstCollectibleOwner(COLLECTIBLE_RESTOCK, &rng, true)) {
		TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, icons_anim, 4);
	}
#pragma endregion

#pragma region Curse icons
	TryAddAllIcons(icons_to_render, num_icons, active_custom_icons_it, ICON_POSITION_BEFORE_CURSE_ICONS);

	unsigned int curses = g_Game->GetCurses();

	for (size_t i = 0; i < MAX_CURSES; i++) {
		if ((curses & 1 << i) != 0) {
			TryAddIcon(icons_to_render, num_icons, &this->_itemIconsSprite, curses_anim, i);
		}
	}

	std::map<uint32_t, CurseSpriteData> curseSpriteMap = GetCurseSpriteMap();

	if (!curseSpriteMap.empty() && num_icons < MAX_MINIMAP_ICONS) {
		for (const auto& entry : curseSpriteMap) {
			int adjustedCurseId = (int)entry.first;
			ANM2* curseSprite = entry.second.customANM2;

			if (adjustedCurseId >= (MAX_CURSES + 1) && curseSprite->_loaded && (curses & (1U << (adjustedCurseId - 1)))) {
				TryAddIcon(icons_to_render, num_icons, curseSprite, curses_anim, entry.second.frameNum);

				if (num_icons >= MAX_MINIMAP_ICONS) break;
			}
		}
	}

	TryAddAllIcons(icons_to_render, num_icons, active_custom_icons_it, ICON_POSITION_AFTER_CURSE_ICONS);
#pragma endregion

#pragma region Rendering
	Vector offset(-BASE_ICON_X_OFFSET, 0);
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

LUA_FUNCTION(Lua_MinimapRegisterCustomIcon) {
	int position = (int)luaL_checkinteger(L, 1);
	const char* anm2 = luaL_checkstring(L, 2);
	std_string anm2_string(anm2);
	const char* anim = luaL_checkstring(L, 3);
	std_string anim_string(anim);
	int frame = (int)luaL_checkinteger(L, 4);

	RegisteredMinimapIcon* minimapIcon = new RegisteredMinimapIcon;
	minimapIcon->position = position;

	minimapIcon->sprite.Load(anm2_string, true);

	if (!ValidateSprite(minimapIcon->sprite, anim, frame)) {
		lua_pushnil(L);
	} else {
		minimapIcon->sprite.SetFrame(&anim_string, frame);

		minimapIcon->id = RegisteredCustomIcons.size();
		int index_to_place_at = 0;

		auto lb = std::lower_bound(RegisteredCustomIcons.begin(), RegisteredCustomIcons.end(), minimapIcon, CompareIcons);
		RegisteredCustomIcons.insert(lb, minimapIcon);

		lua_pushinteger(L, minimapIcon->id);
	}

	return 1;
}

LUA_FUNCTION(Lua_MinimapActivateCustomIcon) {
	int id = (int)luaL_checkinteger(L, 1);
	
	if (id < 0 || id >= RegisteredCustomIcons.size()) {
		return 0;
	}

	for (size_t i = 0; i < RegisteredCustomIcons.size(); i++)
	{
		RegisteredMinimapIcon* icon = RegisteredCustomIcons.at(i);
		if (icon->id == id) {
			ActiveCustomIcons.insert(i);
			return 0;
		}
	}

	return 0;
}

LUA_FUNCTION(Lua_MinimapDeactivateCustomIcon) {
	int id = (int)luaL_checkinteger(L, 1);

	if (id < 0 || id >= RegisteredCustomIcons.size()) {
		return 0;
	}

	for (size_t i = 0; i < RegisteredCustomIcons.size(); i++)
	{
		RegisteredMinimapIcon* icon = RegisteredCustomIcons.at(i);
		if (icon->id == id) {
			ActiveCustomIcons.erase(i);
			return 0;
		}
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
	lua::TableAssoc(L, "RegisterIcon", Lua_MinimapRegisterCustomIcon);
	lua::TableAssoc(L, "ActivateIcon", Lua_MinimapActivateCustomIcon);
	lua::TableAssoc(L, "DeactivateIcon", Lua_MinimapDeactivateCustomIcon);
	//};
	lua_setglobal(L, "Minimap");
	//lua::RegisterNewClass(L, lua::metatables::MinimapMT, lua::metatables::MinimapMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMinimap(_state);
}