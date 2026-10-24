#include <filesystem>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../ShaderLoader.h"

MOD_EXPORT bool L_Renderer_LoadImage(const char* path, KAGE_SmartPointer_ImageBase* out) {
	if (!std::filesystem::path(path).is_relative()) {
		return false;
	}

	KAGE_SmartPointer_ImageBase image;
	Manager::LoadImage(&image, path, __ptr_g_VertexAttributeDescriptor_Position, false);
	if (!image.image) {
		return false;
	}

	new (out) KAGE_SmartPointer_ImageBase(image);
	return true;
}

MOD_EXPORT bool L_Renderer_CreateImage(uint32_t width, uint32_t height, const char* name, KAGE_SmartPointer_ImageBase* out) {
	KColor color = KColor(0.0, 0.0, 0.0, 0.0);

	// prevent name clashes with real images (since these are added to the cache for some reason, even though they are never loaded)
	std::string trueName = REPENTOGON::StringFormat("%s.procedural", name);
	KAGE_SmartPointer_ImageBase image = KAGE_Graphics_ImageManager::CreateProceduralImage(width, height, trueName.c_str(), color);
	if (!image.image) {
		return false;
	}

	new (out) KAGE_SmartPointer_ImageBase(image);
	return true;
}

MOD_EXPORT bool L_Renderer_IsProceduralImage(KAGE_SmartPointer_ImageBase* image) {
	return (image->image->_flags & (uint64_t)eImageFlag::PROCEDURAL) != 0;
}

MOD_EXPORT void L_Renderer_BeginRenderToImage(KAGE_SmartPointer_ImageBase* image, BlendMode* previousBlendMode) {
	Rendering::PushCurrentRenderTarget();
	g_KAGE_Graphics_Manager.SetCurrentRenderTarget(image->image, false);
	*previousBlendMode = g_KAGE_Graphics_Manager._blendMode;
}

MOD_EXPORT void L_Renderer_EndRenderToImage(BlendMode* previousBlendMode) {
	g_KAGE_Graphics_Manager._blendMode = *previousBlendMode;
	g_KAGE_Graphics_Manager.Present();
	Rendering::RestorePreviousRenderTarget();
}

MOD_EXPORT KAGE_Graphics_Shader* L_Renderer_GetShaderByType(int shaderType) {
	return __ptr_g_AllShaders[shaderType];
}

static std::string s_loadShaderError;

MOD_EXPORT KAGE_Graphics_Shader* L_Renderer_LoadShader(const char* path, const char** names, const int* formats, int count, const char** error) {
	std::vector<KAGE_Graphics_VertexAttributeDescriptor> descriptor;
	descriptor.reserve(count + 1); // + 1 for terminator
	for (int i = 0; i < count; i++) {
		descriptor.emplace_back(names[i], formats[i]);
	}
	descriptor.emplace_back();

	auto shader = ShaderLoader::LoadShader(path, descriptor.data());
	if (shader.is_err()) {
		s_loadShaderError = shader.unwrap_err();
		*error = s_loadShaderError.c_str();
		return nullptr;
	}
	return shader.unwrap();
}

MOD_EXPORT float L_Renderer_GetPixelationAmount() {
	return g_ANM2_PixelationAmount;
}

MOD_EXPORT void L_Renderer_GetClipPaneNormal(Vector* out) {
	*out = g_ANM2_ClipPaneNormal;
}

MOD_EXPORT float L_Renderer_GetClipPaneThreshold() {
	return g_ANM2_ClipPaneThreshold;
}
