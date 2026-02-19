#include "ShaderLoader.h"
#include "Utils/ShaderUtils.hpp"
#include "MiscFunctions.h"
#include <filesystem>
#include <regex>

namespace {
    struct ShaderMetadata
    {
        std::string path;
        std::vector<KAGE_Graphics_VertexAttributeDescriptor> vertexDescriptor;

    private:
        void free_vertex_descriptor()
        {
            for (int i = 0; i < (int)this->vertexDescriptor.size() - 1; i++)
            {
                auto& attribute = this->vertexDescriptor[i];
                delete[] attribute.name;
                attribute.name = nullptr;
            }
        }

    public:
        ~ShaderMetadata()
        {
            this->free_vertex_descriptor();
        }
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
                auto& myAttribute = vertexDescriptor.emplace_back();
                auto& attribute = descriptor[i];

                size_t len = std::strlen(attribute.name);
                char* name = new char[len + 1];
                std::memcpy(name, attribute.name, len + 1);
                myAttribute.name = name;

                myAttribute.format = attribute.format;
            }

            auto& terminator = vertexDescriptor.emplace_back();
            terminator.name = "";
            terminator.format = (int)eVertexAttributeFormat::TERMINATOR;
        }
    };
}

static std::unordered_map<std::string, Shader> shaderMap;

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
		if (!mod->IsEnabled()) continue;

		const std::string fullpath = (std::filesystem::path(mod->_resourcesDirectory) / path).string();
		if (std::filesystem::exists(fullpath + ".fs") && std::filesystem::exists(fullpath + ".vs")) {
			return fullpath;
		}
	}

	return "";
}

KAGE_Graphics_Shader* ShaderLoader::LoadShader(const std::string& path, const KAGE_Graphics_VertexAttributeDescriptor* vertexDesc)
{
    const std::string key = normalize_shader_path(path);
    auto [it, inserted] = shaderMap.try_emplace(key);
    Shader& shader = it->second;

    if (inserted)
    {
        const std::string modPath = find_shader(path);
        if (modPath.empty())
        {
            shaderMap.erase(it);
            return nullptr;
        }

        KAGE_Graphics_Manager_GL::LoadShader(&shader.shader, vertexDesc, modPath.c_str());
        if (!shader.shader._initialized)
        {
            shaderMap.erase(it);
            return nullptr;
        }

        shader.SetMetadata(path, vertexDesc);
    }

    return &shader.shader;
}

void ShaderLoader::detail::ReloadShaders()
{
    for (auto& [key, value] : shaderMap)
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
        }

        if (failed)
        {
            std::string message = REPENTOGON::StringFormat("Unable to reload shader \"%s\"", metadata.path.c_str());
            g_Game->GetConsole()->PrintError(message);
            ZHL::Log(message.c_str());
        }
    }
}