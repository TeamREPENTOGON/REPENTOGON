#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Patches/ASMPatches/ASMCallbacks.h"
#include "../../Patches/EntityPlus.h"

void CopyStatusEffects(Entity* ent1, Entity* ent2, bool overwrite);
void ForceCollideLaser(Entity_Laser* laser, Entity* entity);

namespace {
	template<typename Ret, typename... Args>
	Ret CallVirtual(Entity* entity, int offset, Args... args) {
		using Function = Ret(__thiscall*)(Entity*, Args...);
		void** vtable = *reinterpret_cast<void***>(entity);
		return reinterpret_cast<Function>(vtable[offset / sizeof(void*)])(entity, args...);
	}
}

MOD_EXPORT void L_Entity_Update(Entity* entity) {
	entity->Update();
}

MOD_EXPORT void L_Entity_PostRender(Entity* entity) {
	CallVirtual<void>(entity, 0x1c);
}

MOD_EXPORT void L_Entity_Render(Entity* entity, Vector* offset) {
	entity->Render(offset);
}

MOD_EXPORT bool L_Entity_RenderShadowLayer(Entity* entity, Vector* offset) {
	return entity->RenderShadowLayer(offset);
}

MOD_EXPORT bool L_Entity_TakeDamage(Entity* entity, float damage, uint64_t flags, EntityRef* source, int damageCountdown) {
	return entity->TakeDamage(damage, flags, source, damageCountdown);
}

MOD_EXPORT void L_Entity_Kill(Entity* entity) {
	EntityRef source;
	entity->Kill(&source);
}

MOD_EXPORT void L_Entity_KillWithSource(Entity* entity, EntityRef* source) {
	entity->Kill(source);
}

MOD_EXPORT void L_Entity_Remove(Entity* entity) {
	entity->Remove();
}

MOD_EXPORT void L_Entity_BloodExplode(Entity* entity) {
	entity->BloodExplode();
}

MOD_EXPORT bool L_Entity_CanShutDoors(Entity* entity) {
	return CallVirtual<bool>(entity, 0x48);
}

MOD_EXPORT bool L_Entity_IsBoss(Entity* entity) {
	return CallVirtual<bool>(entity, 0x4c);
}

MOD_EXPORT void L_Entity_SetCollisionDamage(Entity* entity, float damage) {
	CallVirtual<void>(entity, 0x40, damage);
}

MOD_EXPORT void L_Entity_SetColor(Entity* entity, ColorMod* color, int duration, int priority, bool fadeout, bool share) {
	entity->SetColor(color, duration, priority, fadeout, share);
}

MOD_EXPORT void L_Entity_AddVelocity(Entity* entity, Vector* velocity) {
	entity->AddVelocity(velocity, false);
}

MOD_EXPORT void L_Entity_SetSize(Entity* entity, float size, Vector* sizeMulti, int numGridCollisionPoints) {
	entity->SetSize(size, *sizeMulti, numGridCollisionPoints);
}

MOD_EXPORT void L_Entity_SetSizeKeepingMulti(Entity* entity, float size) {
	entity->SetSize(size, Vector(-1.0f, -1.0f), -1);
}

MOD_EXPORT bool L_Entity_TryThrow(Entity* entity, EntityRef* source, Vector* direction, float force) {
	return entity->TryThrow(*source, direction, force);
}

MOD_EXPORT void L_Entity_TeleportToRandomPosition(Entity* entity) {
	entity->TeleportToRandomPosition(true);
}

MOD_EXPORT bool L_Entity_ForceCollide(Entity* first, Entity* second, bool low) {
	if (*first->GetType() == 7) {
		ForceCollideLaser((Entity_Laser*)first, second);
	}
	if (*second->GetType() == 7) {
		ForceCollideLaser((Entity_Laser*)second, first);
	}
	return first->ForceCollide(first, second, low);
}

MOD_EXPORT void L_Entity_SetVariant(Entity* entity, unsigned int variant) {
	if (Entity_Effect* effect = entity->ToEffect()) {
		effect->_varData = BitSet128();
	}
	entity->_variant = variant;
}

MOD_EXPORT void L_Entity_SetParent(Entity* entity, Entity* parent) {
	entity->SetParent(parent);
}

MOD_EXPORT void L_Entity_SetChild(Entity* entity, Entity* child) {
	entity->SetChild(child);
}

MOD_EXPORT void L_Entity_SetTarget(Entity* entity, Entity* target) {
	entity->SetTarget(target);
}

MOD_EXPORT void L_Entity_SetSpawnerEntity(Entity* entity, Entity* spawner) {
	entity->SetSpawnerEntity(spawner);
}

MOD_EXPORT Entity* L_Entity_GetMinecart(Entity* entity) {
	return entity->GetMinecart();
}

