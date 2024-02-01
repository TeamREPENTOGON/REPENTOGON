#include "IsaacRepentance.h"
#include "LuaCore.h"

const BitSet128 zero(0, 0, 0, 0);

extern "C" {
	BitSet128* L_EntityBomb_GetTearFlags(Entity_Bomb* bomb) {
		return &bomb->_tearFlags;
	}

	void L_EntityBomb_SetTearFlags(Entity_Bomb* bomb, BitSet128 flags) {
		bomb->_tearFlags = flags;
	}

	void L_EntityBomb_AddTearFlags(Entity_Bomb* bomb, BitSet128 flags) {
		bomb->_tearFlags &= flags;
	}

	void L_EntityBomb_ClearTearFlags(Entity_Bomb* bomb, BitSet128 flags) {
		bomb->_tearFlags &= ~flags;
	}

	bool L_EntityBomb_HasTearFlags(Entity_Bomb* bomb, BitSet128 flags) {
		return (bomb->_tearFlags & flags) != zero;
	}

	int L_EntityBomb_SetExplosionCountdown(Entity_Bomb* bomb, int value) {
		bomb->_explosionCountdown1 = value;
		bomb->_explosionCountdown2 = value;
	}

	float L_EntityBomb_GetExplosionDamage(Entity_Bomb* bomb) {
		return bomb->_explosionDamage;
	}

	void L_EntityBomb_SetExplosionDamage(Entity_Bomb* bomb, float damage) {
		bomb->_explosionDamage = damage;
	}

	bool L_EntityBomb_GetIsFetus(Entity_Bomb* bomb) {
		return bomb->_isFetus;
	}

	void L_EntityBomb_SetIsFetus(Entity_Bomb* bomb, bool value) {
		bomb->_isFetus = value;
	}

	float L_EntityBomb_GetRadiusMultiplier(Entity_Bomb* bomb) {
		return bomb->_radiusMultiplier;
	}

	void L_EntityBomb_SetRadiusMultiplier(Entity_Bomb* bomb, float value) {
		bomb->_radiusMultiplier = value;
	}
}