#include "IsaacRepentance.h"
#include "HookSystem.h"

HOOK_STATIC(KAGE_Graphics_Manager_GL, LoadShader, (KAGE_Graphics_Shader* shader, KAGE_Graphics_VertexAttributeDescriptor* descriptor, const char* shaderPath) -> void*) {
const char* targetShader = "shaders/coloroffset_gold";
	// shaders/coloroffset_gold
	printf("%s\n", shaderPath);
	if (strcmp(shaderPath, targetShader) == 0) {
	    printf("shadow REALM, JIMBOy\n");
	    shaderPath = "shaders/coloroffset_gold_mesafix";
    }
	return super(shader, descriptor, shaderPath);
}