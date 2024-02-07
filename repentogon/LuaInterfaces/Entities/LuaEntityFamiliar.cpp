#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_EntityFamiliar_AddCoins(Entity_Familiar* familiar, int value) {
		familiar->AddCoins(value);
	}

	void L_EntityFamiliar_AddHearts(Entity_Familiar* familiar, int value) {
		familiar->_hearts += value;
	}

	void L_EntityFamiliar_AddKeys(Entity_Familiar* familiar, int value) {
		familiar->_keys += value;
	}

	void L_EntityFamiliar_AddToDelayed(Entity_Familiar* familiar) {
		familiar->AddToDelayed();
	}

	void L_EntityFamiliar_AddToFollowers(Entity_Familiar* familiar) {
		familiar->AddToFollowers();
	}

	void L_EntityFamiliar_AddToOrbit(Entity_Familiar* familiar, int layer) {
		familiar->AddToOrbit(layer);
	}

	// TODO: investigate default unk value
	Entity_Tear* L_EntityFamiliar_FireProjectile(Entity_Familiar* familiar, Vector* dir) {
		return familiar->FireProjectile(*dir, false);
	}

	void L_EntityFamiliar_FollowParent(Entity_Familiar* familiar) {
		familiar->FollowParent();
	}

	void L_EntityFamiliar_FollowPosition(Entity_Familiar* familiar, Vector* pos) {
		familiar->FollowPosition(pos);
	}

	Vector* L_EntityFamiliar_StaticGetOrbitDistance(int layer) {
		return Entity_Familiar::GetOrbitDistance(layer);
	}

	Vector* L_EntityFamiliar_GetOrbitPosition(Entity_Familiar* familiar, Vector* pos) {
		Vector ret;
		familiar->GetOrbitPosition(&ret, pos);
		return &ret;
	}

	void L_EntityFamiliar_MoveDelayed(Entity_Familiar* familiar, int value) {
		familiar->MoveDelayed(value);
	}

	void L_EntityFamiliar_MoveDiagonally(Entity_Familiar* familiar, float speed) {
		familiar->MoveDiagonally(speed);
	}

	void L_EntityFamiliar_PickEnemyTarget(Entity_Familiar* familiar, float maxDistance, int frameInterval, int flags, Vector* coneDir, float coneAngle) {
		familiar->PickEnemyTarget(maxDistance, frameInterval, flags, coneDir, coneAngle);
	}

	void L_EntityFamiliar_PlayChargeAnim(Entity_Familiar* familiar, unsigned int dir) {
		familiar->PlayChargeAnim(dir);
	}

	void L_EntityFamiliar_PlayFloatAnim(Entity_Familiar* familiar, unsigned int dir) {
		familiar->PlayFloatAnim(dir);
	}

	void L_EntityFamiliar_PlayShootAnim(Entity_Familiar* familiar, unsigned int dir) {
		familiar->PlayShootAnim(dir);
	}

	unsigned int L_EntityFamiliar_RecalculateOrbitOffset(Entity_Familiar* familiar, int layer, bool add) {
		return familiar->RecalculateOrbitOffset(layer, add);
	}

	void L_EntityFamiliar_RemoveFromDelayed(Entity_Familiar* familiar) {
		familiar->RemoveFromDelayed();
	}

	void L_EntityFamiliar_RemoveFromFollowers(Entity_Familiar* familiar) {
		familiar->RemoveFromFollowers();
	}

	void L_EntityFamiliar_RemoveFromOrbit(Entity_Familiar* familiar) {
		familiar->RemoveFromOrbit();
	}

	void L_EntityFamiliar_Shoot(Entity_Familiar* familiar) {
		familiar->Shoot();
	}

	int L_EntityFamiliar_GetCoins(Entity_Familiar* familiar) {
		return familiar->_coins;
	}

	void L_EntityFamiliar_SetCoins(Entity_Familiar* familiar, int value) {
		familiar->_coins = value;
	}

	unsigned int L_EntityFamiliar_GetFireCooldown(Entity_Familiar* familiar) {
		return familiar->_fireCooldown;
	}

	void L_EntityFamiliar_SetFireCooldown(Entity_Familiar* familiar, unsigned int value) {
		familiar->_fireCooldown = value;
	}

	unsigned int L_EntityFamiliar_GetHeadFrameDelay(Entity_Familiar* familiar) {
		return familiar->_headFrameDelay;
	}

	void L_EntityFamiliar_SetHeadFrameDelay(Entity_Familiar* familiar, int value) {
		familiar->_headFrameDelay = value;
	}

	int L_EntityFamiliar_GetHearts(Entity_Familiar* familiar) {
		return familiar->_hearts;
	}

	void L_EntityFamiliar_SetHearts(Entity_Familiar* familiar, int value) {
		familiar->_hearts = value;
	}

	bool L_EntityFamiliar_GetIsDelayed(Entity_Familiar* familiar) {
		return familiar->_isDelayed;
	}

	void L_EntityFamiliar_SetIsDelayed(Entity_Familiar* familiar, bool value) {
		familiar->_isDelayed = value;
	}

	bool L_EntityFamiliar_GetIsFollower(Entity_Familiar* familiar) {
		return familiar->_isFollower;
	}

	void L_EntityFamiliar_SetIsFollower(Entity_Familiar* familiar, bool value) {
		familiar->_isFollower = value;
	}

	int L_EntityFamiliar_GetKeys(Entity_Familiar* familiar) {
		return familiar->_keys;
	}

	void L_EntityFamiliar_SetKeys(Entity_Familiar* familiar, int value) {
		familiar->_keys = value;
	}

	unsigned int L_EntityFamiliar_GetLastDirection(Entity_Familiar* familiar) {
		return familiar->_lastDirection;
	}

	void L_EntityFamiliar_SetLastDirection(Entity_Familiar* familiar, unsigned int value) {
		familiar->_lastDirection = value;
	}

	unsigned int L_EntityFamiliar_GetMoveDirection(Entity_Familiar* familiar) {
		return familiar->_moveDirection;
	}

	void L_EntityFamiliar_SetMoveDirection(Entity_Familiar* familiar, unsigned int value) {
		familiar->_moveDirection = value;
	}

	float L_EntityFamiliar_GetOrbitAngleOffset(Entity_Familiar* familiar) {
		return familiar->_orbitAngleOffset;
	}

	void L_EntityFamiliar_SetOrbitAngleOffset(Entity_Familiar* familiar, float value) {
		familiar->_orbitAngleOffset = value;
	}

	Vector* L_EntityFamiliar_GetOrbitDistance(Entity_Familiar* familiar) {
		return &familiar->_orbitDistance;
	}

	void L_EntityFamiliar_SetOrbitDistance(Entity_Familiar* familiar, Vector* value) {
		familiar->_orbitDistance = *value;
	}

	int L_EntityFamiliar_GetOrbitLayer(Entity_Familiar* familiar) {
		return familiar->_orbitLayer;
	}

	void L_EntityFamiliar_SetOrbitLayer(Entity_Familiar* familiar, int value) {
		familiar->_orbitLayer = value;
	}

	float L_EntityFamiliar_GetOrbitSpeed(Entity_Familiar* familiar) {
		return familiar->_orbitSpeed;
	}

	void L_EntityFamiliar_SetOrbitSpeed(Entity_Familiar* familiar, float value) {
		familiar->_orbitSpeed = value;
	}

	Entity_Player* L_EntityFamiliar_GetPlayer(Entity_Familiar* familiar) {
		return familiar->_player;
	}

	void L_EntityFamiliar_SetPlayer(Entity_Familiar* familiar, Entity_Player* player) {
		familiar->_player = player;
	}

	int L_EntityFamiliar_GetRoomClearCount(Entity_Familiar* familiar) {
		return familiar->_roomClearCount;
	}

	void L_EntityFamiliar_SetRoomClearCount(Entity_Familiar* familiar, int value) {
		familiar->_roomClearCount = value;
	}

	unsigned int L_EntityFamiliar_GetShootDirection(Entity_Familiar* familiar) {
		return familiar->_shootDirection;
	}

	void L_EntityFamiliar_SetShootDirection(Entity_Familiar* familiar, unsigned int value) {
		familiar->_shootDirection = value;
	}

	int L_EntityFamiliar_GetState(Entity_Familiar* familiar) {
		return familiar->_state;
	}

	void L_EntityFamiliar_SetState(Entity_Familiar* familiar, int value) {
		familiar->_state = value;
	}
}