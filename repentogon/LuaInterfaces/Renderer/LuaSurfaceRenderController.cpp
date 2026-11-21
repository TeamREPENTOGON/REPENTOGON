#include "IsaacRepentance.h"
#include "LuaCore.h"

MOD_EXPORT void L_SurfaceRenderController_Clear() {
	g_KAGE_Graphics_Manager.Clear();
	g_KAGE_Graphics_Manager._blendMode = BlendMode();
}

MOD_EXPORT void L_SurfaceRenderController_SetBlendMode(BlendMode* blendMode) {
	g_KAGE_Graphics_Manager._blendMode = *blendMode;
}
