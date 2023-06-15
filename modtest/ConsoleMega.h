#include "imgui.h"
#include "XMLData.h"

#include <sstream>
#include <cctype>

struct ConsoleCommand {
    std::string name = ""; // The name of the command.
    const char* desc = ""; // A short (usually one sentence) description of the command. Will show up when just typing `help`
    const char* helpText = ""; // The help text for the command. Will show up when doing `help (command)`
    bool showOnMenu = false;
    unsigned int autocompleteType = 0;
    std::vector<std::string> aliases;

    ConsoleCommand(const char* cmdName, const char* cmdDesc, const char* cmdHelpText, bool cmdShowOnMenu, unsigned int cmdAutocompleteType = 0, std::vector<std::string> cmdAliases = {}) {
        name = cmdName;
        desc = cmdDesc;
        helpText = cmdHelpText;
        showOnMenu = cmdShowOnMenu;
        autocompleteType = cmdAutocompleteType;
        aliases = cmdAliases;
    }
    
    ConsoleCommand() {

    }
};

struct AutocompleteEntry {
    std::string autocompleteText;
    std::string autocompleteDesc;

    AutocompleteEntry(std::string text, std::string desc = "") {
        autocompleteText = text;
        autocompleteDesc = desc;
    }
};

struct ConsoleMacro {
    std::string name;
    std::vector<std::string> commands;

    ConsoleMacro(std::string macroName, std::vector<std::string>* macroCommands) {
        name = macroName;
        commands = *macroCommands;
    }
};

