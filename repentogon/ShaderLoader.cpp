#include "ShaderLoader.h"
#include "Utils/ShaderUtils.hpp"
#include "MiscFunctions.h"
#include <numeric>
#include <filesystem>
#include <regex>
#include <glad/glad.h>

namespace {
    struct ShaderMetadata
    {
        std::string path;
        std::vector<KAGE_Graphics_VertexAttributeDescriptor> vertexDescriptor;
        bool forceReload = false; // forcibly reload the shader next time ShaderLoader::LoadShader is called on this shader.
    };

    struct Shader
    {
        KAGE_Graphics_Shader shader;
        ShaderMetadata metadata;

        void SetMetadata(std::string_view name, const KAGE_Graphics_VertexAttributeDescriptor* descriptor)
        {
            this->metadata.path = name;

            // Set attributes
            size_t numAttributes = ShaderUtils::GetNumVertexAttributes(descriptor);
            auto& vertexDescriptor = this->metadata.vertexDescriptor;
            vertexDescriptor.clear();
            vertexDescriptor.reserve(numAttributes + 1);

            // copy descriptor
            for (size_t i = 0; i < numAttributes; i++)
            {
                vertexDescriptor.emplace_back(descriptor[i]);
            }
            auto& terminator = vertexDescriptor.emplace_back();
        }
    };
}

static std::unordered_map<std::string, Shader> s_shaderMap;

// Normalizes input shader paths for use as keys and for identification.
// Converts to lowercase and strips excess slashes, ie `shaders\\MyShader` -> `shaders/myshader`
static std::string normalize_shader_path(const std::string& input_path) {
	std::string path = std::regex_replace(input_path.c_str(), std::regex(R"([\/\\]+)"), "/");
	std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) { return std::tolower(c); });
	return path;
}

// Finds the full filepath to the shader and checks if the .fs and .vs files exist.
// Returns an empty string if no matching files are found.
static std::string find_shader(const std::string& path)
{
	for (ModEntry* mod : g_Manager->GetModManager()->_mods)
    {
		if (!mod->_loaded) continue;

		const std::string fullpath = (std::filesystem::path(mod->_resourcesDirectory) / path).string();
		if (std::filesystem::exists(fullpath + ".fs") && std::filesystem::exists(fullpath + ".vs")) {
			return fullpath;
		}
	}

	return "";
}

namespace
{
    struct GLVertexAttribute
    {
        std::string name;
        GLenum type = 0;

        GLVertexAttribute(std::string_view name, GLenum type)
            : name(name), type(type)
        {}
    };
}

static std::vector<GLVertexAttribute> get_shader_vertex_attributes(const KAGE_Graphics_Shader& shader)
{
    std::vector<GLVertexAttribute> attributes;
    GLuint program = shader._glProgram;

    GLint count;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    attributes.reserve(count);

    GLint maxLength;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    std::vector<char> name(maxLength);
    GLsizei length;
    GLint size;
    GLenum type;

    for (GLuint i = 0; i < (GLuint)count; i++)
    {
        glGetActiveAttrib(program, i, maxLength, &length, &size, &type, name.data());
        attributes.emplace_back(std::string_view(name.data(), length), type);
    }
    
    return attributes;
}

// VertexDescriptor is valid if it is a superset of the shader attributes.
static bool is_vertex_descriptor_compatible(const KAGE_Graphics_Shader& shader, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc)
{
    auto shaderAttributes = get_shader_vertex_attributes(shader);
    size_t descSize = ShaderUtils::GetNumVertexAttributes(vertexDesc);
    if (shaderAttributes.size() > descSize)
    {
        return false;
    }

    std::sort(shaderAttributes.begin(), shaderAttributes.end(),
    [](const GLVertexAttribute& a, const GLVertexAttribute& b)
    {
        return a.name < b.name;
    });

    // use indirect sorting to avoid copying or modifying the vertexDesc.
    std::vector<size_t> sortIndices(descSize);
    std::iota(sortIndices.begin(), sortIndices.end(), 0);

    std::sort(sortIndices.begin(), sortIndices.end(),
    [&](size_t a, size_t b)
    {
        return std::strcmp(vertexDesc[a].name, vertexDesc[b].name) < 0;
    });


    size_t s = 0;
    size_t v = 0;

    size_t shaderCount = shaderAttributes.size();
    while (s < shaderCount && v < descSize)
    {
        const auto& shaderAttribute = shaderAttributes[s];
        const auto& attribute = vertexDesc[sortIndices[v]];

        int cmp = std::strcmp(shaderAttribute.name.c_str(), attribute.name);

        if (cmp == 0)
        {
            if (!ShaderUtils::IsGLtypeCompatible(shaderAttribute.type, attribute.format))
            {
                return false;
            }

            s++;
            v++;
        }
        else if (cmp > 0)
        {
            // Extra attribute
            v++;
        }
        else
        {
            // Shader attribute not found
            return false;
        }
    }

    return s == shaderCount;
}

