#pragma once

#include <map>
#include <tuple>
struct Entity_NPC;

namespace delirium {
	extern const char* DeliriumMetatable;

	constexpr const unsigned int ENTITY_DELIRIUM = 412;

	extern std::map<Entity_NPC*, std::tuple<int, int, bool>> ForcedTransformations;

	void PatchCompanion();
	void PatchSkipFrames();
	void PatchVadeRetro();
	void AddPreTransformationCallback();
	void AddTransformationCallback();
	void AddPostTransformationCallback();
}