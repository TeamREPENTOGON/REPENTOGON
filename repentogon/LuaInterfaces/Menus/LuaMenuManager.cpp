#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Patches/MainMenuBlock.h"

int luaactivemenulie = 0;

MOD_EXPORT int L_MenuManager_GetActiveMenu() {
	if (g_MenuManager->_selectedMenuID == 0) {
		return luaactivemenulie;
	}
	else {
		luaactivemenulie = 0;
		return g_MenuManager->_selectedMenuID;
	}
}

MOD_EXPORT ColorModState* L_MenuManager_GetColorModifierLerpAmount() {
	return &g_MenuManager->_lerpColorModState;
}

MOD_EXPORT ColorModState* L_MenuManager_GetCurrentColorModifier() {
	return &g_MenuManager->_currentColorModState;
}

MOD_EXPORT unsigned int L_MenuManager_GetInputMask() {
	return MainMenuInputBlock::GetInputMask();
}

MOD_EXPORT Seeds* L_MenuManager_GetSeeds() {
	return &g_MenuManager->_seedsObject;
}

MOD_EXPORT ANM2* L_MenuManager_GetShadowSprite() {
	return &g_MenuManager->_MenuShadowSprite;
}

MOD_EXPORT ColorModState* L_MenuManager_GetTargetColorModifier() {
	return &g_MenuManager->_targetColorModState;
}

MOD_EXPORT Vector* L_MenuManager_GetViewPosition() {
	return &g_MenuManager->_ViewPosition;
}

MOD_EXPORT bool L_MenuManager_IsActive() {
	return g_MenuManager != nullptr;
}

MOD_EXPORT void L_MenuManager_SetActiveMenu(int target) {
	if ((target < 1) || (target > 22)) {
		luaactivemenulie = target;
		g_MenuManager->_selectedMenuID = 0;
	}
	else {
		g_MenuManager->_selectedMenuID = target;
	}
}

MOD_EXPORT void L_MenuManager_SetColorModifier(ColorModState* colorModifier, bool lerp, float rate) {
	g_MenuManager->_targetColorModState = *colorModifier;
	if (lerp) {
		ColorModState lerp = (*colorModifier - g_MenuManager->_currentColorModState);
		lerp *= (rate / 2);
		lerp.r = abs(lerp.r);
		lerp.g = abs(lerp.g);
		lerp.b = abs(lerp.b);
		lerp.a = abs(lerp.a);
		lerp.brightness = abs(lerp.brightness);
		lerp.contrast = abs(lerp.contrast);

		g_MenuManager->_lerpColorModState = lerp;
		g_MenuManager->_shouldLerpColorModState = true;
	}
	else {
		g_MenuManager->_currentColorModState = *colorModifier;
		g_MenuManager->_shouldLerpColorModState = false;
	}
}

MOD_EXPORT void L_MenuManager_SetInputMask(unsigned int mask) {
	MainMenuInputBlock::SetInputMask(mask);
}

MOD_EXPORT void L_MenuManager_SetViewPosition(Vector* position) {
	g_MenuManager->_ViewPosition = *position;
}

MOD_EXPORT int L_Isaac_WorldToMenuPosition(int menuid, Vector* pos, Vector* out)
{
	if (g_MenuManager == NULL) {
		return 2;
	}

	Vector* ref = &g_MenuManager->_ViewPosition; //-49~ 72~ worldpos of ref // 10 95 is 0,0 on title // 59 23 offset on title
	Vector posbase = *ref + Vector(39, 15);
	ref = &posbase;
	Vector offset;

	if (menuid >= TITLE && menuid <= ONLINEAWARDS) {
		offset = Vector(ref->x + (-g_MenuManager->_viewPositionSet[menuid].x), ref->y + (-g_MenuManager->_viewPositionSet[menuid].y));
	}
	else {
		return 1;
	}

	*out = Vector(offset.x + pos->x, offset.y + pos->y);
	return 0;
}

HOOK_METHOD(InputManager, IsActionTriggered, (int btn, int controllerid, int unk)->bool) {
	if (MainMenuInputBlock::_enabled==true) {
		return (MainMenuInputBlock::_inputmask & (1 << btn)) && super(btn, controllerid, unk);
	}
	else {
		return super(btn, controllerid, unk);
	}
};

HOOK_METHOD(InputManager, IsActionPressed, (int btn, int controllerid, int unk)->bool) {
	if (MainMenuInputBlock::_enabled==true) {
		return (MainMenuInputBlock::_inputmask & (1 << btn)) && super(btn, controllerid, unk);
	}
	else{
		return super(btn, controllerid, unk);
	}
};

HOOK_STATIC(LuaEngine, PostGameStart, (unsigned int state)->void,__stdcall) {
	MainMenuInputBlock::ClearInputMask();
	super(state);
};
