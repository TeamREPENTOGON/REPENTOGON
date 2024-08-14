#pragma once

namespace ASMPatches {
	bool FixGodheadEntityPartition();
	bool FixTearDetonatorEntityList();
	bool FixHushFXVeins();

	namespace BerserkSpiritShacklesCrash {
		bool Patch();
	}
}