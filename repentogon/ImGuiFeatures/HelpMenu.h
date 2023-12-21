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
        ImVec4 colorWhite(1, 1, 1, 1);
        ImVec4 colorBrown(160 / 255.0f, 82 / 255.0f, 45 / 255.0f, 1);
        ImVec4 colorRed(1.0f, 75 / 255.0f, 75 / 255.0f, 1);
        ImVec4 colorGold(219 / 255.0f, 172 / 255.0f, 52 / 255.0f, 1);

        AddState(GameState(1, ICON_FA_GIFTS, "", ImVec4(1, 1, 1, 0.1f), 0));
        AddState(GameState(10, ICON_FA_GIFT, "A present!", colorWhite, 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(3, ICON_FA_COOKIE, "A delicious cookie. You take a bite.", colorBrown, 22)); // sfx: SOUND_CHEST_OPEN
        AddState(GameState(5, ICON_FA_COOKIE_BITE, "The cookie tastes great!", colorBrown, 128)); // sfx: SOUND_POWERUP1
        AddState(GameState(5, ICON_FA_EXPAND, "You have eaten the cookie.", colorWhite, 128)); // sfx: SOUND_POWERUP1

        AddState(GameState(5, ICON_FA_GIFTS, "There is another gift...", colorWhite, 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(10, ICON_FA_GIFT, "You took another present! It smells weird...", ImVec4(0, 0.5f, 1, 1), 18)); // sfx: SOUND_PAPER_OUT
        AddState(GameState(5, ICON_FA_POOP, "An ordinary piece of shit...\nDoesn't taste very nutricious...", colorBrown, 392)); // sfx: SOUND_RAGMAN_1

        AddState(GameState(10, ICON_FA_TOILET, "You flushed the turd down the toilet!", colorWhite, 396)); // sfx: SOUND_FLUSH
        AddState(GameState(15, ICON_FA_GIFT, "You look behind the toilet and find a fancy looking present.", colorGold, 49)); // sfx: SOUND_SCYTHE_BREAK
        AddState(GameState(25, ICON_FA_EGG, "An ancient looking egg. Feels kinda warm...", colorRed, 22)); // sfx: SOUND_CHEST_OPEN
        AddState(GameState(15, ICON_FA_DRAGON, "A dragon hatched from the egg!", colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(15, ICON_FA_DRAGON, "Your dragon looks hungry and neglected.", colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_DRAGON, "You try to feed the dragon...", colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_LEMON, "You gave the dragon a lemon.\nIt looks at you confused...", ImVec4(1, 1, 0, 1), 263)); // sfx: SOUND_WEIRD_WORM_SPIT
        AddState(GameState(10, ICON_FA_CARROT, "You feed the dragon a carrot.\nIt doesn't seem to like that...", ImVec4(1, 0.5f, 0, 1), 307)); // sfx: SOUND_GRROOWL
        AddState(GameState(10, ICON_FA_BACON, "You feed the dragon some bacon.\nIt eats it with delight!", ImVec4(1, 100 / 255.0f, 86 / 255.0f, 1), 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(25, ICON_FA_DRAGON, "Your dragon wants attention!", colorRed, 816)); // sfx: SOUND_BEAST_GRUMBLE
        AddState(GameState(10, ICON_FA_HEART, "You pet the dragon a lot and give him lots of love.", ImVec4(1, 0, 0, 1), 321)); // sfx: SOUND_HEARTBEAT
        AddState(GameState(10, ICON_FA_DRAGON, "Your dragon looks happy!", colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW


        AddState(GameState(10, ICON_FA_QUESTION, "You begin to wonder if the dragon is rideable.", colorRed, 408)); // sfx: SOUND_QUESTION_MARK
        AddState(GameState(15, ICON_FA_CLOUD, "You and your dragon take off to the sky!", colorWhite, 2)); //sfx: SOUND_BIRD_FLAP
        AddState(GameState(10, ICON_FA_DRAGON, "You feel as if you and your dragon companion are inseparable.", colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(10, ICON_FA_CROSS, "You keep ascending!\nYou have reached the heavens.", colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(5, ICON_FA_BOLT_LIGHTNING, "Unfortunately, God cares not for your antics.\nHe smites your dragon on the spot, as you descend into the unknown depths below.", colorGold, 273)); // sfx: SOUND_LASERRING_STRONG
        AddState(GameState(5, ICON_FA_SKULL, "You survive the fall, but your dragon dies on impact..", colorRed, 217)); // sfx: SOUND_ISAACDIES

        AddState(GameState(5, ICON_FA_FACE_ANGRY, "You will have your revenge.\nBut how?", colorRed, 654)); // sfx: SOUND_FAMINE_DASH_START
        AddState(GameState(10, ICON_FA_HAMMER, "We can rebuild him.", colorGold, 229)); // sfx: SoundEffect.SOUND_METAL_BLOCKBREAK
        AddState(GameState(10, ICON_FA_DUMBBELL, "We can make him stronger.", colorGold, 365)); // sfx: SoundEffect.SOUND_STRENGTH
        AddState(GameState(10, ICON_FA_GUN, "Your dragon has been reborn as a gun!", colorWhite, 482)); // sfx: SoundEffect.SOUND_BULLET_SHOT

        AddState(GameState(10, ICON_FA_QUESTION, "You begin to wonder if your legally distinct gun dragon is rideable.", colorRed, 408)); // sfx: SOUND_QUESTION_MARK
        AddState(GameState(15, ICON_FA_CLOUD, "You and your blatant reference take off to the sky!", colorWhite, 2)); //sfx: SOUND_BIRD_FLAP
        AddState(GameState(10, ICON_FA_DRAGON, "You feel as if you and your reminiscence are inseparable.", colorRed, 270)); // sfx: SOUND_HAPPY_RAINBOW
        AddState(GameState(10, ICON_FA_CROSS, "You keep ascending!", colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, "You keep ascending!", colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, "You keep ascending!", colorGold, 266)); // sfx: SOUND_SUPERHOLY
        AddState(GameState(10, ICON_FA_CROSS, "You keep ascending...", colorGold, 266)); // sfx: SOUND_SUPERHOLY

        AddState(GameState(15, ICON_FA_BOOK_SKULL, "You feel your sins crawling on your back.", colorRed, 417)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_LAUGH
        AddState(GameState(15, ICON_FA_POO_STORM, "Fool that you are, for there is no heaven.", colorRed, 418)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_SCREAM
        AddState(GameState(15, ICON_FA_DRAGON, "There is no dragon.", colorRed, 422)); // sfx: SoundEffect.SOUND_HUSH_GROWL

        AddState(GameState(15, ICON_FA_SQUARE_FULL, "There is nothing.", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(15, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT
        AddState(GameState(100, ICON_FA_SQUARE_FULL, "", colorRed, 569)); // sfx: SoundEffect.SOUND_MENU_FLIP_LIGHT

        AddState(GameState(15, ICON_FA_FAN, "Howdy! It's me, FLOWEY! FLOWEY the FLOWER!", colorWhite, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(15, ICON_FA_FAN, "I owe you a HUGE thanks! You really did a number on that old fool!\nWithout you, I NEVER could have gotten past him!", colorGold, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(10, ICON_FA_FAN, "But now, with YOUR help...", colorRed, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(5, ICON_FA_FAN, "He's DEAD.", colorRed, 217)); // sfx: SOUND_ISAACDIES
        AddState(GameState(5, ICON_FA_FAN, "And I'VE got the human SOULS!", colorGold, 418)); // sfx: SoundEffect.SOUND_THE_FORSAKEN_SCREAM
        AddState(GameState(10, ICON_FA_FAN, "Boy! I've been empty for so long...\nIt feels great to have a SOUL inside me again.", colorWhite, 426)); // sfx: SoundEffect.SOUND_MAW_OF_VOID
        AddState(GameState(5, ICON_FA_FAN, "Mmmm, I can feel them wriggling...", colorWhite, 3)); // sfx: SoundEffect.SOUND_BLOBBY_WIGGLE
        AddState(GameState(10, ICON_FA_FAN, "Awww, you're feeling left out, aren't you?", colorWhite, 9)); // sfx: SoundEffect.SOUND_BOSS_BUG_HISS
        AddState(GameState(15, ICON_FA_FAN, "Well, that's just perfect. After all, I only have six souls.\nI still need one more...", colorWhite, 44)); // sfx: SoundEffect.SOUND_FLOATY_BABY_ROAR
        AddState(GameState(5, ICON_FA_FAN, "Before I become GOD.", colorRed, 34)); // sfx: SoundEffect.SOUND_DEVIL_CARD
        AddState(GameState(5, ICON_FA_FAN, "And then, with my newfound powers...", colorGold, 569)); // sfx: SoundEffect.SOUND_BROWNIE_LAUGH
        AddState(GameState(3, ICON_FA_FAN, "Monsters...", colorBrown, 167)); // sfx: SoundEffect.SOUND_ZOMBIE_WALKER_KID
        AddState(GameState(3, ICON_FA_FAN, "Humans...", colorGold, 55)); // sfx: SoundEffect.SOUND_ISAAC_HURT_GRUNT
        AddState(GameState(3, ICON_FA_FAN, "Everyone...", colorRed, 52)); // sfx: SoundEffect.SOUND_HELLBOSS_GROUNDPOUND
        AddState(GameState(10, ICON_FA_FAN, "I'll show them all the REAL meaning of this world!", colorRed, 707)); // sfx: SoundEffect.SOUND_BEAST_GHOST_ROAR
        AddState(GameState(25, ICON_FA_GUN, "There may be no dragon, but there still is a gun.\nYou pull out your gun and shoot Flowey, killing him instantly.", colorWhite, 482)); // sfx: SoundEffect.SOUND_BULLET_SHOT
        AddState(GameState(10, ICON_FA_EXPLOSION, "The day is saved! That's the power of REPENTOGON.", colorGold, 184)); // SoundEffect.SOUND_EXPLOSION_STRONG
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

    string controlsPS[7][2] = {
        { "D-PAD", "Move, Tweak values (when input active)" },
        { "Touch Panel", "Mouse" },
        { "L Stick", "Scroll" },
        { "Square", "Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window" },
        { "Triangle", "Edit Text / on-screen keyboard" },
        { "Circle", "Cancel / Close / Exit" },
        { "Cross", "Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)" },
    };
    string controlsXB[6][2] = {
        { "D-PAD", "Move, Tweak values (when input active)" },
        { "L Stick", "Scroll" },
        { "X", "Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window" },
        { "Y", "Edit Text / on-screen keyboard" },
        { "B", "Cancel / Close / Exit" },
        { "A", "Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)" },
    };
    string controlsSwitch[6][2] = {
        { "D-PAD", "Move, Tweak values (when input active)" },
        { "L Stick", "Scroll" },
        { "Y", "Tap: Toggle Menu\nHold+L1/R1: Change Window focus\nHold+D-Pad: Resize Window\nHold+L-Stick: Move Window" },
        { "X", "Edit Text / on-screen keyboard" },
        { "B", "Cancel / Close / Exit" },
        { "A", "Activate / Open / Toggle\nTweak value with D-Pad\n(+L1/R1 to tweak slower/faster)" },
    };

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
        if (ImGui::BeginMenu(ICON_FA_CIRCLE_QUESTION " Help")) {
            if (ImGui::BeginMenu(ICON_FA_GAMEPAD " Controls")) {
                if (ImGui::MenuItem("Mouse")) {
                    ImGui::Text("Scale Text: Mousewheel + CTRL and hover over the text/window you want to scale");
                }
                if (ImGui::MenuItem("Keyboard")) { }
                if (ImGui::BeginMenu("Controller")) {
                    if (ImGui::BeginMenu("Playstation")) {
                        ImGui::Text("Controls for DualShock4 controllers");
                        if (ImGui::BeginTable("controls-ps", 2)) {
                            for each (string entry in controlsPS) {
                                ImGui::TableNextColumn();
                                ImGui::Text(entry.c_str());
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("XBox")) {
                        ImGui::Text("Controls for XBox controllers");
                        if (ImGui::BeginTable("controls-xb", 2)) {
                            for each (string entry in controlsXB) {
                                ImGui::TableNextColumn();
                                ImGui::Text(entry.c_str());
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Switch")) {
                        ImGui::Text("Controls for Joy-Con controllers");
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
            if (ImGui::BeginMenu(ICON_FA_BOOK " Documentation")) {
                ImGui::Text("The documentation can be found here: ");
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
