#pragma once

#include "IsaacRepentance.h"
#include "imgui.h"
#include <string>
#include <vector>

extern ImFont* imFontUnifont;

enum ImGuiFontRenderStyle {
	ImGuiFontRenderStyle_PIXELATED,
	ImGuiFontRenderStyle_SMOOTH
};

struct PredefinedFont {
	const char * fontName;
	const char * fontPath;
	bool maybe_missing = false;
};

extern std::vector<PredefinedFont> predefinedFonts;