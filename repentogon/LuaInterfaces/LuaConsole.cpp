#include "Log.h"
#include "IsaacRepentance.h"
#include "../ImGuiFeatures/ConsoleMega.h"

MOD_EXPORT int L_Console_GetCommandHistorySize() {
	return (int)g_Game->_console._commandHistory.size();
}

MOD_EXPORT const char* L_Console_GetCommandHistoryEntry(int idx) {
	return g_Game->_console._commandHistory[idx].c_str();
}

MOD_EXPORT int L_Console_GetHistorySize() {
	return (int)g_Game->_console._history.size();
}

MOD_EXPORT const char* L_Console_GetHistoryEntry(int idx) {
	return g_Game->_console._history[idx]._text.c_str();
}

MOD_EXPORT void L_Console_PopHistory(int amount) {
	std::deque<Console_HistoryEntry>* history = &g_Game->_console._history;
	amount++;

	for (int i = 0; i < amount; ++i) {
		if (history->size() > 0)
			history->pop_front();
	}
}

MOD_EXPORT void L_Console_PrintError(const char* err) {
	g_Game->_console.PrintError(err);
}

MOD_EXPORT void L_Console_PrintWarning(const char* text) {
	g_Game->_console.Print(text + std::string("\n"), 0xFFFCCA03, 0x96u);
}

MOD_EXPORT void L_Console_RegisterCommand(const char* name, const char* desc, const char* helpText, bool showOnMenu, int autocompleteType) {
	console.RegisterCommand(name, desc, helpText, showOnMenu, (ConsoleMega::AutocompleteType)autocompleteType);
}

MOD_EXPORT void L_Console_RegisterMacro(const char* name, const char** commands, int count) {
	std::vector<std::string> macroCommands(commands, commands + count);
	console.RegisterMacro(name, macroCommands);
}
