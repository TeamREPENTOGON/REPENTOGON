#include "IsaacRepentance.h"

MOD_EXPORT Minimap* L_Minimap_Get() { 
	return &g_Game->_minimap; 
}

MOD_EXPORT void L_Minimap_GetDisplayedSize(Vector* out) {
	g_Game->_minimap.GetDisplayedSize(*out);
}

MOD_EXPORT void L_Minimap_Refresh() {
	g_Game->_minimap.Refresh();
}