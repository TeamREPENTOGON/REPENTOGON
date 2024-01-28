#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../ImGuiFeatures/ConsoleMega.h"

extern "C" {
	void L_Console_Print(const char* str, unsigned int color) {
		g_Game->GetConsole()->Print(std::string(str), color, 0x96U);
	}

	void L_Console_PrintError(const char* str) {
		g_Game->GetConsole()->PrintError(str);
	}

	void L_KAGE_LogMessage(const char* str) {
		KAGE::LogMessage(0, str);
	}

	FFI_StringTable L_Console_GetHistory() {
		Console* console = g_Game->GetConsole();
		std::deque<Console_HistoryEntry>* history = console->GetHistory();

		FFI_StringTable table;
		table.strings = (char**)malloc(history->size() * sizeof(char*));
		table.length = history->size();

		unsigned int idx = 0;
		for (const Console_HistoryEntry entry : *history) {
			table.strings[idx] = strdup(entry._text.c_str());
			++idx;
		}
		return table;
	}

	FFI_StringTable L_Console_GetCommandHistory() {
		Console* console = g_Game->GetConsole();
		std::deque<std::string> history = *console->GetCommandHistory();
		std::reverse(history.begin(), history.end());

		FFI_StringTable table;
		table.strings = (char**)malloc(history.size() * sizeof(char*));
		table.length = history.size();
		unsigned int idx = 0;
		for (const std::string entry : history) {
			table.strings[idx] = strdup(entry.c_str());
			++idx;
		}
		return table;
	}

	void L_Console_PopHistory(int amount) {
		Console* console = g_Game->GetConsole();
		std::deque<Console_HistoryEntry>* history = console->GetHistory();
		amount++;

		for (int i = 0; i < amount; ++i) {
			if (history->size() > 0)
				history->pop_front();
		}
	}
	
	// This is so I can just pass a pointer to and from Lua. Passing 4 bytes on the stack should be quicker than 17.
	typedef struct {
		const char* name;
		const char* desc;
		const char* helpText;
		bool showOnMenu;
		int autocompleteType;
	} L_RegisterCommandHolder;

	void L_Console_RegisterCommand(L_RegisterCommandHolder* holder) {
		console.RegisterCommand(holder->name, holder->desc, holder->helpText, holder->showOnMenu, (ConsoleMega::AutocompleteType)holder->autocompleteType);
	}
	
	void L_Console_RegisterMacro(const char* name, FFI_StringTable mdata) {
		std::vector<std::string> macro;
		
		for (int i = 0; i < mdata.length; ++i) {
			macro.push_back(mdata.strings[i]);
		}

		console.RegisterMacro(name, macro);
	}
}