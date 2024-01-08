#pragma once
#include <imgui_internal.h>
#include "Lang.h"

struct CustomImGuiWindowSettings : ImGuiWindowSettings {
	std::string Name = "";
	bool Visible = false;
	bool Pinned = false;
	bool IsManagedManually = false;
	float FontWindowScale = 0.0f;

	CustomImGuiWindowSettings()
	{ }
};

struct ImGuiWindowObject {
	std::string windowName;
	bool enabled = false;
	bool pinned = false;

	ImGuiWindowObject(const char* name)
	{
		windowName = std::string(name);
	}

	void LoadSettings(CustomImGuiWindowSettings* settings) {
		if (settings == nullptr)
			return;
		enabled = settings->Visible;
		//pinned = settings->Pinned; // Currently crashes the game somehow :(
	}

	void SaveSettings(CustomImGuiWindowSettings* settings) {
		if (settings == nullptr)
			return;
		settings->Visible = enabled;
		settings->Pinned = pinned;
		settings->IsManagedManually = true;
	}

	void SetIsDirty() {
		ImGuiContext& g = *GImGui;
		g.SettingsDirtyTimer = g.IO.IniSavingRate;
	}

	void AddWindowContextMenu()
	{
		if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
		{
			if (ImGui::MenuItem(LANG.IMGUI_WIN_CTX_MENU_PIN_WINDOW, NULL, &pinned))
				SetIsDirty();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal) && ImGui::BeginTooltip()) {
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(LANG.IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::EndPopup();
		}
	}
};
