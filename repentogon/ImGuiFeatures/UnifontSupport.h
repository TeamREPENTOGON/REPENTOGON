#pragma once

#include "IsaacRepentance.h"
#include "imgui.h"

extern ImFont* imFontUnifont;

class UnifontRange {
	ImVector<ImWchar> ranges;

public:
	UnifontRange() {
		ImFontGlyphRangesBuilder builder;

		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesKorean());
		builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

		builder.BuildRanges(&ranges);
	}
	ImWchar* Get() {
		return ranges.Data;
	}
};