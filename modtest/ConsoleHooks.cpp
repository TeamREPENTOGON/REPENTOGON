#include "IsaacRepentance.h"
#include "HookSystem.h"

HOOK_METHOD(Console, RunCommand, (const std_string& in, const std_string& out, Entity_Player* player) -> void) {

    // If we're in-game, return the player; otherwise don't. Many functions obviously don't work! Throw errors for those.
    if (g_Manager->GetState() != 2 || !g_Game) {

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
            "seed",
            "spawn",
            "time", // always returns 0
            "restart", // same as challenge
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

    super(in, out, player);
}
