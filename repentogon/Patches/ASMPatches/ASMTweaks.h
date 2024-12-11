#pragma once

namespace ASMPatches {
	bool FixGodheadEntityPartition();
	bool FixTearDetonatorEntityList();
	bool FixHushFXVeins();
	bool SkipArchiveChecksums();
	bool LeaderboarEntryCheckerUpdate();
	bool AllowConsoleInOnline();

	namespace BerserkSpiritShacklesCrash {
		bool Patch();
	}
}