#pragma once
#include "imgui.h"
#include "ImGuiEx.h"
#include "../Patches/XMLData.h"
#include "natural_sort.hpp"
#include "LuaCore.h"
#include "UnifontSupport.h"
#include "Lang.h"

#include <sstream>
#include <cctype>
#include <regex>

extern int handleWindowFlags(int flags);
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
extern bool imguiResized;
extern ImVec2 imguiSizeModifier;

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
        // Trailing zeros are messing up sorting- after all, to a human, 5.1 and 5.10 are the same.
        // Replace periods with dashes to overcome this.

        std::string comp1 = autocompleteText;
        std::string comp2 = ae.autocompleteText;

        std::string bad = ".";
        std::string repl = "-";

        size_t found = comp1.find(bad);
        while (found != std::string::npos) {
            comp1.replace(found, bad.length(), repl);
            found = comp1.find(bad);
        }

        found = comp2.find(bad);
        while (found != std::string::npos) {
            comp2.replace(found, bad.length(), repl);
            found = comp2.find(bad);
        }

        return (SI::natural::compare(comp1, comp2));
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
        cmdlet.erase(std::remove_if(cmdlet.begin(), cmdlet.end(), isspace), cmdlet.end());
        cmdlets.push_back(cmdlet);
        if (size > 0 && cmdlets.size() == size) {
            break;
        }
    }
    return cmdlets;
}

