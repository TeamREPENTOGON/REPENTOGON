#include "IsaacRepentance.h"

MOD_EXPORT Shape* L_DebugRenderer_Get(int index, bool unk) {
	return g_Game->_debugRenderer.Get(index, unk);
}