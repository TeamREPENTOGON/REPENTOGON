#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../ImGuiFeatures/ConsoleMega.h"
#include "../ImGuiFeatures/LogViewer.h"
#include "LuaCore.h"
#include "../Patches/ModReloading.h"
#include "../REPENTOGONFileMap.h"
#include "Anm2Extras.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

void LuaReset() {
    // We are building our own map here so that this is *solely* a Lua reset and not an XML one.
    // Calling existing game functions will try to load XML too, they are intertwined.
    std::map<std::string, std::string> modsToReload;

    for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
        if (mod != NULL && mod->IsEnabled()) {
            modsToReload[mod->GetName()] = mod->GetDir();
        }
    }

    bool debug = g_LuaEngine->GetLuaDebug();

    g_LuaEngine->destroy();
    g_LuaEngine->Init(false);

    // Reset the lua data of all entities in the room before we continue.
    // Right now they're invalid, dangling pointers. Let's reset them.
    Room* room = g_Game->GetCurrentRoom();
    EntityList_EL* res = room->GetEntityList()->GetUpdateEL();
    unsigned int size = res->_size;
    lua_State* L = g_LuaEngine->_state;
    if (size) {
        Entity** data = res->_data;
        while (size) {
            Entity* ent = *data;

            if (*ent->GetLuaRef()) { // Should always exist, this is just a safeguard
                LuaBridgeRef* newRef = (LuaBridgeRef*)Isaac::operator_new(8u);
                lua_createtable(L, 0, 0);
                newRef->_state = g_LuaEngine->_state;
                newRef->_ref = luaL_ref(L, -1001000);
                *ent->GetLuaRef() = newRef;
            }

            ++data;
            --size;

        }
    }
    // This is an ordered map and we stored by mod name, so the load order should be identical to the vanilla game.
    for (auto& mod : modsToReload) {
        std::string modPath = std::filesystem::current_path().string() + "/mods/" + mod.second + "/main.lua";
        if (std::filesystem::exists(modPath)) 
            g_LuaEngine->RunScript(modPath.c_str());
    }
}

HOOK_METHOD(Console, Print, (const std::string& text, unsigned int color, unsigned int fadeTime) -> void) {
    // Commands always print in history as (">") with a color of 0xFF808080.
    // Armed with this info, we can fix commands not saving on game crash by saving it every time.
    // Kinda hacky, but whatever.
    if (text.rfind(">", 0) == 0 && color == 0xFF808080) {
        int state = this->_state;
        this->SaveCommandHistory();
        this->_state = state;
    }

    // Change Repentance into REPENTOGON in the console
    if (text.rfind("Repentance Console\n", 0) == 0) {
        super("REPENTOGON Console\n", color, fadeTime);
        return;
    }

    super(text, color, fadeTime);
}

void PrintToConsole(Console* console, std::string* out) {

}

HOOK_METHOD(Console, RunCommand, (std::string& in, std::string* out, Entity_Player* player) -> void) {

    // Normally, the console explicitly expects a player to run any command, and will actively try to find one (and will crash otherwise)
    // We ASM patched this out to let commands run on the menu, so now we reintroduce this ourselves.
    // If we're in-game, return the player; otherwise don't. Many functions obviously don't work out of the game! Throw errors for those.
    bool inGame = !(g_Manager->GetState() != 2 || !g_Game);
    std::string res;

    if (!inGame) {

        std::vector<std::string> bannedCommands = {
            "challenge", // goes to a daily run, maybe another patch would fix that? one way ticket to cheat city otherwise
            "clearseeds",
            "combo",
            "costumetest",
            "curse",
            "debug",
            "delirious",
            "goto",
            "giveitem",
            "g",
            "gridspawn",
            "listcollectibles",
            "macro",
            "m",
            "metro",
            "remove",
            "reseed",
            "restart", // same as challenge
            "seed",
            "spawn",
            "time", // always returns 0
        };

        for (std::string command : bannedCommands) {
            if (in == command || in.rfind(command + " ", 0) == 0) {
                char err[256];
                sprintf(err, LANG.CONSOLE_CANT_BE_USED_IF_NOT_IN_GAME, command.c_str());
                this->PrintError(err);
                return;
            }
        }
    }
    else if (!player)
        player = g_Game->GetPlayerManager()->GetPlayer(0);

    if ((in == "luareset") || (in.rfind("luareset ", 0) == 0)) {
        LuaReset();
        return;
    }

    if ((in == "fullrestart") || (in.rfind("fullrestart ", 0) == 0)) {
        GameRestart();
        return;
    }

    if ((in == "clearcache") || (in.rfind("clearcache ", 0) == 0)) {
        REPENTOGONFileMap::_filemap.clear();
        REPENTOGONFileMap::map_init = false;
        REPENTOGONFileMap::GenerateMap();
        return super(in,out,player);
    };

    if ((in == "reloadshaders") || (in.rfind("reloadshaders ", 0) == 0)) {
        printf("[REPENTOGON] Attempting to reload custom shaders\n");
        if (!custom_shaders.empty()) {
            for (auto & [ key, value ] : custom_shaders)
                LoadCustomShader(key, &value.shader, false);
        }
        if (!custom_champion_shaders.empty()) {
            for (auto & [ key, value ] : custom_champion_shaders)
                LoadCustomShader(key, &value.shader, true);
        }
        return;
    };

    if ((in == "help") || (in.rfind("help ", 0) == 0)) {

        std::vector<std::string> cmdlets = ParseCommand(in, 2);

        if (!inGame)
            res.append(LANG.CONSOLE_HELP_OUTSIDE_GAME_HINT);

        if (cmdlets.size() == 1) {
            for (ConsoleCommand command : console.commands) {
                if (inGame || (!inGame && command.showOnMenu)) {
                    // i know it isn't pretty but sprintf was crashing and this works, so whatever
                    res.append("  - ").append(command.name).append(" - ").append(command.desc).append("\n");
                }
            }
        }
        else {
            for (ConsoleCommand command : console.commands) {
                if (inGame || (!inGame && command.showOnMenu)) {
                    if (command.name == cmdlets[1]) {
                        res.append(command.name).append(" - ").append(command.helpText).append("\n");
                        break;
                    }
                }
            }
        }


        if (out == nullptr)
            this->Print(res.c_str(), Console::Color::WHITE, 0x96U);
        else
            out->append(res);

        return;
    }

    if ((in == "macro") || (in.rfind("macro ", 0) == 0)) {
        std::vector<std::string> cmdlets = ParseCommand(in, 2);
        for (ConsoleMacro macro : console.macros) {
            if (cmdlets[1] == macro.name) {
                bool firstCommandRan = false;
                int test_it = 0;
                for (std::string command : macro.commands) {
                    this->_commandHistory.push_front(command);
                    this->RunCommand(command, out, player);
                }

                for (unsigned int i = 0; i < macro.commands.size(); ++i) {
                    this->_commandHistory.pop_front();
                }

                res.append("Macro finished.\n");

                if (out == nullptr)
                    this->Print(res.c_str(), Console::Color::WHITE, 0x96U);
                else
                    out->append(res);
                return;
            }
        }
        this->PrintError(LANG.CONSOLE_NO_MACRO_HINT);
        return;
    }

    super(in, out, player);
}
