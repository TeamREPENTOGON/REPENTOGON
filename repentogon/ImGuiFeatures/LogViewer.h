#pragma once

#include "IconsFontAwesome6.h"
#include "ImGuiEx.h"
#include "imgui.h"
#include "Log.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

extern int handleWindowFlags(int flags);
extern void HelpMarker(const char* desc);
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
extern bool imguiResized;
extern ImVec2 imguiSizeModifier;

static constexpr const size_t LogViewerBufMaxSize = 4 * 1024 * 1024;
static constexpr const size_t LogViewerReduceSize = size_t(LogViewerBufMaxSize * 0.25f);

struct LogViewer : ImGuiWindowObject {
    static constexpr const char* Game = "[GAME]";
    static constexpr const char* Repentogon = "[REPENTOGON]";
    static constexpr const char* Console = "[CONSOLE]";

    bool showGame;
    bool showRepentogon;
    bool showConsole;
    bool autoscroll;

    ImGuiTextBuffer logBuf;
    ImGuiTextFilter filter;
    ImGuiTextFilter internalFilter;
    // Start index of every line
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
        bool recomputeOffsets = false;

        // ZHL::Log("[DEBUG] logBuf size = %d, offsets size = %d\n", logBuf.size(), offsets.size());

        // Split the format string on newline, in order to add each line separately in the buffer
        while (std::getline(ss, token, '\n')) {
            int currentSize = logBuf.size();
            size_t textSize = strlen(type) + token.length() + 3 /* ' ' + '\n' + '\0' */;

            size_t cumulatedSize = (size_t)currentSize + textSize;
            /* Make space for the new string.
             * 
             * If adding more text would cause the threshold to be reached or exceeded, we remove 
             * content from the buffer in order to make space.
             */
            if (cumulatedSize >= LogViewerBufMaxSize) {
                recomputeOffsets = true;

                // If the new text is longer than our reduction amount, make room for the text directly
                size_t reductionAmount = std::max(LogViewerReduceSize, textSize);
                if (cumulatedSize - reductionAmount >= LogViewerBufMaxSize) {
                    reductionAmount = size_t(cumulatedSize * 0.75);
                }

                /* Do not cut lines in the middle, repeatedly split the buffer on '\n' to remove full lines
                 * This should handle backslashes nicely as imgui should have already formatted strings in 
                 * such a way that '\n' is treated as a character and '\\n' is treated as '\' followed by '\n'.
                 */
                char* content = logBuf.Buf.Data;
                size_t readSize = 0;
                char* delimiter = NULL;
                // Do not use strtok as it would replace newlines with '\0'
                while ((delimiter = strpbrk(content, "\n")) && readSize < reductionAmount) {
                    ptrdiff_t length = delimiter - content;
                    readSize += length;
                    content = delimiter + 1;
                }

                /* I suspect this arises if the '\0' is encountered too early. We need this patched
                 * so let's commit it for now.
                 */
                if (readSize < reductionAmount) {
                    ZHL::Log("[ERROR] Unable to read sufficient data in the log viewer while balancing memory usage, forcibly clearing history\n");
                    logBuf.clear();
                }
                else {
                    memmove(logBuf.Buf.Data, content, currentSize - readSize);
                    logBuf.Buf.Size -= (int)readSize;
                }
            }

            char* buffer = (char*)malloc(textSize);
            if (!buffer) {
                ZHL::Log("[ERROR] Failed to allocate %llu bytes to log string %s %s\n", textSize, type, token.c_str());
                continue;
            }

            sprintf(buffer, "%s %s\n", type, token.c_str());

            va_list args;
            va_start(args, fmt);
            logBuf.appendfv(buffer, args);
            va_end(args);

            free(buffer);
        }

        if (recomputeOffsets) {
            // ZHL::Log("[DEBUG] Recomputing offsets in imgui\n");
            offsets.clear();
            const char* start = logBuf.Buf.Data;
            const char* content = start;
            const char* delimiter = NULL;
            while (delimiter = strpbrk(content, "\n")) {
                offsets.push_back(delimiter - start + 1);
                content = delimiter + 1;
            }
        }
        else {
            for (int new_size = logBuf.size(); old_size < new_size; old_size++)
                if (logBuf[old_size] == '\n')
                    offsets.push_back(old_size + 1);
        }
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
