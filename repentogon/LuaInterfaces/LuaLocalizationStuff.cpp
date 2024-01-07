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
	{"zh", 12},
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

LUA_FUNCTION(Lua_IsaacGetString)
{
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();

	const char* category = luaL_checkstring(L, 1);
	const char* translateString = luaL_checkstring(L, 2);
	if (*translateString == '#') {
		++translateString;
	}
	uint32_t unk;

	const char* retStr = stringTable->GetString(category, stringTable->language, translateString, &unk);
	lua_pushstring(L, retStr);

	return 1;
}

LUA_FUNCTION(Lua_IsaacGetLocalizedString) {
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();

	const char* category = luaL_checkstring(L, 1);
	const char* translateString = luaL_checkstring(L, 2);
	if (*translateString == '#') {
		++translateString;
	}
	uint32_t language;
	uint32_t unk;

	if ((lua_type(L, 3) == LUA_TSTRING)) {
		std::string langCode = luaL_checkstring(L, 3);
		language = GetLanguageId(langCode);
	}
	else {
		language = (uint32_t)luaL_checkinteger(L, 3);
	}

	const char* retStr = stringTable->GetString(category, language, translateString, &unk);
	lua_pushstring(L, retStr);

	return 1;
}

static void RegisterIsaacGetString(lua_State* L) {
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetString", Lua_IsaacGetString);
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetLocalizedString", Lua_IsaacGetLocalizedString);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterIsaacGetString(_state);
}