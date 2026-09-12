#include "IsaacRepentance.h"
#include "../Patches/Anm2Extras.h"

extern "C" {

	__declspec(dllexport) void L_LayerState_ClearCustomShader(LayerState* layer, bool champion) {
		ClearCustomShader(layer, champion);
	}
	
	__declspec(dllexport) const char* L_LayerState_GetDefaultSpritesheetPath(LayerState* layer) {
		return layer->_layerData->defaultSpriteSheetPath.c_str();
	}

	__declspec(dllexport) const char* L_LayerState_GetName(LayerState* layer) {
		return layer->_layerData->name.c_str();
	}

	__declspec(dllexport) const char* L_LayerState_GetSpritesheetPath(LayerState* layer) {
		return layer->_spriteSheetPath.c_str();
	}

	__declspec(dllexport) bool L_LayerState_HasCustomShader(LayerState* layer, bool champion) {
		return HasCustomShader(layer, champion);
	}

	__declspec(dllexport) bool L_LayerState_HasCustomShaderWithPath(LayerState* layer, const char* path, bool champion) {
		return HasCustomShader(layer, path, champion);
	}

	__declspec(dllexport) bool L_LayerState_SetCustomShader(LayerState* layer, const char* path, bool champion) {
		return SetCustomShader(layer, path, champion);
	}
}