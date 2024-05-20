#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"


namespace SkipIntro {
	bool IsIntroSkip = false;
};

bool cutsceneskipped = false; //it will only play once and be the first thing that plays anyway

HOOK_METHOD(Cutscene, Show, (int cutsceneid) -> void) {
	//	printf("%d %d\n", cutsceneid, repentogonOptions.skipIntroCutscene);
	//
	//	printf("%d\n", *(int*)((char*)g_Manager + 0x1ea834));
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

HOOK_METHOD(MenuManager, Update, ()->void) {
	MenuManager* mngr = g_MenuManager;
	if (SkipIntro::IsIntroSkip && mngr!=nullptr) {
		mngr->_fadeInColor.r = mngr->_fadeInColor.g = mngr->_fadeInColor.b = 0;

		SkipIntro::IsIntroSkip = false;
	}
	super();
};