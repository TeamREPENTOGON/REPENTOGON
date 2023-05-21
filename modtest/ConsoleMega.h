#include "imgui.h"

struct ConsoleMega {
    bool enabled;

    ImVector<int> offsets;

    ConsoleMega()
    {
        enabled = true;
    }

    void Draw() {
        if (ImGui::Begin("Console", &enabled)) {
            std::deque<Console_HistoryEntry>* history = g_Game->GetConsole()->GetHistory();

            for (auto entry = history->rbegin(); entry != history->rend(); ++entry) {
                int colorMap = entry->GetColorMap();

                // The console stores colors like this, because we can't have nice things.
                // g_colorDouble is used for other things but it isn't really evident what those things are yet, so this will have to do.
                // Decomp shows it as 0 but it... clearly isn't, so whatever.
                float red = ((colorMap >> 0x10 & 0xFF) + g_colorDouble) / 255;
                float green = ((colorMap >> 8 & 0xFF) + g_colorDouble) / 255;
                float blue = ((colorMap & 0xFF) + g_colorDouble) / 255;

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(red, green, blue, 1));
                ImGui::TextWrapped(entry->_text.Get());
                ImGui::PopStyleColor();
            }

            ImGui::End();
        }
    }
};

extern ConsoleMega console;