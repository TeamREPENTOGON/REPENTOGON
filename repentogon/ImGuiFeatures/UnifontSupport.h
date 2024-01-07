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

enum UnifontRenderMode {
	UNIFONT_RENDER_NORMAL = 0,
	UNOFONT_RENDER_LARGE = 1,
	UNIFONT_RENDER_MEDIUM = 2,
	UNIFONT_RENDER_TINY = 3,
	UNIFONT_RENDER_TINY_LOW = 4
};
