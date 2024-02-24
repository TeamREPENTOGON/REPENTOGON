#pragma once

#include <wx/wx.h>
#include <wx/gbsizer.h>

enum LaunchMode {
	LAUNCH_MODE_VANILLA,
	LAUNCH_MODE_REPENTOGON
};

struct IsaacOptions {
	LaunchMode mode;

	// Repentogon options
	bool updates;
	bool console;

	// Game options
	bool lua_debug;
	int level_stage;
	int stage_type;
	const char* lua_heap_size;
};

namespace IsaacLauncher {
	static constexpr const char* version = "alpha";

	struct Version {
		const char* hash;
		const char* version;
		bool valid;
	};

	extern Version const knownVersions[];

	enum Windows {
		WINDOW_COMBOBOX_LEVEL,
		WINDOW_COMBOBOX_LAUNCH_MODE,
		WINDOW_CHECKBOX_REPENTOGON_CONSOLE,
		WINDOW_CHECKBOX_REPENTOGON_UPDATES,
		WINDOW_CHECKBOX_VANILLA_LUADEBUG,
		WINDOW_TEXT_VANILLA_LUAHEAPSIZE,
		WINDOW_BUTTON_LAUNCH_BUTTON
	};

	class App : public wxApp {
	public:
		bool OnInit() override;
	};

	class MainFrame : public wxFrame {
	public:
		MainFrame();

		void Log(const char* fmt, ...);
		void LogError(const char* fmt, ...);
		void PostInit();

		static Version const* GetVersion(const char* hash);
		static bool FileExists(const char* name);

	private:
		/* Window building. */
		void AddLaunchOptions();
		void AddRepentogonOptions();
		void AddVanillaOptions();

		/* Event handlers. */
		void OnLevelSelect(wxCommandEvent& event);
		void OnLauchModeSelect(wxCommandEvent& event);
		void OnCharacterWritten(wxCommandEvent& event);
		void OnOptionSelected(wxCommandEvent& event);
		void Launch(wxCommandEvent& event);

		/* Post init stuff. */
		bool CheckIsaacVersion();
		bool CheckRepentogonInstallation();
		bool CheckRepentogonVersion();

		IsaacOptions _options;
		wxGridBagSizer* _grid;
		wxCheckBox* _updates;
		wxCheckBox* _console;
		wxTextCtrl* _luaHeapSize;
		wxTextCtrl* _logWindow;

		bool _enabledRepentogon;

		wxDECLARE_EVENT_TABLE();
	};
}