struct ConsoleMega : ImGuiWindowObject {
    char inputBuf[1024];
    std::set<ConsoleCommand> commands;
    unsigned int historyPos;
    std::vector<ConsoleMacro> macros;
    std::vector<AutocompleteEntry> autocompleteBuffer;
    unsigned int autocompletePos;
    bool autocompleteActive = false;
    bool autocompleteNeedFocusChange = false;
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
        PLAYER,
		ACHIEVEMENT,
        MODFOLDER,
        CUSTOM
    };


    void RegisterCommand(const char* name, const char* desc, const char* helpText, bool showOnMenu, AutocompleteType autocomplete = NONE, std::vector<std::string> aliases = {}) {
        commands.insert(ConsoleCommand(name, desc, helpText, showOnMenu, autocomplete, aliases));
    }

    void RegisterMacro(const char* macroName, std::vector<std::string> &macroCommands) {
        macros.push_back(ConsoleMacro(macroName, &macroCommands));
    }

    ConsoleMega() : ImGuiWindowObject("Console")
    {
        enabled = true;
        memset(inputBuf, 0, sizeof(inputBuf));
        historyPos = 0;

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

    void InitAfterLanguageAvaliable() {
        windowName = LANG.CONSOLE_WINDOW_NAME;
        
        RegisterCommand("achievement", LANG.CONSOLE_ACHIEVEMENT_DESC, LANG.CONSOLE_ACHIEVEMENT_HELP, true, ACHIEVEMENT);
        RegisterCommand("addplayer", LANG.CONSOLE_ADD_PLAYER_DESC, LANG.CONSOLE_ADD_PLAYER_HELP, false, PLAYER);
        RegisterCommand("challenge", LANG.CONSOLE_CHALLENGE_DESC, LANG.CONSOLE_CHALLENGE_HELP, false, CHALLENGE);
        RegisterCommand("clear", LANG.CONSOLE_CLEAR_DESC, LANG.CONSOLE_CLEAR_HELP, true);
        RegisterCommand("clearcache", LANG.CONSOLE_CLEARCACHE_DESC, LANG.CONSOLE_CLEARCACHE_HELP, true);
        RegisterCommand("clearseeds", LANG.CONSOLE_CLEARSEEDS_DESC, LANG.CONSOLE_CLEARSEEDS_HELP, false);
        RegisterCommand("combo", LANG.CONSOLE_COMBO_DESC, LANG.CONSOLE_COMBO_HELP, false, COMBO);
        RegisterCommand("copy", LANG.CONSOLE_COPY_DESC, LANG.CONSOLE_COPY_HELP, true);
        RegisterCommand("costumetest", LANG.CONSOLE_COSTUMETEST_DESC, LANG.CONSOLE_COSTUMETEST_HELP, false);
        RegisterCommand("curse", LANG.CONSOLE_CURSE_DESC, LANG.CONSOLE_CURSE_HELP, true, CURSE);
        RegisterCommand("cutscene", LANG.CONSOLE_CUTSCENE_DESC, LANG.CONSOLE_CUTSCENE_HELP, true, CUTSCENE);
        RegisterCommand("debug", LANG.CONSOLE_DEBUG_DESC, LANG.CONSOLE_DEBUG_HELP, false, DEBUG_FLAG);
        RegisterCommand("delirious", LANG.CONSOLE_DELIRIOUS_DESC, LANG.CONSOLE_DELIRIOUS_HELP, false, DELIRIOUS);
        RegisterCommand("eggs", LANG.CONSOLE_EGGS_DESC, LANG.CONSOLE_EGGS_HELP, true);
        RegisterCommand("forceroom", LANG.CONSOLE_FORCEROOM_DESC, LANG.CONSOLE_FORCEROOM_HELP, false, GOTO);
        RegisterCommand("fullrestart", LANG.CONSOLE_FULLRESTART_DESC, LANG.CONSOLE_FULLRESTART_HELP, true);
        RegisterCommand("giveitem", LANG.CONSOLE_GIVEITEM_DESC, LANG.CONSOLE_GIVEITEM_HELP, false, ITEM, { "g" });
        RegisterCommand("giveitem2", LANG.CONSOLE_GIVEITEM2_DESC, LANG.CONSOLE_GIVEITEM2_HELP, false, ITEM, { "g2" });
        RegisterCommand("goto", LANG.CONSOLE_GOTO_DESC, LANG.CONSOLE_GOTO_HELP, false, GOTO);
        RegisterCommand("gridspawn", LANG.CONSOLE_GRIDSPAWN_DESC, LANG.CONSOLE_GRIDSPAWN_HELP, false, GRID);
        RegisterCommand("help", LANG.CONSOLE_HELP_DESC, LANG.CONSOLE_HELP_HELP, true);
        RegisterCommand("listcollectibles", LANG.CONSOLE_LISTCOLLECTIBLES_DESC, LANG.CONSOLE_LISTCOLLECTIBLES_HELP, false);
        RegisterCommand("lockachievement", LANG.CONSOLE_LOCKACHIEVEMENT_DESC, LANG.CONSOLE_LOCKACHIEVEMENT_HELP, true, ACHIEVEMENT);
        RegisterCommand("lua", LANG.CONSOLE_LUA_DESC, LANG.CONSOLE_LUA_HELP, true);
        RegisterCommand("luamem", LANG.CONSOLE_LUAMEM_DESC, LANG.CONSOLE_LUAMEM_HELP, true);
        RegisterCommand("luamod", LANG.CONSOLE_LUAMOD_DESC, LANG.CONSOLE_LUAMOD_HELP, true, MODFOLDER);
        RegisterCommand("luareset", LANG.CONSOLE_LUARESET_DESC, LANG.CONSOLE_LUARESET_HELP, true);
        RegisterCommand("luarun", LANG.CONSOLE_LUARUN_DESC, LANG.CONSOLE_LUARUN_HELP, true);
        RegisterCommand("macro", LANG.CONSOLE_MACRO_DESC, LANG.CONSOLE_MACRO_HELP, false, MACRO, { "m" });
        RegisterCommand("metro", LANG.CONSOLE_METRO_DESC, LANG.CONSOLE_METRO_HELP, false, METRO);
        RegisterCommand("netdelay", LANG.CONSOLE_NETDELAY_DESC, LANG.CONSOLE_NETDELAY_HELP, true);
        RegisterCommand("netstart", LANG.CONSOLE_NETSTART_DESC, LANG.CONSOLE_NETSTART_HELP, true);
        RegisterCommand("playsfx", LANG.CONSOLE_PLAYSFX_DESC, LANG.CONSOLE_PLAYSFX_HELP, true, SFX);
        RegisterCommand("prof", LANG.CONSOLE_PROF_DESC, LANG.CONSOLE_PROF_HELP, true);
        RegisterCommand("profstop", LANG.CONSOLE_PROFSTOP_DESC, LANG.CONSOLE_PROFSTOP_HELP, true);
        RegisterCommand("remove", LANG.CONSOLE_REMOVE_DESC, LANG.CONSOLE_REMOVE_HELP, false, ITEM, { "r" });
        RegisterCommand("remove2", LANG.CONSOLE_REMOVE2_DESC, LANG.CONSOLE_REMOVE2_HELP, false, ITEM, { "r2" });
        RegisterCommand("reloadfx", LANG.CONSOLE_RELOADFX_DESC, LANG.CONSOLE_RELOADFX_HELP, false);
        RegisterCommand("reloadshaders", LANG.CONSOLE_RELOADSHADERS_DESC, LANG.CONSOLE_RELOADSHADERS_HELP, false);
        RegisterCommand("reloadwisps", LANG.CONSOLE_RELOADWISPS_DESC, LANG.CONSOLE_RELOADWISPS_HELP, false);
        RegisterCommand("repeat", LANG.CONSOLE_REPEAT_DESC, LANG.CONSOLE_REPEAT_HELP, true);
        RegisterCommand("reseed", LANG.CONSOLE_RESEED_DESC, LANG.CONSOLE_RESEED_HELP, false);
        RegisterCommand("restart", LANG.CONSOLE_RESTART_DESC, LANG.CONSOLE_RESTART_HELP, false, PLAYER);
        RegisterCommand("restock", LANG.CONSOLE_RESTOCK_DESC, LANG.CONSOLE_RESTOCK_HELP, false);
        RegisterCommand("rewind", LANG.CONSOLE_REWIND_DESC, LANG.CONSOLE_REWIND_HELP, false);
        RegisterCommand("seed", LANG.CONSOLE_SEED_DESC, LANG.CONSOLE_SEED_HELP, false);
        RegisterCommand("spawn", LANG.CONSOLE_SPAWN_DESC, LANG.CONSOLE_SPAWN_HELP, false, ENTITY);
        RegisterCommand("stage", LANG.CONSOLE_STAGE_DESC, LANG.CONSOLE_STAGE_HELP, false, STAGE);
        RegisterCommand("time", LANG.CONSOLE_TIME_DESC, LANG.CONSOLE_TIME_HELP, false);
        RegisterCommand("testbosspool", LANG.CONSOLE_TESTBOSSPOOL_DESC, LANG.CONSOLE_TESTBOSSPOOL_HELP, false);
    }

    const ConsoleCommand* GetCommandByName(std::string& commandName) {
      for (auto& commandIter : commands) {
        if (commandName == commandIter.name) {
          return &commandIter;
        }
        for (auto& alias : commandIter.aliases) {
          if (commandName == alias) {
            return &commandIter;
          }
        }
      }
      return nullptr;
    }

    std::string FixSpawnCommand(char* input) {
      std::vector<std::string> cmdlets = ParseCommand(input);
      if (cmdlets.size() >= 2) {
        std::string commandName = cmdlets.front();
        commandName.erase(remove(commandName.begin(), commandName.end(), ' '), commandName.end());
        const ConsoleCommand* command = GetCommandByName(commandName);
        // redirect "giveitem" command to allow for trinket, card and pill being given using their name.
        if (command && (command->autocompleteType == ITEM && !autocompleteBuffer.empty()))
        {
          AutocompleteEntry firstEntry = autocompleteBuffer.front();
          if (firstEntry.autocompleteText.at(firstEntry.autocompleteText.find(" ")+1) != 'c')
          {
            // if first autocomplete entry is not a collectible, replace console input with autocomplete entry.
            return firstEntry.autocompleteText;
          }
        }
      }
      return input;
    }

    void ExecuteCommand(char* input) {
        Console* console = g_Game->GetConsole();

        std::string printin = std::string(">") + input + "\n";
        std::string out;
        bool clear = false;

        if (!console->GetCommandHistory()->empty() && console->GetCommandHistory()->front() == input)
            clear = true;

        console->_input = input;
        console->SubmitInput(false);

        if (clear)
            console->GetCommandHistory()->pop_front();

        memset(inputBuf, 0, sizeof(inputBuf));
        historyPos = 0;
        autocompleteBuffer.clear();
    }

    void Draw(bool isImGuiActive) {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
        
        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            if (imguiResized) {
                ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x * imguiSizeModifier.x, ImGui::GetWindowPos().y * imguiSizeModifier.y));
                ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x * imguiSizeModifier.x, ImGui::GetWindowSize().y * imguiSizeModifier.y));