static REPENTOGON::Result<KAGE_Graphics_Shader*, std::string> load_shader(Shader& shader, const std::string& path, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc)
{
    const std::string modPath = find_shader(path);
    if (modPath.empty())
    {
        return REPENTOGON::err(std::string("No shader exists for the specified path"));
    }

    assert(!shader.shader._initialized);
    KAGE_Graphics_Manager_GL::LoadShader(&shader.shader, vertexDesc, modPath.c_str());
    if (!shader.shader._initialized)
    {
        return REPENTOGON::err(std::string("Shader failed to compile"));
    }

    // Due to how the game handles rendering sprites we can't alter the vertexDescriptor if it is a superset,
    // as the Fill functions will end up writing out of bounds.
    if (!is_vertex_descriptor_compatible(shader.shader, vertexDesc))
    {
        return REPENTOGON::err(std::string("Vertex Descriptor is incompatible with shader attributes"));
    }

    return REPENTOGON::ok(&shader.shader);
}

REPENTOGON::Result<KAGE_Graphics_Shader*, std::string> ShaderLoader::LoadShader(const std::string& path, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc)
{
    const std::string key = normalize_shader_path(path);
    auto [it, inserted] = s_shaderMap.try_emplace(key);
    Shader& shader = it->second;

    if (inserted)
    {
        auto result = load_shader(shader, path, vertexDesc);
        if (result.is_err())
        {
            s_shaderMap.erase(it);
        }
        else
        {
            shader.SetMetadata(path, vertexDesc);
        }

        return result;
    }

    if (shader.metadata.forceReload)
    {
        auto result = load_shader(shader, path, vertexDesc);
        if (result.is_err())
        {
            shader.shader.HardReset();
        }
        else
        {
            shader.SetMetadata(path, vertexDesc);
            shader.metadata.forceReload = false;
        }

        return result;
    }

    if (!shader.shader._initialized)
    {
        return REPENTOGON::err(std::string("Shader failed to compile"));
    }

    if (!ShaderUtils::UsesVertexDescriptor(shader.shader, vertexDesc, ShaderUtils::GetNumVertexAttributes(vertexDesc)))
    {
        return REPENTOGON::err(std::string("Incorrect Vertex Descriptor"));
    }

    return REPENTOGON::ok(&shader.shader);
}

void ShaderLoader::detail::ReloadShaders()
{
    for (auto& [key, value] : s_shaderMap)
    {
        auto& shader = value.shader;
        auto& metadata = value.metadata;

        // A hard reset is necessary, as LoadShader/Initialize does not gracefully handle initializing an already initialized shader
        // causing the glProgram, and other heap allocated resources to leak.
        shader.HardReset();
        const std::string modPath = find_shader(metadata.path);

        bool failed;
        if (modPath.empty())
        {
            failed = true;
        }
        else
        {
            KAGE_Graphics_Manager_GL::LoadShader(&shader, metadata.vertexDescriptor.data(), modPath.c_str());
            failed = !shader._initialized;
            metadata.forceReload = failed;
        }

        if (failed)
        {
            std::string message = REPENTOGON::StringFormat("Unable to reload shader \"%s\"", metadata.path.c_str());
            g_Game->GetConsole()->PrintError(message);
            KAGE::LogMessage(3, message.c_str());
        }
        else if (!is_vertex_descriptor_compatible(shader, metadata.vertexDescriptor.data()))
        {
            std::string message = REPENTOGON::StringFormat("Shader \"%s\" changed vertex descriptor, reload the shader manually", metadata.path.c_str());
            g_Game->GetConsole()->PrintError(message);
            KAGE::LogMessage(3, message.c_str());

            shader.HardReset();
            metadata.forceReload = true;
        }
    }
}