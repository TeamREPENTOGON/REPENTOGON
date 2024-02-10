#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_EntityLaser_AddTearFlags(Entity_Laser* laser, BitSet128* flags) {
		laser->_tearFlags &= *flags;
	}

	void L_EntityLaser_ClearTearFlags(Entity_Laser* laser, BitSet128* flags) {
		laser->_tearFlags &= ~(*flags);
	}

	bool L_EntityLaser_HasTearFlags(Entity_Laser* laser, BitSet128* flags) {
		return (laser->_tearFlags & *flags).AnyFlagsSet();
	}

	// TODO: VectorList / Get[NonOptimized]Samples

	// TODO: GetRenderZ returns a float but docs say int. does this mean RenderZOffset?
	float L_EntityLaser_GetRenderZ(Entity_Laser* laser) {
		return laser->GetRenderZ();
	}

	bool L_EntityLaser_IsCircleLaser(Entity_Laser* laser) {
		return (laser->_subtype == 1 || laser->_subtype == 2 || laser->_subtype == 3);
	}

	// just use GetSampleLaser

	void L_EntityLaser_SetActiveRotation(Entity_Laser* laser, int delay, float angleDegrees, float rotationSpeed, bool timeoutComplete) {
		laser->SetActiveRotation(delay, angleDegrees, rotationSpeed, timeoutComplete);
	}

	void L_EntityLaser_SetMultidimensionalTouched(Entity_Laser* laser, bool value) {
		laser->_multidimensionalTouched = value;
	}

	Entity_Laser* L_EntityLaser_ShootAngle(int variant, Vector* sourcePos, float angleDegrees, int timeout, Vector* posOffset, Entity* source) {
		return Entity_Laser::ShootAngle(variant, sourcePos, angleDegrees, timeout, posOffset, source, false);
	}

	// TODO: wait, where's Angle?
	/*
	float L_EntityLaser_GetAngle(Entity_Laser* laser) {
		return laser->_angle;
	}

	void L_EntityLaser_SetAngle(Entity_Laser* laser, float value) {
		laser->_angle = value;
	}
	*/

	float L_EntityLaser_GetAngleDegrees(Entity_Laser* laser) {
		return laser->_angleDegrees;
	}

	void L_EntityLaser_SetAngleDegrees(Entity_Laser* laser, float value) {
		laser->_angleDegrees = value;
	}

	float L_EntityLaser_GetBlackHpDropChance(Entity_Laser* laser) {
		return laser->_blackHpDropChance;
	}

	void L_EntityLaser_SetBlackHpDropChance(Entity_Laser* laser, float value) {
		laser->_blackHpDropChance = value;
	}

	Entity* L_EntityLaser_GetBounceLaser(Entity_Laser* laser) {
		return laser->_bounceLaser;
	}

	void L_EntityLaser_SetBounceLaser(Entity_Laser* laser, Entity* value) {
		laser->_bounceLaser = value;
	}

	float L_EntityLaser_GetCurveStrength(Entity_Laser* laser) {
		return laser->_curveStrength;
	}

	void L_EntityLaser_SetCurveStrength(Entity_Laser* laser, float value) {
		laser->_curveStrength = value;
	}

	bool L_EntityLaser_GetDisableFollowParent(Entity_Laser* laser) {
		return laser->_curveStrength;
	}

	void L_EntityLaser_SetDisableFollowParent(Entity_Laser* laser, bool value) {
		laser->_disableFollowParent = value;
	}

	Vector* L_EntityLaser_GetEndPoint(Entity_Laser* laser) {
		return &laser->_endPoint;
	}

	void L_EntityLaser_SetEndPoint(Entity_Laser* laser, Vector* value) {
		laser->_endPoint = *value;
	}

	bool L_EntityLaser_GetFirstUpdate(Entity_Laser* laser) {
		return laser->_firstUpdate;
	}

	void L_EntityLaser_SetFirstUpdate(Entity_Laser* laser, bool value) {
		laser->_firstUpdate = value;
	}

	bool L_EntityLaser_GetGridHit(Entity_Laser* laser) {
		return laser->_gridHit;
	}

	void L_EntityLaser_SetGridHit(Entity_Laser* laser, bool value) {
		laser->_gridHit = value;
	}

	HomingLaser* L_EntityLaser_GetHomingLaser(Entity_Laser* laser) {
		return &laser->_homingLaser;
	}

	int L_EntityLaser_GetHomingType(Entity_Laser* laser) {
		return laser->_homingType;
	}

	void L_EntityLaser_SetHomingType(Entity_Laser* laser, int value) {
		laser->_homingType = value;
	}

	bool L_EntityLaser_GetIsActiveRotating(Entity_Laser* laser) {
		return laser->_isActiveRotating;
	}

	void L_EntityLaser_SetIsActiveRotating(Entity_Laser* laser, bool value) {
		laser->_isActiveRotating = value;
	}

	float L_EntityLaser_GetLaserLength(Entity_Laser* laser) {
		return laser->_laserLength;
	}

	void L_EntityLaser_SetLaserLength(Entity_Laser* laser, float value) {
		laser->_laserLength = value;
	}

	float L_EntityLaser_GetLastAngleDegrees(Entity_Laser* laser) {
		return laser->_lastAngleDegrees;
	}

	void L_EntityLaser_SetLastAngleDegrees(Entity_Laser* laser, float value) {
		laser->_lastAngleDegrees = value;
	}

	float L_EntityLaser_GetMaxDistance(Entity_Laser* laser) {
		return laser->_maxDistance;
	}

	bool L_EntityLaser_GetOneHit(Entity_Laser* laser) {
		return laser->_oneHit;
	}

	void L_EntityLaser_SetOneHit(Entity_Laser* laser, bool value) {
		laser->_oneHit = value;
	}

	Vector* L_EntityLaser_GetParentOffset(Entity_Laser* laser) {
		return &laser->_parentOffset;
	}

	void L_EntityLaser_SetParentOffset(Entity_Laser* laser, Vector* value) {
		laser->_parentOffset = *value;
	}

	float L_EntityLaser_GetRadius(Entity_Laser* laser) {
		return laser->_radius;
	}

	void L_EntityLaser_SetRadius(Entity_Laser* laser, float value) {
		laser->_radius = value;
	}

	float L_EntityLaser_GetRotationDegrees(Entity_Laser* laser) {
		return laser->_rotationDegrees;
	}

	void L_EntityLaser_SetRotationDegrees(Entity_Laser* laser, float value) {
		laser->_rotationDegrees = value;
	}

	int L_EntityLaser_GetRotationDelay(Entity_Laser* laser) {
		return laser->_rotationDelay;
	}

	void L_EntityLaser_SetRotationDelay(Entity_Laser* laser, int value) {
		laser->_rotationDelay = value;
	}

	float L_EntityLaser_GetRotationSpeed(Entity_Laser* laser) {
		return laser->_rotationSpeed;
	}

	void L_EntityLaser_SetRotationSpeed(Entity_Laser* laser, float value) {
		laser->_rotationSpeed = value;
	}

	bool L_EntityLaser_GetSampleLaser(Entity_Laser* laser) {
		return laser->_sampleLaser;
	}

	void L_EntityLaser_SetSampleLaser(Entity_Laser* laser, bool value) {
		laser->_sampleLaser = value;
	}

	bool L_EntityLaser_GetShrink(Entity_Laser* laser) {
		return laser->_shrink;
	}

	void L_EntityLaser_SetShrink(Entity_Laser* laser, bool value) {
		laser->_shrink = value;
	}

	float L_EntityLaser_GetStartAngleDegrees(Entity_Laser* laser) {
		return laser->_startAngleDegrees;
	}

	void L_EntityLaser_SetStartAngleDegrees(Entity_Laser* laser, float value) {
		laser->_startAngleDegrees = value;
	}

	BitSet128* L_EntityLaser_GetTearFlags(Entity_Laser* laser) {
		return &laser->_tearFlags;
	}

	void L_EntityLaser_SetTearFlags(Entity_Laser* laser, BitSet128* flags) {
		laser->_tearFlags = *flags;
	}

	int L_EntityLaser_GetTimeout(Entity_Laser* laser) {
		return laser->_timeout;
	}

	void L_EntityLaser_SetTimeout(Entity_Laser* laser, int value) {
		laser->_timeout = value;
	}
}