#include "imgui.h"
#include "XMLData.h"
#include "natural_sort.hpp"
#include "LuaCore.h"

#include <sstream>
#include <cctype>

extern int handleWindowFlags(int flags);
extern void AddWindowContextMenu(bool* pinned);

struct ConsoleCommand {
    std::string name = ""; // The name of the command.
    std::string desc = ""; // A short (usually one sentence) description of the command. Will show up when just typing `help`
    std::string helpText = ""; // The help text for the command. Will show up when doing `help (command)`
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

    bool operator<(const ConsoleCommand& cc) const
    {
        return (SI::natural::compare(name, cc.name));
    }
};

struct AutocompleteEntry {
    std::string autocompleteText;
    std::string autocompleteDesc;

    AutocompleteEntry(std::string text, std::string desc = "") {
        autocompleteText = text;
        autocompleteDesc = desc;
    }

    AutocompleteEntry() {
        autocompleteText = "";
        autocompleteDesc = "";
    }

    bool operator<(const AutocompleteEntry& ae) const
    {
        return (SI::natural::compare(autocompleteText, ae.autocompleteText));
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
    bool pinned;
    char inputBuf[1024];
    std::set<ConsoleCommand> commands;
    unsigned int historyPos;
    std::vector<ConsoleMacro> macros;
    std::vector<AutocompleteEntry> autocompleteBuffer;
    unsigned int autocompletePos;
    bool autocompleteActive = false;
    bool reclaimFocus = false;

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
        DELIRIOUS,
        CUSTOM
    };


    void RegisterCommand(const char* name, const char* desc, const char* helpText, bool showOnMenu, AutocompleteType autocomplete = NONE, std::vector<std::string> aliases = {}) {
        commands.insert(ConsoleCommand(name, desc, helpText, showOnMenu, autocomplete, aliases));
    }

    void RegisterMacro(const char* macroName, std::vector<std::string> &macroCommands) {
        macros.push_back(ConsoleMacro(macroName, &macroCommands));
    }

    ConsoleMega()
    {
        enabled = true;
        pinned = false;
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
        RegisterCommand("delirious", "Force Delirious to be a certain boss", "Overrides the next boss the Delirious item will become.\nExample:\n(delirious 3) will force Delirious to be a Chub.", false, DELIRIOUS);
        RegisterCommand("eggs", "Unlock all easter egg seeds", "PERMANENTLY unlocks all easter eggs in this save file.", true);
        RegisterCommand("giveitem", "Give the character items, trinkets, cards, and pills", "Gives the main player items, trinkets, cards and pills. These can either be by name or by prefix. Prefixes are (c) for items, (t) for trinkets, (p) for pills, and (k) for cards. Most pocket items count as cards.\nThis command also has shorthand which is just (g).\nExamples:\n(giveitem c1) will give the player The Sad Onion.\n(giveitem t1) will give the player Gulp!\n(giveitem p1) will give the player a Bad Trip pill.\n(giveitem k1) will give the player 0 - The Fool.", false, ITEM, {"g"});
        RegisterCommand("goto", "Teleport to a new room", "Teleports the character to a new room. Use (d) for a standard room, (s) for a special room, or three numbers to teleport to an existing room on the floor.\nExample:\n(goto s.boss.1010) will go to a Monstro fight.", false, GOTO);
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

    void Draw(bool isImGuiActive) {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
        if (ImGui::Begin("Console", &enabled, handleWindowFlags(0))) {
            AddWindowContextMenu(&pinned);
            std::deque<Console_HistoryEntry>* history = g_Game->GetConsole()->GetHistory();

            // fill remaining window space minus the current font size (+ padding). fixes issue where the input is outside the window frame
            if (ImGui::BeginChild("Text View", ImVec2(0, (-14 - ImGui::GetIO().FontDefault->FontSize)), true)) {
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
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }
                ImGui::EndChild();
            }

            ImGui::Separator();
            
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
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1));

                       ImGui::TextUnformatted(entry.autocompleteText.c_str());

