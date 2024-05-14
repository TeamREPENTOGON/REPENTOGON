#include "REPENTOGONFileMap.h"
#include <chrono>
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <Log.h>
#include "REPENTOGONOptions.h"

#include <locale>
#include <sstream>
namespace fs = std::filesystem;
namespace REPENTOGONFileMap {
	std::unordered_map<size_t, FileMapEntry> _filemap;
	bool map_init = false;
	std::vector<std::wstring> _stringByFType = {
		L"resources",
		L"resources-dlc3",
		L"",
	};
	std::wstring _modsPath = L"";
	std::wstring _producePath = L"";

	fs::path moddir;
	std::string modname;
	std::locale utf8loc = std::locale(".UTF-8");

	void ProduceString(std::wstring* part_path, FileMapEntry* entry, std::wstring* outstring) {
		std::string& moddir = g_Manager->GetModManager()->_mods[entry->ModFolder]->_directory;
		int wstr_size = MultiByteToWideChar(CP_UTF8, 0, moddir.data(), moddir.size(), _producePath.data(), 0);

		_producePath.resize(wstr_size);
		MultiByteToWideChar(CP_UTF8, 0, moddir.data(), moddir.size(), _producePath.data(), wstr_size);
		const std::wstring& subdirname = (_stringByFType[entry->FolderType]);
		*outstring = *outstring + _modsPath + L"/" + _producePath + L"/" + subdirname + L"/" + *part_path;
	};
	FileMapEntry* GetEntry(size_t input) {
		auto mapentry = _filemap.find(input);
		if (mapentry != _filemap.end()) {
			return &(_filemap[input]);
		};
		return nullptr;
	};
	void format_path(const std::wstring_view& input, std::wstring& out) {
		for (size_t i = 0; i < input.size(); i++) {
			wchar_t buf = input[i];
			switch (buf) {
			case L'\\':
				out.push_back(L'/');
				break;
			default:
				out.push_back(buf);
			};
		};
	};
	std::wstring wpathbuf(260, 0);
	std::wstring_view relpath;
	std::wstring tokenholder;
	std::wstring outputholder;
	std::wistringstream normalize_stringstream;
	std::vector<size_t> elemstoskip;
	bool NormalizePathString(std::wstring& input) {
		normalize_stringstream = std::wistringstream(input);
		wpathbuf.resize(0);
		elemstoskip.resize(0);
		size_t i = 0;
		size_t maxi = 0;
		while (std::getline(normalize_stringstream, tokenholder,L'/')) {
			if (tokenholder == L"..") {
				if (i == 0) {
					return false;
				};
				elemstoskip.push_back(i - 1);
				elemstoskip.push_back(i);
			}
			else if (tokenholder == L".") {
				elemstoskip.push_back(i);
			};
			i++;
			maxi++;
		};
		normalize_stringstream.clear();
		normalize_stringstream.seekg(0, std::ios::beg);
		i = 0;
		while (std::getline(normalize_stringstream, tokenholder, L'/')) {
			if ( std::find(elemstoskip.begin(), elemstoskip.end(), i)!=elemstoskip.end() ) {
				i++;
				continue;
			};
			wpathbuf += tokenholder;
			i++;
			if (i != maxi) {
				wpathbuf += L'/';
			};
		};
		input = wpathbuf;
		return true;
	};
	void AddModEntry(const std::wstring_view& input, FolderType modfoldertype, size_t modid) {
		FileMapEntry entry;

		ModManager* mods = g_Manager->GetModManager();
		entry.FolderType = modfoldertype;
		entry.ModFolder = modid;
		wpathbuf.resize(0);
		format_path(input, wpathbuf);
//		NormalizePathString(pathbuf);
		std::transform(wpathbuf.begin(), wpathbuf.end(), wpathbuf.begin(),
			[](wchar_t c) { return std::tolower(c,utf8loc); });
		size_t hashentry = std::hash<std::wstring>{}(wpathbuf);
		_filemap[hashentry] = entry;
	};


