#pragma once

namespace ASMPatches {
	bool FixGodheadEntityPartition();
	bool FixTearDetonatorEntityList();
	bool FixHushFXVeins();
	bool SkipArchiveChecksums();

	namespace BerserkSpiritShacklesCrash {
		bool Patch();
	}
}