                       if (!entry.autocompleteDesc.empty()) {
                           ImGui::SameLine();
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1));
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
            if (reclaimFocus) {
                ImGui::SetKeyboardFocusHere(-1);
                reclaimFocus = false;
            }
                

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

                        std::set<AutocompleteEntry> entries;

                        switch (command.autocompleteType) {
                            case ENTITY: {
                                std::unordered_map<tuple<int, int, int>, XMLAttributes> entities = XMLStuff.EntityData->nodes;
                                for (auto entity : entities) {
                                    int type = get<0>(entity.first);
                                    int variant = get<1>(entity.first);
                                    int subtype = get<2>(entity.first);

                                    std::string name = entity.second["name"];
                                    std::string id = std::to_string(type) + "." + std::to_string(variant) + "." + std::to_string(subtype);

                                    entries.insert(AutocompleteEntry(id, name));
                                }

                                break;
                            }
                            case GOTO: {
                                unsigned int stbID = RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1);
                                RoomConfigs stage = g_Game->GetRoomConfigHolder()->configs[stbID];
                                RoomConfig* config = stage.configs;
                                std::map<int, std::string> specialRoomTypes = {
                                    std::pair<int, std::string>(1, "default"),
                                    std::pair<int, std::string>(2, "shop"),
                                    std::pair<int, std::string>(3, "error"),
                                    std::pair<int, std::string>(4, "treasure"),
                                    std::pair<int, std::string>(5, "boss"),
                                    std::pair<int, std::string>(6, "miniboss"),
                                    std::pair<int, std::string>(7, "secret"),
                                    std::pair<int, std::string>(8, "supersecret"),
                                    std::pair<int, std::string>(9, "arcade"),
                                    std::pair<int, std::string>(10, "curse"),
                                    std::pair<int, std::string>(11, "challenge"),
                                    std::pair<int, std::string>(12, "library"),
                                    std::pair<int, std::string>(13, "sacrifice"),
                                    std::pair<int, std::string>(14, "devil"),
                                    std::pair<int, std::string>(15, "angel"),
                                    std::pair<int, std::string>(16, "itemdungeon"),
                                    std::pair<int, std::string>(17, "bossrush"),
                                    std::pair<int, std::string>(18, "isaacs"),
                                    std::pair<int, std::string>(19, "barren"),
                                    std::pair<int, std::string>(20, "chest"),
                                    std::pair<int, std::string>(21, "dice"),
                                    std::pair<int, std::string>(22, "blackmarket"),
                                    std::pair<int, std::string>(23, "greedexit"),
                                    std::pair<int, std::string>(24, "planetarium"),
                                    std::pair<int, std::string>(25, "teleporter"),
                                    std::pair<int, std::string>(26, "teleporterexit"),
                                    std::pair<int, std::string>(27, "secret"),
                                    std::pair<int, std::string>(28, "blue"),
                                    std::pair<int, std::string>(29, "ultrasecret"),
                                };

                                for (unsigned int i = 0; i < stage.nbRooms; ++i) {       
                                    entries.insert(AutocompleteEntry(std::string("d.") + std::to_string(config->Variant), config->Name));
                                    config++;
                                }

                                RoomConfigs special = g_Game->GetRoomConfigHolder()->configs[0];
                                config = special.configs;

                                for (unsigned int i = 0; i < special.nbRooms; ++i) {
                                    entries.insert(AutocompleteEntry(std::string("s.") + specialRoomTypes[config->Type] + "." + std::to_string(config->Variant), config->Name));
                                    config++;
                                }

                                for (std::pair<int, std::string> specialType : specialRoomTypes) {
                                    entries.insert(AutocompleteEntry(std::string("x.") + specialType.second));
                                }

                                break;
                            }

                            case STAGE: {

                                if (g_Game->GetDifficulty() == 2 || g_Game->GetDifficulty() == 3) { // Greed mode
                                    entries = {
                                        AutocompleteEntry("1", "Basement"),
                                        AutocompleteEntry("1a", "Cellar"),
                                        AutocompleteEntry("1b", "Burning Basement"),
                                        AutocompleteEntry("2", "Caves"),
                                        AutocompleteEntry("2a", "Catacombs"),
                                        AutocompleteEntry("2b", "Flooded Caves"),
                                        AutocompleteEntry("3", "Depths"),
                                        AutocompleteEntry("3a", "Necropolis"),
                                        AutocompleteEntry("3b", "Dank Depths"),
                                        AutocompleteEntry("4", "Womb"),
                                        AutocompleteEntry("4a", "Utero"),
                                        AutocompleteEntry("4b", "Scarred Womb"),
                                        AutocompleteEntry("5", "Sheol"),
                                        AutocompleteEntry("6", "The Shop"),
                                        AutocompleteEntry("7", "Ultra Greed")
                                    };
                                }
                                else {
                                    entries = {
                                        AutocompleteEntry("1", "Basement I"),
                                        AutocompleteEntry("1a", "Cellar I"),
                                        AutocompleteEntry("1b", "Burning Basement I"),
                                        AutocompleteEntry("1c", "Downpour I"),
                                        AutocompleteEntry("1d", "Dross I"),
                                        AutocompleteEntry("2", "Basement II"),
                                        AutocompleteEntry("2a", "Cellar II"),
                                        AutocompleteEntry("2b", "Burning Basement II"),
                                        AutocompleteEntry("2c", "Downpour II"),
                                        AutocompleteEntry("2d", "Dross II"),
                                        AutocompleteEntry("3", "Caves I"),
                                        AutocompleteEntry("3a", "Catacombs I"),
                                        AutocompleteEntry("3b", "Flooded Caves I"),
                                        AutocompleteEntry("3c", "Mines I"),
                                        AutocompleteEntry("3d", "Ashpit I"),
                                        AutocompleteEntry("4", "Caves II"),
                                        AutocompleteEntry("4a", "Catacombs II"),
                                        AutocompleteEntry("4b", "Flooded Caves II"),
                                        AutocompleteEntry("4c", "Mines II"),
                                        AutocompleteEntry("4d", "Ashpit II"),
                                        AutocompleteEntry("5", "Depths I"),
                                        AutocompleteEntry("5a", "Necropolis I"),
                                        AutocompleteEntry("5b", "Dank Depths I"),
                                        AutocompleteEntry("5c", "Mausoleum I"),
                                        AutocompleteEntry("5d", "Gehenna I"),
                                        AutocompleteEntry("6", "Depths II"),
                                        AutocompleteEntry("6a", "Necropolis II"),
                                        AutocompleteEntry("6b", "Dank Depths II"),
                                        AutocompleteEntry("6c", "Mausoleum II"),
                                        AutocompleteEntry("6d", "Gehenna II"),
                                        AutocompleteEntry("7", "Womb I"),
                                        AutocompleteEntry("7a", "Utero I"),
                                        AutocompleteEntry("7b", "Scarred Womb I"),
                                        AutocompleteEntry("7c", "Corpse I"),
                                        AutocompleteEntry("8", "Womb II"),
                                        AutocompleteEntry("8a", "Utero II"),
                                        AutocompleteEntry("8b", "Scarred Womb II"),
                                        AutocompleteEntry("8c", "Corpse II"),
                                        AutocompleteEntry("9", "??? / Blue Womb"),
                                        AutocompleteEntry("10", "Sheol"),
                                        AutocompleteEntry("10a", "Cathedral"),
                                        AutocompleteEntry("11", "Dark Room"),
                                        AutocompleteEntry("11a", "Chest"),
                                        AutocompleteEntry("12", "The Void"),
                                        AutocompleteEntry("13", "Home (day)"),
                                        AutocompleteEntry("13a", "Home (night)")
                                    };
                                }
                                break;
                            }

                            case DEBUG_FLAG: {
                                entries = {
                                    AutocompleteEntry("1", "Entity Positions"),
                                    AutocompleteEntry("2", "Grid"),
                                    AutocompleteEntry("3", "Infinite HP"),
                                    AutocompleteEntry("4", "High Damage"),
                                    AutocompleteEntry("5", "Show Room Info"),
                                    AutocompleteEntry("6", "Show Hitspheres"),
                                    AutocompleteEntry("7", "Show Damage Values"),
                                    AutocompleteEntry("8", "Infinite Item Charges"),
                                    AutocompleteEntry("9", "High Luck"),
                                    AutocompleteEntry("10", "Quick Kill"),
                                    AutocompleteEntry("11", "Grid Info"),
                                    AutocompleteEntry("12", "Player Item Info"),
                                    AutocompleteEntry("13", "Show Grid Collision Points"),
                                    AutocompleteEntry("14", "Show Lua Memory Usage")
                                };
                                break;
                            }

                            case ITEM: {
                                std::vector<std::pair<XMLNodes, std::string>> XMLPairs = {
                                    {XMLStuff.ItemData->nodes, "c"},
                                    {XMLStuff.TrinketData->nodes, "t"},
                                    {XMLStuff.CardData->nodes, "k"},
                                    {XMLStuff.PillData->nodes, "p"},
                                };

                                for (std::pair<XMLNodes, std::string> XMLPair : XMLPairs) {
                                    for (auto node : XMLPair.first) {
                                        int id = node.first;
                                        std::string name = node.second["name"];
                                        entries.insert(AutocompleteEntry(XMLPair.second + std::to_string(id), name));
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

                                    entries.insert(AutocompleteEntry(std::to_string(id), name));
                                }
                                break;
                            }

                            case COMBO: {
                                entries = {
                                    AutocompleteEntry("0.", "Treasure"),
                                    AutocompleteEntry("1.", "Shop"),
                                    AutocompleteEntry("2.", "Boss"),
                                    AutocompleteEntry("3.", "Devil"),
                                    AutocompleteEntry("4.", "Angel"),
                                    AutocompleteEntry("5.", "Secret"),
                                    AutocompleteEntry("6.", "Library"),
                                    AutocompleteEntry("7.", "Challenge"),
                                    AutocompleteEntry("8.", "Golden Chest"),
                                    AutocompleteEntry("9.", "Red Chest"),
                                    AutocompleteEntry("10.", "Beggar"),
                                    AutocompleteEntry("11.", "Demon Beggar"),
                                    AutocompleteEntry("12.", "Curse"),
                                    AutocompleteEntry("13.", "Key Master"),
                                    AutocompleteEntry("14.", "Boss Rush"),
                                    AutocompleteEntry("15.", "Dungeon"),
                                };
                                break;
                            }

                            case CUTSCENE: {
                                XMLNodes cutscenes = XMLStuff.CutsceneData->nodes;
                                for (auto node : cutscenes) {
                                    int id = node.first;
                                    std::string name = node.second["name"];
                                    entries.insert(AutocompleteEntry(std::to_string(id), name));
                                }
                                break;
                            }
                                  
                            case MACRO: {
                                for (ConsoleMacro macro : macros) {
                                    entries.insert(AutocompleteEntry(macro.name));
                                }
                                break;
                            }

                            case SFX: {
                                XMLNodes sounds = XMLStuff.SoundData->nodes;
                                for (auto node : sounds) {
                                    int id = node.first;
                                    std::string name = node.second["name"];
                                    entries.insert(AutocompleteEntry(std::to_string(id), name));
                                }
                                break;
                            }

                            case CURSE: {
                                XMLNodes curses = XMLStuff.CurseData->nodes;
                                for (auto node : curses) {
                                    int id = node.first;
                                    std::string name = node.second["name"];
                                    entries.insert(AutocompleteEntry(std::to_string(id), name));
                                }

                                // This is a cut-down version of the parsing the other commands do.
                                // We handle this ourselves to do bitwise calculation later.
                                for (AutocompleteEntry entry : entries) {
                                    entry.autocompleteText = cmdlets.front() + " " + entry.autocompleteText;
                                    std::string lowerDescBuf;

                                    for (auto it = cmdlets.begin() + 1; it != cmdlets.end(); ++it) {
                                        lowerDescBuf += *it;
                                        if (it != cmdlets.end() - 1) {
                                            lowerDescBuf += " ";
                                        }
                                    }

                                    std::transform(lowerDescBuf.begin(), lowerDescBuf.end(), lowerDescBuf.begin(),
                                        [](unsigned char c) { return std::tolower(c); });


                                    std::string lowerDesc = entry.autocompleteDesc;
                                    std::transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(),
                                        [](unsigned char c) { return std::tolower(c); });

                                    if (entry.autocompleteText.rfind(data->Buf, 0) == 0 || lowerDesc.find(lowerDescBuf) != std::string::npos) {
                                        autocompleteBuffer.push_back(entry);
                                    }
                                }

                                // If no result, let's calculate ourselves to give a preview of what curses this command will activate
                                if (autocompleteBuffer.empty() && cmdlets.size() == 2) {
                                    long mask;
                                    std::string calcCurses;

                                    try {
                                        mask = std::stoi(cmdlets[1]);
                                    }
                                    #pragma warning(suppress: 4101)
                                    catch (std::invalid_argument& err) {
                                        return 0;
                                    }
                                    for (auto node : curses) {
                                        int id = node.first;
                                        if ((mask & id) != 0) {
                                            if (!calcCurses.empty())
                                                calcCurses = calcCurses + " + ";
                                            calcCurses = calcCurses + node.second["name"];
                                        }
                                    }
                                    autocompleteBuffer.push_back(AutocompleteEntry(cmdlets.front() + " " + std::to_string(mask), calcCurses));
                                }

                                return 0;
                            }

                            case METRO: {
                                XMLNodes items = XMLStuff.ItemData->nodes;
                                for (auto node : items) {
                                    int id = node.first;
                                    std::string name = node.second["name"];
                                    entries.insert(AutocompleteEntry("c" + std::to_string(id), name));
                                }
                                break;
                            }

                            case DELIRIOUS: {
                                entries = {
                                    AutocompleteEntry("0", "None"),
                                    AutocompleteEntry("1", "Monstro"),
                                    AutocompleteEntry("2", "Larry Jr."),
                                    AutocompleteEntry("3", "Chub"),
                                    AutocompleteEntry("4", "Gurdy"),
                                    AutocompleteEntry("5", "Monstro II"),
                                    AutocompleteEntry("6", "Scolex"),
                                    AutocompleteEntry("7", "Famine"),
                                    AutocompleteEntry("8", "Pestilence"),
                                    AutocompleteEntry("9", "War"),
                                    AutocompleteEntry("10", "Death"),
                                    AutocompleteEntry("11", "The Duke of Flies"),
                                    AutocompleteEntry("12", "Peep"),
                                    AutocompleteEntry("13", "Loki"),
                                    AutocompleteEntry("14", "Blastocyst"),
                                    AutocompleteEntry("15", "Gemini"),
                                    AutocompleteEntry("16", "Fistula"),
                                    AutocompleteEntry("17", "Gish"),
                                    AutocompleteEntry("18", "Steven"),
                                    AutocompleteEntry("19", "C.H.A.D."),
                                    AutocompleteEntry("20", "The Headless Horseman"),
                                    AutocompleteEntry("21", "The Fallen"),
                                    AutocompleteEntry("22", "The Hollow"),
                                    AutocompleteEntry("23", "Carrion Queen"),
                                    AutocompleteEntry("24", "Gurdy Jr."),
                                    AutocompleteEntry("25", "The Husk"),
                                    AutocompleteEntry("26", "The Bloat"),
                                    AutocompleteEntry("27", "Lokii"),
                                    AutocompleteEntry("28", "The Blighted Ovum"),
                                    AutocompleteEntry("29", "Teratoma"),
                                    AutocompleteEntry("30", "The Widow"),
                                    AutocompleteEntry("31", "Mask of Infamy"),
                                    AutocompleteEntry("32", "The Wretched"),
                                    AutocompleteEntry("33", "Pin"),
                                    AutocompleteEntry("34", "Conquest"),
                                    AutocompleteEntry("35", "Daddy Long Legs"),
                                    AutocompleteEntry("36", "Triachnid"),
                                    AutocompleteEntry("37", "The Haunt"),
                                    AutocompleteEntry("38", "Dingle"),
                                    AutocompleteEntry("39", "Mega Maw"),
                                    AutocompleteEntry("40", "The Gate"),
                                    AutocompleteEntry("41", "Mega Fatty"),
                                    AutocompleteEntry("42", "The Cage"),
                                    AutocompleteEntry("43", "Mama Gurdy"),
                                    AutocompleteEntry("44", "Dark One"),
                                    AutocompleteEntry("45", "The Adversary"),
                                    AutocompleteEntry("46", "Polycephalus"),
                                    AutocompleteEntry("47", "Mr. Fred"),
                                    AutocompleteEntry("48", "Mega Satan"),
                                    AutocompleteEntry("49", "Gurgling"),
                                    AutocompleteEntry("50", "The Stain"),
                                    AutocompleteEntry("51", "Brownie"),
                                    AutocompleteEntry("52", "The Forsaken"),
                                    AutocompleteEntry("53", "Little Horn"),
                                    AutocompleteEntry("54", "Rag Man"),
                                    AutocompleteEntry("55", "Hush"),
                                    AutocompleteEntry("56", "Dangle"),
                                    AutocompleteEntry("57", "Turdling"),
                                    AutocompleteEntry("58", "The Frail"),
                                    AutocompleteEntry("59", "Rag Mega"),
                                    AutocompleteEntry("60", "Sisters Vis"),
                                    AutocompleteEntry("61", "Big Horn")
                                };
                                break;
                            }

                            case CUSTOM: {
                                // This is a Lua command with the CUSTOM AutocompleteType defined. It wants to add its own autocomplete.
                                // Register the callback MC_CONSOLE_AUTOCOMPLETE with the command as an optional param.

                                extern std::bitset<500> CallbackState;

                                int callbackId = 1120;
                                if (CallbackState.test(callbackId - 1000)) {
                                    lua_State* L = g_LuaEngine->_state;
                                    lua::LuaStackProtector protector(L);
                                    lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
                                    std::string args;

                                    for (auto it = cmdlets.begin() + 1; it != cmdlets.end(); ++it) {
                                        args += *it;
                                        if (it != cmdlets.end() - 1) {
                                            args += " ";
                                        }
                                    }

                                    lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
                                        .push(cmdlets.front().c_str())
                                        .push(cmdlets.front().c_str())
                                        .push(args.c_str())
                                        .call(1);

                                    if (!results) {
                                        if (lua_istable(L, -1)) {
                                            for (unsigned int i = 1; i <= (unsigned int)lua_rawlen(L, -1); ++i) {
                                                AutocompleteEntry entry;
                                                lua_pushinteger(L, i);
                                                lua_gettable(L, -2);

                                                if (lua_istable(L, -1)) {
                                                    lua_pushinteger(L, 1);
                                                    lua_gettable(L, -2);
                                                    entry.autocompleteText = lua_tostring(L, -1);
                                                    lua_pop(L, 1);

                                                    if (lua_rawlen(L, -1) == (unsigned int)2) {
                                                        lua_pushinteger(L, 2);
                                                        lua_gettable(L, -2);
                                                        entry.autocompleteDesc = lua_tostring(L, -1);
                                                        lua_pop(L, 1);
                                                    }
                                                }

                                                else if (lua_isstring(L, -1)) {
                                                    entry.autocompleteText = lua_tostring(L, -1);
                                                }

                                                lua_pop(L, 1);
                                                entries.insert(entry);
                                            }
                                        }
                                    }
                                }
                                break;
                            }

                        }

                        for (AutocompleteEntry entry : entries) {
                            entry.autocompleteText = cmdlets.front() + " " + entry.autocompleteText;

                            std::string lowerBuf = data->Buf;
                            std::transform(lowerBuf.begin(), lowerBuf.end(), lowerBuf.begin(),
                                [](unsigned char c) { return std::tolower(c); });

                            std::string lowerText = entry.autocompleteText;
                            std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(),
                                [](unsigned char c) { return std::tolower(c); });

                            // Since we already split by space, let's take advantage of that. 
                            // Construct a new string with our current input minus the command, and use it for position-agnostic description searching.
                            std::string lowerDescBuf;

                            for (auto it = cmdlets.begin() + 1; it != cmdlets.end(); ++it) {
                                lowerDescBuf += *it;
                                if (it != cmdlets.end() - 1) {
                                    lowerDescBuf += " ";
                                }
                            }

                            std::transform(lowerDescBuf.begin(), lowerDescBuf.end(), lowerDescBuf.begin(),
                                [](unsigned char c) { return std::tolower(c); });


                            std::string lowerDesc = entry.autocompleteDesc;
                            std::transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(),
                                [](unsigned char c) { return std::tolower(c); });

                            if (lowerText.rfind(lowerBuf, 0) == 0 || lowerDesc.find(lowerDescBuf) != std::string::npos) {
                                autocompleteBuffer.push_back(entry);
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