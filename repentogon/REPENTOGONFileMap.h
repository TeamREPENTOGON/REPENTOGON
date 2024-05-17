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
    void ProduceString(std::wstring* part_path, FileMapEntry* entry, std::wstring* outstring);
    void AddModEntry(const std::wstring_view& input, FolderType modfoldertype, size_t modid);
  //  void FindFiles(const std::filesystem::directory_entry& input, FolderType modfoldertype, size_t modid);
    void FindFiles(const std::filesystem::path& input, FolderType modfoldertype, size_t modid);
    extern std::vector<std::wstring> _stringByFType;
    extern std::unordered_map<size_t, FileMapEntry> _filemap;
    extern std::wstring _modsPath;
    extern bool map_init;
    void GenerateMap();
    FileMapEntry* GetEntry(size_t input);
};
