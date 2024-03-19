#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	bool L_GridEntityDesc_GetInitialized(GridEntityDesc* desc) {
		return desc->_initialized;
	}

	void L_GridEntityDesc_SetIntitialized(GridEntityDesc* desc, bool value) {
		desc->_initialized = value;
	}

	int L_GridEntityDesc_GetSpawnCount(GridEntityDesc* desc) {
		return desc->_spawnCount;
	}

	void L_GridEntityDesc_SetSpawnCount(GridEntityDesc* desc, int value) {
		desc->_spawnCount = value;
	}

	unsigned int L_GridEntityDesc_GetSpawnSeed(GridEntityDesc* desc) {
		return desc->_spawnSeed;
	}

	void L_GridEntityDesc_SetSpawnSeed(GridEntityDesc* desc, unsigned int value) {
		desc->_spawnSeed = value;
	}

	int L_GridEntityDesc_GetState(GridEntityDesc* desc) {
		return desc->_state;
	}

	void L_GridEntityDesc_SetState(GridEntityDesc* desc, int value) {
		desc->_state = value;
	}

	int L_GridEntityDesc_GetType(GridEntityDesc* desc) {
		return desc->_type;
	}

	void L_GridEntityDesc_SetType(GridEntityDesc* desc, int value) {
		desc->_type = value;
	}

	int L_GridEntityDesc_GetVarData(GridEntityDesc* desc) {
		return desc->_varData;
	}

	void L_GridEntityDesc_SetVarData(GridEntityDesc* desc, int value) {
		desc->_varData = value;
	}

	void L_GridEntityDesc_SetSpawnCount(GridEntityDesc* desc, int value) {
		desc->_spawnCount = value;
	}

	unsigned int L_GridEntityDesc_GetVariableSeed(GridEntityDesc* desc) {
		return desc->_variableSeed;
	}

	void L_GridEntityDesc_SetVariableSeed(GridEntityDesc* desc, unsigned int value) {
		desc->_variableSeed = value;
	}

	int L_GridEntityDesc_GetVariant(GridEntityDesc* desc) {
		return desc->_variableSeed;
	}

	void L_GridEntityDesc_SetVariant(GridEntityDesc* desc, int value) {
		desc->_variant = value;
	}
}