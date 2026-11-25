#include "IsaacRepentance.h"
#include "LuaCore.h"

MOD_EXPORT void L_Font_Init(Font* font) {
	font->constructor();
}

MOD_EXPORT void L_Font_Destroy(Font* font) {
	font->Unload();
	font->~Font();
	memset(font, 0, sizeof(Font));
}

MOD_EXPORT void L_Font_Load(Font* font, const char* path) {
	font->Load(path, false);
}

MOD_EXPORT void L_Font_Unload(Font* font) {
	font->Unload();
}

MOD_EXPORT int L_Font_GetCharacterWidth(Font* font, int c) {
	return font->GetCharacterWidth(c);
}

MOD_EXPORT int L_Font_GetStringWidth(Font* font, const char* str) {
	return font->GetStringWidth(str);
}

MOD_EXPORT void L_Font_SetMissingCharacter(Font* font, int c) {
	font->SetMissingCharacter(c);
}

MOD_EXPORT void L_Font_DrawString(Font* font, const char* str, float posX, float posY, float scaleX, float scaleY, KColor* color, FontSettings* settings) {
	font->DrawString(str, Vector(posX, posY), Vector(scaleX, scaleY), color, settings);
}