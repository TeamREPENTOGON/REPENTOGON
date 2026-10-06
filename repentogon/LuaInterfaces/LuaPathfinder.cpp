#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

MOD_EXPORT void L_PathFinder_Reset(NPCAI_Pathfinder* pathfinder) {
	pathfinder->Reset();
}

MOD_EXPORT bool L_PathFinder_MoveRandomly(NPCAI_Pathfinder* pathfinder, bool ignoreEffects) {
	return pathfinder->MoveRandomly(ignoreEffects);
}

MOD_EXPORT void L_PathFinder_MoveRandomlyBoss(NPCAI_Pathfinder* pathfinder, bool ignoreEffects) {
	pathfinder->MoveRandomlyBoss(ignoreEffects);
}

MOD_EXPORT void L_PathFinder_MoveRandomlyAxisAligned(NPCAI_Pathfinder* pathfinder, float speed, bool ignoreEffects) {
	pathfinder->MoveRandomlyAxisAligned(speed, ignoreEffects);
}

MOD_EXPORT void L_PathFinder_FindGridPath(NPCAI_Pathfinder* pathfinder, Vector* position, float speed, int pathMarker, bool directPath) {
	pathfinder->FindGridPath(position, speed, pathMarker, directPath);
}

MOD_EXPORT bool L_PathFinder_HasPathToPos(NPCAI_Pathfinder* pathfinder, Vector* position, bool ignorePoop) {
	return pathfinder->HasPathToPos(position, ignorePoop);
}

MOD_EXPORT void L_PathFinder_EvadeTarget(NPCAI_Pathfinder* pathfinder, Vector* position, bool ignoreEffects) {
	pathfinder->EvadeTarget(position, ignoreEffects);
}

MOD_EXPORT void L_PathFinder_ResetMovementTarget(NPCAI_Pathfinder* pathfinder) {
	pathfinder->ResetMovementTarget();
}

MOD_EXPORT void L_PathFinder_UpdateGridIndex(NPCAI_Pathfinder* pathfinder) {
	pathfinder->UpdateGridIndex();
}

MOD_EXPORT void L_PathFinder_SimulatePlayerMovement(NPCAI_Pathfinder* pathfinder, Vector* movement, float speed) {
	pathfinder->SimulatePlayerMovement(movement, speed, false);
}
