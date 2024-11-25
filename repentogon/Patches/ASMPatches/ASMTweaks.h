#pragma once

namespace ASMPatches {
	bool FixGodheadEntityPartition();
	bool FixTearDetonatorEntityList();
	bool FixHushFXVeins();
	bool SkipArchiveChecksums();
	bool LeaderboarEntryCheckerUpdate();

	namespace BerserkSpiritShacklesCrash {
		bool Patch();
	}
}