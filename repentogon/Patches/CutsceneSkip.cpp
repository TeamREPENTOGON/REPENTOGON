#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"

//HOOK_METHOD(Cutscene, Show, (int cutsceneid) -> void) {
//	printf("%d %d\n", cutsceneid, repentogonOptions.skipIntroCutscene);
//
//	printf("%d\n", *(int*)((char*)g_Manager + 0x1ea834));
//
//	if (cutsceneid != 1 && !repentogonOptions.skipIntroCutscene) {
//		super(cutsceneid);
//	}
//}