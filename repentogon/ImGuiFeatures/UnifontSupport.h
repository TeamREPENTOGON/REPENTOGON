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

		//add all GB2312 chars
		for (unsigned int high = 0xA1; high <= 0xF7; high++) {
			char bytes_line[0x201];
			wchar_t chars_line[0x201];
			int idx = 0;
			for (unsigned int low = 0xA1; low <= 0xFE; low++) {
				assert(idx + 1 < sizeof(bytes_line));
				bytes_line[idx++] = high;
				bytes_line[idx++] = low;
			}
			int n = ::MultiByteToWideChar(936, MB_PRECOMPOSED, bytes_line, idx, chars_line, sizeof(chars_line) / sizeof(*chars_line));
			if (n <= 0)
				continue;
			for (int i = 0; i < n; i++) {
				builder.AddChar(chars_line[i]);
			}
		}

		builder.BuildRanges(&ranges);
	}
	ImWchar* Get() {
		return ranges.Data;
	}
};