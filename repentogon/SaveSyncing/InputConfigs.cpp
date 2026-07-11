#include "InputConfigs.h"

#include <filesystem>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../REPENTOGONOptions.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

// The way that axis controller mappings are encoded (joystick/etc) was changed at some point after Rep+ v1.9.7.12
// Old encoding: 0xABABXXXX (e.g. "-1414856704")
// New encoding: 0xABAB + 0xXXXX (e.g. "43947")
// Additionally, the face buttons (left/right/up/down) were changed from 6/5/7/4 to 4/5/6/7, for whatever reason.
// (They also swapped the default buttons for MenuTab and MenuEx, maybe not intentionally, but that is less consequential...)
// These changes can cause controller mappings to get scrambled when you swap between vanilla and repentogon.
// (Kinda like how a bunch of people's controller mappings got scrambled whenever nicalis pushed out this update...)
// While we do know how to convert the current differences, the face buttons remapping is more difficult to identify 100%.
// We split our inputconfigs out into a separate file to prevent these issues and future-proof if nicalis changes anything else.
// We only import from the vanilla file once ever, so users may need to map their controllers separately on vanilla and rgon,
// but that's not a massive deal in the grand scheme of things. Being separate makes some things simpler.
// (Also, prior to this change, controller mappings couldn't be shared between vanilla and rgon anyway, without borking.)
// We still have the option of introducing automatic syncing in the future if we really wanted to, but its probably not a huge deal.

constexpr char VANILLA_INPUTCONFIGS_PATH[] = "%s/inputconfigs.dat";
constexpr char RGON_INPUTCONFIGS_PATH[] = "%s/inputconfigs_rgon.dat";

void PatchInputConfigsPath(const char* def, const char* name) {
	void* addr = sASMDefinitionHolder->GetDefinition(def);

	printf("[REPENTOGON] Patching %s for the inputconfigs.dat path @ %p\n", name, addr);

	const char* ptr = RGON_INPUTCONFIGS_PATH;

	ASMPatch patch;
	patch.AddBytes("\x68").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4))
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}
void ASMPatchInputConfigsPaths() {
	PatchInputConfigsPath(&AsmDefinitions::SaveButtonMaps_InputConfigsPath, "SaveButtonMaps");
	PatchInputConfigsPath(&AsmDefinitions::LoadButtonMaps_InputConfigsPath, "LoadButtonMaps");
}

constexpr int PATH_BUFFER_SIZE = 1024;

std::string GetFullInputConfigsPath(const bool repentogon) {
	auto pathBuffer = std::make_unique<char[]>(PATH_BUFFER_SIZE);
	const char* format = repentogon ? RGON_INPUTCONFIGS_PATH : VANILLA_INPUTCONFIGS_PATH;
	snprintf(pathBuffer.get(), PATH_BUFFER_SIZE, format, &g_SaveDataPath);
	return std::string(pathBuffer.get());
}

// 0xABABXXXX -> (0xABAB + 0xXXXX)
int32_t __stdcall ConvertOldToNewAxis(const int32_t value) {
	const uint32_t raw = static_cast<uint32_t>(value);
	const uint32_t upper = raw >> 16;
	const uint32_t lower = raw & 0xFFFF;

	if (upper == 0xABAB) {
		return static_cast<int32_t>(upper + lower);
	}
	return value;
}

// (0xABAB + 0xXXXX) -> 0xABABXXXX
int32_t __stdcall ConvertNewToOldAxis(int32_t value) {
	if (value >= 0xABAB && value <= 0xFFFF) {
		const uint32_t lower = value - 0xABAB;
		const uint32_t packed = 0xABAB0000u | lower;
		return static_cast<int32_t>(packed);
	}
	return value;
}

enum InputConfigVersion {
	INPUT_CONFIG_UNKNOWN,
	INPUT_CONFIG_OLD,
	INPUT_CONFIG_NEW,
};

// We use the axis mappings to determine the config version as it is consistently identifiable.
// If we find no axis mappings, or somehow find both, we consider the version inconclusive/unknown.
InputConfigVersion GetConfigVersion(xml_node<char>* node) {
	bool foundNew = false;
	bool foundOld = false;

	for (xml_attribute<char>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		const std::string stringValue = attr->value();
		if (!stringValue.empty()) {
			const int32_t intValue = std::atoi(stringValue.c_str());
			if (intValue != ConvertOldToNewAxis(intValue)) {
				foundOld = true;
			}
			if (intValue != ConvertNewToOldAxis(intValue)) {
				foundNew = true;
			}
		}
	}

	if (foundNew && !foundOld) {
		return INPUT_CONFIG_NEW;
	}
	if (foundOld && !foundNew) {
		return INPUT_CONFIG_OLD;
	}
	return INPUT_CONFIG_UNKNOWN;
}

struct ButtonPair {
	int32_t oldVal;
	int32_t newVal;
};

static const std::vector<ButtonPair> s_ChangedFaceButtons = {
	// Old -> New
	{6, 4},
	// {5, 5},
	{7, 6},
	{4, 7},
};

