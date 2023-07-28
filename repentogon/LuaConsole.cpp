#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "ImGuiFeatures/ConsoleMega.h"

static constexpr const char* ConsoleMT = "Console";

static int Lua_GetConsole(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Console** ud = (Console**)lua_newuserdata(L, sizeof(Console*));
	*ud = game->GetConsole();
	luaL_setmetatable(L, ConsoleMT);
	return 1;
}

int Lua_ConsolePrintError(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	const char* err = luaL_checkstring(L, 2);
	console->PrintError(err);
	return 0;
}

int Lua_ConsoleGetHistory(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	std::deque<Console_HistoryEntry> *history = console->GetHistory();

	lua_newtable(L);
	unsigned int idx = 1;
	for (const Console_HistoryEntry entry : *history) {
		lua_pushnumber(L, idx);
		lua_pushstring(L, entry._text.c_str());
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

int Lua_ConsoleGetCommandHistory(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	std::deque<std::string> commandHistory = *console->GetCommandHistory();

	lua_newtable(L);
	unsigned int idx = 1;

	std::reverse(commandHistory.begin(), commandHistory.end());

	for (const std::string entry : commandHistory) {
		lua_pushnumber(L, idx);
		lua_pushstring(L, entry.c_str());
		lua_settable(L, -3);
		idx++;
		
	}
	return 1;
}

int Lua_ConsolePrintWarning(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	std::string err = luaL_checkstring(L, 2) + std::string("\n");

	console->Print(err, 0xFFFCCA03, 0x96u);
	return 0;
}

int Lua_ConsolePopHistory(lua_State* L) {
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	int amount = (int)luaL_optinteger(L, 2, 1);
	std::deque<Console_HistoryEntry>* history = console->GetHistory();
	amount++;

	for (int i = 0; i < amount; ++i) {
		if(history->size() > 0)
			history->pop_front();
	}
	return 0;
}

int Lua_RegisterCommand(lua_State* L) {
	const char* name = luaL_checkstring(L, 2);
	const char* desc = luaL_checkstring(L, 3);
	const char* helpText = luaL_checkstring(L, 4);
	bool showOnMenu = lua::luaL_optboolean(L, 5, false);
	ConsoleMega::AutocompleteType autocompleteType = (ConsoleMega::AutocompleteType)luaL_optnumber(L, 6, 0);

	console.RegisterCommand(name, desc, helpText, showOnMenu, autocompleteType);

	return 0;
}

int Lua_RegisterMacro(lua_State* L) {
	lua::LuaStackProtector protector(L);
	const char* name = luaL_checkstring(L, 2);
	if (!lua_istable(L, 3)) {

		std::string err = "Expected a table of strings, got ";
		err.append(lua_typename(L, lua_type(L, 3)));
		luaL_argerror(L, 3, err.c_str());

		return 0;
	}

	std::vector<std::string> commands;
	// Get the number of vertices we received.
	unsigned int len = (unsigned int)lua_rawlen(L, 3);

	for (unsigned int i = 1; i <= len; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 3);
		if (lua_type(L, -1) == LUA_TNIL) break;
		commands.push_back(luaL_checkstring(L, -1));
		lua_pop(L, 1);
	}

	console.RegisterMacro(name, commands);
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
		{ "GetCommandHistory", Lua_ConsoleGetCommandHistory },
		{ "GetHistory", Lua_ConsoleGetHistory },
		{ "PopHistory", Lua_ConsolePopHistory },
		{ "PrintWarning", Lua_ConsolePrintWarning },
		{ "RegisterCommand", Lua_RegisterCommand },
		{ "RegisterMacro", Lua_RegisterMacro },
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

