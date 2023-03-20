#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ConsoleMT = "Console";

static int Lua_GetConsole(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1BB60;
	luaL_setmetatable(L, ConsoleMT);
	return 1;
}

int Lua_ConsolePrintError(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	// std::string err = luaL_checkstring(L, 2);
	IsaacString str;
	const char* err = luaL_checkstring(L, 2);
	if (strlen(err) < 16) {
		strcpy(str.text, err);
	}
	else {
		*(char**)str.text = (char*)err;
	}
	str.unk = str.size = strlen(err);
	console->PrintError(str);
	return 1;
}

int Lua_ConsoleShow(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	*console->GetState() = 2;
	return 0;
}

int Lua_ConsoleGetHistory(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	std::deque<Console_HistoryEntry> *history = console->GetHistory();

	lua_newtable(L, 0);
	unsigned int idx = 1;
	for (Console_HistoryEntry entry : *history) {
		lua_pushnumber(L, idx);
		lua_pushstring(L, entry._text.Get());
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

int Lua_ConsolePrintWarning(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	IsaacString str;
	std::string err = luaL_checkstring(L, 2) + std::string("\n");
	const char* errstr = err.c_str();

	if (strlen(errstr) < 16) {
		strcpy(str.text, errstr);
	}
	else {
		*(char**)str.text = (char*)errstr;
	}
	str.unk = str.size = strlen(errstr);
	console->Print(str, 0xFFFCCA03, 0x96u);
	return 1;
}

int Lua_ConsolePopHistory(lua_State* L) {
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	int amount = luaL_optinteger(L, 2, 1);
	std::deque<Console_HistoryEntry>* history = console->GetHistory();
	amount++;

	for (int i = 0; i < amount; ++i) {
		if(history->size() > 0)
			history->pop_front();
	}
	return 0;
}

static void RegisterConsole(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetConsole");
	lua_pushcfunction(L, Lua_GetConsole);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, ConsoleMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "PrintError", Lua_ConsolePrintError },
		{ "Show", Lua_ConsoleShow },
		{ "GetHistory", Lua_ConsoleGetHistory },
		{ "PopHistory", Lua_ConsolePopHistory },
		{ "PrintWarning", Lua_ConsolePrintWarning },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterConsole(state);
}