int32_t ConvertMapping(const int32_t value, const InputConfigVersion sourceVersion, const InputConfigVersion destVersion) {
	// Old -> New
	if (destVersion == INPUT_CONFIG_NEW) {
		// Only remap face buttons when the source version is confirmed.
		if (sourceVersion == INPUT_CONFIG_OLD) {
			for (const auto& pair : s_ChangedFaceButtons) {
				if (value == pair.oldVal) {
					return pair.newVal;
				}
			}
		}

		// Axis mappings are always safe to convert.
		return ConvertOldToNewAxis(value);
	}

	// New -> Old
	if (destVersion == INPUT_CONFIG_OLD) {
		// Only remap face buttons when the source version is confirmed.
		if (sourceVersion == INPUT_CONFIG_NEW) {
			for (const auto& pair : s_ChangedFaceButtons) {
				if (value == pair.newVal) {
					return pair.oldVal;
				}
			}
		}

		// Axis mappings are always safe to convert.
		return ConvertNewToOldAxis(value);
	}

	return value;
}

bool ConvertInputConfigsXml(xml_document<char>* doc, const InputConfigVersion destVersion) {
	xml_node<char>* root = doc->first_node("KeyConfigs");
	if (!root) {
		ZHL::Log("[InputConfigs] [WARNING] - XML contains no nodes!\n");
		return false;
	}
	for (xml_node<char>* node = root->first_node("Controller"); node; node = node->next_sibling("Controller")) {
		xml_attribute<char>* nameAttr = node->first_attribute("Name");
		if (!nameAttr) {
			continue;
		}
		if (destVersion == INPUT_CONFIG_OLD) {
			if (xml_attribute<char>* attr = node->first_attribute("ToggleDescriptions-0")) {
				node->remove_attribute(attr);
			}
			if (xml_attribute<char>* attr = node->first_attribute("ToggleDescriptions-1")) {
				node->remove_attribute(attr);
			}
		}
		if (std::string(nameAttr->value()) != "Keyboard") {
			const InputConfigVersion sourceVersion = GetConfigVersion(node);
			if (sourceVersion != destVersion) {
				for (xml_attribute<char>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
					const std::string stringValue = attr->value();
					if (!stringValue.empty()) {
						const int32_t intValue = std::atoi(stringValue.c_str());
						const int32_t convertedValue = ConvertMapping(intValue, sourceVersion, destVersion);
						if (intValue != convertedValue) {
							const std::string newStringValue = std::to_string(convertedValue);
							const size_t len = newStringValue.size();
							attr->value(doc->allocate_string(newStringValue.c_str(), len), len);
						}
					}
				}
			}
		}
	}
	return true;
}

void TryImportInputConfigs() {
	if (!repentogonOptions.didInputConfigsImport) {
		// This is not important enough to try again if something goes wrong.
		ZHL::Log("[InputConfigs] [INFO] - Performing first-time initialization of inputconfigs.dat...\n");
		repentogonOptions.didInputConfigsImport = true;
		repentogonOptions.Save();

		const std::string vanillaPath = GetFullInputConfigsPath(false);
		const std::string rgonPath = GetFullInputConfigsPath(true);

		bool shouldDoImport = false;

		try {
			shouldDoImport = std::filesystem::exists(vanillaPath) && !std::filesystem::exists(rgonPath);
		} catch (const std::filesystem::filesystem_error& err) {
			ZHL::Log("[InputConfigs] [ERROR] Failed to import %s: %s\n", vanillaPath.c_str(), err.what());
			return;
		}

		if (!shouldDoImport) {
			ZHL::Log("[InputConfigs] [INFO] Skipping import.\n");
			return;
		}

		ZHL::Log("[InputConfigs] [INFO] - Importing input configs from %s to %s\n", vanillaPath.c_str(), rgonPath.c_str());

		ZHL::Log("[InputConfigs] [INFO] - Reading vanilla file...\n");
		std::string buffer((std::istreambuf_iterator<char>(std::ifstream(vanillaPath))), std::istreambuf_iterator<char>());
		if (buffer.empty()) {
			ZHL::Log("[InputConfigs] [ERROR] - Failed to read %s\n", vanillaPath.c_str());
			return;
		}

		ZHL::Log("[InputConfigs] [INFO] - Parsing XML...\n");
		auto doc = std::make_unique<xml_document<char>>();
		try {
			doc->parse<0>(buffer.data());
		} catch (const rapidxml::parse_error& e) {
			ZHL::Log("[InputConfigs] [ERROR] Failed to parse inputconfigs.dat: %s\n", e.what());
			return;
		}

		if (!ConvertInputConfigsXml(doc.get(), INPUT_CONFIG_OLD)) {
			return;
		}

		ZHL::Log("[InputConfigs] [INFO] - Writing rgon file...\n");
		std::ofstream outFile(rgonPath);
		if (!outFile) {
			ZHL::Log("[InputConfigs] [ERROR] - Failed to write to %s\n", rgonPath.c_str());
			return;
		}
		outFile << *doc;
		outFile.close();
		ZHL::Log("[InputConfigs] [INFO] - Input config import complete!\n");
	}
}

HOOK_STATIC(Manager, Init, () -> void, __stdcall) {
	TryImportInputConfigs();
	super();
}
