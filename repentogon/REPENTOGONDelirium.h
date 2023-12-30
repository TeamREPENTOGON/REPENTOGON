#pragma once

struct Entity_NPC;

#include <map>
#include <tuple>

namespace delirium {
	extern const char* DeliriumMetatable;
	constexpr const unsigned int ENTITY_DELIRIUM = 412;
	extern std::map<Entity_NPC*, std::tuple<int, int, bool>> ForcedTransformations;
}