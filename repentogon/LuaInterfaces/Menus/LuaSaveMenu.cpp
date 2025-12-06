#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SaveMenu_SaveSelectMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSaveSelectMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_DeleteButtonSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeleteButtonSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_DeletePopupSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeletePopupSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save1DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave1DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save2DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave2DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save3DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave3DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_GetSelectedSave)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushinteger(L, menu->SelectedSave);

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_SetSelectedSave)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	menu->SelectedSave = (int)luaL_checkinteger(L, 1);

	return 0;
}

LUA_FUNCTION(Lua_SaveMenu_IsDeleteActive)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static std::string CustomSaveSlotSprite1;
static std::string CustomSaveSlotSprite2;
static std::string CustomSaveSlotSprite3;

void TryReplaceSlotGraphic(ANM2* sprite, std::string& customSprite) {
	if (customSprite.empty()) {
		return;
	}

	bool success = sprite->ReplaceSpritesheet(0, customSprite);
	if (!success) {
		return;
	}

	for (int i = 1; i < sprite->GetLayerCount(); i++) {
		LayerState* layer = sprite->GetLayer(i);
		if (layer != nullptr) {
			layer->_visible = false;
		}
	}

	sprite->LoadGraphics(false);
}

LUA_FUNCTION(Lua_SaveMenu_SetSlotSpritesheet)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	
	int slot = (int)luaL_checkinteger(L, 1);
	const char* customSprite = luaL_checkstring(L, 2);

	if (slot == 1) {
		CustomSaveSlotSprite1 = customSprite;
		TryReplaceSlotGraphic(&menu->Save1DrawingSprite, CustomSaveSlotSprite1);
	} else if (slot == 2) {
		CustomSaveSlotSprite2 = customSprite;
		TryReplaceSlotGraphic(&menu->Save2DrawingSprite, CustomSaveSlotSprite2);
	} else if (slot == 3) {
		CustomSaveSlotSprite3 = customSprite;
		TryReplaceSlotGraphic(&menu->Save3DrawingSprite, CustomSaveSlotSprite3);
	}

	return 0;
}

HOOK_METHOD(Menu_Save, replace_slot_graphics, (ANM2* sprite) -> void) {
	super(sprite);

	if (g_Manager->_currentSaveSlot == 1) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite1);
	} else if (g_Manager->_currentSaveSlot == 2) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite2);
	} else if (g_Manager->_currentSaveSlot == 3) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite3);
	}
}

static void RegisterSaveMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSaveSelectMenuSprite", Lua_SaveMenu_SaveSelectMenuSprite);
	lua::TableAssoc(L, "GetDeleteButtonSprite", Lua_SaveMenu_DeleteButtonSprite);
	lua::TableAssoc(L, "GetDeletePopupSprite", Lua_SaveMenu_DeletePopupSprite);
	lua::TableAssoc(L, "GetSave1DrawingSprite", Lua_SaveMenu_Save1DrawingSprite);
	lua::TableAssoc(L, "GetSave2DrawingSprite", Lua_SaveMenu_Save2DrawingSprite);
	lua::TableAssoc(L, "GetSave3DrawingSprite", Lua_SaveMenu_Save3DrawingSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_SaveMenu_GetSelectedSave);
	lua::TableAssoc(L, "SetSelectedElement", Lua_SaveMenu_SetSelectedSave);
	lua::TableAssoc(L, "IsDeleteActive", Lua_SaveMenu_IsDeleteActive);
	lua::TableAssoc(L, "SetSlotSpritesheet", Lua_SaveMenu_SetSlotSpritesheet);
	lua_setglobal(L, lua::metatables::SaveMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterSaveMenuGame(state);
}