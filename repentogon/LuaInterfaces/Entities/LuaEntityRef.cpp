#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {

	void L_EntityRef_game_constructor(EntityRef* ref, Entity* entity) {
		ref->game_constructor(entity);
	}

	bool L_EntityRef_GetIsCharmed(EntityRef* ref) {
		return ref->_flags & 1;
	}

	void L_EntityRef_SetIsCharmed(EntityRef* ref, bool value) {
		ref->_flags = value ? ref->_flags | 1 : ref->_flags & 0xfffffffe;
	}

	bool L_EntityRef_GetIsFriendly(EntityRef* ref) {
		return (ref->_flags >> 1) & 1;
	}

	void L_EntityRef_SetIsFriendly(EntityRef* ref, bool value) {
		ref->_flags = value ? ref->_flags | 2 : ref->_flags & 0xfffffffd;
	}

	Entity* L_EntityRef_GetEntity(EntityRef* ref) {
		return ref->_entity;
	}
}