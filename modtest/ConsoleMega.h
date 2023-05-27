#include "imgui.h"

struct ConsoleCommand {
    const char* name; // The name of the command.
    const char* desc; // A short (usually one sentence) description of the command. Will show up when just typing `help`
    const char* helpText; // The help text for the command. Will show up when doing `help (command)`
    bool showOnMenu;

    ConsoleCommand(const char* cmdName, const char* cmdDesc, const char* cmdHelpText, bool cmdShowOnMenu) {
        name = cmdName;
        desc = cmdDesc;
        helpText = cmdHelpText;
        showOnMenu = cmdShowOnMenu;
    }
};

struct ConsoleMega {
    bool enabled;
    char inputBuf[1024];
    std::vector<ConsoleCommand> commands;

    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    ConsoleMega()
    {
        enabled = true;
        memset(inputBuf, 0, sizeof(inputBuf));

        RegisterCommand("challenge", "Start a challenge run", "Stops the current run and starts a new run on a random seed with the given challenge ID.\nExample:\n(challenge 20) will start a new Purist challenge run.\n", false);
        RegisterCommand("clear", "Clear the debug console", "Clears all text currently displayed in the debug console. Only the line \"Repentance Console\" will remain.", true);
        RegisterCommand("clearcache", "Clear the sprite cache", "Clears the game's sprite cache. This can be useful for trying to deal with memory issues.\nThis also has the side effect of reloading modded sprites without needing a full game restart.", true);
        RegisterCommand("clearseeds", "Clear easter egg seeds in the current run", "Clears any \"special\" seed effects in the current run.\nExample:\nThe seed effect GFVE LLLL is applied in a run. Running clearseeds will remove this effect.", false);
        RegisterCommand("combo", "Give items from a specified pool", "Gives a specified number of items from a specified item pool.\nExample:\n(combo 4.6) will give six random items from the Angel item pool.\nNo, I don't know why they made a bespoke ID system for this one (1) command.", false);
        RegisterCommand("copy", "Copy previous commands to clipboard", "Copies a specified amount of previous console commands to the system clipboard.\nExample:\n(copy 3) will copy the previous three commands.", true);
        RegisterCommand("costumetest", "Give the player random costumes", "Gives the player a specified amount of random costumes.\nExample:\n(costumetest 34) will give the player 34 random costumes.", false);
        RegisterCommand("curse", "Add curses to the current run", "Permanently (until overridden) adds curses to the run. This command uses a bitmask- the curse with an ID of 1 is 1, 2 is 2, 3 is 4, 4 is 8, and so on. In this manner, desired curse ID's are tallied up and multiple can be enabled simultaneously.\nExample:\n(curse 96) will enable Curse of the Blind and Curse of the Maze simultaneously.", false);
        RegisterCommand("cutscene", "Play a cutscene", "Immediately plays the specified cutscenne.\nExample:\n(cutscene 1) will immediately play the game's intro.", true);
        RegisterCommand("delirious", "Force Delirious to be a certain boss", "Overrides the next boss the Delirious item will become.\nExample:\n(delirious 3) will force Delirious to be a Chub.", false);
        RegisterCommand("eggs", "Unlock all easter egg seeds", "PERMANENTLY unlocks all easter eggs in this save file.", true);
        RegisterCommand("giveitem", "Give the character items, trinkets, cards, and pills", "Gives the main player items, trinkets, cards and pills. These can either be by name or by prefix. Prefixes are (c) for items, (t) for trinkets, (p) for pills, and (k) for cards. Most pocket items count as cards.\nThis command also has shorthand which is just (g).\nExamples:\n(giveitem c1) will give the player The Sad Onion.\n(giveitem t1) will give the player Gulp!\n(giveitem p1) will give the player a Bad Trip pill.\n(giveitem k1) will give the player 0 - The Fool.", false);
        RegisterCommand("goto", "Teleport to a new room", "Teleports the character to a new room. Use (d) for a standard room, (s) for a special room, or three numbers to teleport to an existing room on the floor.\nExample:\n(goto s.boss.1010) will go to a Monstro fight.", false);
        RegisterCommand("gridspawn", "Spawn a grid entity", "Spawns a new grid entity of the given ID at a random place in the room.", false);
        RegisterCommand("listcollectibles", "List current items", "Lists the items the player currently has.", false);
        RegisterCommand("lua", "Run Lua code", "Runs the given Lua code immediately. Anything which would work in a standard file will work here.\nThis command also has shorthand which is just (l).", true);
        RegisterCommand("luamod", "Reload a Lua mod", "Reloads Lua code for the given mod folder.\nExample:\n(luamod testmod) will reload Lua code for the mod in the folder \"testmod\".", true);
        RegisterCommand("luarun", "Run a Lua file", "Runs a given Lua file immediately.\nExample:\n(luarun mods/test/test.lua) would run \"test.lua\" inside the \"test\" mod folder.", true);
        RegisterCommand("macro", "Trigger a set of commands", "The game has some precoded macros that act as shortcuts. Not gonna bother documenting them because this feature sucks and we're gonna overhaul it. Sorry!", false);
        RegisterCommand("metro", "Force Metronome to be a certain item", "Overrides the next item Metronome will become.\nExample:\n(metro c1) will force Metronome to become The Sad Onion.", false);
        RegisterCommand("playsfx", "Play a sound effect", "Plays a sound effect immediately.\nExample:\n(playsfx 187) will play an incorrect buzzer.", true);
        RegisterCommand("prof", "[BROKEN] Start profiling", "Supposed to log information to a CSV. Blame Nicalis!", true);
        RegisterCommand("profstop", "[BROKEN] Stop profiling", "Supposed to stop profiling but profiling is broken because we can't have nice things.", true);
        RegisterCommand("remove", "Remove an item", "Removes an item from the player immediately. Accepts the same syntax as give, look at that command's help for more info.", false);
        RegisterCommand("reloadfx", "Reload floor overlays", "Reloads the current floor's effects.", false);
        RegisterCommand("reloadshaders", "Reload in-game shaders", "Reloads any currently loaded shaders.", false);
        RegisterCommand("repeat", "Repeat prior commands", "Repeats the previously entered command X amount of times.\nExample:\n(giveitem 1) is used to give the player The Sad Onion. (repeat 5) is then used to give the player The Sad Onion five more times.", true);
        RegisterCommand("reseed", "Reseed the current floor", "Reseeds the current floor, generating a brand new layout for it.", false);
        RegisterCommand("restart", "Restart on a new run", "Restarts the game on a new run. Accepts an optional argument which is the character ID.\nExample:\n(restart 3) starts a new run as Judas.", false);
        RegisterCommand("seed", "Start a new run with the given seed", "Starts a new run with the given seed.\nExample:\n(seed N1CA L1SY) will start a new run with the seed N1CA L1SY.", false);
        RegisterCommand("spawn", "Spawn an entity", "Spawns a new entity. Syntax is (type).(variant).(subtype).(champion).\nExample:\n(spawn 5.40.1) will spawn a bomb.", false);
        RegisterCommand("stage", "Go to a stage", "Immediately goes to the specified stage. Accepts (a-d) as modifiers, with (a) corresponding to WOTL alts, (b) corresponding to Afterbirth alts, (c) corresponding to Antibirth alts, and (d) corresponding to Repentance alts.\nExample:\n(stage 4d) will take the player to Ashpit II.", false);
        RegisterCommand("time", "Print frames since run started", "Prints the amount of frames which has passed since the run started.", false);


    }

