#include <filesystem>
#include <regex>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "SigScan.h"

// Normalizes input shader paths for use as keys and for identification.
// Converts to lowercase and strips excess slashes, ie `shaders\\MyShader` -> `shaders/myshader`
std::string NormalizeShaderPath(const std::string& input_path) {
	std::string path = std::regex_replace(input_path.c_str(), std::regex(R"([\/\\]+)"), "/");
	std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) { return std::tolower(c); });
	return path;
}

struct CustomShader {
	std::string path;  // Relative path starting from `.../resources/`, without the file extensions, ie `shaders/myshader`
	KAGE_Graphics_Shader shader;
};

// Layer-specific extra data.
struct Anm2LayerExtras {
	CustomShader* custom_shader = nullptr;
	CustomShader* custom_champion_shader = nullptr;

	void SetCustomShader(CustomShader* shader, const bool champion) {
		if (champion) {
			custom_champion_shader = shader;
		}
		else {
			custom_shader = shader;
		}
	}
	CustomShader* GetCustomShader(const bool champion) {
		return champion ? custom_champion_shader : custom_shader;
	}
	bool HasCustomShader(const bool champion) {
		return GetCustomShader(champion) != nullptr;
	}
	bool HasCustomShader(const std::string& path, const bool champion) {
		CustomShader* shader = GetCustomShader(champion);
		return shader != nullptr && shader->path == NormalizeShaderPath(path);
	}
	void ClearCustomShader(const bool champion) {
		if (champion) {
			custom_champion_shader = nullptr;
		}
		else {
			custom_shader = nullptr;
		}
	}
};

// Extra data for an ANM2.
struct Anm2Extras {
	Anm2LayerExtras default_layer_extras;
	std::unordered_map<int, Anm2LayerExtras> layer_extras;
};

std::unordered_map<ANM2*, Anm2Extras> anm2_extras;

std::unordered_map<std::string, CustomShader> custom_shaders;
std::unordered_map<std::string, CustomShader> custom_champion_shaders;

// Clear an ANM2's data whenever it is destroyed or reset.
HOOK_METHOD(ANM2, destructor, ()->void) {
	anm2_extras.erase(this);
	super();
}
HOOK_METHOD(ANM2, Reset, ()->void) {
	anm2_extras.erase(this);
	super();
}

Anm2Extras* GetAnm2Extras(ANM2* anm2) {
	if (anm2 && anm2_extras.count(anm2) != 0) {
		return &anm2_extras[anm2];
	}
	return nullptr;
}

Anm2LayerExtras* GetDefaultLayerExtras(ANM2* anm2) {
	Anm2Extras* extras = GetAnm2Extras(anm2);

	if (extras) {
		return &extras->default_layer_extras;
	}
	return nullptr;
}

Anm2LayerExtras* GetDefaultLayerExtras(LayerState* layer) {
	if (layer) {
		return GetDefaultLayerExtras(layer->_animation);
	}
	return nullptr;
}

Anm2LayerExtras* GetLayerExtras(ANM2* anm2, const int layerID) {
	Anm2Extras* extras = GetAnm2Extras(anm2);

	if (extras && extras->layer_extras.count(layerID) != 0) {
		return &extras->layer_extras[layerID];
	}
	return nullptr;
}

Anm2LayerExtras* GetLayerExtras(LayerState* layer) {
	if (layer) {
		return GetLayerExtras(layer->_animation, layer->GetLayerID());
	}
	return nullptr;
}

// Returns the custom shader that this layer should use, if any.
// Prefer layer-specific shader, if none check the ANM2 instead.
CustomShader* GetCustomShaderForLayer(LayerState* layer, const bool champion) {
	Anm2LayerExtras* layer_extras = GetLayerExtras(layer);
	if (layer_extras) {
		CustomShader* shader = layer_extras->GetCustomShader(champion);
		if (shader) {
			return shader;
		}
	}

	Anm2LayerExtras* default_extras = GetDefaultLayerExtras(layer);
	if (default_extras) {
		return default_extras->GetCustomShader(champion);
	}
	return nullptr;
}

// Finds the full filepath to the shader and checks if the .fs and .vs files exist.
// Returns an empty string if no matching files are found.
std::string FindShaderFullPath(const std::string& path) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (!mod->IsEnabled()) continue;

		const std::string fullpath = (std::filesystem::path(mod->_resourcesDirectory) / path).string();

		if (std::filesystem::exists(fullpath + ".fs") && std::filesystem::exists(fullpath + ".vs")) {
			return fullpath;
		}
	}
	return "";
}

// Returns the custom shader corresponding to the relative path.
// Initializes the shader if necessary.
CustomShader* GetOrLoadCustomShader(const std::string& input_path, const bool champion) {
	if (input_path.empty()) {
		return nullptr;
	}
	const std::string path = NormalizeShaderPath(input_path);
	auto& shader_map = champion ? custom_champion_shaders : custom_shaders;
	if (shader_map.count(path) == 0) {
		const std::string fullpath = FindShaderFullPath(path);
		if (fullpath.empty()) {
			return nullptr;
		}

		// Initialize the CustomShaderInfo object and contained KAGE_Graphics_Shader.
		custom_shaders[path].path = path;
		KAGE_Graphics_VertexAttributeDescriptor* desc = champion ? &g_ColorOffset_Champion_VertexAttributes : &g_ColorOffset_VertexAttributes;
		KAGE_Graphics_Manager_GL::LoadShader(&shader_map[path].shader, desc, fullpath.c_str());
	}
	return &shader_map[path];
}

