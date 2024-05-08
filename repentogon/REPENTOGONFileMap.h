#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace REPENTOGONFileMap {
    enum FolderType {
        RESOURCES,
        RESOURCES_DLC3,
        LAST
    };
    struct FileMapEntry {
        int ModFolder;
        FolderType FolderType;
    };
    void ProduceString(std::string* part_path, FileMapEntry* entry, std::string* outstring);
    void AddModEntry(const char* input, FolderType modfoldertype,size_t modid);
  //  void FindFiles(const std::filesystem::directory_entry& input, FolderType modfoldertype, size_t modid);
    void FindFiles(const std::filesystem::path& input, FolderType modfoldertype, size_t modid);
    extern std::vector<std::string> _stringByFType;
    extern std::unordered_map<size_t, FileMapEntry> _filemap;
    extern std::string _modsPath;
    void GenerateMap();
    FileMapEntry* GetEntry(size_t input);
};
