#pragma once

extern constexpr unsigned int BUFFER_STAGEID = 23;

extern std::unordered_map<std::string, RoomSet> binaryMap;
extern bool stageOverwritten[37] = { false };

const char* suffixes[36] = {
	"",
	"_basement",
	"_cellar",
	"_burningbasement",
	"_caves",
	"_catacombs",
	"_floodedcaves",
	"_depths",
	"_necropolis",
	"_dankdepths",
	"_womb",
	"_utero",
	"_scarredwomb",
	"_bluewomb",
	"_sheol",
	"_cathedral",
	"_darkroom",
	"_chest",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // void doesn't work, local stage ids are used
	"_downpour",
	"_dross",
	"_mines",
	"_ashpit",
	"_mausoleum",
	"_gehenna",
	"_corpse",
	"", // there will never be a mortis
	"_home"
};