    void RegisterCommand(const char* name, const char* desc, const char* helpText, bool showOnMenu) {
        commands.push_back(ConsoleCommand(name, desc, helpText, showOnMenu));
    }

    void ExecuteCommand(char* input) {
        std::string printin = std::string("> ") + input + "\n";
        std::string out;

        g_Game->GetConsole()->Print(printin, 0xFF808080, 0x96u);
        g_Game->GetConsole()->RunCommand(input, out, NULL);
        g_Game->GetConsole()->Print(out.c_str(), 0XFFD3D3D3, 0x96u);
        memset(inputBuf, 0, sizeof(inputBuf));
    }

    void Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
        if (ImGui::Begin("Console", &enabled)) {
            std::deque<Console_HistoryEntry>* history = g_Game->GetConsole()->GetHistory();

            if (ImGui::BeginChild("Text View", ImVec2(0, 0), true)) {
                /* For "simplicity" and so we don't have duplicated memory while still allowing both old and new console to be usable,
                * we reuse existing console history.
                * The vanilla console stores history backwards, so we iterate over it in reverse.
                */
                for (auto entry = history->rbegin(); entry != history->rend(); ++entry) {
                    int colorMap = entry->GetColorMap();

                    /* 
                    * The vanilla console stores colors like this, because we can't have nice things.
                    * g_colorDouble is used for other things but it isn't really evident what those things are yet, so this will have to do.
                    * Decomp shows it as 0 but it... clearly isn't, so whatever.
                    */

                    float red = ((colorMap >> 0x10 & 0xFF) + g_colorDouble) / 255;
                    float green = ((colorMap >> 8 & 0xFF) + g_colorDouble) / 255;
                    float blue = ((colorMap & 0xFF) + g_colorDouble) / 255;

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, blue, 1));
                    ImGui::TextWrapped(entry->_text.Get());
                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();
            }
            ImGui::Separator();

            bool reclaimFocus = false;
            ImGuiInputTextFlags consoleFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
            if (ImGui::InputTextWithHint("", "Type your command here (\"help\" for help)", inputBuf, 1024, consoleFlags)) {
                char* s = inputBuf;
                Strtrim(s);
                if (s[0])
                    ExecuteCommand(s);
                reclaimFocus = true;
            }

            ImGui::SetItemDefaultFocus();
            if (reclaimFocus)
                ImGui::SetKeyboardFocusHere(-1);

            ImGui::End();
        }
        ImGui::PopStyleVar();
    }
};

extern ConsoleMega console;