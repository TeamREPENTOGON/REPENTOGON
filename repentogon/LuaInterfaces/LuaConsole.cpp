#include "Log.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../ImGuiFeatures/ConsoleMega.h"

/*LUA_FUNCTION(Lua_GetConsole) {
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Console** ud = (Console**)lua_newuserdata(L, sizeof(Console*));
	*ud = game->GetConsole();
	luaL_setmetatable(L, lua::metatables::ConsoleMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_ConsolePrintError)
{
	Console* console = g_Game->GetConsole();
	const char* err = luaL_checkstring(L, 1);
	console->PrintError(err);
	return 0;
}

LUA_FUNCTION(Lua_ConsoleGetHistory)
{
	Console* console = g_Game->GetConsole();
	std::deque<Console_HistoryEntry>* history = &console->_history;

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

LUA_FUNCTION(Lua_ConsoleGetCommandHistory)
{
	Console* console = g_Game->GetConsole();
	std::deque<std::string> commandHistory = console->_commandHistory;

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

LUA_FUNCTION(Lua_ConsolePrintWarning)
{
	Console* console = g_Game->GetConsole();
	std::string err = luaL_checkstring(L, 1) + std::string("\n");

	console->Print(err, 0xFFFCCA03, 0x96u);
	return 0;
}

LUA_FUNCTION(Lua_ConsolePopHistory) {
	Console* console = g_Game->GetConsole();
	int amount = (int)luaL_optinteger(L, 1, 1);
	std::deque<Console_HistoryEntry>* history = &console->_history;
	amount++;

	for (int i = 0; i < amount; ++i) {
		if (history->size() > 0)
			history->pop_front();
	}
	return 0;
}

LUA_FUNCTION(Lua_RegisterCommand) {
	const char* name = luaL_checkstring(L, 1);
	const char* desc = luaL_checkstring(L, 2);
	const char* helpText = luaL_checkstring(L, 3);
	bool showOnMenu = lua::luaL_optboolean(L, 4, false);
	ConsoleMega::AutocompleteType autocompleteType = (ConsoleMega::AutocompleteType)luaL_optnumber(L, 5, 0);

	console.RegisterCommand(name, desc, helpText, showOnMenu, autocompleteType);

	return 0;
}

LUA_FUNCTION(Lua_RegisterMacro) {
	lua::LuaStackProtector protector(L);
	const char* name = luaL_checkstring(L, 1);
	if (!lua_istable(L, 2)) {

		std::string err = "Expected a table of strings, got ";
		err.append(lua_typename(L, lua_type(L, 2)));
		luaL_argerror(L, 2, err.c_str());

		return 0;
	}

	std::vector<std::string> commands;
	// Get the number of vertices we received.
	unsigned int len = (unsigned int)lua_rawlen(L, 2);

	for (unsigned int i = 1; i <= len; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 2);
		if (lua_type(L, -1) == LUA_TNIL) break;
		commands.push_back(luaL_checkstring(L, -1));
		lua_pop(L, 1);
	}

	console.RegisterMacro(name, commands);
	return 0;
}

static void RegisterConsole(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetConsole", Lua_GetConsole);
	lua_newtable(L);

		lua::TableAssoc(L, "PrintError", Lua_ConsolePrintError );
		lua::TableAssoc(L, "GetCommandHistory", Lua_ConsoleGetCommandHistory );
		lua::TableAssoc(L, "GetHistory", Lua_ConsoleGetHistory );
		lua::TableAssoc(L, "PopHistory", Lua_ConsolePopHistory );
		lua::TableAssoc(L, "PrintWarning", Lua_ConsolePrintWarning );
		lua::TableAssoc(L, "RegisterCommand", Lua_RegisterCommand );
		lua::TableAssoc(L, "RegisterMacro", Lua_RegisterMacro );

	lua_setglobal(L, "Console");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterConsole(_state);
}

