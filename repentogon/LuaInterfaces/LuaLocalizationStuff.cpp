#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include <map>
#include <string>

std::map<std::string, unsigned int> languageMap = {
	{"en", 0},
	{"jp", 2},
	{"fr", 3},
	{"es", 4},
	{"de", 5},
	{"it", 6},
	{"nl", 7},
	{"pt", 8},
	{"ru", 10},
	{"kr", 11},
	{"zh", 13},
	{"fi", 14},
	{"sv", 15},
	{"da", 16},
	{"nn", 17},
	{"pl", 18},
	{"tr", 19}
};

unsigned int GetLanguageId(std::string langCode) {
	return languageMap[langCode];
}

MOD_EXPORT const char* L_Isaac_GetString(const char* category, const char* translateString)
{
	StringTable* stringTable = g_Manager->GetStringTable();

	if (*translateString == '#') {
		++translateString;
	}
	bool unk;

	return stringTable->GetString(category, stringTable->language, translateString, &unk);
}

MOD_EXPORT unsigned int L_Isaac_GetLanguageId(const char* langCode) {
	return GetLanguageId(langCode);
}

MOD_EXPORT const char* L_Isaac_GetLocalizedString(const char* category, const char* translateString, unsigned int language) {
	StringTable* stringTable = g_Manager->GetStringTable();

	if (*translateString == '#') {
		++translateString;
	}
	bool unk;

	return stringTable->GetString(category, language, translateString, &unk);
}
