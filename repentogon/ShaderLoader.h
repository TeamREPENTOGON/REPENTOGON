#pragma once

#include "IsaacRepentance.h"
#include "MiscFunctions.h"

namespace ShaderLoader
{
    REPENTOGON::Result<KAGE_Graphics_Shader*, std::string> LoadShader(const std::string& path, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc);
}

namespace ShaderLoader::detail
{
    void ReloadShaders();
}