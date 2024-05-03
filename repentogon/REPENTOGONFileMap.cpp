#include "REPENTOGONFileMap.h"
#include <chrono>
#include "IsaacRepentance.h"
namespace fs = std::filesystem;
namespace REPENTOGONFileMap {
    std::unordered_map<size_t, FileMapEntry> _filemap;
    bool map_init = false;
    std::vector<std::string> _stringByFType = {
        "resources",
        "resources-dlc3",
        "",
    };
    std::string _modsPath="";

    fs::path moddir;
    std::string modname;

    //void AddModEntry(std::string& input, FolderType modfoldertype) {
    //    FileMapEntry entry;
    //    entry.FolderType = modfoldertype;
    //    entry.ModFolder = modname;
    //    _filemap[input] = entry;
    //};
    void ProduceString(std::string* part_path, FileMapEntry* entry, std::string* outstring) {
        const std::string& subdirname = (_stringByFType[entry->FolderType]);
        *outstring = *outstring + _modsPath + "/" + g_Manager->GetModManager()->_mods[entry->ModFolder]->_directory + "/" +subdirname +"/" + *part_path;
//        printf("resolved string %s\n",(*outstring).c_str());
    };
    FileMapEntry* GetEntry(size_t hash) {
        auto mapentry = _filemap.find(hash);
        if (mapentry != _filemap.end()) {
            return &(_filemap[hash]);
        };
        return nullptr;
    };
    void format_path(const char* input, std::string& out) {
        char buf = *input;
        do {
            switch (buf) {
            case '\\':
                out.push_back('/');
                break;
            default:
                out.push_back(buf);
            };
            input++;
            buf = *input;
        } while (buf != 0);
    };
    std::string pathbuf(260,0);
    void AddModEntry(const char* input, FolderType modfoldertype,size_t modid) {
        
        ModManager* mods = g_Manager->GetModManager();
        FileMapEntry entry;
        entry.FolderType = modfoldertype;
        entry.ModFolder = modid;
        pathbuf.resize(0);
        format_path(input,pathbuf);
        size_t hashentry = std::hash<std::string>{}(pathbuf);
        //if (strstr(pathbuf.c_str(), "shadow")) {
        //    printf("shadow!\n");
        //};
        _filemap[hashentry] = entry;
    };
 //   void FindFiles(const fs::directory_entry& input, FolderType modfoldertype, size_t modid) {
 //       for (const auto& file : fs::directory_iterator(input)) {
 //           if (file.is_directory()) {
 //               FindFiles(file, modfoldertype,modid);
 //           }
 //           else {
 ////               auto path = fs::proximate(file, moddir);
 //               const char* relpath = file.path().string().c_str() + moddir.string().size() + 1 ;   //account for "\\"
 //               AddModEntry(relpath, modfoldertype,modid);
 //           };
 //       };
 //   };


    void FindFiles(const fs::path& input,FolderType modfoldertype,size_t modid) {
        for (const auto& file : fs::directory_iterator(input)) {
            if (file.is_directory()) {
                FindFiles(file.path(), modfoldertype, modid);
            }
            else {
                const char* relpath = file.path().string().c_str() + moddir.string().size() + 1;   //account for "\\"
                AddModEntry(relpath, modfoldertype,modid);
                //                printf("path is %ws\n",file.path().wstring().c_str());
 //               auto path = fs::proximate(file, moddir);
 //               AddModEntry(path.string(), modfoldertype);
            };
        };
    };
    void GenerateMap() {
        //modman isn't initted yet
        if (map_init) {
            return;
        };
        map_init = true;
        auto basepath = fs::current_path() / "mods";
        _modsPath = basepath.string();
        auto start_time = std::chrono::high_resolution_clock::now();
        if (g_Manager && g_Manager->GetModManager()) {
            ModManager* modmngr = g_Manager->GetModManager();
            for (size_t i = 0; i < modmngr->_mods.size(); i++) {
                ModEntry* modentry = modmngr->_mods[i];
                if (modentry && modentry->_loaded) {
                    for (size_t modfoldertype = FolderType::RESOURCES; modfoldertype < FolderType::LAST; modfoldertype++) {
                        //                const std::string& subdirname = (_stringByFType[modfoldertype]);
                        //                auto = modpath / subdirname;
                        const std::string& subdirname = (_stringByFType[modfoldertype]);
                        moddir = basepath / (modentry->_directory) / subdirname;
            //            moddir = scanfolder;
                        if (fs::is_directory(moddir)) {
                            FindFiles(moddir, (FolderType)modfoldertype, i);
                        };
                    };
                };
            };
        };
 //       auto diriter = fs::directory_iterator(basepath);
//        for (const auto& dirEntry : fs::directory_iterator(basepath)) {
//            auto modpath = dirEntry.path();
//            modname = modpath.filename().string();
//            if (!fs::is_directory(modpath)) {
//                continue;
//            }
//            if (fs::exists(modpath / "disable.it")) {
////                printf("skipping this mod!\n");
//                continue;
//            };
//
//            for (size_t modfoldertype = FolderType::RESOURCES; modfoldertype <= FolderType::LAST; modfoldertype++) {
//                const std::string& subdirname = (_stringByFType[modfoldertype]);
//                auto scanfolder = modpath / subdirname;
//                moddir = scanfolder;
//
//                if (fs::exists(scanfolder) && fs::is_directory(scanfolder)) {
//                    FindFiles(scanfolder, (FolderType)modfoldertype);
//                };
//            };
//        };
        auto finish_time = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time).count() / 1000.0f;
        printf("done file map gen in %lf secs!\n", (double)diff);
    };
}