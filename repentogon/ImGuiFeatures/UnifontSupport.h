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

		//french chars
		ImWchar frenchRanges[] = {
			0x20, 0x5F,
			0x61, 0x7A,
			0xB2, 0xB3,
			0xC6, 0xCB,
			0xCE, 0xCF,
			0xDB, 0xDC,
			0xE6, 0xEB,
			0xEE, 0xEF,
			0xFB, 0xFC,
			0x152, 0x153,
			0x1D48, 0x1D49,
			0x2010, 0x2011,
			0x2013, 0x2014,
			0x201C, 0x201D,
			0x2020, 0x2021,
			0x202F, 0x2030
		};
		builder.AddRanges(frenchRanges);
		builder.AddText(u8"\x007C\x00A0\x00A7\x00A9\x00AB\x00BB\x00C0\x00C2\x00D4\x00D9\x00E0\x00E2\x00F4\x00F9\x00FF\x0178\x02B3\x02E2\x2019\x2026\x20AC\x2212");

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
