#pragma once

namespace delirium {
	extern const char* DeliriumMetatable;

	constexpr const unsigned int ENTITY_DELIRIUM = 412;

	void PatchSkipFrames();
	void AddPreTransformationCallback();
	void AddTransformationCallback();
}