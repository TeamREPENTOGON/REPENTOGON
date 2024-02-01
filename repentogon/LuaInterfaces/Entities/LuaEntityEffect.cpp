#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_EntityEffect_FollowParent(Entity_Effect* effect, Entity* parent) {
		effect->FollowParent(parent);
	}

	bool L_EntityEffect_IsPlayerCreep(Entity_Effect* effect, unsigned int variant) {
		return effect->IsPlayerCreep(variant);
	}

	// the SetDamageSource func is identical to setting the DamageSource variable

	void L_EntityEffect_SetRadii(Entity_Effect* effect, float min, float max) {
		effect->_radiusMin = min;
		effect->_radiusMax = max;
	}

	void L_EntityEffect_SetTimeoutFunc(Entity_Effect* effect, int timeout) {
		effect->_timeout = timeout;
		effect->_lifespan = timeout;
	}

	unsigned int L_EntityEffect_GetDamageSource(Entity_Effect* effect) {
		return effect->_damageSource;
	}

	void L_EntityEffect_SetDamageSource(Entity_Effect* effect, unsigned int type) {
		effect->_damageSource = type;
	}

	float L_EntityEffect_GetFallingAcceleration(Entity_Effect* effect) {
		return effect->_fallingAcceleration;
	}

	void L_EntityEffect_SetFallingAcceleration(Entity_Effect* effect, float value) {
		effect->_fallingAcceleration = value;
	}

	float L_EntityEffect_GetFallingSpeed(Entity_Effect* effect) {
		return effect->_fallingSpeed;
	}

	void L_EntityEffect_SetFallingSpeed(Entity_Effect* effect, float value) {
		effect->_fallingSpeed = value;
	}

	bool L_EntityEffect_GetIsFollowing(Entity_Effect* effect) {
		return effect->_isFollowing;
	}

	void L_EntityEffect_SetIsFollowing(Entity_Effect* effect, bool value) {
		effect->_isFollowing = value;
	}

	int L_EntityEffect_GetLifespan(Entity_Effect* effect) {
		return effect->_lifespan;
	}

	void L_EntityEffect_SetLifespan(Entity_Effect* effect, int value) {
		effect->_lifespan = value;
	}

	float L_EntityEffect_GetHeight(Entity_Effect* effect) {
		return effect->_height;
	}

	void L_EntityEffect_SetHeight(Entity_Effect* effect, float value) {
		effect->_height = value;
	}

	float L_EntityEffect_GetMinRadius(Entity_Effect* effect) {
		return effect->_radiusMin;
	}

	void L_EntityEffect_SetMinRadius(Entity_Effect* effect, float min) {
		effect->_radiusMin = min;
	}

	float L_EntityEffect_GetMaxnRadius(Entity_Effect* effect) {
		return effect->_radiusMax;
	}

	void L_EntityEffect_SetMaxRadius(Entity_Effect* effect, float max) {
		effect->_radiusMax = max;
	}

	Vector * L_EntityEffect_GetParentOffset(Entity_Effect* effect) {
		return &effect->_parentOffset;
	}

	void L_EntityEffect_SetParentOffset(Entity_Effect* effect, Vector* value) {
		effect->_parentOffset = *value;
	}

	float L_EntityEffect_GetRotation(Entity_Effect* effect) {
		return effect->_rotation;
	}

	void L_EntityEffect_SetRotation(Entity_Effect* effect, float min) {
		effect->_rotation = min;
	}

	float L_EntityEffect_GetScale(Entity_Effect* effect) {
		return effect->_scale;
	}

	void L_EntityEffect_SetScale(Entity_Effect* effect, float value) {
		effect->_scale = value;
	}

	int L_EntityEffect_GetState(Entity_Effect* effect) {
		return effect->_state;
	}

	void L_EntityEffect_SetState(Entity_Effect* effect, int value) {
		effect->_state = value;
	}

	int L_EntityEffect_GetTimeout(Entity_Effect* effect) {
		return effect->_timeout;
	}

	void L_EntityEffect_SetTimeout(Entity_Effect* effect, int value) {
		effect->_timeout = value;
	}
}