	void FindFiles(const fs::path& input, FolderType modfoldertype, size_t modid) {
		for (const auto& file : fs::recursive_directory_iterator(input, fs::directory_options::skip_permission_denied)) {
			if (!file.is_directory()) {
				//                const char* relpath = file.path().string().c_str() + moddir.string().size() + 1;   //account for "\\"
				relpath = std::wstring_view{ file.path().wstring() }.substr(moddir.wstring().size() + 1);
				AddModEntry(relpath, modfoldertype, modid);
			};
		};
	};
	void GenerateMap() {
		//kill off for now
		if (map_init || !repentogonOptions.fileMap) {
			return;
		};
		map_init = true;
		auto basepath = fs::current_path() / L"mods";
		_modsPath = basepath.wstring();
		auto start_time = std::chrono::high_resolution_clock::now();
		if (g_Manager && g_Manager->GetModManager()) {
			ModManager* modmngr = g_Manager->GetModManager();
			//            for (size_t i = 0; i < modmngr->_mods.size(); i++) {
			for (int i = modmngr->_mods.size() - 1; i >= 0;i--) {   //inverse order
				ModEntry* modentry = modmngr->_mods[i];
				if (modentry && modentry->_loaded) {
					for (size_t modfoldertype = FolderType::RESOURCES; modfoldertype < FolderType::LAST; modfoldertype++) {
						const std::wstring& subdirname = (_stringByFType[modfoldertype]);
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

std::wstring tempwidestr;

HOOK_METHOD(ModManager, TryRedirectPath, (std_string* param_1, std_string* param_2)->void) {
//	bool spoofmods = false;
//	return super(param_1, param_2);
	//kill off map for now
	if (param_1 == nullptr || param_2 == nullptr || !repentogonOptions.fileMap) {
		return super(param_1, param_2);
	};
	std::string input = *param_2;
//	tempwidestr.resize(input.size() * 4);
	int wstr_size=MultiByteToWideChar(CP_UTF8,0,input.data(),input.size(),tempwidestr.data(),0 );
	tempwidestr.resize(wstr_size);
	MultiByteToWideChar(CP_UTF8, 0, input.data(), input.size(), tempwidestr.data(), wstr_size);
	for (wchar_t& c : tempwidestr) {
		if (c == L'\\') {
			c = L'/';
		};
	};
	bool success=REPENTOGONFileMap::NormalizePathString(tempwidestr);
	if (!success) {
		return super(param_1,param_2);
	};
	std::transform(tempwidestr.begin(), tempwidestr.end(), tempwidestr.begin(),
		[](wchar_t c) { return std::tolower(c, REPENTOGONFileMap::utf8loc); });
	size_t hashentry = std::hash<std::wstring>{}(tempwidestr);
	REPENTOGONFileMap::FileMapEntry* mapentry = REPENTOGONFileMap::GetEntry(hashentry);
	if (mapentry) {
		new (param_1) std::string("");
		REPENTOGONFileMap::outputholder.resize(0);
//		REPENTOGONFileMap::outputholder.reserve(260);
//		param_1->reserve(260);
		ProduceString(&tempwidestr, mapentry, &(REPENTOGONFileMap::outputholder));
		int out_str_size=WideCharToMultiByte(CP_ACP,0,REPENTOGONFileMap::outputholder.data(),REPENTOGONFileMap::outputholder.size(),param_1->data(),0,0,0);
		param_1->resize(out_str_size);
		WideCharToMultiByte(CP_ACP, 0, REPENTOGONFileMap::outputholder.data(), REPENTOGONFileMap::outputholder.size(), param_1->data(), out_str_size, 0, 0);

		if (!fs::exists(REPENTOGONFileMap::outputholder)) {
			ZHL::Log("[REPENTOGON] File %s doesn't exist in a mod %s, hash mismatch?\n",param_2->c_str(), g_Manager->GetModManager()->_mods[mapentry->ModFolder]->_name.c_str());
			return super(param_1, param_2);
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