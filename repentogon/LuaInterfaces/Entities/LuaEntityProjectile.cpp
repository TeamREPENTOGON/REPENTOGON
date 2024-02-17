#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_EntityProjectile_AddChangeFlags(Entity_Projectile* projectile, lua_Integer flags) {
		projectile->_changeFlags &= flags;
	}

	void L_EntityProjectile_AddFalingAccel(Entity_Projectile* projectile, float value) {
		projectile->_fallingAccel += value;
	}

	void L_EntityProjectile_AddFallingSpeed(Entity_Projectile* projectile, float value) {
		projectile->_fallingSpeed += value;
	}

	void L_EntityProjectile_AddHeight(Entity_Projectile* projectile, float value) {
		projectile->_height += value;
	}

	void L_EntityProjectile_AddProjectileFlags(Entity_Projectile* projectile, lua_Integer flags) {
		projectile->_projectileFlags &= flags;
	}

	void L_EntityProjectile_AddScale(Entity_Projectile* projectile, float value) {
		projectile->_projectileScale += value;
	}

	void L_EntityProjectile_ClearProjectileFLags(Entity_Projectile* projectile, lua_Integer flags) {
		projectile->_projectileFlags &= ~(flags);
	}

	bool L_EntityProjectile_HasProjectileFlags(Entity_Projectile* projectile, lua_Integer flags) {
		return (projectile->_projectileFlags & flags) != 0;
	}

	float L_EntityProjectile_GetAcceleration(Entity_Projectile* projectile) {
		return projectile->_accel;
	}

	void L_EntityProjectile_SetAcceleration(Entity_Projectile* projectile, float value) {
		projectile->_accel = value;
	}

	lua_Integer L_EntityProjectile_GetChangeFlags(Entity_Projectile* projectile) {
		return projectile->_changeFlags;
	}

	void L_EntityProjectile_SetChangeFlags(Entity_Projectile* projectile, lua_Integer flags) {
		projectile->_changeFlags = flags;
	}

	int L_EntityProjectile_GetChangeTimeout(Entity_Projectile* projectile) {
		return projectile->_changeTimeout;
	}

	void L_EntityProjectile_SetChangeTimeout(Entity_Projectile* projectile, int value) {
		projectile->_changeTimeout = value;
	}

	float L_EntityProjectile_GetChangeVelocity(Entity_Projectile* projectile) {
		return projectile->_changeVelocity;
	}

	void L_EntityProjectile_SetChangeVelocity(Entity_Projectile* projectile, float value) {
		projectile->_changeVelocity = value;
	}

	float L_EntityProjectile_GetCurvingStrength(Entity_Projectile* projectile) {
		return projectile->_curvingStrength;
	}

	void L_EntityProjectile_SetCurvingStrength(Entity_Projectile* projectile, float value) {
		projectile->_curvingStrength = value;
	}

	float L_EntityProjectile_GetDamage(Entity_Projectile* projectile) {
		return projectile->_damage;
	}

	void L_EntityProjectile_SetDamage(Entity_Projectile* projectile, float value) {
		projectile->_damage = value;
	}

	float L_EntityProjectile_GetDepthOffset(Entity_Projectile* projectile) {
		return projectile->_depthOffset;
	}

	void L_EntityProjectile_SetDepthOffset(Entity_Projectile* projectile, float value) {
		projectile->_depthOffset = value;
	}

	float L_EntityProjectile_GetFallingAccel(Entity_Projectile* projectile) {
		return projectile->_fallingAccel;
	}

	void L_EntityProjectile_SetFallingAccel(Entity_Projectile* projectile, float value) {
		projectile->_fallingAccel = value;
	}

	float L_EntityProjectile_GetFallingSpeed(Entity_Projectile* projectile) {
		return projectile->_fallingSpeed;
	}

	void L_EntityProjectile_SetFallingSpeed(Entity_Projectile* projectile, float value) {
		projectile->_fallingSpeed = value;
	}

	float L_EntityProjectile_GetHeight(Entity_Projectile* projectile) {
		return projectile->_height;
	}

	void L_EntityProjectile_SetHeight(Entity_Projectile* projectile, float value) {
		projectile->_height = value;
	}

	float L_EntityProjectile_GetHomingStrength(Entity_Projectile* projectile) {
		return projectile->_homingStrength;
	}

	void L_EntityProjectile_SetHomingStrength(Entity_Projectile* projectile, float value) {
		projectile->_homingStrength = value;
	}

	lua_Integer L_EntityProjectile_GetProjectileFlags(Entity_Projectile* projectile) {
		return projectile->_projectileFlags;
	}

	void L_EntityProjectile_SetProjectileFlags(Entity_Projectile* projectile, lua_Integer flags) {
		projectile->_projectileFlags = flags;
	}

	float L_EntityProjectile_GetScale(Entity_Projectile* projectile) {
		return projectile->_projectileScale;
	}

	void L_EntityProjectile_SetScale(Entity_Projectile* projectile, float value) {
		projectile->_projectileScale = value;
	}

	int L_EntityProjectile_GetWiggleFrameOffset(Entity_Projectile* projectile) {
		return projectile->_wiggleFrameOffset;
	}

	void L_EntityProjectile_SetWiggleFrameOffset(Entity_Projectile* projectile, int value) {
		projectile->_wiggleFrameOffset = value;
	}
}