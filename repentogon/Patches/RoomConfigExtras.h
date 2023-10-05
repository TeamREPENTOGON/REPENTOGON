static unsigned int GetStageID(unsigned int LevelStage, unsigned int StageType, unsigned int Mode) {
		if (Mode == -1 || Mode == 1) {
			if (g_Game && ((g_Game->_difficulty == 2 || (g_Game->_difficulty == 3)))) {

				if (LevelStage == 7)
					return 0x19;

				if (LevelStage == 6)
					return 0x18;

				if (LevelStage == 5)
					return 0xE;

				if (StageType == 4) {
					return LevelStage * 2 + 0x19;
				}

				if (StageType == 5) {
					return LevelStage * 2 + 0x1A;
				}

				return StageType + LevelStage * 3 + -2;
			}
		}

		if (LevelStage == 0xD) {
			return 0x23;
		}

		if (LevelStage > (unsigned int)8) {
			if (LevelStage == 9) {
				if (StageType == 4)
					return 0x24;
				return 0xD;
			}

			if (LevelStage == 0xC)
				return 0x1A;
			return StageType + (LevelStage - 3) * 2;
		}

		if (StageType == 4)
			return ((LevelStage - 1) & 0xFFFFFFFE) + 0x1B;

		if (StageType == 5)
			return ((LevelStage - 1) & 0xFFFFFFFE) + 0x1C;

		return StageType + 1 + ((LevelStage - 1) & 0XFFFFFFFE) + (((LevelStage - 1) & 0XFFFFFFFE) >> 1);
	}