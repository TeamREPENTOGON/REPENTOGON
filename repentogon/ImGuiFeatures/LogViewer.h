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

    void InitAfterLanguageAvaliable()
    {
        windowName = LANG.LOGV_WIN_NAME;
    }

    void AddLogNoFormat(const char* type, const char* str)
    {
        const char* logFormat = "%.*s %.*s\n";
        std::string_view _type = type;
        size_t additionalSize = _type.size() + 2; // +2 for added ' ' and '\n'.
        constexpr size_t MAX_CONTENT_SIZE = LogViewerBufMaxSize - 1; // the ImGuiTextBuffer always null terminates the string, so we need to leave space for that.

        std::vector<std::string_view> lines;
        // split lines at \n (excluding the delimiter)
        {
            std::string_view remainingStr = str;
            size_t newlinePos;
            while ((newlinePos = remainingStr.find('\n')) != std::string_view::npos)
            {
                std::string_view line = remainingStr.substr(0, newlinePos);

                size_t lineTotalSize = additionalSize + line.size();
                if (lineTotalSize <= MAX_CONTENT_SIZE)
                {
                    lines.push_back(line);
                }

                remainingStr.remove_prefix(newlinePos + 1);
            }

            if (!remainingStr.empty() && remainingStr.size() + additionalSize <= MAX_CONTENT_SIZE)
            {
                lines.push_back(remainingStr);
            }
        }

        if (lines.empty())
        {
            return;
        }

        // get total memory occupied by all lines.
        // if the total memory occupied exceeds the buffer size, then we ignore the first lines that don't fit.
        size_t totalSize = 0;
        size_t firstLine = lines.size();
        for (size_t i = lines.size(); i-- > 0;)
        {
            size_t lineSize = lines[i].size() + additionalSize;
            assert(lineSize <= MAX_CONTENT_SIZE); // we should have already rejected lines that exceed the max size.
            if (totalSize + lineSize > MAX_CONTENT_SIZE)
            {
                break;
            }
            firstLine = i;
            totalSize += lineSize;
        }

        // check if we need to make space for the new logs
        size_t currentSize = this->logBuf.size();
        bool overflow = totalSize + currentSize > MAX_CONTENT_SIZE;
        if (overflow)
        {
            size_t reductionAmount = std::max(LogViewerReduceSize, totalSize);
            assert(reductionAmount >= currentSize + totalSize - MAX_CONTENT_SIZE); // reduction amount must always be greater than the overflow.

            std::string_view content = std::string_view(this->logBuf.Buf.Data, this->logBuf.size());
            
            // get how much data we need to keep
            size_t pos = 0;
            while (pos < content.size() && pos < reductionAmount)
            {
                size_t newlinePos = content.find('\n', pos);
                if (newlinePos == std::string_view::npos)
                {
                    break;
                }

                pos = newlinePos + 1;
            }

            if (pos < reductionAmount)
            {
                this->logBuf.clear();
            }
            else
            {
                std::string_view keptData = content.substr(pos);
                memmove(this->logBuf.Buf.Data, keptData.data(), keptData.size());
                this->logBuf.Buf.Data[keptData.size()] = '\0'; // zero-terminate the buffer
                this->logBuf.Buf.Size = keptData.size() + 1;
            }
        }

        size_t oldSize = this->logBuf.size();

        // place new logs into the buffer
        for (size_t i = firstLine; i < lines.size(); i++)
        {
            auto& line = lines[i];
            this->logBuf.appendf(logFormat, (int)_type.size(), _type.data(), (int)line.size(), line.data());
        }
        assert(this->logBuf.size() <= MAX_CONTENT_SIZE);

        // update imgui offsets
        {
            size_t pos;
            if (overflow)
            {
                // recompute all offsets
                this->offsets.clear();
                pos = 0;
            }
            else
            {
                pos = oldSize;
            }

            std::string_view content = std::string_view(this->logBuf.Buf.Data, this->logBuf.size());
            size_t newlinePos;
            while ((newlinePos = content.find('\n', pos)) != std::string_view::npos)
            {
                pos = newlinePos + 1;
                this->offsets.push_back(pos);
            }
        }
    }

    void AddLog(const char* type, const char* fmt, ...) IM_FMTARGS(2)
    {
        // First get the length of the formatted string.
        va_list args;
        va_start(args, fmt);
        size_t len = std::vsnprintf(NULL, 0, fmt, args);
        va_end(args);

        // Next, write the full formatted string into a buffer.
        auto buffer = std::make_unique<char[]>(len+1);
        va_start(args, fmt);
        std::vsnprintf(buffer.get(), len+1, fmt, args);
        va_end(args);

        // Log the now-formatted string.
        AddLogNoFormat(type, buffer.get());
    }

    void Draw(boolean isImGuiActive)
    {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(550, 300), ImGuiCond_FirstUseEver);

        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            AddWindowContextMenu();

            if (ImGui::BeginTable("LogViewer_Table", 2, ImGuiTableFlags_SizingStretchProp)) { // table layout makes it scale nicer
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                filter.Draw(LANG.LOGV_FILTER);

                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(ICON_FA_GEARS)) {
                    ImGui::OpenPopup("LogViewerPopup");
                }
                if (ImGui::BeginPopup("LogViewerPopup")) {
                    ImGui::Checkbox(LANG.LOGV_PIN_WINDOW, &pinned);
                    ImGui::SameLine();
                    HelpMarker(LANG.LOGV_PIN_WINDOW_HINT);
                    ImGui::Checkbox(LANG.LOGV_AUTOSCROLL, &autoscroll);

                    ImGui::SeparatorText(LANG.LOGV_SHOW_CATEGORIES);
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
