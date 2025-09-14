#include <filesystem>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ini.h"
#include "Log.h"

/*
* Allows REPENTOGON to use a separate file from the vanilla `options.ini`, and keep the options synced between it and the vanilla options file.
* If we are behind versions, there may be options that only exist on our version OR on the latest version, so switching between rgon and vanilla
* can cause options to get forgotten and reset to their default values if we use the same file. This logic prevents such issues.
* 
* The settings in the vanilla options file are always prioritized on startup, and modifications made to it with or without repentogon or even
* editing the file manually are all still respected, so this process of using a separate file is largely invisible.
*/

std::string GetVanillaOptionsIniPath() {
	return std::string(&g_SaveDataPath) + "options.ini";
}

std::string GetRgonVanillaOptionsIniPath() {
	return std::string(&g_SaveDataPath) + "repentogon/vanilla_options.ini";
}

// For all fields that are present in both src and dst, copies the values from src to dst.
// Non-shared fields are not changed.
void CopySharedOptions(const mINI::INIStructure& src, mINI::INIStructure& dst) {
	for (const auto& [category, options] : src) {
		if (dst.has(category)) {
			for (const auto& [field, value] : options) {
				if (dst[category].has(field)) {
					dst[category][field] = value;
				}
			}
		}
	}
}

HOOK_METHOD(OptionsConfig, Load, (const char* defaultLoadPath) -> void) {
	const std::string vanillaPath = GetVanillaOptionsIniPath();
	const std::string rgonPath = GetRgonVanillaOptionsIniPath();

	if (!std::filesystem::exists(rgonPath)) {
		if (defaultLoadPath && std::filesystem::exists(defaultLoadPath)) {
			ZHL::Log("[OptionsSyncing::Load] Initializing vanilla options from `%s`...\n", defaultLoadPath);
			if (!std::filesystem::copy_file(defaultLoadPath, rgonPath)) {
				ZHL::Log("[OptionsSyncing::Load] ERROR: Failed to copy defaultLoadPath `%s` to rgonPath `%s`\n", defaultLoadPath, rgonPath.c_str());
				return;
			}
			ZHL::Log("[OptionsSyncing::Load] Initialized `%s`", rgonPath.c_str());
		} else if (std::filesystem::exists(vanillaPath)) {
			ZHL::Log("[OptionsSyncing::Load] Initializing vanilla options from `%s`...\n", vanillaPath.c_str());
			if (!std::filesystem::copy_file(vanillaPath, rgonPath)) {
				ZHL::Log("[OptionsSyncing::Load] ERROR: Failed to copy vanillaPath `%s` to rgonPath `%s`\n", vanillaPath.c_str(), rgonPath.c_str());
				return;
			}
			ZHL::Log("[OptionsSyncing::Load] Initialized `%s`\n", rgonPath.c_str());
		}
	}

	if (std::filesystem::exists(vanillaPath) && std::filesystem::exists(rgonPath)) {
		ZHL::Log("[OptionsSyncing::Load] Syncing vanilla options from `%s` to `%s`...\n", vanillaPath.c_str(), rgonPath.c_str());

		mINI::INIFile vanillaFile(vanillaPath);
		mINI::INIStructure vanillaData;
		if (!vanillaFile.read(vanillaData)) {
			ZHL::Log("[OptionsSyncing::Load] ERROR: Failed to read vanilla INI data from `%s`\n", vanillaPath.c_str());
			return;
		}

		mINI::INIFile rgonFile(rgonPath);
		mINI::INIStructure rgonData;
		if (!rgonFile.read(rgonData)) {
			ZHL::Log("[OptionsSyncing::Load] ERROR: Failed to read rgon INI data from `%s`\n", rgonPath.c_str());
			return;
		}
		CopySharedOptions(vanillaData, rgonData);
		if (!rgonFile.write(rgonData)) {
			ZHL::Log("[OptionsSyncing::Load] ERROR: Failed to write rgon INI data to `%s`\n", rgonPath.c_str());
			return;
		}

		ZHL::Log("[OptionsSyncing::Load] Sync completed.\n");
	}

	super(rgonPath.c_str());
}

constexpr char DISCLAIMER[] = "Note: The options in this file are overwritten by the ones in `.../Binding of Isaac Repentance+/options.txt` when REPENTOGON starts. Modifying this file may have no effect.";

HOOK_METHOD(OptionsConfig, Save, () -> void) {
	super();

	const std::string vanillaPath = GetVanillaOptionsIniPath();
	const std::string rgonPath = GetRgonVanillaOptionsIniPath();

	if (!std::filesystem::exists(rgonPath)) {
		ZHL::Log("[OptionsSyncing::Save] ERROR: Expected rgon file does not exist @ `%s`\n", rgonPath.c_str());
	} else {
		ZHL::Log("[OptionsSyncing::Save] Syncing vanilla options from `%s` to `%s`...\n", rgonPath.c_str(), vanillaPath.c_str());

		mINI::INIFile rgonFile(rgonPath);
		mINI::INIStructure rgonData;
		if (!rgonFile.read(rgonData)) {
			ZHL::Log("[OptionsSyncing::Save] ERROR: Failed to read rgon INI data from `%s`\n", rgonPath.c_str());
			return;
		}

		if (!std::filesystem::exists(vanillaPath)) {
			if (!std::filesystem::copy_file(rgonPath, vanillaPath)) {
				ZHL::Log("[OptionsSyncing::Save] ERROR: Failed to copy rgonPath `%s` to vanillaPath `%s`\n", rgonPath.c_str(), vanillaPath.c_str());
				return;
			}
		} else {
			mINI::INIFile vanillaFile(vanillaPath);
			mINI::INIStructure vanillaData;
			if (!vanillaFile.read(vanillaData)) {
				ZHL::Log("[OptionsSyncing::Save] ERROR: Failed to read vanilla INI data from `%s`\n", vanillaPath.c_str());
				return;
			}
			CopySharedOptions(rgonData, vanillaData);
			if (!vanillaFile.write(vanillaData)) {
				ZHL::Log("[OptionsSyncing::Save] ERROR: Failed to write vanilla INI data to `%s`\n", vanillaPath.c_str());
				return;
			}
		}

		// Insert a disclaimer message at the beginning of the rgon file to clarify that modifying it may have no effect.
		mINI::INIStructure updatedRgonData;
		updatedRgonData[DISCLAIMER];
		for (const auto& [category, options] : rgonData) {
			updatedRgonData[category] = options;
		}
		if (!rgonFile.generate(updatedRgonData)) {
			ZHL::Log("[OptionsSyncing::Save] ERROR: Failed to write rgon INI data to `%s`\n", rgonPath.c_str());
			return;
		}

		ZHL::Log("[OptionsSyncing::Save] Sync completed.\n");
	}
}
