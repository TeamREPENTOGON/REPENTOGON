#include <memory>

#include "EntityPlus.h"

#include "HookSystem.h"
#include "IsaacRepentance.h"

namespace {

void InitEntityPlusHolder(Entity* entity) {
	EntityPlusHolder* holder = new EntityPlusHolder();

	uintptr_t val = (uintptr_t)holder;
	unsigned short up = (val & 0xFFFF0000) >> 16;
	unsigned short low = val & 0x0000FFFF;

	entity->_entityExtraDataPtrUpperBytes = up;
	entity->_entityExtraDataPtrLowerBytes = low;
}

EntityPlusHolder* GetEntityPlusHolder(Entity* entity, const bool initIfMissing) {
	if (entity->_entityExtraDataPtrUpperBytes == 0 && entity->_entityExtraDataPtrLowerBytes == 0) {
		if (initIfMissing) {
			g_Game->GetConsole()->Print("ERROR??!?!?!?\n", 0xFF0000FF, 60);
			InitEntityPlusHolder(entity);
		}
		else {
			return nullptr;
		}
	}

	const unsigned short up = entity->_entityExtraDataPtrUpperBytes;
	const unsigned short low = entity->_entityExtraDataPtrLowerBytes;

	const uintptr_t val = ((uintptr_t)up << 16) + low;

	return (EntityPlusHolder*)val;
}

}  // namespace


EntityPlus* GetEntityPlus(Entity* entity) {
	return GetEntityPlusHolder(entity, true)->data.get();
}

EntityLaserPlus* GetEntityPlus(Entity_Laser* laser) {
	return dynamic_cast<EntityLaserPlus*>(GetEntityPlusHolder(laser, true)->data.get());
}


HOOK_METHOD(Entity, constructor, () -> void) {
	super();
	InitEntityPlusHolder(this);
}

HOOK_METHOD(Entity, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int seed) -> void) {
	EntityPlusHolder* holder = GetEntityPlusHolder(this, true);
	holder->data.reset();
	if (type == 7) {
		holder->data = std::make_unique<EntityLaserPlus>();
	} else {
		holder->data = std::make_unique<EntityPlus>();
	}

	super(type, variant, subtype, seed);
}

HOOK_METHOD(Entity, Remove, ()->void) {
	super();
	GetEntityPlusHolder(this, true)->data.reset();
}

HOOK_METHOD(Entity, destructor, () -> void) {
	delete GetEntityPlusHolder(this, false);
	super();
}
