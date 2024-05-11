#include "REPENTOGONFileMap.h"
#include <chrono>
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <Log.h>
namespace fs = std::filesystem;
namespace REPENTOGONFileMap {
	std::unordered_map<size_t, FileMapEntry> _filemap;
	bool map_init = false;
	std::vector<std::string> _stringByFType = {
		"resources",
		"resources-dlc3",
		"",
	};
	std::string _modsPath = "";

	fs::path moddir;
	std::string modname;

	void ProduceString(std::string* part_path, FileMapEntry* entry, std::string* outstring) {
		const std::string& subdirname = (_stringByFType[entry->FolderType]);
		*outstring = *outstring + _modsPath + "/" + g_Manager->GetModManager()->_mods[entry->ModFolder]->_directory + "/" + subdirname + "/" + *part_path;
	};
	FileMapEntry* GetEntry(size_t input) {
		auto mapentry = _filemap.find(input);
		if (mapentry != _filemap.end()) {
			return &(_filemap[input]);
		};
		return nullptr;
	};
	void format_path(const std::string_view& input, std::string& out) {
		for (size_t i = 0; i < input.size(); i++) {
			char buf = input[i];
			switch (buf) {
			case '\\':
				out.push_back('/');
				break;
			default:
				out.push_back(buf);
			};
		};
	};
	std::string pathbuf(260, 0);
	std::string_view relpath;
	void AddModEntry(const std::string_view& input, FolderType modfoldertype, size_t modid) {
		FileMapEntry entry;

		ModManager* mods = g_Manager->GetModManager();
		entry.FolderType = modfoldertype;
		entry.ModFolder = modid;
		pathbuf.resize(0);
		format_path(input, pathbuf);
		std::transform(pathbuf.begin(), pathbuf.end(), pathbuf.begin(),
			[](unsigned char c) { return std::tolower(c); });
		size_t hashentry = std::hash<std::string>{}(pathbuf);
		_filemap[hashentry] = entry;
	};

	void FindFiles(const fs::path& input, FolderType modfoldertype, size_t modid) {
		for (const auto& file : fs::recursive_directory_iterator(input)) {
			if (!file.is_directory()) {
				//                const char* relpath = file.path().string().c_str() + moddir.string().size() + 1;   //account for "\\"
				relpath = std::string_view{ file.path().u8string() }.substr(moddir.u8string().size() + 1);
				AddModEntry(relpath, modfoldertype, modid);
			};
		};
	};
	void GenerateMap() {
		if (map_init) {
			return;
		};
		map_init = true;
		auto basepath = fs::current_path() / "mods";
		_modsPath = basepath.string();
		auto start_time = std::chrono::high_resolution_clock::now();
		if (g_Manager && g_Manager->GetModManager()) {
			ModManager* modmngr = g_Manager->GetModManager();
			//            for (size_t i = 0; i < modmngr->_mods.size(); i++) {
			for (int i = modmngr->_mods.size() - 1; i >= 0;i--) {   //inverse order
				ModEntry* modentry = modmngr->_mods[i];
				if (modentry && modentry->_loaded) {
					for (size_t modfoldertype = FolderType::RESOURCES; modfoldertype < FolderType::LAST; modfoldertype++) {
						const std::string& subdirname = (_stringByFType[modfoldertype]);
						moddir = basepath / (modentry->_directory) / subdirname;
						if (fs::is_directory(moddir)) {
							FindFiles(moddir, (FolderType)modfoldertype, i);
						};
					};
				};
			};
		};
		auto finish_time = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time).count() / 1000.0f;
		ZHL::Log("[REPENTOGON] Generated file map in %lf seconds\n", diff);
	};
}

//HOOK_METHOD(KAGE_Filesys_FileManager, GetExpandedPath, (char* path)->char*) {
//	char* out = super(path);
//	//	printf("[FileMan::GetExpandedPath] Input is %s\nOut is   %s\n",path,out);
//	return out;
//};
//HOOK_METHOD(ModManager, LoadConfigs, (void)->void) {
//	super();
//	return;
//};
HOOK_METHOD(ModManager, ListMods, (void)->void) {
	super();
	REPENTOGONFileMap::GenerateMap();
};


HOOK_METHOD(ModManager, TryRedirectPath, (std_string* param_1, std_string* param_2)->void) {
//	bool spoofmods = false;
	if (param_1 == nullptr || param_2 == nullptr) {
		return super(param_1, param_2);
	};
	std::transform(param_2->begin(), param_2->end(), param_2->begin(),
		[](unsigned char c) { return std::tolower(c); });
	for (char& c : *(std::string*)param_2) {
		if (c == '\\') {
			c = '/';
		};
	};
	std::string input = *param_2;
	size_t hashentry = std::hash<std::string>{}(input);
	REPENTOGONFileMap::FileMapEntry* mapentry = REPENTOGONFileMap::GetEntry(hashentry);
	if (mapentry) {
		new (param_1) std::string("");
		param_1->reserve(260);
		ProduceString(param_2, mapentry, param_1);
		if (!fs::exists(*(std::string*)param_1)) {
			ZHL::Log("[REPENTOGON] File %s doesn't exist in a mod %s, hash mismatch?\n",param_2->c_str(), g_Manager->GetModManager()->_mods[mapentry->ModFolder]->_name.c_str());
			super(param_1, param_2);
		};
		return;
	}
	else {
		std::vector<ModEntry*> backup = std::move(this->_mods);
		this->_mods.clear();
		super(param_1, param_2);
		this->_mods = std::move(backup);
	};
};