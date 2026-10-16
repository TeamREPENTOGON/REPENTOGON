#include "_Internals.h"
#include <fstream>
#include <regex>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../MiscFunctions.h"
#include "../Patches/ExtraRenderSteps.h"
#include "../SaveStateManagement/EntitySaveStateManagement.h"

namespace ErrorDisplay = ExtraRenderSteps::ErrorDisplay;
#define MOD_MANAGER_ERROR_HEADER "[ERROR] [ModManager] - "

static std::string_view check_string_view(lua_State* L, int idx)
{
	size_t len;
	const char* string = luaL_checklstring(L, idx, &len);
	return std::string_view(string, len);
}

static ModReference& check_mod_reference(lua_State* L, int idx)
{
	if (!lua_istable(L, idx))
	{
		luaL_argerror(L, idx, "Expected a table.");
	}

	ModReference* modRef = g_LuaEngine->GetModRefByTable(idx);
	if (!modRef)
	{
		luaL_argerror(L, idx, "Invalid ModReference.");
	}

	return *modRef;
}

static std::filesystem::path GetModDataPath(const ModReference& mod, std::string_view filename)
{
	return std::filesystem::path(mod._dataDirectory) / (filename);
}

LUA_FUNCTION(Lua_RaiseModError)
{
    const char* modName = luaL_checkstring(L, 1);
    std::string message = std::string(modName) + " is causing errors!";

    ErrorDisplay::RaiseError(message, ErrorDisplay::Priority::LUA_ERROR);

    return 0;
}

LUA_FUNCTION(Lua_GetModId)
{
	const int tblIdx = 1;
	if (!lua_istable(L, tblIdx))
	{
		return luaL_argerror(L, tblIdx, "Expected a table.");
	}

	ModReference* modRef = g_LuaEngine->GetModRefByTable(tblIdx);
	if (!modRef)
	{
		lua_pushnil(L);
		return 1;
	}

	lua_pushstring(L, modRef->_mainLuaPath.c_str());
	return 1;
}

static bool save_mod_file(ModReference& mod, std::string_view fileName, std::string_view data)
{
	std::filesystem::path path = GetModDataPath(mod, fileName);
	bool exists = REPENTOGON::EnsureParentPathExists(path);
	if (!exists)
	{
		ZHL::Log(MOD_MANAGER_ERROR_HEADER "unable to create directory for file at \"%s\"\n", path.string().c_str());
		return false;
	}

	std::ofstream file = std::ofstream(path, std::ios::binary);
	if (!file)
	{
		ZHL::Log(MOD_MANAGER_ERROR_HEADER "unable to open file for writing at \"%s\"\n", path.string().c_str());
		return false;
	}

	file.write(data.data(), data.size());
	return true;
}

static std::optional<std::string> load_mod_file(ModReference& mod, std::string_view fileName)
{
	std::filesystem::path path = GetModDataPath(mod, fileName);
	std::error_code errorCode;
	if (!std::filesystem::exists(path, errorCode))
	{
		return std::nullopt;
	}

	std::ifstream file = std::ifstream(path, std::ios::binary);
	if (!file)
	{
		ZHL::Log(MOD_MANAGER_ERROR_HEADER "unable to open file for reading at \"%s\"\n", path.string().c_str());
		return std::nullopt;
	}

	file.seekg(0, std::ios::end);
	std::streamoff lenoff = file.tellg();

	if (lenoff < 0)
	{
		ZHL::Log(MOD_MANAGER_ERROR_HEADER "unable to get file length at \"%s\"\n", path.string().c_str());
		return std::nullopt;
	}

	size_t len = static_cast<size_t>(lenoff);
	file.seekg(0);

	std::string data(len, '\0');
	file.read(data.data(), len);

	return data;
}

static void remove_mod_file(const std::filesystem::path& path)
{
	std::error_code errorCode;
	if (!std::filesystem::exists(path, errorCode))
	{
		return;
	}

	std::filesystem::remove(path, errorCode);
	if (errorCode)
	{
		ZHL::Log(MOD_MANAGER_ERROR_HEADER "unable to delete file at \"%s\"\n", path.string().c_str());
	}
}

static bool lua_check_valid_file_name(const std::string_view name, lua_State* L, int argIdx)
{
    static const std::regex validFilename(
        R"re([^\x00-\x1F"*/:<>?\\|]+)re"
    );

    if (!std::regex_match(name.begin(), name.end(), validFilename))
    {
        luaL_argerror(L, argIdx, "invalid filename");
        return false;
    }

    if (std::filesystem::path(name).extension() != ".dat") // this also covers "." and ".."
    {
        luaL_argerror(L, argIdx, "filename must be a .dat file");
        return false;
    }
    
    return true;
}

template<bool isLuaAPI = false>
static int HasModData(lua_State* L)
{
    ModReference& modReference = check_mod_reference(L, 1);
    std::string_view fileName = check_string_view(L, 2);

    if constexpr (isLuaAPI)
    {
        lua_check_valid_file_name(fileName, L, 2);
    }

    std::filesystem::path path = GetModDataPath(modReference, fileName);
    std::error_code errorCode;
    bool result = std::filesystem::exists(path, errorCode);

    lua_pushboolean(L, result);
    return 1;
}

