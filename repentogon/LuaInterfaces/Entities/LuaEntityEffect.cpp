#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../LuaClasses.h"

MOD_EXPORT void L_EntityEffect_FollowParent(Entity_Effect* effect, Entity* parent) {
	effect->FollowParent(parent);
}

// color may be null
MOD_EXPORT Entity_Effect* L_EntityEffect_CreateLight(Vector* position, float scale, int lifespan, int state, ColorMod* color) {
	ColorMod effectColor;
	if (color) {
		effectColor = *color;
	}

	if (lifespan < 1) {
		lifespan = -1;
	}
	if (state < 1) {
		state = 6;
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 121, *position, Vector(0, 0), nullptr, 0, Isaac::genrand_int32(), 0);
	if (!effect) {
		return nullptr;
	}

	effect->_state = state;
	effect->_timeout = lifespan;
	effect->_lifespan = lifespan;
	effect->SetColor(&effectColor, -1, 255, false, false);
	effect->_sprite._scale *= scale;
	return effect;
}

MOD_EXPORT Entity_Effect* L_EntityEffect_CreateLootPreview(LootList* loot, Vector* position, Entity_Pickup* owner, Entity_Effect* effect) {
	return Entity_Effect::CreateLootPreview(loot, position, owner, effect);
}

// Only the effect variants with a grid entity description in their varData
MOD_EXPORT GridEntityDesc* L_EntityEffect_GetGridEntityDesc(Entity_Effect* effect) {
	if (effect->_variant == 136) {
		return (GridEntityDesc*)&effect->_varData;
	}
	return nullptr;
}
