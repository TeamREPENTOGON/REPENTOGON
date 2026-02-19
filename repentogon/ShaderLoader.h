#pragma once

#include "IsaacRepentance.h"

namespace ShaderLoader
{
    KAGE_Graphics_Shader* LoadShader(const std::string& path, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc);
}

namespace ShaderLoader::detail
{
    void ReloadShaders();
}