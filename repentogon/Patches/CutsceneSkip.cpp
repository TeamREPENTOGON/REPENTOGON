#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"


namespace SkipIntro {
	bool IsIntroSkip = false;
};

bool cutsceneskipped = false; //it will only play once and be the first thing that plays anyway

HOOK_METHOD(Cutscene, Show, (int cutsceneid) -> void) {
	//	ZHL::Log("%d %d\n", cutsceneid, repentogonOptions.skipIntroCutscene);
	//
	//	ZHL::Log("%d\n", *(int*)((char*)g_Manager + 0x1ea834));
	//
	if (!repentogonOptions.skipIntro) {
		return super(cutsceneid);
	};
	if (repentogonOptions.skipIntro && (cutsceneskipped)) {
		return super(cutsceneid);
	};
	cutsceneskipped = true;
	SkipIntro::IsIntroSkip = true;
};

// [HOOK-ORDER experiment] TEMPORARY instrumentation only, no behavior change. Shared helpers/state
// defined in MiscFixes.cpp; see that file for the full explanation of this diagnostic.
extern int HookOrder_Enter(const char* fileTag, int selectedMenuID);
extern void HookOrder_BeforeSuper(const char* fileTag, int callID, int selectedMenuID);
extern void HookOrder_AfterSuper(const char* fileTag, int callID, int selectedMenuID);

HOOK_METHOD(MenuManager, Update, ()->void) {
	MenuManager* mngr = g_MenuManager;
	if (SkipIntro::IsIntroSkip && mngr!=nullptr) {
		mngr->_fadeInColor._red = mngr->_fadeInColor._green = mngr->_fadeInColor._blue = 0;

		SkipIntro::IsIntroSkip = false;
	}
	int hookOrderCallID = HookOrder_Enter("CutsceneSkip.cpp", this->_selectedMenuID);
	HookOrder_BeforeSuper("CutsceneSkip.cpp", hookOrderCallID, this->_selectedMenuID);
	super();
	HookOrder_AfterSuper("CutsceneSkip.cpp", hookOrderCallID, this->_selectedMenuID);
};