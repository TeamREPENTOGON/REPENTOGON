#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_Entity_AddBurn(Entity* ent, EntityRef* ref, int duration, float damage) {
		ent->AddBurn(*ref, duration, damage);
	}

	void L_Entity_AddCharm(Entity* ent, EntityRef* ref, int duration) {
		ent->AddCharmed(*ref, duration);
	}

	void L_Entity_AddConfusion(Entity* ent, EntityRef* ref, int duration) {
		ent->AddConfusion(*ref, duration);
	}

	void L_Entity_AddEntityFlags(Entity* ent, lua_Integer flags) {
		ent->_flags &= flags;
	}

	void L_Entity_AddFreeze(Entity* ent, EntityRef* ref, int duration) {
		ent->AddFreeze(*ref, duration);
	}

	void L_Entity_AddHealth(Entity* ent, float health) {
		ent->_health = min(ent->_health + health, ent->_maxHealth);
	}

	void L_Entity_AddMidasFreeze(Entity* ent, EntityRef* ref, int duration) {
		ent->AddMidasFreeze(*ref, duration);
	}

	void L_Entity_AddPoison(Entity* ent, EntityRef* ref, int duration, float damage) {
		ent->AddPoison(*ref, duration, damage);
	}

	void L_Entity_AddShrink(Entity* ent, EntityRef* ref, int duration) {
		ent->AddShrink(*ref, duration);
	}

	//todo: investigate AddSlowing unk value
	void L_Entity_AddSlowing(Entity* ent, EntityRef* ref, int duration, ColorMod* color) {
		ent->AddSlowing(*ref, duration, 0, color);
	}

	//todo: investigate useSpeedModifer default value
	void L_Entity_AddVelocity(Entity* ent, Vector* velocity) {
		ent->AddVelocity(velocity, false);
	}

	void L_Entity_BloodExplode(Entity* ent) {
		ent->BloodExplode();
	}
	
	//todo: i'd like to be able to properly call the vtable for this,
	//but there's no chance of getting a unique signature for "return false"
	bool L_Entity_CanShutDoors(Entity* ent) {
		return false;
	}

	void L_Entity_ClearEntityFlags(Entity* ent, lua_Integer flags) {
		ent->_flags &= ~flags;
	}

	bool L_Entity_CollidesWithGrid(Entity* ent) {
		return ent->_collidesWithGrid;
	}

	void L_Entity_Die(Entity* ent) {
		ent->_isDead = true;
	}

	bool L_Entity_Exists(Entity* ent) {
		return ent->_exists;
	}

	int L_Entity_GetBossID(Entity* ent) {
		return ent->_entityConfig->bossID;
	}

	//wack
	ColorMod* L_Entity_GetColorFunc(Entity* ent) {
		return &ent->_sprite._color;
	}

	//////////
	// TODO: GetData
	//////////

	RNG* L_Entity_GetDropRNG(Entity* ent) {
		return &ent->_dropRNG;
	}

	// lua_Integer needs to become 64 bit
	lua_Integer L_Entity_GetEntityFlags(Entity* ent) {
		return ent->_flags;
	}

	Entity* L_Entity_GetLastChild(Entity* ent) {
		Entity* child = nullptr;
		while (child = ent->_child, child != nullptr) {
			child = child->_child;
		}
		return child;
	}

	Entity* L_Entity_GetLastParent(Entity* ent) {
		Entity* parent = nullptr;
		while (parent = ent->_parent, parent != nullptr) {
			parent = parent->_parent;
		}
		return parent;
	}

	ANM2* L_Entity_GetSprite(Entity* ent) {
		return &ent->_sprite;
	}

	bool L_Entity_HasCommonParentWithEntity(Entity* ent, Entity* right) {
		return ent->HasCommonParentWithEntity(right);
	}

	bool L_Entity_HasEntityFlags(Entity* ent, lua_Integer flags) {
		return (ent->_flags | flags) != 0;
	}

	bool L_Entity_HasFullHealth(Entity* ent) {
		return ent->_health >= ent->_maxHealth;
	}

	bool L_Entity_HasMortalDamage(Entity* ent) {
		return ent->_health <= ent->_mortalHealth;
	}

	bool L_Entity_IsActiveEnemy(Entity* ent, bool includeDead) {
		return ent->IsActiveEnemy(includeDead);
	}

	//todo: vtable func
	bool L_Entity_IsBoss(Entity* ent) {
		return false;
	}
	
	bool L_Entity_IsDead(Entity* ent) {
		return ent->_isDead;
	}

	bool L_Entity_IsEnemy(Entity* ent) {
		return ent->_type - 10 < 990;
	}

	bool L_Entity_IsFlying(Entity* ent) {
		return ent->IsFlying();
	}

	bool L_Entity_IsFrame(Entity* ent, int frame, int offset) {
		return ent->IsFrame(frame, offset);
	}

	bool L_Entity_IsInvincible(Entity* ent) {
		return ent->_invincible;
	}

	bool L_Entity_IsVisible(Entity* ent) {
		return ent->_visible;
	}

	//todo: investigate unk
	bool L_Entity_IsVulnerableEnemy(Entity* ent) {
		return ent->IsVulnerableEnemy(nullptr);
	}

	void L_Entity_Kill(Entity* ent) {
		ent->Kill();
	}

	void L_Entity_MultiplyFriction(Entity* ent, float value) {
		ent->_friction *= value;
	}

	void L_Entity_PostRender(Entity* ent) {
		ent->PostRender();
	}

	void L_Entity_Remove(Entity* ent) {
		ent->Remove();
	}

	void L_Entity_RemoveStatusEffects(Entity* ent) {
		ent->RemoveStatusEffects();
	}

	void L_Entity_Render(Entity* ent, Vector* offset) {
		ent->Render(offset);
	}

	bool L_Entity_RenderShadowLayer(Entity* ent, Vector* offset) {
		return ent->RenderShadowLayer(offset);
	}

	void L_Entity_SetColorFunc(Entity* ent, ColorMod* color, int duration, int priority, bool fadeout, bool share) {
		ent->SetColor(color, duration, priority, fadeout, share);
	}

	void L_Entity_SetSizeFunc(Entity* ent, float size, Vector* sizeMulti, int numGridPoints) {
		ent->SetSize(size, *sizeMulti, numGridPoints);
	}

	void L_Entity_SetSpriteFrame(Entity* ent, const char* name, int frame) {
		std::string namestr = name;
		ent->SetSpriteFrame(&namestr, frame);
	}

	bool L_Entity_TakeDamage(Entity* ent, float damage, lua_Integer flags, EntityRef* source, int countdown) {
		return ent->TakeDamage(damage, flags, source, countdown);
	}

	void L_Entity_Update(Entity* ent) {
		ent->Update();
	}

	Entity_Bomb* L_Entity_ToBomb(Entity* ent) {
		return ent->ToBomb();
	}

	Entity_Effect* L_Entity_ToEffect(Entity* ent) {
		return ent->ToEffect();
	}

	Entity_Familiar* L_Entity_ToFamiliar(Entity* ent) {
		return ent->ToFamiliar();
	}

	Entity_Knife* L_Entity_ToKnife(Entity* ent) {
		return ent->ToKnife();
	}

	Entity_Laser* L_Entity_ToLaser(Entity* ent) {
		return ent->ToLaser();
	}

	Entity_NPC* L_Entity_ToNPC(Entity* ent) {
		return ent->ToNPC();
	}

	Entity_Pickup* L_Entity_ToPickup(Entity* ent) {
		return ent->ToPickup();
	}

	Entity_Player* L_Entity_ToPlayer(Entity* ent) {
		return ent->ToPlayer();
	}

	Entity_Projectile* L_Entity_ToProjectile(Entity* ent) {
		return ent->ToProjectile();
	}

	/*
	// not sure how to get a clean reference to Entity_Slot rtti type descriptor
	Entity_Slot* L_Entity_ToSlot(Entity* ent) {
		return ent->ToSlot();
	}
	*/

	Entity_Tear* L_Entity_ToTear(Entity* ent) {
		return ent->ToTear();
	}

	Entity* L_Entity_GetChild(Entity* ent) {
		return ent->_child;
	}

	void L_Entity_SetChild(Entity* ent, Entity* child) {
		ent->_child = child;
	}

	Entity* L_Entity_GetParent(Entity* ent) {
		return ent->_parent;
	}

	void L_Entity_SetParent(Entity* ent, Entity* parent) {
		ent->_parent = parent;
	}

	float L_Entity_GetCollisionDamage(Entity* ent) {
		return ent->_collisionDamage;
	}

	void L_Entity_SetCollisionDamage(Entity* ent, float value) {
		ent->_collisionDamage = value;
	}

	ColorMod* L_Entity_GetColorField(Entity* ent) {
		return &ent->_color;
	}

	void L_Entity_SetColorField(Entity* ent, ColorMod* color) {
		ent->_color = *color;
	}

	float L_Entity_GetDepthOffset(Entity* ent) {
		return ent->_depthOffset;
	}

	void L_Entity_SetDepthOffset(Entity* ent, float offset) {
		ent->_depthOffset = offset;
	}

	unsigned int L_Entity_GetDropSeed(Entity* ent) {
		return ent->_dropRNG._seed;
	}

	void L_Entity_SetDropSeed(Entity* ent, unsigned int seed) {
		ent->_dropRNG._seed = seed;
	}

	unsigned int L_Entity_GetEntityCollisionClass(Entity* ent) {
		return ent->_entityCollisionClass;
	}

	void L_Entity_SetEntityCollisionClass(Entity* ent, unsigned int collClass) {
		ent->_entityCollisionClass = collClass;
	}

	unsigned int L_Entity_GetGridCollisionClass(Entity* ent) {
		return ent->_gridCollisionClass;
	}

	void L_Entity_SetGridCollisionClass(Entity* ent, unsigned int collClass) {
		ent->_gridCollisionClass = collClass;
	}

	bool L_Entity_GetFlipX(Entity* ent) {
		return ent->_sprite._flipX;
	}

	void L_Entity_SetFlipX(Entity* ent, bool value) {
		ent->_sprite._flipX = value;
	}

	unsigned int L_Entity_GetFrameCount(Entity* ent) {
		return g_Game->_frameCount - ent->_frameCount;
	}

	void L_Entity_SetFrameCount(Entity* ent, unsigned int frame) {
		ent->_frameCount = frame;
	}

	float L_Entity_GetFriction(Entity* ent) {
		return ent->_friction;
	}

	void L_Entity_SetFriction(Entity* ent, float value) {
		ent->_friction = value;
	}

	float L_Entity_GetHitPoints(Entity* ent) {
		return ent->_health;
	}

	void L_Entity_SetHitPoints(Entity* ent, float value) {
		ent->_health = value;
	}

	float L_Entity_GetMaxHitPoints(Entity* ent) {
		return ent->_maxHealth;
	}

	void L_Entity_SetMaxHitPoints(Entity* ent, float value) {
		ent->_maxHealth = value;
	}

	unsigned int L_Entity_GetIndex(Entity* ent) {
		return ent->_index;
	}

	// no setter here

	unsigned int L_Entity_GetInitSeed(Entity* ent) {
		return ent->_initSeed;
	}

	void L_Entity_SetInitSeed(Entity* ent, unsigned int seed) {
		ent->_initSeed = seed;
	}

	float L_Entity_GetMass(Entity* ent) {
		return ent->_mass;
	}

	void L_Entity_SetMass(Entity* ent, float value) {
		ent->_mass = value;
	}

	Vector* L_Entity_GetPosition(Entity* ent) {
		return &ent->_position;
	}

	void L_Entity_SetPosition(Entity* ent, Vector* value) {
		ent->_position = *value;
	}

	Vector* L_Entity_GetPositionOffset(Entity* ent) {
		return &ent->_positionOffset;
	}

	void L_Entity_SetPositionOffset(Entity* ent, Vector* value) {
		ent->_positionOffset = *value;
	}

	int L_Entity_GetRenderZOffset(Entity* ent) {
		return ent->_renderZOffset;
	}

	void L_Entity_SetRenderZOffset(Entity* ent, int offset) {
		ent->_renderZOffset = offset;
	}

	float L_Entity_GetSizeField(Entity* ent) {
		return ent->_size;
	}

	void L_Entity_SetSizeField(Entity* ent, float value) {
		ent->_size = value;
	}

	Vector* L_Entity_GetSizeMulti(Entity* ent) {
		return &ent->_sizeMulti;
	}

	void L_Entity_SetSizeMulti(Entity* ent, Vector* value) {
		ent->_sizeMulti = *value;
	}

	int L_Entity_GetSortingLayer(Entity* ent) {
		return ent->_sortingLayer;
	}

	void L_Entity_SetSortingLayer(Entity* ent, int value) {
		ent->_sortingLayer = value;
	}

	Entity* L_Entity_GetSpawnerEntity(Entity* ent) {
		return ent->_spawnerEntity;
	}

	void L_Entity_SetSpawner(Entity* ent, Entity* spawner) {
		ent->_spawnerEntity = spawner;
	}

	unsigned int L_Entity_GetSpawnerType(Entity* ent) {
		return ent->_spawnerType;
	}

	void L_Entity_SetSpawnerType(Entity* ent, unsigned int type) {
		ent->_spawnerType = type;
	}

	unsigned int L_Entity_GetSpawnerVariant(Entity* ent) {
		return ent->_spawnerVariant;
	}

	void L_Entity_SetSpawnerVariant(Entity* ent, unsigned int variant) {
		ent->_spawnerVariant = variant;
	}

	int L_Entity_GetSpawnGridIndex(Entity* ent) {
		return ent->_spawnGridIdx;
	}

	void L_Entity_SetSpawnGridIndex(Entity* ent, unsigned int idx) {
		ent->_spawnGridIdx = idx;
	}

	Vector* L_Entity_GetSpriteOffset(Entity* ent) {
		return &ent->_sprite._offset;
	}

	void L_Entity_SetSpriteOffset(Entity* ent, Vector* offset) {
		ent->_sprite._offset = *offset;
	}

	ColorMod* L_Entity_GetSplatColor(Entity* ent) {
		return &ent->_splatColor;
	}

	void L_Entity_SetSplatColor(Entity* ent, ColorMod* color) {
		ent->_splatColor = *color;
	}

	Vector* L_Entity_GetSpriteScale(Entity* ent) {
		return &ent->_sprite._scale;
	}

	void L_Entity_SetSpriteScale(Entity* ent, Vector* value) {
		ent->_sprite._scale = *value;
	}

	float L_Entity_GetSpriteRotation(Entity* ent) {
		return ent->_sprite._rotation;
	}

	void L_Entity_SetSpriteRotation(Entity* ent, float value) {
		ent->_sprite._rotation = value;
	}

	Entity* L_Entity_GetTarget(Entity* ent) {
		return ent->_target;
	}

	void L_Entity_SetTarget(Entity* ent, Entity* target) {
		ent->_target = target;
	}

	Vector* L_Entity_GetTargetPosition(Entity* ent) {
		return &ent->_targetPosition;
	}

	void L_Entity_SetTargetPosition(Entity* ent, Vector* value) {
		ent->_targetPosition = *value;
	}

	unsigned int L_Entity_GetType(Entity* ent) {
		return ent->_type;
	}

	// CHAOS CHAOS
	void L_Entity_SetType(Entity* ent, unsigned int type) {
		ent->_type = type;
	}

	unsigned int L_Entity_GetVariant(Entity* ent) {
		return ent->_variant;
	}

	void L_Entity_SetVariant(Entity* ent, unsigned int variant) {
		ent->_variant = variant;
	}

	unsigned int L_Entity_GetSubType(Entity* ent) {
		return ent->_subtype;
	}

	void L_Entity_SetSubType(Entity* ent, unsigned int subtype) {
		ent->_subtype = subtype;
	}

	Vector* L_Entity_GetVelocity(Entity* ent) {
		return &ent->_velocity;
	}

	void L_Entity_SetVelocity(Entity* ent, Vector* value) {
		ent->_velocity = *value;
	}

	bool L_Entity_GetVisible(Entity* ent) {
		return ent->_visible;
	}

	void L_Entity_SetVisible(Entity* ent, bool value) {
		ent->_visible = value;
	}
}