template<bool isLuaAPI = false>
static int SaveModData(lua_State* L)
{
    ModReference& modReference = check_mod_reference(L, 1);
    std::string_view fileName = check_string_view(L, 2);
    std::string_view data = check_string_view(L, 3);

    if constexpr (isLuaAPI)
    {
        lua_check_valid_file_name(fileName, L, 2);
    }

    bool success = save_mod_file(modReference, fileName, data);
    lua_pushboolean(L, success);
    return 1;
}

template<bool isLuaAPI = false>
static int LoadModData(lua_State* L)
{
	ModReference& modReference = check_mod_reference(L, 1);
	std::string_view fileName = check_string_view(L, 2);

    if constexpr (isLuaAPI)
    {
        lua_check_valid_file_name(fileName, L, 2);
    }

	std::optional<std::string> result = load_mod_file(modReference, fileName);
	if (result.has_value())
	{
		const std::string& data = *result;
		lua_pushlstring(L, data.data(), data.size());
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

template<bool isLuaAPI = false>
static int RemoveModData(lua_State* L)
{
	ModReference& modReference = check_mod_reference(L, 1);
	std::string_view fileName = check_string_view(L, 2);

    if constexpr (isLuaAPI)
    {
        lua_check_valid_file_name(fileName, L, 2);
    }

	std::filesystem::path path = GetModDataPath(modReference, fileName);
	remove_mod_file(path);
	return 0;
}

LUA_FUNCTION(Lua_HasModData)
{
    return HasModData<false>(L);
}

LUA_FUNCTION(Lua_SaveModData)
{
	return SaveModData<false>(L);
}

LUA_FUNCTION(Lua_LoadModData)
{
	return LoadModData<false>(L);
}

LUA_FUNCTION(Lua_RemoveModData)
{
	return RemoveModData<false>(L);
}

LUA_FUNCTION(Lua_RemoveAllData)
{
	namespace fs = std::filesystem;

	std::string_view fileName = check_string_view(L, 1);
	std::filesystem::path rootFolder = &g_ModSaveDataPath;

	std::error_code ec;
	fs::directory_iterator it(rootFolder, fs::directory_options::skip_permission_denied, ec);
	const fs::directory_iterator end;

	while (it != end)
	{
		ec.clear();
		if (it->is_directory(ec) && !ec)
		{
			const fs::path& path = it->path() / fileName;
			remove_mod_file(path);
		}
		
		ec.clear();
		it.increment(ec);
	}

	return 0;
}

LUA_FUNCTION(ModReference_HasModData)
{
    return HasModData<true>(L);
}

LUA_FUNCTION(ModReference_SaveModData)
{
    return SaveModData<true>(L);
}

LUA_FUNCTION(ModReference_LoadModData)
{
    return LoadModData<true>(L);
}

LUA_FUNCTION(ModReference_RemoveModData)
{
    return RemoveModData<true>(L);
}

void LuaInternals::RegisterInternals(lua_State *L)
{
    lua::TableAssoc(L, "RaiseModError", Lua_RaiseModError);
	lua::TableAssoc(L, "GetModId", Lua_GetModId);
	lua::TableAssoc(L, "HasModData", Lua_HasModData);
	lua::TableAssoc(L, "SaveModData", Lua_SaveModData);
	lua::TableAssoc(L, "LoadModData", Lua_LoadModData);
	lua::TableAssoc(L, "RemoveModData", Lua_RemoveModData);
	lua::TableAssoc(L, "RemoveAllData", Lua_RemoveAllData);
    lua::TableAssoc(L, "ModReference_HasModFile", ModReference_HasModData);
    lua::TableAssoc(L, "ModReference_SaveModFile", ModReference_SaveModData);
    lua::TableAssoc(L, "ModReference_LoadModFile", ModReference_LoadModData);
    lua::TableAssoc(L, "ModReference_RemoveModFile", ModReference_RemoveModData);

	lua_newtable(L);
	{
		lua::LuaStackProtector protector(L);
		EntitySaveStateManagement::detail::Init::RegisterLuaInternals(L, -1);
	}
	lua_setfield(L, -2, "ESSM");
}

bool s_raiseInitError = false;

// have to delay it since globals haven't been initialized yet
void LuaInternals::RaiseInitError()
{
	s_raiseInitError = true;
}

void LuaInternals::detail::RaiseInitError()
{
	if (!s_raiseInitError)
	{
		return;
	}

	ErrorDisplay::RaiseError("REPENTOGON failed to initialize!", ErrorDisplay::Priority::REPENTOGON_CRITICAL);
	
	const char* consoleString = "REPENTOGON's Lua Internals failed to initialize!\n"
		"Some core systems may not behave as expected.\n"
		"Please report this issue to the REPENTOGON developers.";

	g_Game->GetConsole()->PrintError(consoleString);
}
