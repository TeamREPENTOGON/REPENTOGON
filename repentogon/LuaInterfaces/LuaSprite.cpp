#include "IsaacRepentance.h"
#include "../Patches/Anm2Extras.h"

extern "C" {

	__declspec(dllexport) ANM2* L_Sprite_New() {
		return new ANM2();
	}

	__declspec(dllexport) void L_Sprite_Delete(ANM2* self) {
		delete self;
	}

	__declspec(dllexport) void L_Sprite_ClearCustomShader(ANM2* sprite, bool champion) {
		ClearCustomShader(sprite, champion);
	}

	__declspec(dllexport) const char* L_Sprite_GetAnimation(ANM2* sprite) {
		AnimationState* state = sprite->GetAnimationState();
		if (!state->_animData) return "";
		return state->_animData->_name.c_str();
	}

	__declspec(dllexport) AnimationData* L_Sprite_GetAnimationData(ANM2* sprite, const char* animName) {
		return sprite->GetAnimationData(animName);
	}

	__declspec(dllexport) const char* L_Sprite_GetDefaultAnimation(ANM2* sprite) {
		return sprite->_animDefaultName.c_str();
	}

	__declspec(dllexport) const char* L_Sprite_GetFilename(ANM2* sprite) {
		return sprite->_filename.c_str();
	}
	
	__declspec(dllexport) LayerState* L_Sprite_GetLayerById(ANM2* sprite, int layerId) {
		return sprite->GetLayer(layerId);
	}

	__declspec(dllexport) LayerState* L_Sprite_GetLayerByName(ANM2* sprite, const char* layerName) {
		return sprite->GetLayer(layerName);
	}

	__declspec(dllexport) KColor* L_Sprite_GetTexel(ANM2* sprite, KColor* out, Vector samplePos, Vector renderPos, float alphaThreshold, int layerID) {
		return sprite->GetTexel(out, samplePos, renderPos, alphaThreshold, layerID);
	}

	__declspec(dllexport) bool L_Sprite_HasCustomShader(ANM2* sprite, bool champion) {
		return HasCustomShader(sprite, champion);
	}

	__declspec(dllexport) bool L_Sprite_HasCustomShaderWithPath(ANM2* sprite, const char* path, bool champion) {
		return HasCustomShader(sprite, path, champion);
	}

	__declspec(dllexport) bool L_Sprite_IsFinished(ANM2* sprite, const char* name) {
		std::string nameStr = name;
		return sprite->IsFinished(nameStr);
	}

	__declspec(dllexport) bool L_Sprite_IsOverlayFinished(ANM2* sprite, const char* name) {
		std::string nameStr = name;
		return sprite->IsOverlayFinished(nameStr);
	}

	__declspec(dllexport) void L_Sprite_Load(ANM2* sprite, const char* path, bool loadGraphics) {
		std::string pathStr = path;
		sprite->Load(pathStr, loadGraphics);
	}

	__declspec(dllexport) void L_Sprite_LoadGraphics(ANM2* sprite) {
		sprite->LoadGraphics(false);
	}

	__declspec(dllexport) void L_Sprite_Play(ANM2* sprite, const char* name, bool force) {
		sprite->Play(name, force);
	}
	
	__declspec(dllexport) void L_Sprite_PlayOverlay(ANM2* sprite, const char* name, bool force) {
		std::string nameStr = name;
		sprite->PlayOverlay(nameStr, force);
	}
	
	__declspec(dllexport) void L_Sprite_PlayRandom(ANM2* sprite, unsigned int seed) {
		sprite->PlayRandom(seed);
	}
	
	__declspec(dllexport) void L_Sprite_Reload(ANM2* sprite) {
		sprite->Reload();
	}
	
	__declspec(dllexport) void L_Sprite_Render(ANM2* sprite, Vector* position, Vector* topLeftClamp, Vector* bottomRightClamp) {
		sprite->Render(position, topLeftClamp, bottomRightClamp);
	}

	__declspec(dllexport) void L_Sprite_RenderLayer(ANM2* sprite, int layer, Vector* position, Vector* topLeftClamp, Vector* bottomRightClamp) {
		sprite->RenderLayer(layer, position, topLeftClamp, bottomRightClamp);
	}

	__declspec(dllexport) bool L_Sprite_ReplaceSpritesheet(ANM2* sprite, int layerId, const char* filename) {
		std::string filenameStr = filename;
		return sprite->ReplaceSpritesheet(layerId, filenameStr);
	}
	
	__declspec(dllexport) void L_Sprite_Reset(ANM2* sprite) {
		sprite->Reset();
	}

	__declspec(dllexport) bool L_Sprite_SetAnimation(ANM2* sprite, const char* animation, bool reset) {
		return sprite->SetAnimation(animation, reset);
	}

	__declspec(dllexport) bool L_Sprite_SetCustomShader(ANM2* sprite, const char* path, bool champion) {
		return SetCustomShader(sprite, path, champion);
	}
	
	__declspec(dllexport) void L_Sprite_SetFrameWithAnim(ANM2* sprite, const char* animation, int frame) {
		std::string animationStr = animation;
		sprite->SetFrame(&animationStr, frame);
	}

	__declspec(dllexport) void L_Sprite_SetFrame(ANM2* sprite, int frame) {
		sprite->SetFrame(frame);
	}

	__declspec(dllexport) void L_Sprite_SetLayerFrame(ANM2* sprite, int id, int frame) {
		sprite->SetLayerFrame(id, frame);
	}

	__declspec(dllexport) void L_Sprite_SetOverlayAnimation(ANM2* sprite, const char* anim, bool reset) {
		sprite->SetOverlayAnimation(anim, reset);
	}

	__declspec(dllexport) void L_Sprite_SetOverlayFrameWithAnim(ANM2* sprite, const char* anim, int frame) {
		sprite->SetOverlayFrame(anim, frame);
	}

	__declspec(dllexport) void L_Sprite_SetOverlayFrame(ANM2* sprite, int frame) {
		sprite->SetOverlayFrame(frame);
	}

	__declspec(dllexport) void L_Sprite_Update(ANM2* sprite) {
		sprite->Update();
	}
}