MOD_EXPORT Entity_NPC* L_Entity_GiveMinecart(Entity* entity, Vector* position, Vector* velocity) {
	return (Entity_NPC*)entity->GiveMinecart(position, velocity);
}

MOD_EXPORT bool L_Entity_HasCommonParentWithEntity(Entity* entity, Entity* other) {
	return entity->HasCommonParentWithEntity(other);
}

MOD_EXPORT bool L_Entity_IsEnemy(Entity* entity) {
	return entity->IsEnemy();
}

MOD_EXPORT bool L_Entity_IsActiveEnemy(Entity* entity, bool includeDead) {
	return entity->IsActiveEnemy(includeDead);
}

MOD_EXPORT bool L_Entity_IsVulnerableEnemy(Entity* entity, Entity* source) {
	return entity->IsVulnerableEnemy(source);
}

MOD_EXPORT bool L_Entity_IsFlying(Entity* entity) {
	return entity->IsFlying();
}

MOD_EXPORT bool L_Entity_IsFrame(Entity* entity, int frame, int offset) {
	return entity->IsFrame(frame, offset);
}

MOD_EXPORT bool L_Entity_CanDevolve(Entity* entity) {
	return entity->CanDevolve();
}

MOD_EXPORT unsigned int L_Entity_GetHitListIndex(Entity* entity) {
	return entity->GetHitListIndex();
}

MOD_EXPORT int L_Entity_GetFrameCount(Entity* entity) {
	return g_Game->_frameCount - *reinterpret_cast<int*>(reinterpret_cast<char*>(entity) + 0x328);
}

MOD_EXPORT int L_Entity_GetBossID(Entity* entity) {
	char* config = *reinterpret_cast<char**>(reinterpret_cast<char*>(entity) + 0x330);
	return config ? *reinterpret_cast<int*>(config + 0x30) : 0;
}

MOD_EXPORT void L_Entity_RemoveStatusEffects(Entity* entity) {
	entity->RemoveStatusEffects();
}

MOD_EXPORT void L_Entity_AddBaited(Entity* entity, EntityRef* source, int duration) {
	entity->AddBaited(*source, duration, false);
}

MOD_EXPORT void L_Entity_AddBleeding(Entity* entity, EntityRef* source, int duration) {
	entity->AddBleeding(*source, duration, false);
}

MOD_EXPORT void L_Entity_AddMagnetized(Entity* entity, EntityRef* source, int duration) {
	entity->AddMagnetized(*source, duration, false);
}

MOD_EXPORT void L_Entity_AddWeakness(Entity* entity, EntityRef* source, int duration) {
	entity->AddWeakness(*source, duration);
}

MOD_EXPORT void L_Entity_AddBrimstoneMark(Entity* entity, EntityRef* source, int duration) {
	entity->AddBrimstoneMark(*source, duration);
}

MOD_EXPORT void L_Entity_AddIce(Entity* entity, EntityRef* source, int duration) {
	entity->AddIce(*source, duration);
}

MOD_EXPORT void L_Entity_AddKnockback(Entity* entity, EntityRef* source, Vector* pushDirection, int duration, bool takeImpactDamage) {
	entity->AddKnockback(*source, *pushDirection, duration, takeImpactDamage);
}

