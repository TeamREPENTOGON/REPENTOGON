// #include <Windows.h>

#include <cstdio>
#include <regex>

// #include <cryptopp/hex.h>
// #include <cryptopp/sha.h>

#include "injector/window.h"

wxBEGIN_EVENT_TABLE(IsaacLauncher::MainFrame, wxFrame)
	EVT_COMBOBOX(IsaacLauncher::WINDOW_COMBOBOX_LEVEL, IsaacLauncher::MainFrame::OnLevelSelect)
	EVT_COMBOBOX(IsaacLauncher::WINDOW_COMBOBOX_LAUNCH_MODE, IsaacLauncher::MainFrame::OnLauchModeSelect)
	EVT_CHECKBOX(IsaacLauncher::WINDOW_CHECKBOX_REPENTOGON_CONSOLE, IsaacLauncher::MainFrame::OnOptionSelected)
	EVT_CHECKBOX(IsaacLauncher::WINDOW_CHECKBOX_REPENTOGON_UPDATES, IsaacLauncher::MainFrame::OnOptionSelected)
	EVT_CHECKBOX(IsaacLauncher::WINDOW_CHECKBOX_VANILLA_LUADEBUG, IsaacLauncher::MainFrame::OnOptionSelected)
	EVT_TEXT(IsaacLauncher::WINDOW_TEXT_VANILLA_LUAHEAPSIZE, IsaacLauncher::MainFrame::OnCharacterWritten)
	EVT_BUTTON(IsaacLauncher::WINDOW_BUTTON_LAUNCH_BUTTON, IsaacLauncher::MainFrame::Launch)
wxEND_EVENT_TABLE()

class LuaHeapSizeValidator : public wxValidator {
public:
	LuaHeapSizeValidator() { }
	LuaHeapSizeValidator(const LuaHeapSizeValidator& ) { }

	wxObject* Clone() const override {
		return new LuaHeapSizeValidator();
	}

private:
	void OnCharacterWritten(wxKeyEvent& event) {
		wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
		wxString original = ctrl->GetValue();
		
	}

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(LuaHeapSizeValidator, wxValidator)
	// EVT_CHAR(LuaHeapSizeValidator::OnCharacterWritten)
wxEND_EVENT_TABLE()

namespace IsaacLauncher {
	static std::tuple<wxFont, wxFont> MakeBoldFont(wxFrame* frame);
	static wxComboBox* CreateLevelsComboBox(wxFrame* frame);

	static const char* mandatoryFileNames[] = {
		"libzhl.dll",
		"zhlRepentogon.dll",
		"freetype.dll",
		NULL
	};

	Version const knownVersions[] = {
		{ "04469d0c3d3581936fcf85bea5f9f4f3a65b2ccf96b36310456c9626bac36dc6", "v1.7.9b.J835 (Steam)", true },
		{ NULL, NULL, false }
	};

	Version const* const validVersions[] = {
		NULL
	};

	Version const* MainFrame::GetVersion(const char* hash) {
		Version const* version = knownVersions;
		while (version->version) {
			if (!strcmp(hash, version->hash)) {
				return version;
			}

			++version;
		}

		return NULL;
	}

	bool MainFrame::FileExists(const char* name) {
		WIN32_FIND_DATAA data;
		memset(&data, 0, sizeof(data));
		HANDLE result = FindFirstFileA(name, &data);
		bool ret = result != INVALID_HANDLE_VALUE;
		if (ret) {
			FindClose(result);
		}

		return ret;
	}

	MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "REPENTOGON Launcher") {
		memset(&_options, 0, sizeof(_options));
		_grid = new wxGridBagSizer(0, 20);
		_updates = _console = nullptr;
		_luaHeapSize = nullptr;
		_enabledRepentogon = false;

		AddLaunchOptions();
		AddRepentogonOptions();
		AddVanillaOptions();

		wxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
		horizontalSizer->Add(_grid, 0, wxLEFT, 20);

		wxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
		wxTextCtrl* logWindow = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(-1, 200), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
		logWindow->SetBackgroundColour(*wxWHITE);
		_logWindow = logWindow;
		verticalSizer->Add(logWindow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 20);
		verticalSizer->Add(horizontalSizer);
		SetSizer(verticalSizer);

		SetBackgroundColour(wxColour(237, 237, 237));
	}

