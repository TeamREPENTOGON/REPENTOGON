#include "IsaacRepentance.h"
#include "HookSystem.h"

HOOK_STATIC(KAGE_Graphics_Manager_GL, LoadShader, (KAGE_Graphics_Shader* shader, KAGE_Graphics_VertexAttributeDescriptor* descriptor, const char* shaderPath) -> void, __cdecl) {
	const char* targetShader = "shaders/coloroffset_gold";
	if (strcmp(shaderPath, targetShader) == 0) {
	    shaderPath = "shaders/coloroffset_gold_mesafix";
    }
	super(shader, descriptor, shaderPath);
}