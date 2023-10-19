#pragma once
#include "Version.h"
#include <Log.h>
#include <Windows.h>

#include <chrono>
#include <iomanip>
#include <sstream>



static char titlebar[128];
static auto gameStartTime = std::chrono::high_resolution_clock::now();

namespace REPENTOGON {

	static void ChangeWindowTitle(const char* text) {
		sprintf(titlebar, "The Binding of Isaac: Repentance (+ REPENTOGON %s)%s", VERSION, text);
		SetWindowTextA(GetActiveWindow(), titlebar);
	}

	inline std::string FloatToStringPrecision(float number, int precision) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(precision) << number;
		return ss.str();
	}

	static void FinishProgressDisplay() {
		auto finish = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish - gameStartTime).count() / 1000.0f;
		ZHL::Log("STARTUP PROGRESS FINISHED: %s\n", std::to_string(diff).c_str());

		ChangeWindowTitle("");
	}

	static void UpdateProgressDisplay(const char* text) {
		auto finish = std::chrono::high_resolution_clock::now();
		auto diffTotal = std::chrono::duration_cast<std::chrono::milliseconds>(finish - gameStartTime).count() / 1000.0f;
		std::string formattedDiff(FloatToStringPrecision(diffTotal, 3));
		ZHL::Log("STARTUP PROGRESS: %s\t%s\n", text, formattedDiff.c_str());

		char newText[128];
		sprintf(newText, " - %s (cur startup time: %s s)", text, formattedDiff.c_str());
		ChangeWindowTitle(newText);
	}

}