	void MainFrame::AddLaunchOptions() {
		auto [sourceFont, boldFont] = MakeBoldFont(this);
		boldFont.SetPointSize(14);
		SetFont(boldFont);
		wxStaticText* text = new wxStaticText(this, -1, "Launch mode");
		SetFont(sourceFont);

		wxSizer* box = new wxBoxSizer(wxHORIZONTAL);
		box->Add(new wxStaticText(this, -1, "Launch mode: "));

		wxComboBox* modes = new wxComboBox(this, WINDOW_COMBOBOX_LAUNCH_MODE);
		modes->Insert("Repentogon", 0, (void*)nullptr);
		modes->Insert("Vanilla", 0, (void*)nullptr);
		modes->SetValue("Repentogon");

		box->Add(modes);

		_grid->Add(text, wxGBPosition(0, 2), wxDefaultSpan, wxALIGN_CENTER);
		_grid->Add(box, wxGBPosition(1, 2));

		wxButton* launchButton = new wxButton(this, WINDOW_BUTTON_LAUNCH_BUTTON, "Launch game");
		_grid->Add(launchButton, wxGBPosition(2, 2), wxDefaultSpan, wxEXPAND);
	}

	void MainFrame::AddRepentogonOptions() {
		auto [sourceFont, boldFont] = MakeBoldFont(this);
		boldFont.SetPointSize(14);
		SetFont(boldFont);
		wxStaticText* text = new wxStaticText(this, -1, "REPENTOGON Options");
		SetFont(sourceFont);

		wxCheckBox* updates = new wxCheckBox(this, WINDOW_CHECKBOX_REPENTOGON_UPDATES, "Check for updates");
		updates->SetValue(true);
		wxCheckBox* console = new wxCheckBox(this, WINDOW_CHECKBOX_REPENTOGON_CONSOLE, "Enable console window");
		console->SetValue(false);

		_updates = updates;
		_console = console;

		_grid->Add(text, wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTER);
		_grid->Add(updates, wxGBPosition(1, 0));
		_grid->Add(console, wxGBPosition(2, 0));
	}

	void MainFrame::AddVanillaOptions() {
		auto [sourceFont, boldFont] = MakeBoldFont(this);
		boldFont.SetPointSize(14);
		SetFont(boldFont);
		wxStaticText* text = new wxStaticText(this, -1, "Universal Options");
		SetFont(sourceFont);

		wxSizer* levelSelectSizer = new wxBoxSizer(wxHORIZONTAL);
		levelSelectSizer->Add(new wxStaticText(this, -1, "Starting stage: "));
		wxComboBox* levelSelect = CreateLevelsComboBox(this);
		levelSelectSizer->Add(levelSelect);

		_grid->Add(text, wxGBPosition(0, 1), wxDefaultSpan, wxALIGN_CENTER);
		_grid->Add(levelSelectSizer, wxGBPosition(1, 1));
		_grid->Add(new wxCheckBox(this, WINDOW_CHECKBOX_VANILLA_LUADEBUG, "Enable luadebug (unsafe)"), wxGBPosition(2, 1));

		LuaHeapSizeValidator validator;
		wxSizer* heapSizeBox = new wxBoxSizer(wxHORIZONTAL);
		wxTextCtrl* heapSizeCtrl = new wxTextCtrl(this, WINDOW_TEXT_VANILLA_LUAHEAPSIZE, "1024M");
		heapSizeCtrl->SetValidator(validator);
		_luaHeapSize = heapSizeCtrl;
		wxStaticText* heapSizeText = new wxStaticText(this, -1, "Lua heap size: ");
		heapSizeBox->Add(heapSizeText);
		heapSizeBox->Add(heapSizeCtrl);
		_grid->Add(heapSizeBox, wxGBPosition(3, 1));
	}

	void MainFrame::OnLevelSelect(wxCommandEvent& event) {
		wxComboBox* box = dynamic_cast<wxComboBox*>(event.GetEventObject());
		wxString string = box->GetValue();
		std::cmatch match;
		if (std::regex_search(string.c_str().AsChar(), match, std::basic_regex("([0-9])\\.([0-9])"))) {
			int stage = std::stoi(match[1].str(), NULL, 0);
			int type = std::stoi(match[2].str(), NULL, 0);
			_options.level_stage = stage;
			_options.stage_type = type;
		}
	}

	void MainFrame::OnLauchModeSelect(wxCommandEvent& event) {
		wxComboBox* box = dynamic_cast<wxComboBox*>(event.GetEventObject());
		if (box->GetValue() == "Vanilla") {
			_updates->Disable();
			_console->Disable();
			_options.mode = LAUNCH_MODE_VANILLA;
		}
		else {
			_updates->Enable();
			_console->Enable();
			_options.mode = LAUNCH_MODE_REPENTOGON;
		}
	}

