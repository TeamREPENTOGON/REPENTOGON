#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

#pragma region DLL export

extern "C" {
	__declspec(dllexport) void L_Font_DrawString(Font* font, const char* str, float posX, float posY, float scaleX, float scaleY, KColor* color, FontSettings* settings) {
		font->DrawString(str, Vector(posX, posY), Vector(scaleX, scaleY), color, settings);
	}
}

#pragma endregion

static void Font_DrawStringScaled(Font* font, const char* str, float x, float y, float sx, float sy, KColor* col, int boxWidth, bool center)
{
    int align = 0; // TOP_LEFT
    if (boxWidth != 0)
    {
        int width = font->GetStringWidth(str);
        if (center)
        {
            align = 1; // TOP_CENTER
            x = (float)(x + (boxWidth * 0.5));
        }
        else
        {
            align = 2; // TOP_RIGHT
            x = (float)boxWidth;
        }
    }

    FontSettings settings;
    settings._align = align;
    L_Font_DrawString(font, str, x, y, sx, sy, col, &settings);
}

LUA_FUNCTION(Lua_FontDrawString_Legacy)
{
    Font* font = LuaFont::Get(L, 1);
    const char* string = luaL_checkstring(L, 2);
    float positionX = (float)luaL_checknumber(L, 3);
    float positionY = (float)luaL_checknumber(L, 4);
    KColor* renderColor = LuaKColor::Get(L, 5);
    int boxWidth = (int)luaL_optinteger(L, 6, 0);
    bool center = lua_isboolean(L, 7) ? lua_toboolean(L, 7) : false; // DrawStringScaled checks specifically for center == true
    
    Font_DrawStringScaled(font, string, positionX, positionY, 1.0, 1.0, renderColor, boxWidth, center);
    return 0;
}

LUA_FUNCTION(Lua_FontDrawString)
{
    bool isLegacyOverload = LuaKColor::IsUnderlyingType(L, 5);
    if (isLegacyOverload)
    {
        Lua_FontDrawString_Legacy(L);
        return 0;
    }

    Font* font = LuaFont::Get(L, 1);
    const char* string = luaL_checkstring(L, 2);
    float positionX = (float)luaL_checknumber(L, 3);
    float positionY = (float)luaL_checknumber(L, 4);
    float sizeX = (float)luaL_checknumber(L, 5);
    float sizeY = (float)luaL_checknumber(L, 6);
    KColor* renderColor = LuaKColor::Get(L, 7);
    FontSettings* settings = LuaFontRenderSettings::Get(L, 8);

    L_Font_DrawString(font, string, positionX, positionY, sizeX, sizeY, renderColor, settings);
    return 0;
}

LUA_FUNCTION(Lua_FontDrawStringScaled)
{
    Font* font = LuaFont::Get(L, 1);
    const char* string = luaL_checkstring(L, 2);
    float positionX = (float)luaL_checknumber(L, 3);
    float positionY = (float)luaL_checknumber(L, 4);
    float sizeX = (float)luaL_checknumber(L, 5);
    float sizeY = (float)luaL_checknumber(L, 6);
    KColor* renderColor = LuaKColor::Get(L, 7);
    int boxWidth = (int)luaL_optinteger(L, 8, 0);
    bool center = lua_isboolean(L, 9) ? lua_toboolean(L, 9) : false; // DrawStringScaled checks specifically for center == true

    Font_DrawStringScaled(font, string, positionX, positionY, sizeX, sizeY, renderColor, boxWidth, center);
    return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	luaL_Reg functions[] = {
        { "DrawString", Lua_FontDrawString },
        { "DrawStringScaled", Lua_FontDrawStringScaled },
        { "DrawStringUTF8", Lua_FontDrawString_Legacy },
        { "DrawStringScaledUTF8", Lua_FontDrawStringScaled },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::FONT, functions);
}