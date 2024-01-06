#include "IconsFontAwesome6.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <string>

struct EasterEggGame {
    // cool dragon hatching game

    struct GameState {
        int clicksTillNext = 0;
        int startOfNeededClicks = 0;
        int soundEffectID = 0;
        string icon {};
        string description {};
        ImVec4 iconColor;

        GameState(int clicks, const char* iconStr, const char* desc, ImVec4 color, int sfxID)
        {
            clicksTillNext = clicks;
            icon = string(iconStr);
            description = string(desc);
            iconColor = color;
            soundEffectID = sfxID;
        }

        int GetNextClickTargetNum()
        {
            return startOfNeededClicks + clicksTillNext;
        }
    };

    int totalClicks = 0;
    std::vector<GameState> states;
    unsigned int currentGameState = 0;
    int sumClicksNeeded = 0;
    EasterEggGame()
    {
    }

    void InitAfterLanguageAvaliable()
    {

        ImVec4 colorWhite(1, 1, 1, 1);
        ImVec4 colorBrown(160 / 255.0f, 82 / 255.0f, 45 / 255.0f, 1);
        ImVec4 colorRed(1.0f, 75 / 255.0f, 75 / 255.0f, 1);
        ImVec4 colorGold(219 / 255.0f, 172 / 255.0f, 52 / 255.0f, 1);

        AddState(GameState(1, ICON_FA_GIFTS, "", ImVec4(1, 1, 1, 0.1f), 0));
        AddState(GameState(10, ICON_FA_GIFT, LANG.HELP_EGG_PRESENT, colorWhite, 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(3, ICON_FA_COOKIE, LANG.HELP_EGG_DELI_COOKIE, colorBrown, 22)); // sfx: SOUND_CHEST_OPEN
        AddState(GameState(5, ICON_FA_COOKIE_BITE, LANG.HELP_EGG_GREAT_COOKIE, colorBrown, 128)); // sfx: SOUND_POWERUP1
        AddState(GameState(5, ICON_FA_EXPAND, LANG.HELP_EGG_EATEN_COOKIE, colorWhite, 128)); // sfx: SOUND_POWERUP1

        AddState(GameState(5, ICON_FA_GIFTS, LANG.HELP_EGG_ANOTHER_GIFT, colorWhite, 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(10, ICON_FA_GIFT, LANG.HELP_EGG_WEIRED, ImVec4(0, 0.5f, 1, 1), 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(5, ICON_FA_POOP, LANG.HELP_EGG_PIECE_OF_SHIT, colorBrown, 392)); // sfx: SOUND_RAGMAN_1

        AddState(GameState(10, ICON_FA_TOILET, LANG.HELP_EGG_TOILET, colorWhite, 396)); // sfx: SOUND_FLUSH
        AddState(GameState(15, ICON_FA_GIFT, LANG.HELP_EGG_PRESENT_BEHIND_TOILET, colorGold, 49)); // sfx: SOUND_SCYTHE_BREAK
        AddState(GameState(25, ICON_FA_EGG, LANG.HELP_EGG_ANCIENT_EGG, colorRed, 22)); // sfx: SOUND_CHEST_OPEN
        AddState(GameState(15, ICON_FA_DRAGON, LANG.HELP_EGG_DRAGON_FROM_EGG, colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(15, ICON_FA_DRAGON, LANG.HELP_EGG_HUNGRY_DRAGON, colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_DRAGON, LANG.HELP_EGG_FEED_DRAGON, colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_LEMON, LANG.HELP_EGG_LEMON, ImVec4(1, 1, 0, 1), 263)); // sfx: SOUND_WEIRD_WORM_SPIT
        AddState(GameState(10, ICON_FA_CARROT, LANG.HELP_EGG_CARROT, ImVec4(1, 0.5f, 0, 1), 307)); // sfx: SOUND_GRROOWL
        AddState(GameState(10, ICON_FA_BACON, LANG.HELP_EGG_BACON, ImVec4(1, 100 / 255.0f, 86 / 255.0f, 1), 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(25, ICON_FA_DRAGON, LANG.HELP_EGG_ATTENTION, colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_HEART, LANG.HELP_EGG_PET_AND_LOVE, ImVec4(1, 0, 0, 1), 321)); // sfx: SOUND_HEARTBEAT
        AddState(GameState(10, ICON_FA_DRAGON, LANG.HELP_EGG_DRAGON_HAPPY, colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW


        AddState(GameState(10, ICON_FA_QUESTION, LANG.HELP_EGG_RIDEABLE, colorRed, 408)); // sfx: SOUND_QUESTION_MARK
        AddState(GameState(15, ICON_FA_CLOUD, LANG.HELP_EGG_TAKE_OFF, colorWhite, 2)); //sfx: SOUND_BIRD_FLAP
        AddState(GameState(10, ICON_FA_DRAGON, LANG.HELP_EGG_TAKE_OFF, colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(10, ICON_FA_CROSS, LANG.HELP_EGG_REACH_THE_HEAVEN, colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(5, ICON_FA_BOLT_LIGHTNING, LANG.HELP_EGG_GOD_SMITES_DRAGON, colorGold, 273)); // sfx: SOUND_LASERRING_STRONG
        AddState(GameState(5, ICON_FA_SKULL, LANG.HELP_EGG_YOU_SURVIVE, colorRed, 217)); // sfx: SOUND_ISAACDIES

        AddState(GameState(5, ICON_FA_FACE_ANGRY, LANG.HELP_EGG_REVENGE, colorRed, 654)); // sfx: SOUND_FAMINE_DASH_START
        AddState(GameState(10, ICON_FA_HAMMER, LANG.HELP_EGG_REBUILD, colorGold, 229)); // sfx: SoundEffect.SOUND_METAL_BLOCKBREAK
        AddState(GameState(10, ICON_FA_DUMBBELL, LANG.HELP_EGG_STRONGER, colorGold, 365)); // sfx: SoundEffect.SOUND_STRENGTH
        AddState(GameState(10, ICON_FA_GUN, LANG.HELP_EGG_REBORN_AS_GUN, colorWhite, 482)); // sfx: SoundEffect.SOUND_BULLET_SHOT

        AddState(GameState(10, ICON_FA_QUESTION, LANG.HELP_EGG_RIDEABLE_AGAIN, colorRed, 408)); // sfx: SOUND_QUESTION_MARK
        AddState(GameState(15, ICON_FA_CLOUD, LANG.HELP_EGG_TAKE_OFF_AGAIN, colorWhite, 2)); //sfx: SOUND_BIRD_FLAP
        AddState(GameState(10, ICON_FA_DRAGON, LANG.HELP_EGG_INSEPARABLE_AGAIN, colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(10, ICON_FA_CROSS, LANG.HELP_EGG_ASCENDING_1, colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, LANG.HELP_EGG_ASCENDING_2, colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, LANG.HELP_EGG_ASCENDING_3, colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, LANG.HELP_EGG_ASCENDING, colorGold, 266)); // sfx: SOUND_SUPERHOLY

        AddState(GameState(15, ICON_FA_BOOK_SKULL, LANG.HELP_EGG_SINS_CRAWLING, colorRed, 417)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_LAUGH
        AddState(GameState(15, ICON_FA_POO_STORM, LANG.HELP_EGG_NO_HEAVEN, colorRed, 418)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_SCREAM
        AddState(GameState(15, ICON_FA_DRAGON, LANG.HELP_EGG_NO_DRAGON, colorRed, 422)); // sfx: SoundEffect.SOUND_HUSH_GROWL

        AddState(GameState(15, ICON_FA_SQUARE_FULL, LANG.HELP_EGG_NOTHING, colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(100, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT

        AddState(GameState(15, ICON_FA_FAN, LANG.HELP_EGG_HOWDY, colorWhite, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(15, ICON_FA_FAN, LANG.HELP_EGG_I_OWE_YOU, colorGold, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(10, ICON_FA_FAN, LANG.HELP_EGG_WITH_YOUR_HELP, colorRed, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(5, ICON_FA_FAN, LANG.HELP_EGG_HE_DEAD, colorRed, 217)); // sfx: SOUND_ISAACDIES
        AddState(GameState(5, ICON_FA_FAN, LANG.HELP_EGG_GOT_HUMAN_SOUL, colorGold, 418)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_SCREAM
        AddState(GameState(10, ICON_FA_FAN, LANG.HELP_EGG_SOUL_INSIDE_ME_AGAIN, colorWhite, 426)); // sfx: SoundEffect.SOUND_MAW_OF_VOID
        AddState(GameState(5, ICON_FA_FAN, LANG.HELP_EGG_FELL_WRIGGLING, colorWhite, 3)); // sfx: SoundEffect.SOUND_BLOBBY_WIGGLE
        AddState(GameState(10, ICON_FA_FAN, LANG.HELP_EGG_FEELING_LEFT_OUT, colorWhite, 9)); // sfx: SoundEffect.SOUND_BOSS_BUG_HISS
        AddState(GameState(15, ICON_FA_FAN, LANG.HELP_EGG_NEED_ONE_MORE_SOUL, colorWhite, 44)); // sfx: SoundEffect.SOUND_FLOATY_BABY_ROAR
        AddState(GameState(5, ICON_FA_FAN, LANG.HELP_EGG_BEFORE_BECOME_GOD, colorRed, 34)); // sfx: SoundEffect.SOUND_DEVIL_CARD
        AddState(GameState(5, ICON_FA_FAN, LANG.HELP_EGG_POWER, colorGold, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(3, ICON_FA_FAN, LANG.HELP_EGG_MONSTER, colorBrown, 167)); // sfx: SoundEffect.SOUND_ZOMBIE_WALKER_KID
        AddState(GameState(3, ICON_FA_FAN, LANG.HELP_EGG_HUMAN, colorGold, 55)); // sfx: SoundEffect.SOUND_ISAAC_HURT_GRUNT
        AddState(GameState(3, ICON_FA_FAN, LANG.HELP_EGG_EVERYONE, colorRed, 52)); // sfx: SoundEffect.SOUND_HELLBOSS_GROUNDPOUND
        AddState(GameState(10, ICON_FA_FAN, LANG.HELP_EGG_SHOW_MEANING, colorRed, 707)); // sfx: SoundEffect.SOUND_BEAST_GHOST_ROAR
        AddState(GameState(25, ICON_FA_GUN, LANG.HELP_EGG_STILL_GUN, colorWhite, 482)); // sfx: SoundEffect.SOUND_BULLET_SHOT
        AddState(GameState(10, ICON_FA_EXPLOSION, LANG.HELP_EGG_POWER_OF_REPENTOGON, colorGold, 184)); // SoundEffect.SOUND_EXPLOSION_STRONG
    }

    void AddState(GameState state)
    {
        if (!states.empty())
            state.startOfNeededClicks = states.back().GetNextClickTargetNum();

        states.push_back(state);
    }

    void Draw()
    {
        ImGui::SameLine(0, 15);
        auto gameState = states[currentGameState];

        ImGui::PushStyleColor(ImGuiCol_Text, gameState.iconColor);
        ImGui::Text(gameState.icon.c_str());
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone) && !gameState.description.empty())
            ImGui::SetTooltip(gameState.description.c_str());
        if (ImGui::IsItemClicked()) {
            totalClicks++;

            if (totalClicks >= gameState.GetNextClickTargetNum() && currentGameState < states.size() - 1) {
                currentGameState++;

                int sfxID = states[currentGameState].soundEffectID;
                if (sfxID > 0)
                    g_Game->GetConsole()->RunCommand(string("playsfx ").append(to_string(sfxID)), NULL, NULL);
            }
        }
    }
};

struct HelpMenu {
    EasterEggGame easterEggGame;

    HelpMenu() { }

    static const int numTests = 1;
    string executeableTests[numTests][3] = {
        { "Lua->ImGui Interface tests", "Uses lua functions to add imgui elements with different behavior.", "luarun resources/scripts/repentogon_tests/test_imgui.lua" }
    };
    bool testsExecuted[numTests];

    std::vector<string> controlsPS;
    std::vector<string> controlsXB;
    std::vector<string> controlsSwitch;

    void InitAfterLanguageAvaliable()
    {
        easterEggGame.InitAfterLanguageAvaliable();

        string controlsPS[7][2] = {
            {LANG.HELP_CTRL_PS_DPAD, LANG.HELP_CTRL_PS_DPAD_DESC},
            {LANG.HELP_CTRL_PS_TOUCH, LANG.HELP_CTRL_PS_TOUCH_DESC},
            {LANG.HELP_CTRL_PS_LSTICK, LANG.HELP_CTRL_PS_LSTICK_DESC},
            {LANG.HELP_CTRL_PS_SQUARE, LANG.HELP_CTRL_PS_SQUARE_DESC},
            {LANG.HELP_CTRL_PS_TRIANGLE, LANG.HELP_CTRL_PS_TRIANGLE_DESC},
            {LANG.HELP_CTRL_PS_CIRCLE, LANG.HELP_CTRL_PS_CIRCLE_DESC},
            {LANG.HELP_CTRL_PS_CROSS, LANG.HELP_CTRL_PS_CROSS_DESC},
        };
        for each(string entry in controlsPS){
            this->controlsPS.push_back(entry);
        }

        string controlsXB[6][2] = {
            {LANG.HELP_CTRL_XB_DPAD, LANG.HELP_CTRL_XB_DPAD_DESC},
            {LANG.HELP_CTRL_XB_STICK, LANG.HELP_CTRL_XB_STICK_DESC},
            {LANG.HELP_CTRL_XB_X, LANG.HELP_CTRL_XB_X_DESC},
            {LANG.HELP_CTRL_XB_Y, LANG.HELP_CTRL_XB_Y_DESC},
            {LANG.HELP_CTRL_XB_B, LANG.HELP_CTRL_XB_B_DESC},
            {LANG.HELP_CTRL_XB_A, LANG.HELP_CTRL_XB_A_DESC},
        };
        for each(string entry in controlsXB){
            this->controlsXB.push_back(entry);
        }

        string controlsSwitch[6][2] = {
            {LANG.HELP_CTRL_SWITCH_DPAD, LANG.HELP_CTRL_SWITCH_DPAD_DESC},
            {LANG.HELP_CTRL_SWITCH_LSTICK, LANG.HELP_CTRL_SWITCH_LSTICK_DESC},
            {LANG.HELP_CTRL_SWITCH_Y, LANG.HELP_CTRL_SWITCH_Y_DESC},
            {LANG.HELP_CTRL_SWITCH_X, LANG.HELP_CTRL_SWITCH_X_DESC},
            {LANG.HELP_CTRL_SWITCH_B, LANG.HELP_CTRL_SWITCH_B_DESC},
            {LANG.HELP_CTRL_SWITCH_A, LANG.HELP_CTRL_SWITCH_A_DESC},
        };
        for each(string entry in controlsSwitch){
            this->controlsSwitch.push_back(entry);
        }
    }

    void OsOpenInShell(const char* path)
    {
#ifdef _WIN32
        // Note: executable path must use backslashes!
        ::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
        const char* open_executable = "open";
#else
        const char* open_executable = "xdg-open";
#endif
        char command[256];
        snprintf(command, 256, "%s \"%s\"", open_executable, path);
        system(command);
#endif
    }

    void AddHyperLink(const char* link)
    {
        ImGui::TextColored(ImVec4(0.25f, 0.25f, 1.0f, 1.0f), link);
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsItemClicked())
            OsOpenInShell(link);
    }

    void Draw()
    {
        if (ImGui::BeginMenu(LANG.HELP_MENU)) {
            if (ImGui::BeginMenu(LANG.HELP_MENU_CTRLS)) {
                if (ImGui::BeginMenu(LANG.HELP_MENU_CTRL_MOUSE)) {
                    ImGui::Text(LANG.HELP_MENU_CTRL_MOUSE_HINT);
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem(LANG.HELP_MENU_CTRL_KEYBOARD)) { }
                if (ImGui::BeginMenu(LANG.HELP_MENU_CTRL_CONTROLLER)) {
                    if (ImGui::BeginMenu(LANG.HELP_MENU_CTRL_PLAYSTATION)) {
                        ImGui::Text(LANG.HELP_MENU_CTRL_PLAYSTATION_TEXT);
                        if (ImGui::BeginTable("controls-ps", 2)) {
                            for each (string entry in controlsPS) {
                                ImGui::TableNextColumn();
                                ImGui::Text(entry.c_str());
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu(LANG.HELP_MENU_CTRL_XBOX)) {
                        ImGui::Text(LANG.HELP_MENU_CTRL_XBOX_TEXT);
                        if (ImGui::BeginTable("controls-xb", 2)) {
                            for each (string entry in controlsXB) {
                                ImGui::TableNextColumn();
                                ImGui::Text(entry.c_str());
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu(LANG.HELP_MENU_CTRL_SWITCH)) {
                        ImGui::Text(LANG.HELP_MENU_CTRL_SWITCH_TEXT);
                        if (ImGui::BeginTable("controls-xb", 2)) {
                            for each (string entry in controlsSwitch) {
                                ImGui::TableNextColumn();
                                ImGui::Text(entry.c_str());
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(LANG.HELP_MENU_DOC)) {
                ImGui::Text(LANG.HELP_MENU_DOC_TEXT);
                AddHyperLink("https://repentogon.com/docs.html");

                easterEggGame.Draw();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(ICON_FA_FLASK_VIAL " Tests")) {
                ImGui::Text("Some feature tests can be executed here");
                ImGui::Separator();
                for (size_t i = 0; i < numTests; i++) {
                    if (ImGui::MenuItem(string("Run: ").append(executeableTests[i][0]).c_str(), NULL, &testsExecuted[i])) {
                        std::string out;
                        // kinda hacky method to load arbitrary lua files
                        g_Game->GetConsole()->RunCommand(executeableTests[i][2], &out, NULL);
                    }
                    ImGui::SetItemTooltip(executeableTests[i][1].c_str());
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
    }
};

extern HelpMenu helpMenu;
