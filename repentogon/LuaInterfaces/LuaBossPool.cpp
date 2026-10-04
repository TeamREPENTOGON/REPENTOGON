#include "IsaacRepentance.h"

MOD_EXPORT BossPool* L_BossPoolManager_Get() {
	return &g_Game->_bossPool;
}

MOD_EXPORT const char* L_BossPool_GetName(BossPool_Pool* pool) {
	return pool->_name.c_str();
}
