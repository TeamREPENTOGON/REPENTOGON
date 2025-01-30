#pragma once

#include "IsaacRepentance.h"

bool SetCustomShader(ANM2* anm2, const std::string& path, const bool champion);
bool SetCustomShader(LayerState* layer, const std::string& path, const bool champion);

void ClearCustomShader(ANM2* anm2, const bool champion);
void ClearCustomShader(LayerState* layer, const bool champion);

bool HasCustomShader(ANM2* anm2, const bool champion);
bool HasCustomShader(ANM2* anm2, const std::string& path, const bool champion);
bool HasCustomShader(LayerState* anm2, const bool champion);
bool HasCustomShader(LayerState* anm2, const std::string& path, const bool champion);

struct CustomShader {
	std::string path;  // Relative path starting from `.../resources/`, without the file extensions, ie `shaders/myshader`
	KAGE_Graphics_Shader shader;
};

void LoadCustomShader(const std::string& path, KAGE_Graphics_Shader* shader, bool champion);

extern std::unordered_map<std::string, CustomShader> custom_shaders;
extern std::unordered_map<std::string, CustomShader> custom_champion_shaders;

void ASMPatchesForANM2Extras();