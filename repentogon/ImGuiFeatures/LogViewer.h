#pragma once
#include "IconsFontAwesome6.h"
#include "ImGuiEx.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <string>

extern int handleWindowFlags(int flags);
extern void HelpMarker(const char* desc);
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
extern bool imguiResized;
extern ImVec2 imguiSizeModifier;

struct LogViewer : ImGuiWindowObject {
    bool showGame;
    bool showRepentogon;
    bool showConsole;
    bool autoscroll;

    ImGuiTextBuffer logBuf;
    ImGuiTextFilter filter;
    ImGuiTextFilter internalFilter;
    ImVector<int> offsets;

    LogViewer()
        : ImGuiWindowObject("Log Viewer")
    {
        showGame = true;
        showRepentogon = true;
        showConsole = true;
        autoscroll = true;
    }

    void AddLog(const char* type, const char* fmt, ...) IM_FMTARGS(2)
    {
        std::stringstream ss(fmt);
        std::string token;
        int old_size = logBuf.size();
        while (std::getline(ss, token, '\n')) {
            int size = std::snprintf(nullptr, 0, "%s %s\n", type, token.c_str());
            std::string res(size + 1, '\0');
            std::sprintf(&res[0], "%s %s\n", type, token.c_str());

            va_list args;
            va_start(args, fmt);
            logBuf.appendfv(res.c_str(), args);
            va_end(args);
        }
        for (int new_size = logBuf.size(); old_size < new_size; old_size++)
            if (logBuf[old_size] == '\n')
                offsets.push_back(old_size + 1);
    }

    void Draw(boolean isImGuiActive)
    {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(550, 300), ImGuiCond_FirstUseEver);

        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            if (imguiResized) {
                ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x * imguiSizeModifier.x, ImGui::GetWindowPos().y * imguiSizeModifier.y));
                ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x * imguiSizeModifier.x, ImGui::GetWindowSize().y * imguiSizeModifier.y));
            }
            AddWindowContextMenu();

            if (ImGui::BeginTable("LogViewer_Table", 2, ImGuiTableFlags_SizingStretchProp)) { // table layout makes it scale nicer
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                filter.Draw("Filter");

                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(ICON_FA_GEARS)) {
                    ImGui::OpenPopup("LogViewerPopup");
                }
                if (ImGui::BeginPopup("LogViewerPopup")) {
                    ImGui::Checkbox("Pin Window", &pinned);
                    ImGui::SameLine();
                    HelpMarker("Pinning a window will keep it visible even after closing Dev Tools.");
                    ImGui::Checkbox("Autoscroll", &autoscroll);

                    ImGui::SeparatorText("Show categories:");
                    ImGui::Checkbox("Game", &showGame);
                    ImGui::Checkbox("REPENTOGON", &showRepentogon);
                    ImGui::Checkbox("Console", &showConsole);

                    ImGui::EndPopup();
                }
                ImGui::EndTable();
            }
            ImGui::Separator();

            std::vector<const char*> internalFilterStrings;

            if (!showGame) {
                internalFilterStrings.push_back("-[GAME]");
            }

            if (!showConsole) {
                internalFilterStrings.push_back("-[CONSOLE]");
            }

            if (!showRepentogon) {
                internalFilterStrings.push_back("-[REPENTOGON]");
            }

            std::stringstream internalFilterRes;
            std::string delim = ",";

            std::copy(internalFilterStrings.begin(), internalFilterStrings.end(),
                std::ostream_iterator<std::string>(internalFilterRes, delim.c_str()));

            internalFilter = internalFilterRes.str().c_str();

            if (ImGui::BeginChild("LogViewScrollable", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
                const char* buf_begin = logBuf.begin();
                const char* buf_end = logBuf.end();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                if (internalFilter.IsActive() || filter.IsActive()) {
                    for (int line_no = 0; line_no < offsets.Size; line_no++) {
                        const char* line_start = buf_begin + offsets[line_no];
                        const char* line_end = (line_no + 1 < offsets.Size) ? (buf_begin + offsets[line_no + 1] - 1) : buf_end;
                        if (!internalFilter.IsActive() || (internalFilter.IsActive() && internalFilter.PassFilter(line_start, line_end)))
                            if (!filter.IsActive() || (filter.IsActive() && filter.PassFilter(line_start, line_end)))
                                ImGui::TextUnformatted(line_start, line_end);
                    }
                } else {
                    ImGuiListClipper clipper;
                    clipper.Begin(offsets.Size);
                    while (clipper.Step()) {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                            const char* line_start = buf_begin + offsets[line_no];
                            const char* line_end = (line_no + 1 < offsets.Size) ? (buf_begin + offsets[line_no + 1] - 1) : buf_end;
                            ImGui::TextUnformatted(line_start, line_end);
                        }
                    }
                    clipper.End();
                }

                if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::PopStyleVar();
                ImGui::EndChild();
            }
        }
        ImGui::End(); // close window element
    }
};

extern LogViewer logViewer;
