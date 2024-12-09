#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static int ValidateDrawStringBase(lua_State* L, void (Font::* fn)(DrawStringEntry)) {
	DrawStringEntry entry;
	Font* font = lua::GetUserdata<Font*>(L, 1, lua::Metatables::FONT, "Font");
	entry._text = luaL_checkstring(L, 2);
	entry._x = (float)luaL_checknumber(L, 3);
	entry._y = (float)luaL_checknumber(L, 4);
	entry._color = *lua::GetUserdata<KColor*>(L, 5, lua::Metatables::KCOLOR, "KColor");
	//entry._boxWidth = (int)luaL_optinteger(L, 6, -1);
	//entry._center = lua::luaL_optboolean(L, 7, false);

	(font->*fn)(entry);
	return 0;
}

static int ValidateDrawStringScaledBase(lua_State* L, void (Font::* fn)(DrawStringScaledEntry)) {
	DrawStringScaledEntry entry;
	Font* font = lua::GetUserdata<Font*>(L, 1, lua::Metatables::FONT, "Font");
	entry._text = luaL_checkstring(L, 2);
	entry._x = (float)luaL_checknumber(L, 3);
	entry._y = (float)luaL_checknumber(L, 4);
	entry._scaleX = (float)luaL_checknumber(L, 5);
	entry._scaleY = (float)luaL_checknumber(L, 6);
	entry._color = *lua::GetUserdata<KColor*>(L, 7, lua::Metatables::KCOLOR, "KColor");
	entry._boxWidth = (int)luaL_optinteger(L, 8, -1);
	entry._center = lua::luaL_optboolean(L, 9, false);

	(font->*fn)(entry);
	return 0;
}

static int ValidateGetStringWidth(lua_State* L, int (Font::* fn)(const char*)) {
	Font* font = lua::GetUserdata<Font*>(L, 1, lua::Metatables::FONT, "Font");
	const char* text = luaL_checkstring(L, 2);

	lua_pushinteger(L, (font->*fn)(text));
	return 1;
}

LUA_FUNCTION(Lua_Font_DrawString) {
	return ValidateDrawStringBase(L, &Font::DrawString);
}

LUA_FUNCTION(Lua_Font_DrawStringUTF8) {
	//return ValidateDrawStringBase(L, &Font::DrawStringUTF8);
}

LUA_FUNCTION(Lua_Font_DrawStringScaled) {
	return ValidateDrawStringScaledBase(L, &Font::DrawStringScaled);
}

LUA_FUNCTION(Lua_Font_DrawStringScaledUTF8) {
	//return ValidateDrawStringScaledBase(L, &Font::DrawStringScaledUTF8);
}

LUA_FUNCTION(Lua_Font_GetStringWidth) {
	return ValidateGetStringWidth(L, &Font::GetStringWidth);

}

LUA_FUNCTION(Lua_Font_GetStringWidthUTF8) {
	//return ValidateGetStringWidth(L, &Font::GetStringWidthUTF8);
}

static void PatchFontMethods(lua_State* L) {
	luaL_Reg functions[] = {
		{ "DrawString", Lua_Font_DrawString },
		{ "DrawStringScaled", Lua_Font_DrawStringScaled },
		//{ "DrawStringUTF8", Lua_Font_DrawStringUTF8 },
		//{ "DrawStringScaledUTF8", Lua_Font_DrawStringScaledUTF8 },
		{ "GetStringWidth", Lua_Font_GetStringWidth },
		//{ "GetStringWidthUTF8", Lua_Font_GetStringWidthUTF8 },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::FONT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	PatchFontMethods(_state);
}