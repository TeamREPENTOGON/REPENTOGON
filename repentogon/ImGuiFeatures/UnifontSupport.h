#pragma once

#include "IsaacRepentance.h"
#include "imgui.h"

extern ImFont* imFontUnifont;

/*
Use UpdateFont to set the text font.
imFontUnifont font are used to render chars that can't randered by FontDefault. Just like what Minecraft does.
The reason why don't merge it into FontDefault is that unifont has a huge memory overhead if not scaled.
The unifont with GB2312 chars uses about 15MB memory for 26px size, and 160MB for 39px.

font-awesome icons will use unifont if possible. don't use UpdateFont if you want font-awesome fnt.
*/

inline void UpdateFont() {
	auto& io = ImGui::GetIO();
	if (GImGui->Font != io.FontDefault)
		ImGui::SetCurrentFont(io.FontDefault);
}

template<typename T>
inline T* UpdateFont(T* txt) {
	if (txt) {
		for (T* it = txt; *it; it++) {
			if (*it & ~0x7F) {
				// txt is non-ascii encoding, use unicode font
				if (GImGui->Font != imFontUnifont)
					ImGui::SetCurrentFont(imFontUnifont);
				return txt;
			}
		}
	}
	UpdateFont();
	return txt;
}


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