	void MainFrame::OnCharacterWritten(wxCommandEvent& event) {
		wxTextCtrl* object = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
		wxString content = object->GetValue();
	}

	void MainFrame::OnOptionSelected(wxCommandEvent& event) {
		wxCheckBox* box = dynamic_cast<wxCheckBox*>(event.GetEventObject());
		switch (box->GetId()) {
		case WINDOW_CHECKBOX_REPENTOGON_CONSOLE:
			_options.console = box->GetValue();
			break;

		case WINDOW_CHECKBOX_REPENTOGON_UPDATES:
			_options.updates = box->GetValue();
			break;

		case WINDOW_CHECKBOX_VANILLA_LUADEBUG:
			_options.lua_debug = box->GetValue();
			break;

		default:
			return;
		}
	}

	void MainFrame::Launch(wxCommandEvent& event) {

	}

	void MainFrame::PostInit() {
		Log("Welcome to the REPENTOGON Launcher version %s", IsaacLauncher::version);
		if (!CheckIsaacVersion()) {
			return;
		}

		if (!CheckRepentogonInstallation()) {
			return;
		}

		if (!CheckRepentogonVersion()) {
			return;
		}
	}

	bool MainFrame::CheckIsaacVersion() {
		Log("Checking Isaac version...");
		WIN32_FIND_DATAA isaac;
		memset(&isaac, 0, sizeof(isaac));
		HANDLE isaacFile = FindFirstFileA("isaac-ng.exe", &isaac);
		if (isaacFile == INVALID_HANDLE_VALUE) {
			LogError("Unable to find isaac-ng.exe");
			return false;
		}
		FindClose(isaacFile);

		DWORD size = isaac.nFileSizeLow;
		char* content = (char*)malloc(size + 2);
		if (!content) {
			LogError("Unable to allocate memory to read game's executable, aborting");
			return false;
		}

		FILE* exe = fopen(isaac.cFileName, "rb");
		fread(content, 1, size, exe);
		content[size] = '\0';
		/* std::string sha256Str;
		CryptoPP::SHA256 sha256;
		CryptoPP::StringSource(content, true,
			new CryptoPP::HashFilter(sha256,
				new CryptoPP::HexEncoder(
					new CryptoPP::StringSink(sha256Str)
				)
			)
		); */
		BCRYPT_ALG_HANDLE alg;
		NTSTATUS err = BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, NULL, 0);
		DWORD buffSize;
		DWORD dummy;
		err = BCryptGetProperty(alg, BCRYPT_OBJECT_LENGTH, (unsigned char*)&buffSize, sizeof(buffSize), &dummy, 0);
		BCRYPT_HASH_HANDLE hashHandle;
		unsigned char* hashBuffer = (unsigned char*)malloc(buffSize);
		err = BCryptCreateHash(alg, &hashHandle, hashBuffer, buffSize, NULL, 0, 0);
		err = BCryptHashData(hashHandle, (unsigned char*)content, size, 0);
		DWORD hashSize;
		err = BCryptGetProperty(alg, BCRYPT_HASH_LENGTH, (unsigned char*)&hashSize, sizeof(hashSize), &dummy, 0);
		unsigned char* hash = (unsigned char*)malloc(hashSize);
		char* hashHex = (char*)malloc(hashSize * 2 + 1);
		err = BCryptFinishHash(hashHandle, hash, hashSize, 0);
		free(hashBuffer);
		err = BCryptCloseAlgorithmProvider(&alg, 0);

		for (int i = 0; i < hashSize; ++i) {
			sprintf(hashHex + 2 * i, "%02hhx", hash[i]);
		}

		// const char* sha256p = sha256Str.c_str();
		Log("\tFound isaac-ng.exe. Hash: %s", hashHex);
		Version const* version = GetVersion(hashHex);
		if (!version) {
			LogError("Unknown version. REPENTOGON will not launch.");
			return false;
		}
		else if (!version->valid) {
			LogError("This version of the game does not support REPENTOGON.");
			return false;
		}

		free(hash);

		Log("\tIdentified REPENTOGON compatible version %s", version->version);

