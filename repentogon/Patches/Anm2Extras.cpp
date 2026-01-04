#include <filesystem>
#include <regex>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "SigScan.h"
#include "Anm2Extras.h"

#include "../repentogon/Utils/ANM2Utils.hpp"
#include "../repentogon/Utils/ShaderUtils.hpp"
#include "../repentogon/Utils/ImageUtils.hpp"
#include "../MiscFunctions.h"
#include "ExtraRenderSteps.h"

//shitass workaround for crashing pause menu anm2
HOOK_METHOD(PauseScreen, Show, ()-> void) {
		if (!this->mainsprite.GetLayer("Invite")) {
			g_Game->GetConsole()->PrintError("A custom Pause menu ANM2 is not compatible with Repentance+ and has not been loaded, check your Pause Menu related mods!.");
			this->mainsprite.Load(REPENTOGON::GetRGONGfxAbsolutePath("gfx/ui/pausescreen.anm2"), true);
		}
	super();
	return;
}

// Normalizes input shader paths for use as keys and for identification.
// Converts to lowercase and strips excess slashes, ie `shaders\\MyShader` -> `shaders/myshader`
std::string NormalizeShaderPath(const std::string& input_path) {
	std::string path = std::regex_replace(input_path.c_str(), std::regex(R"([\/\\]+)"), "/");
	std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) { return std::tolower(c); });
	return path;
}

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

// Abstracted shader loading (for reloading shaders via console)
bool LoadCustomShader(const std::string& path, KAGE_Graphics_Shader* shader, bool champion) {
	const std::string fullpath = FindShaderFullPath(path);
	if (fullpath.empty())
		return false;
	KAGE_Graphics_VertexAttributeDescriptor* desc = champion ? &g_ColorOffset_Champion_VertexAttributes : &g_ColorOffset_VertexAttributes;
	KAGE_Graphics_Manager_GL::LoadShader(shader, desc, fullpath.c_str());
	return true;
}

