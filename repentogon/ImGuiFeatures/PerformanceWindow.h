#include "imgui.h"
#include "ImGuiEx.h"
#include <iostream>
#include <sstream>
#include <string>
#include "LuaCore.h"

extern int handleWindowFlags(int flags);

struct PerformanceWindow : ImGuiWindowObject {
	std::list<float>* memUsages = new std::list<float>();
	float timeframe = 10;
	float minMemUsage = FLT_MAX;
	float maxMemUsage = 0;
	char hintText[256];

	PerformanceWindow() : ImGuiWindowObject("Performance") {
	}

	void Draw(boolean isImGuiActive)
	{
		if (!enabled || !isImGuiActive && !pinned) {
			return;
		}
		ImGui::SetNextWindowSize(ImVec2(450, 200), ImGuiCond_FirstUseEver);

		if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
			if (imguiResized) {
				ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x * imguiSizeModifier.x, ImGui::GetWindowPos().y * imguiSizeModifier.y));
				ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x * imguiSizeModifier.x, ImGui::GetWindowSize().y * imguiSizeModifier.y));
			}
			AddWindowContextMenu();

			lua_State* state = g_LuaEngine->_state;
			ImGui::InputFloat("Timeframe", &timeframe, 1, 5, "%.1f Seconds");

			int curBytes = state->l_G->totalbytes + state->l_G->GCdebt;

			float megaBytes = curBytes / 1048576.0f;
			memUsages->push_back(megaBytes);
			while (memUsages->size() > timeframe * 60)
			{
				memUsages->pop_front();
			}
			minMemUsage = min(minMemUsage, megaBytes);
			maxMemUsage = max(maxMemUsage, megaBytes);

			std::vector<float> values(memUsages->begin(), memUsages->end());

			sprintf(hintText, "%.2f MB", megaBytes);

			ImGui::PlotLines("Mem Usage", &values[0], memUsages->size(), NULL, hintText, minMemUsage - 1, maxMemUsage + 2, ImVec2(0, 100));
		}
		ImGui::End(); // close window element

	}
};

extern PerformanceWindow performanceWindow;