MOD_EXPORT void L_Entity_AddBurn(Entity* entity, EntityRef* source, int duration, float damage, bool ignoreBosses) {
	entity->AddBurn(*source, duration, damage, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddCharmed(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddCharmed(*source, duration, ignoreBosses, true);
}

MOD_EXPORT void L_Entity_AddConfusion(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddConfusion(*source, duration, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddFear(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddFear(*source, duration, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddFreeze(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddFreeze(*source, duration, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddMidasFreeze(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddMidasFreeze(*source, duration, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddPoison(Entity* entity, EntityRef* source, int duration, float damage, bool ignoreBosses) {
	entity->AddPoison(*source, duration, damage, ignoreBosses);
}

MOD_EXPORT void L_Entity_AddShrink(Entity* entity, EntityRef* source, int duration, bool ignoreBosses) {
	entity->AddShrink(*source, duration, ignoreBosses, false);
}

MOD_EXPORT void L_Entity_AddSlowing(Entity* entity, EntityRef* source, int duration, float amount, ColorMod* color, bool ignoreBosses) {
	entity->AddSlowing(*source, duration, amount, *color, ignoreBosses);
}

MOD_EXPORT unsigned int L_Entity_ComputeStatusEffectDuration(Entity* entity, int initial, EntityRef* source) {
	return entity->ComputeStatusEffectDuration((unsigned int)std::max(initial, 0), source);
}

MOD_EXPORT bool L_Entity_IgnoreEffectFromFriendly(Entity* entity, EntityRef* source) {
	return entity->IgnoreEffectFromFriendly(source);
}

MOD_EXPORT void L_Entity_CopyStatusEffects(Entity* entity, Entity* other, bool overwrite) {
	if (other == nullptr) {
		for (Entity* child = entity->GetChild(); child != nullptr; child = child->GetChild()) {
			CopyStatusEffects(entity, child, overwrite);
		}
	}
	else {
		CopyStatusEffects(entity, other, overwrite);
	}
}

MOD_EXPORT unsigned int L_Entity_GetColorParamsCount(Entity* entity) {
	return (unsigned int)entity->_colorParams.size();
}

MOD_EXPORT ColorParams* L_Entity_GetColorParam(Entity* entity, unsigned int index) {
	return &entity->_colorParams[index];
}

MOD_EXPORT void L_Entity_SetColorParams(Entity* entity, ColorParams* params, unsigned int count) {
	if (count == 0) {
		entity->_colorParams.clear();
		return;
	}

	vector_ColorParams list;
	list.reserve(count);
	for (unsigned int i = 0; i < count; i++) {
		list.push_back(params[i]);
	}
	entity->_colorParams = list;
}

MOD_EXPORT void L_Entity_GetNullOffset(Entity* entity, const char* nullLayerName, Vector* out) {
	*out = entity->GetNullOffset(nullLayerName);
}

MOD_EXPORT void L_Entity_GetNullCapsule(Entity* entity, const char* nullLayerName, Capsule* out) {
	*out = entity->GetNullCapsule(nullLayerName);
}

MOD_EXPORT void L_Entity_GetCollisionCapsule(Entity* entity, Vector* offset, Capsule* out) {
	*out = entity->GetCollisionCapsule(offset);
}

MOD_EXPORT void L_Entity_GetPredictedTargetPosition(Entity* entity, Entity* target, float delay, Vector* out) {
	*out = entity->GetPredictedTargetPosition(target, delay);
}

MOD_EXPORT void L_Entity_SpawnWaterImpactEffects(Vector* position, Vector* velocity, float scale) {
	Entity::DoGroundImpactEffects(position, velocity, scale);
}

MOD_EXPORT ANM2* L_Entity_GetSprite(Entity* entity) {
	return &entity->_sprite;
}

MOD_EXPORT RNG* L_Entity_GetDropRNG(Entity* entity) {
	return &entity->_dropRNG;
}

MOD_EXPORT Shape* L_Entity_GetDebugShape(Entity* entity, bool unk) {
	return g_Game->GetDebugRenderer()->Get(entity->GetIndex(), unk);
}

MOD_EXPORT EntityConfig_Entity* L_Entity_GetEntityConfigEntity(Entity* entity) {
	return g_Manager->GetEntityConfig()->GetEntity(*entity->GetType(), *entity->GetVariant(), *entity->GetSubType());
}

MOD_EXPORT Entity_Effect* L_Entity_SpawnBloodEffect(Entity* entity, int subtype, Vector* position, Vector* offset, ColorMod* color, Vector* velocity) {
	Vector pos = position ? *position : *entity->GetPosition();
	Vector off = offset ? *offset : Vector();
	ColorMod col = color ? *color : ColorMod();
	Vector vel = velocity ? *velocity : Vector();

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 2, pos, vel, nullptr, subtype, Isaac::genrand_int32(), 0);
	effect->SetColor(&col, -1, 255, false, true);
	effect->_sprite._offset = off;
	effect->_depthOffset = -10.0f;
	return effect;
}

MOD_EXPORT Entity_Effect* L_Entity_MakeBloodPoof(Entity* entity, Vector* position, ColorMod* color, float scale) {
	Vector pos = position ? *position : *entity->GetPosition();
	ColorMod col = color ? *color : ColorMod();
	return entity->MakeBloodPoof(&pos, &col, scale);
}

MOD_EXPORT Entity_Effect* L_Entity_MakeGroundPoof(Entity* entity, Vector* position, ColorMod* color, float scale) {
	Vector pos = position ? *position : *entity->GetPosition();
	ColorMod col = color ? *color : ColorMod();
	return entity->MakeGroundPoof(&pos, &col, scale);
}

MOD_EXPORT unsigned int L_Entity_GetWaterClipFlags(Entity* entity) {
	WaterClipInfo info;
	entity->GetWaterClipInfo(&info);
	return info.bitFlags;
}

MOD_EXPORT void L_Entity_SetWaterClipFlags(Entity* entity, uint32_t flags) {
	if (EntityPlus* entityPlus = GetEntityPlus(entity)) {
		entityPlus->waterClipInfoFlagsOverride = flags;
	}
}

MOD_EXPORT void L_Entity_ResetWaterClipFlags(Entity* entity) {
	if (EntityPlus* entityPlus = GetEntityPlus(entity)) {
		entityPlus->waterClipInfoFlagsOverride = std::nullopt;
	}
}

MOD_EXPORT Entity_Player* L_Entity_ToPlayer(Entity* entity) {
	return entity->ToPlayer();
}

MOD_EXPORT Entity_NPC* L_Entity_ToNPC(Entity* entity) {
	return entity->ToNPC();
}

MOD_EXPORT Entity_Familiar* L_Entity_ToFamiliar(Entity* entity) {
	return entity->ToFamiliar();
}

MOD_EXPORT Entity_Pickup* L_Entity_ToPickup(Entity* entity) {
	return entity->ToPickup();
}

MOD_EXPORT Entity_Projectile* L_Entity_ToProjectile(Entity* entity) {
	return entity->ToProjectile();
}

MOD_EXPORT void L_EntityProjectile_Deflect(Entity_Projectile* projectile, Vector* velocity) {
	projectile->Reflect(nullptr, velocity);
}

HOOK_STATIC(LuaEngine, GetUserdata, (lua_State* L, int idx, void* key, bool canBeConst) -> void*, __cdecl) {
	if (lua_type(L, idx) == LUA_TCDATA) {
		static const lua::Metatables entityClasses[] = {
			lua::Metatables::ENTITY,
			lua::Metatables::ENTITY_PROJECTILE,
			lua::Metatables::ENTITY_TEAR,
			lua::Metatables::ENTITY_BOMB,
			lua::Metatables::ENTITY_KNIFE,
			lua::Metatables::ENTITY_LASER,
			lua::Metatables::ENTITY_EFFECT,
			lua::Metatables::ENTITY_PICKUP,
			lua::Metatables::ENTITY_NPC,
		};

		lua_rawgetp(L, LUA_REGISTRYINDEX, key);
		lua::Metatables wanted = lua::Metatables::METATABLES_MAX;
		for (lua::Metatables mt : entityClasses) {
			lua::PushMetatable(L, mt);
			const bool match = lua_rawequal(L, -1, -2);
			lua_pop(L, 1);
			if (match) {
				wanted = mt;
				break;
			}
		}
		lua_pop(L, 1);

		if (wanted != lua::Metatables::METATABLES_MAX) {
			if (void* userdata = lua::TestUserdata(L, idx, wanted)) {
				return userdata;
			}
		}
	}
	return super(L, idx, key, canBeConst);
}

namespace {
	Entity* CheckEntityArgument(lua_State* L, int index) {
		if (lua_isnoneornil(L, index)) {
			return nullptr;
		}
		return lua::GetLuabridgeUserdata<Entity*>(L, index, lua::Metatables::ENTITY, "Entity");
	}

	int PushEntityOrNil(lua_State* L, Entity* entity) {
		if (entity) {
			LuaEntity::PushPtr(L, entity);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}

	LUA_FUNCTION(Lua_Entity_GetParentLegacy) {
		return PushEntityOrNil(L, lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity")->GetParent());
	}

	LUA_FUNCTION(Lua_Entity_SetParentLegacy) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
		entity->SetParent(CheckEntityArgument(L, 2));
		return 0;
	}

	LUA_FUNCTION(Lua_Entity_GetChildLegacy) {
		return PushEntityOrNil(L, lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity")->GetChild());
	}

	LUA_FUNCTION(Lua_Entity_SetChildLegacy) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
		entity->SetChild(CheckEntityArgument(L, 2));
		return 0;
	}

	LUA_FUNCTION(Lua_Entity_GetTargetLegacy) {
		return PushEntityOrNil(L, lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity")->GetTarget());
	}

	LUA_FUNCTION(Lua_Entity_SetTargetLegacy) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
		entity->SetTarget(CheckEntityArgument(L, 2));
		return 0;
	}

	LUA_FUNCTION(Lua_Entity_GetSpawnerEntityLegacy) {
		return PushEntityOrNil(L, lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity")->GetSpawnerEntity());
	}

	LUA_FUNCTION(Lua_Entity_SetSpawnerEntityLegacy) {
		Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
		entity->SetSpawnerEntity(CheckEntityArgument(L, 2));
		return 0;
	}
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterVariable(_state, lua::Metatables::ENTITY, "Parent", Lua_Entity_GetParentLegacy, Lua_Entity_SetParentLegacy);
	lua::RegisterVariable(_state, lua::Metatables::ENTITY, "Child", Lua_Entity_GetChildLegacy, Lua_Entity_SetChildLegacy);
	lua::RegisterVariable(_state, lua::Metatables::ENTITY, "Target", Lua_Entity_GetTargetLegacy, Lua_Entity_SetTargetLegacy);
	lua::RegisterVariable(_state, lua::Metatables::ENTITY, "SpawnerEntity", Lua_Entity_GetSpawnerEntityLegacy, Lua_Entity_SetSpawnerEntityLegacy);
}