// Returns the custom shader corresponding to the relative path.
// Initializes the shader if necessary.
CustomShader* GetOrLoadCustomShader(const std::string& input_path, const bool champion) {
	if (input_path.empty())
		return nullptr;
	const std::string path = NormalizeShaderPath(input_path);
	auto& shader_map = champion ? custom_champion_shaders : custom_shaders;
	if (shader_map.count(path) == 0) {
		shader_map[path].path = path;
		if (!LoadCustomShader(path, &shader_map[path].shader, champion)) {
			shader_map.erase(path);
			return nullptr;
		}
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

static KAGE_Graphics_Shader* get_color_offset_shader(LayerState* layerState, const bool champion)
{
	CustomShader* shader = GetCustomShaderForLayer(layerState, champion);
	if (shader) {
		// Use the custom shader.
		return &shader->shader;
	}
	// Use the normal shader.
	return __ptr_g_AllShaders[champion ? SHADER_COLOR_OFFSET_CHAMPION : SHADER_COLOR_OFFSET];
}

static void RenderFrameEx(AnimationLayer& layer, const Vector& position, int frameIndex, const Vector& topLeftClamp, const Vector& bottomRightClamp, ANM2& anm2)
{
	if (!layer._visible)
	{
		return;
	}

	int frameCount = layer._numFrames; // dont change the order of these checks, it prevents being Nicalis'd - Guantol
	if (!(0 <= frameIndex && frameIndex < frameCount))
	{
		return;
	}

	LayerState& state = anm2._layerState[layer._layerID];
	if (!state._visible)
	{
		return;
	}

	AnimationFrame& frame = layer._animFrames[frameIndex];
	if (!frame.visible)
	{
		return;
	}

	uint32_t flags = anm2._bitflags | state._bitflags;
	if ((flags & (uint32_t)eAnimationFlag::IS_LIGHT) && !g_ANM2_LightRendering)
	{
		return;
	}

	KAGE_SmartPointer_ImageBase spriteSheet = state.GetSpriteSheet();
	KAGE_Graphics_ImageBase* image = spriteSheet.image;
	if (!image)
	{
		return;
	}

	static ImageUtils::ShaderRenderData colorOffsetRenderData(*__ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET]);
	static ImageUtils::ShaderRenderData colorOffsetChampionRenderData(*__ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET_CHAMPION]);

	BlendMode blendMode = ANM2Utils::GetFrameBlendMode(state);
	ColorMod color = ANM2Utils::GetFrameColor(anm2, state, frame, flags);
	SourceQuad sourceQuad = ANM2Utils::GetFrameSourceQuad(frame, state, topLeftClamp, bottomRightClamp, anm2, image);
	DestinationQuad destQuad = ANM2Utils::GetFrameDestinationQuad(frame, position, state, topLeftClamp, bottomRightClamp, anm2);

	KAGE_Graphics_Manager& manager = g_KAGE_Graphics_Manager;
	BlendMode previousBlendMode = manager._blendMode;
	manager._blendMode = blendMode;
	KAGE_Graphics_Color kColor = KAGE_Graphics_Color(color._tint[0], color._tint[1], color._tint[2], color._tint[3]);

	if ((flags & (uint32_t)eAnimationFlag::USE_CHAMPION_SHADER) != 0) // use champion shader
	{
		constexpr bool IS_CHAMPION = true;
		KAGE_Graphics_Shader* shader = get_color_offset_shader(&state, IS_CHAMPION);

		float* vertexBuffer = ImageUtils::SubmitQuadForShader(*image, *shader, colorOffsetChampionRenderData, sourceQuad, destQuad, ImageUtils::QuadColor(kColor));
		if (vertexBuffer)
		{
			ShaderUtils::ColorOffsetChampion::FillVertices(vertexBuffer, *image, color, anm2._championColor);
		}
	}
	else if ((flags & ((uint32_t)eAnimationFlag::USE_DOGMA_SHADER | (uint32_t)eAnimationFlag::USE_GOLD_SHADER)) != 0) // use special shader
	{
		size_t shaderType = (flags & (uint32_t)eAnimationFlag::USE_DOGMA_SHADER) ? ShaderType::SHADER_COLOR_OFFSET_DOGMA : ShaderType::SHADER_COLOR_OFFSET_GOLD;
		KAGE_Graphics_Shader* shader = __ptr_g_AllShaders[shaderType];

		bool affectedByPause = (flags & (uint32_t)eAnimationFlag::IGNORE_GAME_TIME) == 0;
		int time = affectedByPause ? g_Game->_frameCount : (g_Manager->_framecount / 2);
		float progression = (float)(time % 256) / 256;
		color._colorize[3] = progression;

		float* vertexBuffer = ImageUtils::SubmitQuadForShader(*image, *shader, colorOffsetRenderData, sourceQuad, destQuad, ImageUtils::QuadColor(kColor));
		if (vertexBuffer)
		{
			ShaderUtils::ColorOffset::FillVertices(vertexBuffer, *image, color);
		}
	}
	else if ((flags & (uint32_t)eAnimationFlag::PROCEDURAL) != 0) // let the graphics manager handle the render
	{
		image->Render(sourceQuad, destQuad, kColor, kColor, kColor, kColor);
	}
	else // use color offset shader
	{
		constexpr bool IS_CHAMPION = false;
		KAGE_Graphics_Shader* shader = get_color_offset_shader(&state, IS_CHAMPION);

		float* vertexBuffer = ImageUtils::SubmitQuadForShader(*image, *shader, colorOffsetRenderData, sourceQuad, destQuad, ImageUtils::QuadColor(kColor));
		if (vertexBuffer)
		{
			ShaderUtils::ColorOffset::FillVertices(vertexBuffer, *image, color);
		}
	}

	manager._blendMode = previousBlendMode;
}

// Low priority since this is a Reimplementation
HOOK_METHOD_PRIORITY(AnimationLayer, RenderFrame, 9999, (Vector const& pos, int frameIndex, Vector const& top_left_clamp, Vector const& bottom_right_clamp, ANM2* anm2) -> void)
{
	RenderFrameEx(*this, pos, frameIndex, top_left_clamp, bottom_right_clamp, *anm2);
}