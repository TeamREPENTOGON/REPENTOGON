#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	bool L_GridEntity_Destroy(GridEntity* grid, bool Immediate) {
		return grid->Destroy(Immediate);
	}
	
	int L_GridEntity_GetGridIndex(GridEntity* grid) {
		return grid->_gridIdx;
	}

	RNG* L_GridEntity_GetRNG(GridEntity* grid) {
		return &grid->_rng;
	}

	GridEntityDesc* L_GridEntity_GetDesc(GridEntity* grid) {
		return &grid->_desc;
	}

	void L_GridEntity_SetDesc(GridEntity* grid, GridEntityDesc* desc) {
		grid->_desc = *desc;
	}

	ANM2* L_GridEntity_GetSprite(GridEntity* grid) {
		return &grid->_anm2;
	}

	int L_GridEntity_GetType(GridEntity* grid) {
		return grid->_desc._type;
	}

	int L_GridEntity_GetVariant(GridEntity* grid) {
		return grid->_desc._variant;
	}

	/*
	bool L_GridEntity_Hurt(GridEntity* grid, int Damage) {
		return grid->Hurt(Damage);
	}
	*/

	void L_GridEntity_Init(GridEntity* grid, unsigned int Seed) {
		grid->Init(Seed);
	}

	void L_GridEntity_PostInit(GridEntity* grid) {
		grid->PostInit();
	}

	void L_GridEntity_Render(GridEntity* grid, Vector* offset) {
		grid->Render(*offset);
	}

	void L_GridEntity_SetType(GridEntity* grid, int Type) {
		grid->_desc._type = Type;
	}

	void L_GridEntity_SetVariant(GridEntity* grid, int Variant) {
		grid->_desc._variant = Variant;
	}

	GridEntity_Door* L_GridEntity_ToDoor(GridEntity* grid) {
		return grid->ToDoor();
	}

	/*
	void L_GridEntity_Update(GridEntity* grid) {
		grid->Update();
	}
	*/

	int L_GridEntity_GetCollisionClass(GridEntity* grid) {
		return grid->_collisionClass;
	}

	void L_GridEntity_GetCollisionClass(GridEntity* grid, int value) {
		grid->_collisionClass = value;
	}

	Vector* L_GridEntity_GetPosition(GridEntity* grid) {
		Vector buffer;
		grid->GetPosition(&buffer);
		return &buffer;
	}

	int L_GridEntity_GetState(GridEntity* grid) {
		return grid->_desc._state;
	}

	void L_GridEntity_SetState(GridEntity* grid, int value) {
		grid->_desc._state = value;
	}

	int L_GridEntity_GetVarData(GridEntity* grid) {
		return grid->_desc._varData;
	}

	void L_GridEntity_SetVarData(GridEntity* grid, int value) {
		grid->_desc._varData = value;
	}
}