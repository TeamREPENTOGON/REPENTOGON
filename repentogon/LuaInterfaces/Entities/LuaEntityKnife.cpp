#include "IsaacRepentance.h"
#include "LuaCore.h"

// just here until i store this a better way
BitSet128 zero = BitSet128(0, 0, 0, 0);

extern "C" {
	void L_EntityKnife_AddTearFlags(Entity_Knife* knife, BitSet128* flags) {
		knife->_tearFlags &= *flags;
	}

	void L_EntityKnife_ClearTearFlags(Entity_Knife* knife, BitSet128* flags) {
		knife->_tearFlags &= ~(*flags);
	}

	float L_EntityKnife_GetKnifeDistance(Entity_Knife* knife) {
		return knife->_distance;
	}

	float L_EntityKnife_GetKnifeVelocity(Entity_Knife* knife) {
		return knife->_velocity;
	}

	int L_EntityKnife_GetRenderZ(Entity_Knife* knife) {
		return knife->GetRenderZ();
	}

	bool L_EntityKnife_HasTearFlags(Entity_Knife* knife, BitSet128* flags) {
		return (knife->_tearFlags & *flags) != zero;
	}

	bool L_EntityKnife_IsFlying(Entity_Knife* knife) {
		return knife->_isFlying;
	}

	void L_EntityKnife_Reset(Entity_Knife* knife) {
		knife->Reset();
	}

	void L_EntityKnife_SetPathFollowSpeed(Entity_Knife* knife, float value) {
		knife->_pathFollowSpeed = value;
	}

	void L_EntityKnife_Shoot(Entity_Knife* knife, float charge, float range) {
		knife->Shoot(charge, range);
	}

	float L_EntityKnife_GetCharge(Entity_Knife* knife) {
		return knife->_charge;
	}

	void L_EntityKnife_SetCharge(Entity_Knife* knife, float value) {
		knife->_charge = value;
	}

	float L_EntityKnife_GetMaxDistance(Entity_Knife* knife) {
		return knife->_maxDistance;
	}

	void L_EntityKnife_SetMaxDistance(Entity_Knife* knife, float value) {
		knife->_maxDistance = value;
	}

	float L_EntityKnife_GetPathFollowSpeed(Entity_Knife* knife) {
		return knife->_pathFollowSpeed;
	}

	void L_EntityKnife_SetPathFollowSpeed(Entity_Knife* knife, float value) {
		knife->_pathFollowSpeed = value;
	}

	float L_EntityKnife_GetPathOffset(Entity_Knife* knife) {
		return knife->_pathOffset;
	}

	void L_EntityKnife_SetPathOffset(Entity_Knife* knife, float value) {
		knife->_pathOffset = value;
	}

	float L_EntityKnife_GetRotation(Entity_Knife* knife) {
		return knife->_rotation;
	}

	void L_EntityKnife_SetRotation(Entity_Knife* knife, float value) {
		knife->_rotation = value;
	}

	float L_EntityKnife_GetRotationOffset(Entity_Knife* knife) {
		return knife->_rotationOffset;
	}

	void L_EntityKnife_SetRotationOffset(Entity_Knife* knife, float value) {
		knife->_rotationOffset = value;
	}

	float L_EntityKnife_GetScale(Entity_Knife* knife) {
		return knife->_scale;
	}

	void L_EntityKnife_SetScale(Entity_Knife* knife, float value) {
		knife->_scale = value;
	}

	BitSet128* L_EntityKnife_GetTearFlags(Entity_Knife* knife) {
		return &knife->_tearFlags;
	}

	void L_EntityKnife_SetTearFlags(Entity_Knife* knife, BitSet128* value) {
		knife->_tearFlags = *value;
	}
}