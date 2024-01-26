#include <iostream>
#include <sstream>

#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "LuaJITState.h"

static std::vector<std::string> ParseCommandA(std::string command, int size = 0) {
	std::vector<std::string> cmdlets;

	std::stringstream sstream(command);
	std::string cmdlet;
	char space = ' ';
	while (std::getline(sstream, cmdlet, space)) {
		cmdlet.erase(std::remove_if(cmdlet.begin(), cmdlet.end(), ispunct), cmdlet.end());
		cmdlets.push_back(cmdlet);
		if (size > 0 && cmdlets.size() == size) {
			break;
		}
	}
	return cmdlets;
}

HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player) -> void) {
	if (in.rfind("lj ", 0) == 0) {
		std::string code = in.substr(3);

		if (luaL_dostring(L, code.c_str()) != 0) {
			if (lua_isstring(L, -1)) {
				g_Game->GetConsole()->PrintError(lua_tostring(L, -1));
			}
		}
	}
	else if (in.rfind("ljrestart", 0) == 0) {
		LuaJITState::Init();
		g_Game->GetConsole()->Print("Restarted LuaJIT context\n", Console::Color::WHITE, 0x96U);
	}

	super(in, out, player);
}