bool SetCustomShader(ANM2* anm2, const std::string& path, const bool champion) {
	CustomShader* shader = GetOrLoadCustomShader(path, champion);
	if (shader) {
		anm2_extras[anm2].default_layer_extras.SetCustomShader(shader, champion);
		return true;
	}
	return false;
}

bool SetCustomShader(LayerState* layer, const std::string& path, const bool champion) {
	ANM2* anm2 = layer->_animation;
	if (!anm2) return false;

	CustomShader* shader = GetOrLoadCustomShader(path, champion);
	if (shader) {
		anm2_extras[anm2].layer_extras[layer->GetLayerID()].SetCustomShader(shader, champion);
		return true;
	}
	return false;
}

void ClearCustomShader(ANM2* anm2, const bool champion) {
	Anm2LayerExtras* extras = GetDefaultLayerExtras(anm2);
	if (extras) {
		extras->ClearCustomShader(champion);
	}
}

void ClearCustomShader(LayerState* layer, const bool champion) {
	Anm2LayerExtras* extras = GetLayerExtras(layer);
	if (extras) {
		extras->ClearCustomShader(champion);
	}
}

bool HasCustomShader(ANM2* anm2, const bool champion) {
	Anm2LayerExtras* extras = GetDefaultLayerExtras(anm2);
	if (extras) {
		return extras->HasCustomShader(champion);
	}
	return false;
}

bool HasCustomShader(ANM2* anm2, const std::string& path, const bool champion) {
	Anm2LayerExtras* extras = GetDefaultLayerExtras(anm2);
	if (extras) {
		return extras->HasCustomShader(path, champion);
	}
	return false;
}

bool HasCustomShader(LayerState* layer, const bool champion) {
	Anm2LayerExtras* extras = GetLayerExtras(layer);
	if (extras) {
		return extras->HasCustomShader(champion);
	}
	return false;
}

bool HasCustomShader(LayerState* layer, const std::string& path, const bool champion) {
	Anm2LayerExtras* extras = GetLayerExtras(layer);
	if (extras) {
		return extras->HasCustomShader(path, champion);
	}
	return false;
}

bool reset_shader = false;

KAGE_Graphics_Shader* __stdcall CustomAnm2ShaderHook(LayerState* layerState, const bool champion) {
	CustomShader* shader = GetCustomShaderForLayer(layerState, champion);
	if (shader) {
		reset_shader = !champion;
		// Use the custom shader.
		return &shader->shader;
	}
	// Use the normal shader.
	return __ptr_g_AllShaders[champion ? 10 : 0];
}

HOOK_METHOD(AnimationLayer, RenderFrame, (Vector const& pos, int unk, Vector const& top_left_clamp, Vector const& bottom_right_clamp, ANM2* anm2) -> void) {
	super(pos, unk, top_left_clamp, bottom_right_clamp, anm2);
	if (reset_shader) {
		// Just in case...
		g_CurrentShader = __ptr_g_AllShaders[0];
		reset_shader = false;
	}
}

// Patches into AnimationLayer::RenderFrame() where the default ColorOffset(_Champion) shader
// would normally be set, to allow overriding with a custom shader.
void ASMPatchCustomAnm2ShaderHook(const char* sig, const bool isChampion) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(isChampion)
		.Push(ASMPatch::Registers::EDI)  // Push the LayerState
		.AddInternalCall(CustomAnm2ShaderHook)
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

/*bool __stdcall HasCustomAnm2ShaderApplied(LayerState* layerState) {
	return GetCustomShaderForLayer(layerState, false) != nullptr;
}

// Patches into AnimationLayer::RenderFrame() to override the champion/dogma shaders with the custom one.
void ASMPatchCustomAnm2ShaderOverride(const char* sig, const int jmp) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI)  // Push the LayerState
		.AddInternalCall(HasCustomAnm2ShaderApplied)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(reg)
		.AddBytes("\x0F\x45\xCF")  // cmovne ecx,edi
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + jmp)  // jump for true (custom shader in use)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overridden bytes
		.AddRelativeJump((char*)addr + 0x5);  // jump for false (no custom shader)
	sASMPatcher.PatchAt(addr, &patch);
}*/

constexpr char kNormalShaderSig[] = "a1????????a3????????8d85";
constexpr char kChampionShaderSig[] = "a1????????8bcfa3";
//constexpr char kGoldShaderSig[] = "a1????????a3????????f6c14074??a1????????8b80????????992bc2d1f8eb??a1????????8b80????????25ff00008079??480d00ffffff40660f6ec08bcf0f5bc08d85????????50f30f5905????????f30f1185????????e8????????c745??06000000";
//constexpr char kDogmaShaderSig[] = "a1????????a3????????f6c14074??a1????????8b80????????992bc2d1f8eb??a1????????8b80????????25ff00008079??480d00ffffff40660f6ec08bcf0f5bc08d85????????50f30f5905????????f30f1185????????e8????????c745??05000000";

void ASMPatchesForANM2Extras() {
	ASMPatchCustomAnm2ShaderHook(kNormalShaderSig, false);
	ASMPatchCustomAnm2ShaderHook(kChampionShaderSig, true);

	//ASMPatchCustomAnm2ShaderOverride(kGoldShaderSig, 0xF8);
	//ASMPatchCustomAnm2ShaderOverride(kDogmaShaderSig, 0x1CB);
}