		return true;
	}

	bool MainFrame::CheckRepentogonInstallation() {
		Log("Checking Repentogon installation...");
		const char** file = mandatoryFileNames;
		bool ok = true;
		while (*file) {
			if (FileExists(*file)) {
				Log("\t%s: found", *file);
			}
			else {
				ok = false;
				LogError("\t%s: not found", *file);
			}

			++file;
		}

		return ok;
	}

	bool MainFrame::CheckRepentogonVersion() {
		Log("Checking Repentogon version...");
		HMODULE repentogon = LoadLibraryExA("zhlRepentogon.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (!repentogon) {
			LogError("Unable to open zhlRepentogon.dll");
			return false;
		}

		HMODULE zhl = LoadLibraryExA("libzhl.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (!zhl) {
			FreeLibrary(repentogon);
			LogError("Unable to open libzhl.dll");
			return false;
		}

		bool result = false;
		FARPROC repentogonVersion = GetProcAddress(repentogon, "__REPENTOGON_VERSION");
		FARPROC zhlVersion = GetProcAddress(zhl, "__ZHL_VERSION");

		if (!repentogonVersion) {
			LogError("Unable to get Repentogon's version (%d)", GetLastError());
			result = false;
			goto end;
		}

		if (!zhlVersion) {
			LogError("Unable to get ZHL's version");
			result = false;
			goto end;
		}

		const char* repentogonVersionStr = *(const char**)repentogonVersion;
		const char* zhlVersionStr = *(const char**)zhlVersion;

		Log("\tFound Repentogon version %s", repentogonVersionStr);
		Log("\tFound ZHL version %s", zhlVersionStr);

		if (strcmp(repentogonVersionStr, zhlVersionStr)) {
			LogError("Repentogon/ZHL version mismatch");
			result = false;
			goto end;
		}

		Log("\tRepentogon and ZHL versions match");
		result = true;

	end:
		FreeLibrary(repentogon);
		FreeLibrary(zhl);
		return result;
	}

	void MainFrame::Log(const char* fmt, ...) {
		char buffer[4096];
		va_list va;
		va_start(va, fmt);
		int count = vsnprintf(buffer, 4096, fmt, va);
		va_end(va);

		if (count == 0)
			return;

		wxString text(buffer);
		if (buffer[count] != '\n')
			text += '\n';
		_logWindow->AppendText(text);
	}

	void MainFrame::LogError(const char* fmt, ...) {
		char buffer[4096];
		va_list va;
		va_start(va, fmt);
		int count = vsnprintf(buffer, 4096, fmt, va);
		va_end(va);

		if (count == 0)
			return;

		wxString text(buffer);
		text.Prepend("[ERROR] ");
		if (buffer[count] != '\n')
			text += '\n';
		wxTextAttr attr = _logWindow->GetDefaultStyle();
		_logWindow->SetDefaultStyle(wxTextAttr(*wxRED));
		_logWindow->AppendText(text);
		_logWindow->SetDefaultStyle(attr);
	}

	std::tuple<wxFont, wxFont> MakeBoldFont(wxFrame* frame) {
		wxFont source = frame->GetFont();
		wxFont bold = source;
		bold.MakeBold();
		return std::make_tuple(source, bold);
	}

	wxComboBox* CreateLevelsComboBox(wxFrame* frame) {
		wxComboBox* box = new wxComboBox(frame, WINDOW_COMBOBOX_LEVEL, "Start level");
		const char* names[] = {
			"Basement",
			"Cellar",
			"Burning Basement",
			"Downpour",
			"Dross",
			"Caves",
			"Catacombs",
			"Flooded Caves",
			"Mines",
			"Ashpit",
			"Depths",
			"Necropolis",
			"Dank Depths", 
			"Mausoleum",
			"Gehenna",
			"Womb",
			"Utero",
			"Scarred Womb",
			"Corpse",
			NULL
		};

		const char* uniqueNames[] = {
			"??? (9.0)",
			"Sheol (10.0)",
			"Cathedral (10.1)",
			"Dark Room (11.0)",
			"Chest (11.1)",
			"The Void (12.0)",
			"Home (13.0)",
			NULL
		};

		int pos = 0;
		box->Insert(wxString("--"), pos++);
		box->SetValue("--");
		int variant = 0;
		for (const char* name : names) {
			if (!name)
				continue;

			wxString s;
			int level = 1 + 2 * (variant / 5);
			box->Insert(s.Format("%s I (%d.%d)", name, level, variant % 5), pos++, (void*)nullptr);
			box->Insert(s.Format("%s II (%d.%d)", name, level + 1, variant % 5), pos++, (void*)nullptr);

			++variant;
		}

		for (const char* name : uniqueNames) {
			if (!name)
				continue;
			box->Insert(wxString(name), pos++, (void*)nullptr);
		}

		return box;
	}
}