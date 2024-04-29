#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"

bool exception_message_shown = false;
HOOK_GLOBAL_PRIORITY(RepCaughtException, 9999, (int x)->void,__cdecl) {
	char message[1024]="Something went wrong in the vanilla part of the game's code! Lua traceback unavailable!";
	char buffer[1024];
	super(x);
	if (!exception_message_shown) {
		if (g_LuaEngine != nullptr) {
			lua_Debug debuginfo;
			int stacklevel = 0;
			int is_valid_stack = lua_getstack(g_LuaEngine->_state, stacklevel, &debuginfo);
			snprintf(message, 1023, "Something went wrong in the vanilla part of the game's code!\n"
				"Lua traceback:\n");
			while (is_valid_stack) {
				lua_getinfo(g_LuaEngine->_state, "Sln", &debuginfo);
				snprintf(buffer, 1023, "%s(%d): %s\n", debuginfo.short_src, debuginfo.currentline, debuginfo.name);
				strncat_s(message, 1023, buffer, 1023);
				stacklevel += 1;
				is_valid_stack = lua_getstack(g_LuaEngine->_state, stacklevel, &debuginfo);
			};
		};
		MessageBoxA(nullptr, message, "A super-basic crash handler!", MB_OK);
		exception_message_shown = true;
	};
};
