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

void ASMPatchesForANM2Extras();
