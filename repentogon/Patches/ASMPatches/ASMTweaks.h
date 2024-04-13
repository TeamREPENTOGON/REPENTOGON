#pragma once

namespace ASMPatches {
	bool FixGodheadEntityPartition();
	bool FixTearDetonatorEntityList();

	namespace BerserkSpiritShacklesCrash {
		bool Patch();
	}
}