static std::vector<std::string> ParseCommand(std::string command, int size = 0) {
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

struct ConsoleMega {
    bool enabled;
    char inputBuf[1024];
    std::vector<ConsoleCommand> commands;
    unsigned int historyPos;
    std::vector<ConsoleMacro> macros;
    std::vector<AutocompleteEntry> autocompleteBuffer;
    unsigned int autocompletePos;
    bool autocompleteActive = false;

    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    enum AutocompleteType {
        NONE,
        ENTITY,
        GOTO,
        STAGE,
        GRID,
        DEBUG_FLAG,
        ITEM,
        CHALLENGE,
        COMBO,
        CUTSCENE,
        MACRO,
        SFX,
        CURSE,
        METRO,
        DELIRIOUS
    };


    void RegisterCommand(const char* name, const char* desc, const char* helpText, bool showOnMenu, AutocompleteType autocomplete = NONE, std::vector<std::string> aliases = {}) {
        commands.push_back(ConsoleCommand(name, desc, helpText, showOnMenu, autocomplete, aliases));
    }

    void RegisterMacro(const char* macroName, std::vector<std::string> &macroCommands) {
        macros.push_back(ConsoleMacro(macroName, &macroCommands));
    }

    ConsoleMega()
    {
        enabled = true;
        memset(inputBuf, 0, sizeof(inputBuf));
        historyPos = 0;

        RegisterCommand("challenge", "Start a challenge run", "Stops the current run and starts a new run on a random seed with the given challenge ID.\nExample:\n(challenge 20) will start a new Purist challenge run.\n", false, CHALLENGE);
        RegisterCommand("clear", "Clear the debug console", "Clears all text currently displayed in the debug console. Only the line \"Repentance Console\" will remain.", true);
        RegisterCommand("clearcache", "Clear the sprite cache", "Clears the game's sprite cache. This can be useful for trying to deal with memory issues.\nThis also has the side effect of reloading modded sprites without needing a full game restart.", true);
        RegisterCommand("clearseeds", "Clear easter egg seeds in the current run", "Clears any \"special\" seed effects in the current run.\nExample:\nThe seed effect GFVE LLLL is applied in a run. Running clearseeds will remove this effect.", false);
        RegisterCommand("combo", "Give items from a specified pool", "Gives a specified number of items from a specified item pool.\nExample:\n(combo 4.6) will give six random items from the Angel item pool.\nNo, I don't know why they made a bespoke ID system for this one (1) command.", false, COMBO);
        RegisterCommand("copy", "Copy previous commands to clipboard", "Copies a specified amount of previous console commands to the system clipboard.\nExample:\n(copy 3) will copy the previous three commands.", true);
        RegisterCommand("costumetest", "Give the player random costumes", "Gives the player a specified amount of random costumes.\nExample:\n(costumetest 34) will give the player 34 random costumes.", false);
        RegisterCommand("curse", "Add curses to the current run", "Permanently (until overridden) adds curses to the run. This command uses a bitmask- the curse with an ID of 1 is 1, 2 is 2, 3 is 4, 4 is 8, and so on. In this manner, desired curse ID's are tallied up and multiple can be enabled simultaneously.\nExample:\n(curse 96) will enable Curse of the Blind and Curse of the Maze simultaneously.", true, CURSE);
        RegisterCommand("cutscene", "Play a cutscene", "Immediately plays the specified cutscenne.\nExample:\n(cutscene 1) will immediately play the game's intro.", true, CUTSCENE);
        RegisterCommand("debug", "Enable a debug flag", "Enables the specified debug flag.\nExample:\n(debug 3) will grant the player infinite HP.", false, DEBUG_FLAG);
        RegisterCommand("delirious", "Force Delirious to be a certain boss", "Overrides the next boss the Delirious item will become.\nExample:\n(delirious 3) will force Delirious to be a Chub.", false);
        RegisterCommand("eggs", "Unlock all easter egg seeds", "PERMANENTLY unlocks all easter eggs in this save file.", true);
        RegisterCommand("giveitem", "Give the character items, trinkets, cards, and pills", "Gives the main player items, trinkets, cards and pills. These can either be by name or by prefix. Prefixes are (c) for items, (t) for trinkets, (p) for pills, and (k) for cards. Most pocket items count as cards.\nThis command also has shorthand which is just (g).\nExamples:\n(giveitem c1) will give the player The Sad Onion.\n(giveitem t1) will give the player Gulp!\n(giveitem p1) will give the player a Bad Trip pill.\n(giveitem k1) will give the player 0 - The Fool.", false, ITEM, {"g"});
        RegisterCommand("goto", "Teleport to a new room", "Teleports the character to a new room. Use (d) for a standard room, (s) for a special room, or three numbers to teleport to an existing room on the floor.\nExample:\n(goto s.boss.1010) will go to a Monstro fight.", false);
        RegisterCommand("gridspawn", "Spawn a grid entity", "Spawns a new grid entity of the given ID at a random place in the room.", false);
        RegisterCommand("help", "Get info about commands", "Retrieve further info about a command and its syntax.", true);
        RegisterCommand("listcollectibles", "List current items", "Lists the items the player currently has.", false);
        RegisterCommand("lua", "Run Lua code", "Runs the given Lua code immediately. Anything which would work in a standard file will work here.\nThis command also has shorthand which is just (l).", true);
        RegisterCommand("luamod", "Reload a Lua mod", "Reloads Lua code for the given mod folder.\nExample:\n(luamod testmod) will reload Lua code for the mod in the folder \"testmod\".", true);
        RegisterCommand("luareset", "[EXPERIMENTAL] Reset the Lua context", "Destroys the current Lua context and recreates it from scratch. This is mostly a backend command meant to help sync up networked play.\nThis has Unforeseen Consequences if done in-game, please only do this on the menu unless you know what you're doing. Please?", true);
        RegisterCommand("luarun", "Run a Lua file", "Runs a given Lua file immediately.\nExample:\n(luarun mods/test/test.lua) would run \"test.lua\" inside the \"test\" mod folder.", true);
        RegisterCommand("macro", "Trigger a set of commands", "Run a set of commands in a specified order. These are effectively shortcuts. Refer to autocomplete for a list of macro commands.", false, MACRO, {"m"});
        RegisterCommand("metro", "Force Metronome to be a certain item", "Overrides the next item Metronome will become.\nExample:\n(metro c1) will force Metronome to become The Sad Onion.", false, METRO);
        RegisterCommand("playsfx", "Play a sound effect", "Plays a sound effect immediately.\nExample:\n(playsfx 187) will play an incorrect buzzer.", true, SFX);
        RegisterCommand("prof", "[BROKEN] Start profiling", "Supposed to log information to a CSV. Blame Nicalis!", true);
        RegisterCommand("profstop", "[BROKEN] Stop profiling", "Supposed to stop profiling but profiling is broken because we can't have nice things.", true);
        RegisterCommand("remove", "Remove an item", "Removes an item from the player immediately. Accepts the same syntax as give, look at that command's help for more info.", false, ITEM);
        RegisterCommand("reloadfx", "Reload floor overlays", "Reloads the current floor's effects.", false);
        RegisterCommand("reloadshaders", "Reload in-game shaders", "Reloads any currently loaded shaders.", false);
        RegisterCommand("repeat", "Repeat prior commands", "Repeats the previously entered command X amount of times.\nExample:\n(giveitem 1) is used to give the player The Sad Onion. (repeat 5) is then used to give the player The Sad Onion five more times.", true);
        RegisterCommand("reseed", "Reseed the current floor", "Reseeds the current floor, generating a brand new layout for it.", false);
        RegisterCommand("restart", "Restart on a new run", "Restarts the game on a new run. Accepts an optional argument which is the character ID.\nExample:\n(restart 3) starts a new run as Judas.", false);
        RegisterCommand("seed", "Start a new run with the given seed", "Starts a new run with the given seed.\nExample:\n(seed N1CA L1SY) will start a new run with the seed N1CA L1SY.", false);
        RegisterCommand("spawn", "Spawn an entity", "Spawns a new entity. Syntax is (type).(variant).(subtype).(champion).\nExample:\n(spawn 5.40.1) will spawn a bomb.", false, ENTITY);
        RegisterCommand("stage", "Go to a stage", "Immediately goes to the specified stage. Accepts (a-d) as modifiers, with (a) corresponding to WOTL alts, (b) corresponding to Afterbirth alts, (c) corresponding to Antibirth alts, and (d) corresponding to Repentance alts.\nExample:\n(stage 4d) will take the player to Ashpit II.", false, STAGE);
        RegisterCommand("time", "Print frames since run started", "Prints the amount of frames which has passed since the run started.", false);


        // Note: these are *functionally* identical, but not *literally* identical to the vanilla macros.
        // The vanilla macros reference items by name, which works for vanilla but not in a modded scenario where item names can change.
        // I also replaced all instances of "giveitem" with "g" for brevity.
        RegisterMacro("beast", std::vector<std::string>{ "stage 13", "goto x.itemdungeon.666", "combo 0.8", "combo 2.8", "combo 4.3" });
        RegisterMacro("bigchest", std::vector<std::string> { "stage 10a", "g c327", "g c328", "debug 3", "debug 4", "g c153", "g c330", "g c28", "repeat 5", "g c82", "debug 10", "g k5" });
        RegisterMacro("fc", std::vector<std::string>{ "debug 3", "debug 8", "debug 9", "g c330", "g c201", "g c41" });
        RegisterMacro("hud", std::vector<std::string>{ "g c81", "repeat 2", "g c212", "g c193", "repeat 12", "g c21", "g c246", "g c54", "g c252", "g p2", "g p3", "g c139", "g t19", "g t2", "g c77"});
        RegisterMacro("hush", std::vector<std::string> { "stage 9", "g k5", "combo 0.8", "combo 2.8", "combo 1.3", "combo 4.3" });
        RegisterMacro("mom", std::vector<std::string>{ "stage 6", "g c33", "g k5", "debug 3", "debug 4" });
        RegisterMacro("momh", std::vector<std::string>{ "stage 8", "g c33", "g k5", "debug 3", "debug 4" });
        RegisterMacro("ms", std::vector<std::string>{ "goto s.boss.5000" }); // lol why is this a macro
        RegisterMacro("mss", std::vector<std::string>{ "goto s.boss.5000", "g c330", "g c153", "debug 3", "debug 4"}); // that's more like it
        RegisterMacro("qk", std::vector<std::string>{ "g c28", "repeat 5", "g c82", "g c54", "g c246", "g c21", "g c260", "g c76", "g c84", "debug 8", "g c18", "g c190", "g c17", "debug 3", "debug 10"});
        RegisterMacro("ug", std::vector<std::string>{ "stage 11a", "goto s.boss.6000" });
        RegisterMacro("ugg", std::vector<std::string>{ "stage 11a", "goto s.boss.6000", "g c1", "g c1", "g c341", "g c341", "g c153", "g c51", "g c18", "g c190", "debug 3"});

    }

    void ExecuteCommand(char* input) {
        Console* console = g_Game->GetConsole();

        std::string printin = std::string(">") + input + "\n";
        std::string out;

        console->GetCommandHistory()->push_front(input);
        console->Print(printin, 0xFF808080, 0x96u);
        console->RunCommand(std::string(input), &out, NULL);
        console->Print(out.c_str(), 0XFFD3D3D3, 0x96u);
        memset(inputBuf, 0, sizeof(inputBuf));
        historyPos = 0;
        autocompleteBuffer.clear();
    }

    void Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
        if (ImGui::Begin("Console", &enabled)) {
            std::deque<Console_HistoryEntry>* history = g_Game->GetConsole()->GetHistory();

            // -27 = fill remaining window space minus 27px. fixes issue where the input is outside the window frame
            if (ImGui::BeginChild("Text View", ImVec2(0, -27), true)) {
                /* For "simplicity" and so we don't have duplicated memory while still allowing both old and new console to be usable,
                * we reuse existing console history.
                * The vanilla console stores history backwards, so we iterate over it in reverse.
                */
                for (auto entry = history->rbegin(); entry != history->rend(); ++entry) {
                    int colorMap = entry->GetColorMap();

                    /* 
                    * The vanilla console stores color as a bitwise flag because we can't have nice things.
                    * g_colorDouble is used for other things but it isn't really evident what those things are yet, so this will have to do.
                    * Decomp shows it as 0 but it... clearly isn't, so whatever.
                    */

                    float red = (float)((colorMap >> 0x10 & 0xFF) + g_colorDouble) / 255.f;
                    float green = (float)((colorMap >> 8 & 0xFF) + g_colorDouble) / 255.f;
                    float blue = (float)((colorMap & 0xFF) + g_colorDouble) / 255.f;

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, blue, 1));
                    ImGui::TextWrapped(entry->_text.Get());
                    ImGui::PopStyleColor();
                }
                ImGui::EndChild();
            }

            ImGui::Separator();

            bool reclaimFocus = false;
            
           ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
           if (autocompleteBuffer.size() > 0) {
               ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y));
               ImGui::SetNextWindowSizeConstraints(ImVec2(ImGui::GetWindowSize().x, 0), ImVec2(ImGui::GetWindowSize().x, 302)); // 302 is chosen here to have a "pixel perfect" scroll to the bottom
               if (ImGui::BeginPopup("Console Autocomplete", ImGuiWindowFlags_NoFocusOnAppearing)) {
                   for (AutocompleteEntry entry : autocompleteBuffer) {
                       if (autocompletePos > 0 && entry.autocompleteText == autocompleteBuffer[autocompletePos - 1].autocompleteText) {
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                           ImGui::SetScrollHereY();
                       }
                       else 
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7, 0.7, 0.7, 1));

                       ImGui::TextUnformatted(entry.autocompleteText.c_str());

                       if (!entry.autocompleteDesc.empty()) {
                           ImGui::SameLine();
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7, 0.7, 0.7, 1));
                           entry.autocompleteDesc = "(" + entry.autocompleteDesc + ")";
                           ImGui::TextUnformatted(entry.autocompleteDesc.c_str());
                           ImGui::PopStyleColor();
                       }

                       ImGui::PopStyleColor();
                   }
                   ImGui::EndPopup();
                }
               ImGui::OpenPopup("Console Autocomplete");
            }
            ImGuiInputTextFlags consoleFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit;
            if (ImGui::InputTextWithHint("##", "Type your command here (\"help\" for help)", inputBuf, 1024, consoleFlags, &TextEditCallbackStub, (void*)this)) {
                char* s = inputBuf;
                Strtrim(s);
                if (s[0])
                    ExecuteCommand(s);
                reclaimFocus = true;
            }
            ImGui::PopItemWidth();

            ImGui::SetItemDefaultFocus();
            if (reclaimFocus)
                ImGui::SetKeyboardFocusHere(-1);

            ImGui::End();
        }
        ImGui::PopStyleVar();
    }

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        ConsoleMega* console = (ConsoleMega*)data->UserData;
        return console->TextEditCallback(data);
    }

    int TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        switch (data->EventFlag)
        {
            case ImGuiInputTextFlags_CallbackCompletion:
            {
                if (autocompleteBuffer.size() > 0) {
                    ++autocompletePos;
                    if (autocompletePos > autocompleteBuffer.size())
                        autocompletePos = 1;

                    autocompleteActive = true;
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, autocompleteBuffer[autocompletePos - 1].autocompleteText.c_str());
                    autocompleteActive = false;
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackEdit:
            {
                if (!autocompleteActive) {
                    std::string strBuf = data->Buf;
                    std::vector<std::string> cmdlets = ParseCommand(strBuf);
                    autocompleteBuffer.clear();
                    autocompletePos = 0;

                    if (cmdlets.size() == 1) {
                        for (ConsoleCommand command : commands) {
                            bool inGame = !(g_Manager->GetState() != 2 || !g_Game);

                            if (command.name.rfind(data->Buf, 0) == 0)
                                if (inGame || (!inGame && command.showOnMenu == true)) {
                                    autocompleteBuffer.push_back(AutocompleteEntry(command.name, command.desc));
                                }
                        }
                    }

                    if (cmdlets.size() >= 2 || (cmdlets.size() < 3 && std::isspace(static_cast<unsigned char>(strBuf.back())))) {
                        std::string commandName = cmdlets.front();
                        commandName.erase(remove(commandName.begin(), commandName.end(), ' '), commandName.end());

                        ConsoleCommand command;
                        for (ConsoleCommand commandIter : commands) {
                            if (commandName == commandIter.name) {
                                command = commandIter;
                                break;
                            }
                            for (std::string alias : commandIter.aliases) {
                                if (commandName == alias) {
                                    command = commandIter;
                                    break;
                                }
                            }
                        }

                        std::vector<AutocompleteEntry> entries;

                        switch (command.autocompleteType) {
                            case ENTITY: {
                                std::unordered_map<tuple<int, int, int>, XMLAttributes> entities = XMLStuff.EntityData->nodes;
                                for (auto entity : entities) {
                                    int type = get<0>(entity.first);
                                    int variant = get<1>(entity.first);
                                    int subtype = get<2>(entity.first);

                                    std::string name = entity.second["name"];
                                    std::string id = std::to_string(type) + "." + std::to_string(variant) + "." + std::to_string(subtype);

                                    AutocompleteEntry entry = AutocompleteEntry(cmdlets.front() + " " + id, name);

                                    std::string lowerBuf = data->Buf;
                                    std::transform(lowerBuf.begin(), lowerBuf.end(), lowerBuf.begin(),
                                        [](unsigned char c) { return std::tolower(c); });

                                    std::string lowerName = (cmdlets.front() + " " + name);
                                    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                                        [](unsigned char c) { return std::tolower(c); });

                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0 || lowerName.rfind(lowerBuf, 0) == 0)
                                        autocompleteBuffer.push_back(AutocompleteEntry(cmdlets.front() + " " + id, name));
                                }

                                break;
                            }
                            case STAGE: {
                                std::vector<AutocompleteEntry> stage;
                                // TODO: I'd *love* to add a callback here for StageAPI stages.
                                if (g_Game->GetDifficulty() == 2 || g_Game->GetDifficulty() == 3) { // Greed mode
                                    stage = {
                                        AutocompleteEntry(cmdlets.front() + " 1", "Basement"),
                                        AutocompleteEntry(cmdlets.front() + " 1a", "Cellar"),
                                        AutocompleteEntry(cmdlets.front() + " 1b", "Burning Basement"),
                                        AutocompleteEntry(cmdlets.front() + " 2", "Caves"),
                                        AutocompleteEntry(cmdlets.front() + " 2a", "Catacombs"),
                                        AutocompleteEntry(cmdlets.front() + " 2b", "Flooded Caves"),
                                        AutocompleteEntry(cmdlets.front() + " 3", "Depths"),
                                        AutocompleteEntry(cmdlets.front() + " 3a", "Necropolis"),
                                        AutocompleteEntry(cmdlets.front() + " 3b", "Dank Depths"),
                                        AutocompleteEntry(cmdlets.front() + " 4", "Womb"),
                                        AutocompleteEntry(cmdlets.front() + " 4a", "Utero"),
                                        AutocompleteEntry(cmdlets.front() + " 4b", "Scarred Womb"),
                                        AutocompleteEntry(cmdlets.front() + " 5", "Sheol"),
                                        AutocompleteEntry(cmdlets.front() + " 6", "The Shop"),
                                        AutocompleteEntry(cmdlets.front() + " 7", "Ultra Greed")
                                    };
                                }
                                else {
                                    stage = {
                                        AutocompleteEntry(cmdlets.front() + " 1", "Basement I"),
                                        AutocompleteEntry(cmdlets.front() + " 1a", "Cellar I"),
                                        AutocompleteEntry(cmdlets.front() + " 1b", "Burning Basement I"),
                                        AutocompleteEntry(cmdlets.front() + " 1c", "Downpour I"),
                                        AutocompleteEntry(cmdlets.front() + " 1d", "Dross I"),
                                        AutocompleteEntry(cmdlets.front() + " 2", "Basement II"),
                                        AutocompleteEntry(cmdlets.front() + " 2a", "Cellar II"),
                                        AutocompleteEntry(cmdlets.front() + " 2b", "Burning Basement II"),
                                        AutocompleteEntry(cmdlets.front() + " 2c", "Downpour II"),
                                        AutocompleteEntry(cmdlets.front() + " 2d", "Dross II"),
                                        AutocompleteEntry(cmdlets.front() + " 3", "Caves I"),
                                        AutocompleteEntry(cmdlets.front() + " 3a", "Catacombs I"),
                                        AutocompleteEntry(cmdlets.front() + " 3b", "Flooded Caves I"),
                                        AutocompleteEntry(cmdlets.front() + " 3c", "Mines I"),
                                        AutocompleteEntry(cmdlets.front() + " 3d", "Ashpit I"),
                                        AutocompleteEntry(cmdlets.front() + " 4", "Caves II"),
                                        AutocompleteEntry(cmdlets.front() + " 4a", "Catacombs II"),
                                        AutocompleteEntry(cmdlets.front() + " 4b", "Flooded Caves II"),
                                        AutocompleteEntry(cmdlets.front() + " 4c", "Mines II"),
                                        AutocompleteEntry(cmdlets.front() + " 4d", "Ashpit II"),
                                        AutocompleteEntry(cmdlets.front() + " 5", "Depths I"),
                                        AutocompleteEntry(cmdlets.front() + " 5a", "Necropolis I"),
                                        AutocompleteEntry(cmdlets.front() + " 5b", "Dank Depths I"),
                                        AutocompleteEntry(cmdlets.front() + " 5c", "Mausoleum I"),
                                        AutocompleteEntry(cmdlets.front() + " 5d", "Gehenna I"),
                                        AutocompleteEntry(cmdlets.front() + " 6", "Depths II"),
                                        AutocompleteEntry(cmdlets.front() + " 6a", "Necropolis II"),
                                        AutocompleteEntry(cmdlets.front() + " 6b", "Dank Depths II"),
                                        AutocompleteEntry(cmdlets.front() + " 6c", "Mausoleum II"),
                                        AutocompleteEntry(cmdlets.front() + " 6d", "Gehenna II"),
                                        AutocompleteEntry(cmdlets.front() + " 7", "Womb I"),
                                        AutocompleteEntry(cmdlets.front() + " 7a", "Utero I"),
                                        AutocompleteEntry(cmdlets.front() + " 7b", "Scarred Womb I"),
                                        AutocompleteEntry(cmdlets.front() + " 7c", "Corpse I"),
                                        AutocompleteEntry(cmdlets.front() + " 8", "Womb II"),
                                        AutocompleteEntry(cmdlets.front() + " 8a", "Utero II"),
                                        AutocompleteEntry(cmdlets.front() + " 8b", "Scarred Womb II"),
                                        AutocompleteEntry(cmdlets.front() + " 8c", "Corpse II"),
                                        AutocompleteEntry(cmdlets.front() + " 9", "??? / Blue Womb"),
                                        AutocompleteEntry(cmdlets.front() + " 10", "Sheol"),
                                        AutocompleteEntry(cmdlets.front() + " 10a", "Cathedral"),
                                        AutocompleteEntry(cmdlets.front() + " 11", "Dark Room"),
                                        AutocompleteEntry(cmdlets.front() + " 11a", "Chest"),
                                        AutocompleteEntry(cmdlets.front() + " 12", "The Void"),
                                        AutocompleteEntry(cmdlets.front() + " 13", "Home (day)"),
                                        AutocompleteEntry(cmdlets.front() + " 13a", "Home (night)")
                                    };
                                }

                                for (AutocompleteEntry entry : stage) {
                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }
                                break;
                            }

                            case DEBUG_FLAG: {
                                std::vector<AutocompleteEntry> debug = {
                                    AutocompleteEntry(cmdlets.front() + " 1", "Entity Positions"),
                                    AutocompleteEntry(cmdlets.front() + " 2", "Grid"),
                                    AutocompleteEntry(cmdlets.front() + " 3", "Infinite HP"),
                                    AutocompleteEntry(cmdlets.front() + " 4", "High Damage"),
                                    AutocompleteEntry(cmdlets.front() + " 5", "Show Room Info"),
                                    AutocompleteEntry(cmdlets.front() + " 6", "Show Hitspheres"),
                                    AutocompleteEntry(cmdlets.front() + " 7", "Show Damage Values"),
                                    AutocompleteEntry(cmdlets.front() + " 8", "Infinite Item Charges"),
                                    AutocompleteEntry(cmdlets.front() + " 9", "High Luck"),
                                    AutocompleteEntry(cmdlets.front() + " 10", "Quick Kill"),
                                    AutocompleteEntry(cmdlets.front() + " 11", "Grid Info"),
                                    AutocompleteEntry(cmdlets.front() + " 12", "Player Item Info"),
                                    AutocompleteEntry(cmdlets.front() + " 13", "Show Grid Collision Points"),
                                    AutocompleteEntry(cmdlets.front() + " 14", "Show Lua Memory Usage")
                                };

                                for (AutocompleteEntry entry : debug) {
                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }
                                break;
                            }

                            case ITEM: {
                                std::vector<std::pair<XMLNodes, std::string>> XMLPairs = {
                                    {XMLStuff.ItemData->nodes, " c"},
                                    {XMLStuff.TrinketData->nodes, " t"},
                                    {XMLStuff.CardData->nodes, " k"},
                                    {XMLStuff.PillData->nodes, " p"},
                                };

                                for (std::pair<XMLNodes, std::string> XMLPair : XMLPairs) {
                                    for (auto node : XMLPair.first) {
                                        int id = node.first;
                                        std::string name = node.second["name"];
                                        AutocompleteEntry entry = AutocompleteEntry(cmdlets.front() + XMLPair.second + std::to_string(id), name);

                                        // We also need to check for name, so tab autocomplete works with names as well.
                                        // Lowercase string for this.
                                        std::string lowerBuf = data->Buf;
                                        std::transform(lowerBuf.begin(), lowerBuf.end(), lowerBuf.begin(),
                                            [](unsigned char c) { return std::tolower(c); });

                                        std::string lowerName = (cmdlets.front() + " " + name);
                                        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                                            [](unsigned char c) { return std::tolower(c); });

                                        if (entry.autocompleteText.rfind(data->Buf, 0) == 0 || lowerName.rfind(lowerBuf, 0) == 0)
                                            autocompleteBuffer.push_back(AutocompleteEntry(cmdlets.front() + XMLPair.second + std::to_string(id), name));
                                    }
                                }

                                break;
                            }

                            case CHALLENGE: {
                                XMLNodes challenges = XMLStuff.ChallengeData->nodes;
                                for (auto node : challenges) {
                                    int id = node.first;
                                    std::string name;
                                    if (id == 45) 
                                        name = "DELETE THIS"; // Internally the challenge has no name, this is the somewhat canon one.
                                    else
                                        name = node.second["name"];

                                    AutocompleteEntry entry = AutocompleteEntry(cmdlets.front() + " " + std::to_string(id), name);

                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0)
                                        autocompleteBuffer.push_back(entry);

                                }
                                break;
                            }

                            case COMBO: {
                                std::vector<AutocompleteEntry> combo = {
                                    AutocompleteEntry(cmdlets.front() + " 0.", "Treasure"),
                                    AutocompleteEntry(cmdlets.front() + " 1.", "Shop"),
                                    AutocompleteEntry(cmdlets.front() + " 2.", "Boss"),
                                    AutocompleteEntry(cmdlets.front() + " 3.", "Devil"),
                                    AutocompleteEntry(cmdlets.front() + " 4.", "Angel"),
                                    AutocompleteEntry(cmdlets.front() + " 5.", "Secret"),
                                    AutocompleteEntry(cmdlets.front() + " 6.", "Library"),
                                    AutocompleteEntry(cmdlets.front() + " 7.", "Challenge"),
                                    AutocompleteEntry(cmdlets.front() + " 8.", "Golden Chest"),
                                    AutocompleteEntry(cmdlets.front() + " 9.", "Red Chest"),
                                    AutocompleteEntry(cmdlets.front() + " 10.", "Beggar"),
                                    AutocompleteEntry(cmdlets.front() + " 11.", "Demon Beggar"),
                                    AutocompleteEntry(cmdlets.front() + " 12.", "Curse"),
                                    AutocompleteEntry(cmdlets.front() + " 13.", "Key Master"),
                                    AutocompleteEntry(cmdlets.front() + " 14.", "Boss Rush"),
                                    AutocompleteEntry(cmdlets.front() + " 15.", "Dungeon"),
                                };

                                for (AutocompleteEntry entry : combo) {
                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }
                                break;
                            }

                            case CUTSCENE: {
                                std::vector<AutocompleteEntry> cutscene = {
                                    AutocompleteEntry(cmdlets.front() + " 1", "Intro"),
                                    AutocompleteEntry(cmdlets.front() + " 2", "Credits"),
                                    AutocompleteEntry(cmdlets.front() + " 3", "Epilogue"),
                                    AutocompleteEntry(cmdlets.front() + " 4", "Womb (Eden)"),
                                    AutocompleteEntry(cmdlets.front() + " 5", "Womb (Rubber Cement)"),
                                    AutocompleteEntry(cmdlets.front() + " 6", "Womb (Noose)"),
                                    AutocompleteEntry(cmdlets.front() + " 7", "Womb (Wire Coat Hanger)"),
                                    AutocompleteEntry(cmdlets.front() + " 8", "Womb (Everything is Terrible)"),
                                    AutocompleteEntry(cmdlets.front() + " 9", "Womb (Ipecac)"),
                                    AutocompleteEntry(cmdlets.front() + " 10", "Womb (Experimental Treatment)"),
                                    AutocompleteEntry(cmdlets.front() + " 11", "Womb (A Quarter)"),
                                    AutocompleteEntry(cmdlets.front() + " 12", "Womb (Dr. Fetus)"),
                                    AutocompleteEntry(cmdlets.front() + " 13", "Womb (???)"),
                                    AutocompleteEntry(cmdlets.front() + " 14", "Womb (It Lives!)"),
                                    AutocompleteEntry(cmdlets.front() + " 15", "Sheol"),
                                    AutocompleteEntry(cmdlets.front() + " 16", "Cathedral"),
                                    AutocompleteEntry(cmdlets.front() + " 17", "Chest"),
                                    AutocompleteEntry(cmdlets.front() + " 18", "Dark Room"),
                                    AutocompleteEntry(cmdlets.front() + " 19", "Mega Satan"),
                                    AutocompleteEntry(cmdlets.front() + " 20", "Blue Womb"),
                                    AutocompleteEntry(cmdlets.front() + " 21", "Greed Mode"),
                                    AutocompleteEntry(cmdlets.front() + " 22", "The Void"),
                                    AutocompleteEntry(cmdlets.front() + " 23", "Greedier"),
                                    AutocompleteEntry(cmdlets.front() + " 24", "Mother"),
                                    AutocompleteEntry(cmdlets.front() + " 25", "Dogma"),
                                    AutocompleteEntry(cmdlets.front() + " 26", "The Beast")
                                };

                                for (AutocompleteEntry entry : cutscene) {
                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }
                                break;
                            }
                                  
                            case MACRO: {
                                for (ConsoleMacro macro : macros) {
                                    if ((cmdlets.front() + " " + macro.name).rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(AutocompleteEntry(cmdlets.front() + " " + macro.name));
                                    }
                                }
                                break;
                            }

                            case SFX: {
                                // TODO (might not be possible cleanly since sounds don't really have names?) 
                                break;
                            }

                            case CURSE: {
                                // Not sure if modded curses show up in the vanilla menu, but this is better than nothing in the meantime.
                                // TODO Would be sick if we did our own bitwise calculation and displayed the result
                                std::vector<AutocompleteEntry> curse = {
                                    AutocompleteEntry(cmdlets.front() + " 0", "None"),
                                    AutocompleteEntry(cmdlets.front() + " 1", "Darkness"),
                                    AutocompleteEntry(cmdlets.front() + " 2", "Labyrinth"),
                                    AutocompleteEntry(cmdlets.front() + " 4", "Lost"),
                                    AutocompleteEntry(cmdlets.front() + " 8", "Unknown"),
                                    AutocompleteEntry(cmdlets.front() + " 16", "Cursed"),
                                    AutocompleteEntry(cmdlets.front() + " 32", "Maze"),
                                    AutocompleteEntry(cmdlets.front() + " 64", "Blind"),
                                    AutocompleteEntry(cmdlets.front() + " 128", "Giant"),
                                };

                                for (AutocompleteEntry entry : curse) {
                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }
                                break;
                            }

                            case METRO: {
                                XMLNodes items = XMLStuff.ItemData->nodes;
                                for (auto node : items) {
                                    int id = node.first;
                                    std::string name = node.second["name"];
                                    AutocompleteEntry entry = AutocompleteEntry(cmdlets.front() + " c" + std::to_string(id), name);

                                    std::string lowerBuf = data->Buf;
                                    std::transform(lowerBuf.begin(), lowerBuf.end(), lowerBuf.begin(),
                                        [](unsigned char c) { return std::tolower(c); });

                                    std::string lowerName = (cmdlets.front() + " " + name);
                                    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                                        [](unsigned char c) { return std::tolower(c); });

                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0 || lowerName.rfind(lowerBuf, 0) == 0)
                                        autocompleteBuffer.push_back(AutocompleteEntry(cmdlets.front() + " c" + std::to_string(id), name));
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory:
            {
                std::deque<std::string> history = *(g_Game->GetConsole())->GetCommandHistory();

                const int prev_history_pos = historyPos;

                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (++historyPos > history.size())
                        historyPos = prev_history_pos;
                }
                else if (data->EventKey == ImGuiKey_DownArrow) {
                    if(historyPos != 0)
                        if (--historyPos <= 0)
                            historyPos = 0;
                }

                if (prev_history_pos != historyPos) {;
                    std::string entry = historyPos ? history[historyPos - 1] : "";
                    entry.erase(std::remove(entry.begin(), entry.end(), '\n'), entry.end());
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, entry.c_str());
                }
                break;
            }
        }
        return 0;
    }
};

extern ConsoleMega console;