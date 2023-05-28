#include "imgui.h"
#include <iostream>
#include <string>
#include <sstream>

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

struct LogViewer {
    bool enabled;
    bool pinned;
    bool showGame;
    bool showRepentogon;
    bool showConsole;
    bool autoscroll;

    ImGuiTextBuffer logBuf;
    ImGuiTextFilter filter;
    ImGuiTextFilter internalFilter;
    ImVector<int> offsets;

    LogViewer()
    {
        enabled = false;
        pinned = false;
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

    void Draw() {
        if (ImGui::Begin("Log Viewer", &enabled))
        {
            ImGui::Checkbox("Pin Window", &pinned);
            ImGui::SameLine();
            HelpMarker("Pinning a window will keep it visible even after closing Dev Tools.");
            ImGui::SameLine();
            ImGui::Checkbox("Autoscroll", &autoscroll);
            ImGui::SameLine();
            filter.Draw("Filter", -100.0f);

            ImGui::Text("Show:");
            ImGui::SameLine();
            ImGui::Checkbox("Game", &showGame);
            ImGui::SameLine();
            ImGui::Checkbox("REPENTOGON", &showRepentogon);
            ImGui::SameLine();
            ImGui::Checkbox("Console", &showConsole);
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

            if (ImGui::BeginChild("LogViewScrollable", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
                const char* buf_begin = logBuf.begin();
                const char* buf_end = logBuf.end();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                if (internalFilter.IsActive() || filter.IsActive()) {
                    for (int line_no = 0; line_no < offsets.Size; line_no++)
                    {
                        const char* line_start = buf_begin + offsets[line_no];
                        const char* line_end = (line_no + 1 < offsets.Size) ? (buf_begin + offsets[line_no + 1] - 1) : buf_end;
                        if (!internalFilter.IsActive() || (internalFilter.IsActive() && internalFilter.PassFilter(line_start, line_end)))
                            if (!filter.IsActive() || (filter.IsActive() && filter.PassFilter(line_start, line_end)))
                                ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                else {
                    ImGuiListClipper clipper;
                    clipper.Begin(offsets.Size);
                    while (clipper.Step())
                    {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                        {
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

            ImGui::End();
        }
    }
};

extern LogViewer logViewer;