#pragma once
#include "Version.h"
#include <Log.h>
#include <IsaacRepentance.h>
#include <Windows.h>

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace REPENTOGON {
	static auto gameStartTime = std::chrono::high_resolution_clock::now();
	static std::string optionsPath(std::string((char*)&g_SaveDataPath) + "Repentogon/");
	extern char stocktitle[256];
	extern char moddedtitle[256];

	static void ChangeWindowTitle(const char* text) {
		sprintf(stocktitle, "The Binding of Isaac: Repentance+ %s (REPENTOGON %s)%s", &g_GameVersionString, VERSION, text);
		SetWindowTextA(GetActiveWindow(), stocktitle);
	}

	static void SetStockWindowTitle() {
		SetWindowTextA(GetActiveWindow(), stocktitle);
	};

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

	static std::string GetRGONGfxAbsolutePath(const char* relpath) {
		char buffer[MAX_PATH];
		DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);
		std::string base = std::string(buffer);
		return base + "/resources-repentogon/" + std::string(relpath);
	}

	static const char* GetRepentogonDataPath() {
		if (CreateDirectory(optionsPath.c_str(), NULL)) {
			ZHL::Log("Newly created REPENTOGON savedata folder @ %s\n", optionsPath.c_str());
			std::string optionsPathMinus = optionsPath;
			optionsPathMinus.erase(optionsPathMinus.find_last_of('+'), 1);  // REP- savedata path is the same, just without the '+'
			ZHL::Log("Checking for legacy REPENTOGON savedata @ %s\n", optionsPathMinus.c_str());
			if (std::filesystem::is_directory(optionsPathMinus)) {
				ZHL::Log("Legacy REPENTOGON savedata directory found. Copying contents to new savedata folder...\n");
				std::filesystem::copy(optionsPathMinus, optionsPath, std::filesystem::copy_options::recursive);
				ZHL::Log("...Done!\n");
			} else {
				ZHL::Log("No legacy REPENTOGON savedata directory found.\n");
			}
		} else if (GetLastError() != ERROR_ALREADY_EXISTS) {
			ZHL::Log("Error %s creating Repentogon Save directory: %s\n", GetLastError(), optionsPath.c_str());
			return "";
		}
		return optionsPath.c_str();
	}

	enum GameStateSlot
	{
		NULL_SLOT = -1,
		SAVE_FILE = 0,
		GLOWING_HOURGLASS_1 = 1,
		GLOWING_HOURGLASS_2 = 2,
	};

	GameStateSlot GetGameStateSlot(GameState* state);
}
