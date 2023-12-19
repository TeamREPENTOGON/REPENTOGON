#include "IconsFontAwesome6.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <string>

struct HelpMenu {
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

                // cool dragon
                ImGui::SameLine(0, 15);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.5f));
                ImGui::Text(ICON_FA_DRAGON);
                ImGui::PopStyleColor();
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