;            }

            AddWindowContextMenu();
            std::deque<Console_HistoryEntry>* history = g_Game->GetConsole()->GetHistory();

            // fill remaining window space minus the current font size (+ padding). fixes issue where the input is outside the window frame
            float textboxHeight = -4 - (ImGui::GetStyle().FramePadding.y * 2) - (imFontUnifont->Scale * imFontUnifont->FontSize);
            if (!isImGuiActive)
            {
              textboxHeight = 0;
            }
            if (ImGui::BeginChild("Text View", ImVec2(0, textboxHeight), ImGuiChildFlags_Border)) {
                /* For "simplicity" and so we don't have duplicated memory while still allowing both old and new console to be usable,
                * we reuse existing console history.
                * The vanilla console stores history backwards, so we iterate over it in reverse.
                */
                for (auto entry = history->rbegin(); entry != history->rend(); ++entry) {
                    int colorMap = entry->GetColorMap() & 0xFFFFFFFF;

                    // The vanilla console stores color as a bitwise flag because we can't have nice things.

                    float red = (float)(colorMap >> 0x10 & 0xFF) / 255.f;
                    float green = (float)(colorMap >> 8 & 0xFF) / 255.f;
                    float blue = (float)(colorMap & 0xFF) / 255.f;

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, blue, 1));
                    ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
                    ImGui::TextUnformatted(entry->_text.c_str()); // needs to be TextUnformatted to prevent unintentional formatting
                    ImGui::PopTextWrapPos();
                    ImGui::PopStyleColor();
                }
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }
            }
            ImGui::EndChild();

            if (isImGuiActive) {
              ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
              if (!autocompleteBuffer.empty()) {
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y));
                ImGui::SetNextWindowSizeConstraints(ImVec2(ImGui::GetWindowSize().x, 0), ImVec2(ImGui::GetWindowSize().x, 302)); // 302 is chosen here to have a "pixel perfect" scroll to the bottom
                if (ImGui::BeginPopup("Console Autocomplete", ImGuiWindowFlags_NoFocusOnAppearing)) {
                  if (ImGui::BeginTable("AutocompleteEntriesTable", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBody)) {
                    for (size_t i = 0; i < autocompleteBuffer.size(); i++) {
                      AutocompleteEntry entry = autocompleteBuffer[i];
                      bool isSelected = autocompletePos == i;
                      ImGui::TableNextRow();
                      ImGui::TableNextColumn();

                      if (isSelected) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                        if (autocompleteNeedFocusChange) {
                          ImGui::SetScrollHereY();
                          autocompleteNeedFocusChange = false;
                        }
                      }
                      else
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1));

                      if (ImGui::Selectable(entry.autocompleteText.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                        strcpy(inputBuf, entry.autocompleteText.c_str());
                        autocompletePos = i;
                        autocompleteNeedFocusChange = true;
                        reclaimFocus = true;
                      }

                      if (!entry.autocompleteDesc.empty()) {
                        entry.autocompleteDesc = "(" + entry.autocompleteDesc + ")";
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(entry.autocompleteDesc.c_str());
                      }

                      ImGui::PopStyleColor();
                    }
                    ImGui::EndTable();
                  }
                  ImGui::EndPopup();
                }
                ImGui::OpenPopup("Console Autocomplete");
              }
              ImVec2 drawPos = ImGui::GetCursorPos();

              ImGuiInputTextFlags consoleFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_CtrlEnterForNewLine;
              if (ImGui::InputTextWithHint("##", LANG.CONSOLE_CMD_HINT, inputBuf, 1024, consoleFlags, &TextEditCallbackStub, (void*)this)) {
                char* s = inputBuf;
                Strtrim(s);
                std::string fixedCommand = FixSpawnCommand(s);
                s = (char*)fixedCommand.c_str();
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

              // Handle preview text
              if (!autocompleteBuffer.empty() && autocompletePos >= 0 && autocompletePos < autocompleteBuffer.size())
              {
                AutocompleteEntry entry = autocompleteBuffer[autocompletePos];
                std::string previewText = entry.autocompleteText.substr(min(strlen(inputBuf), entry.autocompleteText.size()));

                // render at end of input text
                ImVec2 textSize = ImGui::CalcTextSize(inputBuf);
                ImGui::SetCursorPos(ImVec2(drawPos.x + ImGui::GetStyle().FramePadding.x + textSize.x, drawPos.y + ImGui::GetStyle().FramePadding.y));

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1));
                ImGui::Text(previewText.c_str());
                ImGui::PopStyleColor();
              }
            }
        }
        
        ImGui::End(); // close window element
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
                    if (autocompletePos >= autocompleteBuffer.size())
                        autocompletePos = 0;

                    autocompleteActive = true;
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, autocompleteBuffer[autocompletePos].autocompleteText.c_str());
                    autocompleteActive = false;

                    autocompleteNeedFocusChange = true;
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackEdit:
            {
                if (autocompleteActive) return 0; // Dont execute callback when ImGuiInputTextFlags_CallbackCompletion does its thing

                std::string strBuf = data->Buf;
                std::vector<std::string> cmdlets = ParseCommand(strBuf);
                autocompleteBuffer.clear();
                autocompletePos = 0;

                if (cmdlets.size() == 1) {
                    bool inGame = !(g_Manager->GetState() != 2 || !g_Game);
                    for (auto& command : commands) {
                        if (command.name.rfind(data->Buf, 0) == 0)
                            if (inGame || (!inGame && command.showOnMenu == true)) {
                                autocompleteBuffer.push_back(AutocompleteEntry(command.name, command.desc));
                            }
                    }
                }

                if (cmdlets.size() >= 2 || (cmdlets.size() < 3 && std::isspace(static_cast<unsigned char>(strBuf.back())))) {
                    std::string commandName = cmdlets.front();
                    commandName.erase(remove(commandName.begin(), commandName.end(), ' '), commandName.end());

                    const ConsoleCommand* command = GetCommandByName(commandName);
                    if (command == nullptr) return 0;

                    std::set<AutocompleteEntry> entries;

                    switch (command->autocompleteType) {
                        case ENTITY: {
                            std::unordered_map<tuple<int, int, int>, XMLAttributes> entities = XMLStuff.EntityData->nodes;

                            std::vector<std::pair<int, XMLNodes>> XMLPairs = {
                                std::pair<int, XMLNodes>{100, XMLStuff.ItemData->nodes},
                                std::pair<int, XMLNodes>{300, XMLStuff.CardData->nodes},
                                std::pair<int, XMLNodes>{350, XMLStuff.TrinketData->nodes},
                            };
                                
                            for (auto &entity : entities) {
                                int type = get<0>(entity.first);
                                int variant = get<1>(entity.first);
                                int subtype = get<2>(entity.first);

                                std::string name = entity.second["name"];
                                std::string id = std::to_string(type) + "." + std::to_string(variant) + "." + std::to_string(subtype);

                                if (type == 5 && variant == 300) { // This is completely invalid
                                    continue;
                                }

                                entries.insert(AutocompleteEntry(id, name));
                            }

                            for (std::pair<int, XMLNodes> &pair : XMLPairs) {
                                for (std::pair<const int, XMLAttributes> &node : pair.second) {
                                    std::string id = "5." + std::to_string(pair.first) + "." + std::to_string(node.first);
                                    entries.insert(AutocompleteEntry(id, id == "5.300.0" ? "Tarot Card" : node.second["name"]));
                                }
                            }

                            break;
                        }

                        case GOTO: {
                            unsigned int stbID = RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1);
                            RoomConfigs stage = g_Game->GetRoomConfigHolder()->configs[stbID];
                            RoomConfig* config = stage.configs;
                            std::map<int, std::string> specialRoomTypes = {
                                std::pair<int, std::string>(1, LANG.CONSOLE_GOTO_HINT_default),
                                std::pair<int, std::string>(2, LANG.CONSOLE_GOTO_HINT_shop),
                                std::pair<int, std::string>(3, LANG.CONSOLE_GOTO_HINT_error),
                                std::pair<int, std::string>(4, LANG.CONSOLE_GOTO_HINT_treasure),
                                std::pair<int, std::string>(5, LANG.CONSOLE_GOTO_HINT_boss),
                                std::pair<int, std::string>(6, LANG.CONSOLE_GOTO_HINT_miniboss),
                                std::pair<int, std::string>(7, LANG.CONSOLE_GOTO_HINT_secret),
                                std::pair<int, std::string>(8, LANG.CONSOLE_GOTO_HINT_supersecret),
                                std::pair<int, std::string>(9, LANG.CONSOLE_GOTO_HINT_arcade),
                                std::pair<int, std::string>(10, LANG.CONSOLE_GOTO_HINT_curse),
                                std::pair<int, std::string>(11, LANG.CONSOLE_GOTO_HINT_challenge),
                                std::pair<int, std::string>(12, LANG.CONSOLE_GOTO_HINT_library),
                                std::pair<int, std::string>(13, LANG.CONSOLE_GOTO_HINT_sacrifice),
                                std::pair<int, std::string>(14, LANG.CONSOLE_GOTO_HINT_devil),
                                std::pair<int, std::string>(15, LANG.CONSOLE_GOTO_HINT_angel),
                                std::pair<int, std::string>(16, LANG.CONSOLE_GOTO_HINT_itemdungeon),
                                std::pair<int, std::string>(17, LANG.CONSOLE_GOTO_HINT_bossrush),
                                std::pair<int, std::string>(18, LANG.CONSOLE_GOTO_HINT_isaacs),
                                std::pair<int, std::string>(19, LANG.CONSOLE_GOTO_HINT_barren),
                                std::pair<int, std::string>(20, LANG.CONSOLE_GOTO_HINT_chest),
                                std::pair<int, std::string>(21, LANG.CONSOLE_GOTO_HINT_dice),
                                std::pair<int, std::string>(22, LANG.CONSOLE_GOTO_HINT_blackmarket),
                                std::pair<int, std::string>(23, LANG.CONSOLE_GOTO_HINT_greedexit),
                                std::pair<int, std::string>(24, LANG.CONSOLE_GOTO_HINT_planetarium),
                                std::pair<int, std::string>(25, LANG.CONSOLE_GOTO_HINT_teleporter),
                                std::pair<int, std::string>(26, LANG.CONSOLE_GOTO_HINT_teleporterexit),
                                std::pair<int, std::string>(27, LANG.CONSOLE_GOTO_HINT_secretexit),
                                std::pair<int, std::string>(28, LANG.CONSOLE_GOTO_HINT_blue),
                                std::pair<int, std::string>(29, LANG.CONSOLE_GOTO_HINT_ultrasecret),
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

                            for (auto& specialType : specialRoomTypes) {
                                entries.insert(AutocompleteEntry(std::string("x.") + specialType.second));
                            }
                                
                            break;
                        }

                        case STAGE: {

                            if (g_Game->IsGreedMode()) {
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

                        case GRID: {
                            entries = {
                                AutocompleteEntry("0", "Decoration"),
                                AutocompleteEntry("1000", "Rock"),
                                AutocompleteEntry("1001", "Bomb Rock"),
                                AutocompleteEntry("1002", "Alt Rock"),
                                AutocompleteEntry("1003", "Tinted Rock"),
                                AutocompleteEntry("1008", "Marked Skull"),
                                AutocompleteEntry("1009", "Event Rock"),
                                AutocompleteEntry("1010", "Spiked Rock"),
                                AutocompleteEntry("1011", "Fool's Gold Rock"),
                                AutocompleteEntry("1300", "TNT"),
                                AutocompleteEntry("1490", "Red Poop"),
                                AutocompleteEntry("1494", "Rainbow Poop"),
                                AutocompleteEntry("1495", "Corny Poop"),
                                AutocompleteEntry("1496", "Golden Poop"),
                                AutocompleteEntry("1497", "Black Poop"),
                                AutocompleteEntry("1498", "White Poop"),
                                AutocompleteEntry("1499", "Giant Poop"),
                                AutocompleteEntry("1500", "Poop"),
                                AutocompleteEntry("1501", "Charming Poop"),
                                AutocompleteEntry("1900", "Block"),
                                AutocompleteEntry("1901", "Pillar"),
                                AutocompleteEntry("1930", "Spikes"),
                                AutocompleteEntry("1931", "Retracting Spikes"),
                                AutocompleteEntry("1931.1", "Retracting Spikes (Down 1/5)"),
                                AutocompleteEntry("1931.2", "Retracting Spikes (Down 2/5)"),
                                AutocompleteEntry("1931.3", "Retracting Spikes (Down 3/5)"),
                                AutocompleteEntry("1931.4", "Retracting Spikes (Down 4/5)"),
                                AutocompleteEntry("1931.5", "Retracting Spikes (Down 5/5)"),
                                AutocompleteEntry("1931.6", "Retracting Spikes (Up 1/5)"),
                                AutocompleteEntry("1931.7", "Retracting Spikes (Up 2/5)"),
                                AutocompleteEntry("1931.8", "Retracting Spikes (Up 3/5)"),
                                AutocompleteEntry("1931.9", "Retracting Spikes (Up 4/5)"),
                                AutocompleteEntry("1931.10", "Retracting Spikes (Up 5/5)"),
                                AutocompleteEntry("1940", "Cobweb"),
                                AutocompleteEntry("1999", "Invisible Block"),
                                AutocompleteEntry("3000", "Pit"),
                                AutocompleteEntry("3001", "Fissure Spawner"),
                                AutocompleteEntry("3002", "Event Rail"),
                                AutocompleteEntry("3009", "Event Pit"),
                                AutocompleteEntry("4000", "Key Block"),
                                AutocompleteEntry("4500", "Pressure Plate"),
                                AutocompleteEntry("4500.1", "Reward Plate"),
                                AutocompleteEntry("4500.2", "Greed Plate"),
                                AutocompleteEntry("4500.3", "Rail Plate"),
                                AutocompleteEntry("4500.9", "Kill Plate"),
                                AutocompleteEntry("4500.10", "Event Plate (group 0)"),
                                AutocompleteEntry("4500.11", "Event Plate (group 1)"),
                                AutocompleteEntry("4500.12", "Event Plate (group 2)"),
                                AutocompleteEntry("4500.13", "Event Plate (group 3)"),
                                AutocompleteEntry("5000", "Devil Statue"),
                                AutocompleteEntry("5001", "Angel Statue"),
                                AutocompleteEntry("6000", "Rail (horizontal)"),
                                AutocompleteEntry("6000.1", "Rail (vertical)"),
                                AutocompleteEntry("6000.2", "Rail (down-to-right)"),
                                AutocompleteEntry("6000.3", "Rail (down-to-left)"),
                                AutocompleteEntry("6000.4", "Rail (up-to-right)"),
                                AutocompleteEntry("6000.5", "Rail (up-to-left)"),
                                AutocompleteEntry("6000.6", "Rail (crossroad)"),
                                AutocompleteEntry("6000.7", "Rail (end-left)"),
                                AutocompleteEntry("6000.8", "Rail (end-right)"),
                                AutocompleteEntry("6000.9", "Rail (end-up)"),
                                AutocompleteEntry("6000.10", "Rail (end-down)"),
                                AutocompleteEntry("6000.16", "Rail (cart-left)"),
                                AutocompleteEntry("6000.17", "Rail (cart-up)"),
                                AutocompleteEntry("6000.32", "Rail (cart-right)"),
                                AutocompleteEntry("6000.33", "Rail (cart-down)"),
                                AutocompleteEntry("6000.80", "Mineshaft Rail (horizontal 1)"),
                                AutocompleteEntry("6000.81", "Mineshaft Rail (vertical 1)"),
                                AutocompleteEntry("6000.82", "Mineshaft Rail (down-to-right 1)"),
                                AutocompleteEntry("6000.83", "Mineshaft Rail (down-to-left 1)"),
                                AutocompleteEntry("6000.84", "Mineshaft Rail (up-to-right 1)"),
                                AutocompleteEntry("6000.85", "Mineshaft Rail (up-to-left 1)"),
                                AutocompleteEntry("6000.96", "Mineshaft Rail (horizontal 2)"),
                                AutocompleteEntry("6000.97", "Mineshaft Rail (vertical 2)"),
                                AutocompleteEntry("6000.98", "Mineshaft Rail (down-to-right 2)"),
                                AutocompleteEntry("6000.99", "Mineshaft Rail (down-to-left 2)"),
                                AutocompleteEntry("6000.100", "Mineshaft Rail (up-to-right 2)"),
                                AutocompleteEntry("6000.101", "Mineshaft Rail (up-to-left 2)"),
                                AutocompleteEntry("6000.112", "Mineshaft Rail (horizontal 3)"),
                                AutocompleteEntry("6000.113", "Mineshaft Rail (vertical 3)"),
                                AutocompleteEntry("6001", "Rail Pit (horizontal)"),
                                AutocompleteEntry("6001.1", "Rail Pit (vertical)"),
                                AutocompleteEntry("6001.2", "Rail Pit (down-to-right)"),
                                AutocompleteEntry("6001.3", "Rail Pit (down-to-left)"),
                                AutocompleteEntry("6001.4", "Rail Pit (up-to-right)"),
                                AutocompleteEntry("6001.5", "Rail Pit (up-to-left)"),
                                AutocompleteEntry("6001.6", "Rail Pit (crossroad)"),
                                AutocompleteEntry("6001.16", "Rail Pit (cart-left)"),
                                AutocompleteEntry("6001.17", "Rail Pit (cart-up)"),
                                AutocompleteEntry("6001.32", "Rail Pit (cart-right)"),
                                AutocompleteEntry("6001.33", "Rail Pit (cart-down)"),
                                AutocompleteEntry("6100", "Teleporter (square)"),
                                AutocompleteEntry("6100.1", "Teleporter (moon)"),
                                AutocompleteEntry("6100.2", "Teleporter (rhombus)"),
                                AutocompleteEntry("6100.3", "Teleporter (M)"),
                                AutocompleteEntry("6100.4", "Teleporter (pentagram)"),
                                AutocompleteEntry("6100.5", "Teleporter (cross)"),
                                AutocompleteEntry("6100.6", "Teleporter (triangle)"),
                                AutocompleteEntry("9000", "Trap Door"),
                                AutocompleteEntry("9000.1", "Void Portal"),
                                AutocompleteEntry("9100", "Crawlspace"),
                                AutocompleteEntry("10000", "Gravity"),
                            };
                            break;
                        }

                        case DEBUG_FLAG: {
                            entries = {
                                AutocompleteEntry("1", LANG.CONSOLE_DEBUG_HINT_1 ),
                                AutocompleteEntry("2", LANG.CONSOLE_DEBUG_HINT_2 ),
                                AutocompleteEntry("3", LANG.CONSOLE_DEBUG_HINT_3 ),
                                AutocompleteEntry("4", LANG.CONSOLE_DEBUG_HINT_4 ),
                                AutocompleteEntry("5", LANG.CONSOLE_DEBUG_HINT_5 ),
                                AutocompleteEntry("6", LANG.CONSOLE_DEBUG_HINT_6 ),
                                AutocompleteEntry("7", LANG.CONSOLE_DEBUG_HINT_7 ),
                                AutocompleteEntry("8", LANG.CONSOLE_DEBUG_HINT_8 ),
                                AutocompleteEntry("9", LANG.CONSOLE_DEBUG_HINT_9 ),
                                AutocompleteEntry("10", LANG.CONSOLE_DEBUG_HINT_10 ),
                                AutocompleteEntry("11", LANG.CONSOLE_DEBUG_HINT_11 ),
                                AutocompleteEntry("12", LANG.CONSOLE_DEBUG_HINT_12 ),
                                AutocompleteEntry("13", LANG.CONSOLE_DEBUG_HINT_13 ),
                                AutocompleteEntry("14", LANG.CONSOLE_DEBUG_HINT_14)
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

                            for (auto& XMLPair : XMLPairs) {
                                for (auto& node : XMLPair.first) {
                                    int id = node.first;
                                    if (id == 0) // dont display NULL item and trinket
                                      continue;
                                    std::string name = node.second["name"];
                                    entries.insert(AutocompleteEntry(XMLPair.second + std::to_string(id), name));
                                }
                            }
                            break;
                        }

                        case CHALLENGE: {
                            XMLNodes challenges = XMLStuff.ChallengeData->nodes;
                            for (auto& node : challenges) {
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
                                AutocompleteEntry("0.", LANG.CONSOLE_COMBO_HINT_0 ),
                                AutocompleteEntry("1.", LANG.CONSOLE_COMBO_HINT_1 ),
                                AutocompleteEntry("2.", LANG.CONSOLE_COMBO_HINT_2 ),
                                AutocompleteEntry("3.", LANG.CONSOLE_COMBO_HINT_3 ),
                                AutocompleteEntry("4.", LANG.CONSOLE_COMBO_HINT_4 ),
                                AutocompleteEntry("5.", LANG.CONSOLE_COMBO_HINT_5 ),
                                AutocompleteEntry("6.", LANG.CONSOLE_COMBO_HINT_6 ),
                                AutocompleteEntry("7.", LANG.CONSOLE_COMBO_HINT_7 ),
                                AutocompleteEntry("8.", LANG.CONSOLE_COMBO_HINT_8 ),
                                AutocompleteEntry("9.", LANG.CONSOLE_COMBO_HINT_9 ),
                                AutocompleteEntry("10.", LANG.CONSOLE_COMBO_HINT_10 ),
                                AutocompleteEntry("11.", LANG.CONSOLE_COMBO_HINT_11 ),
                                AutocompleteEntry("12.", LANG.CONSOLE_COMBO_HINT_12 ),
                                AutocompleteEntry("13.", LANG.CONSOLE_COMBO_HINT_13 ),
                                AutocompleteEntry("14.", LANG.CONSOLE_COMBO_HINT_14 ),
                                AutocompleteEntry("15.", LANG.CONSOLE_COMBO_HINT_15 ),
                            };
                            break;
                        }

                        case CUTSCENE: {
                            XMLNodes cutscenes = XMLStuff.CutsceneData->nodes;
                            for (auto& node : cutscenes) {
                                int id = node.first;
                                std::string name = node.second["name"];
                                entries.insert(AutocompleteEntry(std::to_string(id), name));
                            }
                            break;
                        }
                                  
                        case MACRO: {
                            for (auto& macro : macros) {
                                entries.insert(AutocompleteEntry(macro.name));
                            }
                            break;
                        }

                        case SFX: {
                            XMLNodes sounds = XMLStuff.SoundData->nodes;
                            for (auto& node : sounds) {
                                int id = node.first;
                                std::string name = node.second["name"];
                                entries.insert(AutocompleteEntry(std::to_string(id), name));
                            }
                            break;
                        }

                        case CURSE: {
                            XMLNodes curses = XMLStuff.CurseData->nodes;
                            for (auto& node : curses) {
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
                                for (auto& node : curses) {
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
                            for (auto& node : items) {
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

                        case PLAYER: {
                          entries = {
                              AutocompleteEntry("-1", "Enemy"),
                          };
                          XMLNodes players = XMLStuff.PlayerData->nodes;
                          for (auto& node : players) {
                            int id = node.first;
                            std::string name = node.second["name"];
                            entries.insert(AutocompleteEntry(std::to_string(id), name));
                          }
                          break;
                        }

                        case ACHIEVEMENT: {
                            XMLNodes achievs = XMLStuff.AchievementData->nodes;
                            for (auto& node : achievs) {
                                int id = node.first;
                                if (id > 0) { //to exclude negative achievement hackies
                                    std::string name;
                                    name = node.second["name"];

                                    entries.insert(AutocompleteEntry(std::to_string(id), name));
                                }
                            }
                            break;
                        }
                        case MODFOLDER: {
                            for (auto& node : XMLStuff.ModData->nodes) {
                                if (node.second["enabled"] == "true") {
                                    entries.insert(AutocompleteEntry(node.second["realdirectory"], node.second["name"]));
                                }
                            }
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
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory:
            {
                std::deque<std::string> history = *(g_Game->GetConsole())->GetCommandHistory();
                autocompleteBuffer.clear();

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