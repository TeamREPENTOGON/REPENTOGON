#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_AnimationState_AdvancePosition(AnimationState* state, int frame) {
		state->AdvancePosition(frame);
	}

	__declspec(dllexport) NullFrame* L_AnimationState_GetNullFrame(AnimationState* state, const char* name) {
		return state->GetNullFrame(name);
	}

	__declspec(dllexport) bool L_AnimationState_IsEventTriggered(AnimationState* state, const char* name) {
		return state->IsEventTriggered(name);
	};

	__declspec(dllexport) void L_AnimationState_Reset(AnimationState* state) {
		state->Reset(nullptr);
	}

	__declspec(dllexport) void L_AnimationState_Rewind(AnimationState* state) {
		state->Rewind();
	};

	__declspec(dllexport) void L_AnimationState_SetLayerFrame(AnimationState* state, int layerId, int frame) {
		state->SetLayerFrame(layerId, frame);
	};

	__declspec(dllexport) bool L_AnimationState_WasEventTriggered(AnimationState* state, const char* name) {
		return state->WasEventTriggered(name);
	};
}