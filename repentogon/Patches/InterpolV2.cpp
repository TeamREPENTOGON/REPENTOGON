#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"

bool _interpol_isgamerender = false;
AnimationState* _interpol_state = 0x0;

HOOK_STATIC(LuaEngine, PostRender, (void)->void,_stdcall) {
	_interpol_isgamerender = false;
	super();
	_interpol_isgamerender = true;
};

HOOK_METHOD(Game, Render, (void)->void) {
	_interpol_isgamerender = true;
	super();
	_interpol_isgamerender = false;
};

inline float _interpol_short_angle_dis(float from, float to) {
	float maxAngle = 360.0f;
	float disAngle = fmod((to - from),maxAngle);

	return fmod((2 * disAngle), maxAngle) - disAngle;
};

inline float _interpol_angle_lerp(float from, float to, float perc) {
	return from + _interpol_short_angle_dis(from, to) * perc;
};

HOOK_METHOD(AnimationLayer, RenderFrame, (const Vector& position, int unk, const Vector& topLeftClamp, const Vector& BottomRightClamp, ANM2* animation)->void) {
	if (!repentogonOptions.interpolV2 || !this->_animFrames || !_interpol_isgamerender || !_interpol_state || !(_interpol_state->_isPlaying) || g_Game->IsPaused()) {
		return super(position, unk, topLeftClamp, BottomRightClamp, animation);
	};
	AnimationFrame* ourframe = this->_animFrames;
	AnimationFrame* nextframe = 0x0;
	if (unk < this->_numFrames) {
		ourframe = ourframe + unk;
	};
	if (!ourframe->interpolated) {
		return super(position, unk, topLeftClamp, BottomRightClamp, animation);
	};
	if (unk + 1 < this->_numFrames) {
		nextframe = ourframe + 1;
	};
	if (nextframe == 0x0) {
		return super(position, unk, topLeftClamp, BottomRightClamp, animation);
	};
	float lerpappend = 0.5f;
	if (g_Manager->_framecount % 2 != 0) {
		lerpappend = 0.0f;
	};
	lerpappend *= (animation->_playbackSpeed);
	float lerpperc;
	lerpperc = (lerpappend+(float)(_interpol_state->_animFrame)-(float)ourframe->startFrame)/(float)(ourframe->duration);
	float lerpbegin = (1.0f - lerpperc);
	if (lerpperc > 1.0f) {
		return super(position, unk, topLeftClamp, BottomRightClamp, animation);	//crappy fix for animations playing too fast, should be later replaced with an actual frame fetch
	};

	Vector oldscale = ourframe->scale;
	Vector oldpos = ourframe->pos;
	float oldrot = ourframe->rotation;

	ColorMod oldc;
	for (int i = 0; i < 3; i++) {
		oldc._offset[i] = ourframe->color._offset[i];
		ourframe->color._offset[i] = ourframe->color._offset[i] * lerpbegin + nextframe->color._offset[i] * lerpperc;
	};
	for (int i = 0; i < 4; i++) {
		oldc._tint[i] = ourframe->color._tint[i];
		ourframe->color._tint[i] = ourframe->color._tint[i] * lerpbegin + nextframe->color._tint[i] * lerpperc;
	};

	ourframe->scale.x = ourframe->scale.x * lerpbegin + nextframe->scale.x * lerpperc;
	ourframe->scale.y = ourframe->scale.y * lerpbegin + nextframe->scale.y * lerpperc;
	ourframe->pos.x = ourframe->pos.x * lerpbegin + nextframe->pos.x * lerpperc;
	ourframe->pos.y = ourframe->pos.y * lerpbegin + nextframe->pos.y * lerpperc;
	ourframe->rotation = _interpol_angle_lerp(ourframe->rotation, nextframe->rotation, lerpperc);

	super(position, unk, topLeftClamp, BottomRightClamp,animation);

	for (int i = 0; i < 3; i++) {
		ourframe->color._offset[i]=oldc._offset[i];
	};
	for (int i = 0; i < 4; i++) {
		ourframe->color._tint[i] = oldc._tint[i];
	};

	ourframe->scale = oldscale;
	ourframe->pos = oldpos;
	ourframe->rotation = oldrot;
};

HOOK_METHOD(AnimationState, Render, (const Vector& position, const Vector& topLeftClamp, const Vector& bottomRightClamp)->void) {
	_interpol_state = this;
	super(position, topLeftClamp, bottomRightClamp);
	_interpol_state = 0x0;
};