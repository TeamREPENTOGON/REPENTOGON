#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ConsoleMega.h"

#include <iostream> 
#include <sstream>

HOOK_METHOD(Console, RunCommand, (const std::string& in, const std::string& out, Entity_Player* player) -> void) {

    // Normally, the console explicitly expects a player to run any command, and will actively try to find one (and will crash otherwise)
    // We ASM patched this out to let commands run on the menu, so now we reintroduce this ourselves.
    // If we're in-game, return the player; otherwise don't. Many functions obviously don't work out of the game! Throw errors for those.
    bool inGame = !(g_Manager->GetState() != 2 || !g_Game);
    if (!inGame) {

        std::vector<const char*> bannedCommands = {
            "challenge", // goes to a daily run, maybe another patch would fix that? one way ticket to cheat city otherwise
            "clearseeds",
            "combo",
            "costumetest",
            "curse",
            "delirious",
            "goto",
            "giveitem",
            "gridspawn",
            "listcollectibles",
            "macro", // this may be useful if reimplemented
            "metro",
            "remove",
            "reseed",
            "restart", // same as challenge
            "seed",
            "spawn",
            "time", // always returns 0
        };

        for (const char* command : bannedCommands) {
            if (in.rfind(command, 0) == 0) {
                char err[256];
                sprintf(err, "[ERROR] %s can't be used if not in-game!", command);
                this->PrintError(err);
                return;
            }
        }
    }
    else if (!player)
        player = g_Game->GetPlayerManager()->GetPlayer(0);

    if (in.rfind("help", 0) == 0) {

        std::vector<std::string> cmdlets; //TODO split this into a generic parsing function

        std::stringstream sstream(in);
        std::string cmdlet;
        char space = ' ';
        while (std::getline(sstream, cmdlet, space)) {
            cmdlet.erase(std::remove_if(cmdlet.begin(), cmdlet.end(), ispunct), cmdlet.end());
            cmdlets.push_back(cmdlet);
            if (cmdlets.size() == 2) {
                break;
            }
        }

        if (!inGame)
            this->Print("(Only commands enabled to show outside of the game will appear right now.)\n", Console::Color::WHITE, 0x96U);

        if (cmdlets.size() == 1) {
            for (ConsoleCommand command : console.commands) {
                if (inGame || (!inGame && command.showOnMenu)) {
                    // i know it isn't pretty but sprintf was crashing and this works, so whatever
                    this->Print("  - ", Console::Color::WHITE, 0x96U);
                    this->Print(command.name, Console::Color::WHITE, 0x96U);
                    this->Print(" - ", Console::Color::WHITE, 0x96U);
                    this->Print(command.desc, Console::Color::WHITE, 0x96U);
                    this->Print("\n", Console::Color::WHITE, 0x96U);
                }
            }
        }
        else {
            for (ConsoleCommand command : console.commands) {
                if (inGame || (!inGame && command.showOnMenu)) {
                    if (command.name == cmdlets[1]) {
                        this->Print(command.name, Console::Color::WHITE, 0x96U);
                        this->Print(" - ", Console::Color::WHITE, 0x96U);
                        this->Print(command.helpText, Console::Color::WHITE, 0x96U);
                        this->Print("\n", Console::Color::WHITE, 0x96U);
                        break;
                    }
                }
            }
        }

        return;
    }

    super(in, out, player);
}
