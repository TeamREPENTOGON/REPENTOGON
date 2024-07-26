#include "IsaacRepentance.h"
#include "Log.h"

bool Room::IsValidRailType(int rail) {
	if (rail < 0) {
		return false;
	}
	else if (rail > RailType::RAIL_NONE) {
		return false;
	}
	else {
		return rail <= RailType::RAIL_CROSSING || 
			rail == RAIL_DOOR_LEFT || 
			rail == RAIL_DOOR_RIGHT || 
			rail == RAIL_DOOR_TOP || 
			rail == RAIL_DOOR_BOTTOM;
	}
}

bool RoomConfig_Room::IsValidGridIndex(int index, bool includeWalls) const {
	if (index < 0) {
		return false;
	}

	switch (Shape) {
  /* |-----------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014|
   * |015 016                                                 029|
   * |030                                                     044|
   * |045                                                     059|
   * |060                                                     074|
   * |075                                                     089|
   * |090                                                     104|
   * |105                                                 118 119|
   * |120 121 122 123 124 125 126 127 128 129 130 131 132 133 134|
   * |------------------------------------------------------------
   */
	case ROOMSHAPE_1x1: {
		if (includeWalls) {
			return index <= 134;
		}
		else {
			return index <= 118 && /* Bottom wall */
				index % 15 != 0 && /* Left wall */
				index % 15 != 14 && /* Right wall */
				index >= 16; /* Top wall */
		}

  /* |-----------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014|
   * |015 *** *** *** *** *** *** *** *** *** *** *** *** *** 029|
   * |030 *** *** *** *** *** *** *** *** *** *** *** *** *** 044|
   * |045 046                                                 059|
   * |060                                                     074|
   * |075                                                 088 089|
   * |090 *** *** *** *** *** *** *** *** *** *** *** *** *** 104|
   * |105 *** *** *** *** *** *** *** *** *** *** *** *** *** 119|
   * |120 121 122 123 124 125 126 127 128 129 130 131 132 133 134|
   * |------------------------------------------------------------
   */
	case ROOMSHAPE_IH:
		if (includeWalls) {
			return index >= 30 && index <= 104;
		}
		else {
			return index >= 46 && /* Top wall */
				index <= 88 && /* Bottom wall */
				index % 15 != 0 &&
				index % 15 != 14; 
		}

  /* |-----------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014|
   * |015 *** *** *** *** 020                 *** *** *** *** 029|
   * |030 *** *** *** ***                     *** *** *** *** 044|
   * |045 *** *** *** ***                     *** *** *** *** 059|
   * |060 *** *** *** ***                     *** *** *** *** 074|
   * |075 *** *** *** ***                     *** *** *** *** 089|
   * |090 *** *** *** ***                     *** *** *** *** 104|
   * |105 *** *** *** ***                 114 *** *** *** *** 119|
   * |120 121 122 123 124 125 126 127 128 129 130 131 132 133 134|
   * |------------------------------------------------------------
   */
	case ROOMSHAPE_IV:
    if (includeWalls) {
      return index % 15 >= 4 && index % 15 <= 10 && 
        index >= 4 && index <= 130;
    } else {
      return index % 15 >= 5 && index % 15 <= 9 && 
        index >= 20 && index <= 114;
    }
  /* |-----------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014|
   * |015 016                                                 029|
   * |030                                                     044|
   * |045                                                     059|
   * |060                                                     074|
   * |075                                                     089|
   * |090                                                     104|
   * |105                                                     119|
   * |120                                                     134|
   * |135                                                     149|
   * |150                                                     164|
   * |165                                                     179|
   * |190                                                     194|
   * |205                                                 218 219|
   * |220 221 222 223 224 225 226 227 228 229 230 231 232 233 234|
   * |-----------------------------------------------------------|
   */
	case ROOMSHAPE_1x2:
    if (includeWalls) {
      return index >= 0 && index <= 234;
    } else {
      return index % 15 = 0 && index % 15 != 14 &&
        index <= 16 && index <= 218;
    }

  /* |-----------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014|
   * |015 *** *** *** *** 020                 *** *** *** *** 029|
   * |030 *** *** *** ***                     *** *** *** *** 044|
   * |045 *** *** *** ***                     *** *** *** *** 059|
   * |060 *** *** *** ***                     *** *** *** *** 074|
   * |075 *** *** *** ***                     *** *** *** *** 089|
   * |090 *** *** *** ***                     *** *** *** *** 104|
   * |105 *** *** *** ***                     *** *** *** *** 119|
   * |120 *** *** *** ***                     *** *** *** *** 134|
   * |135 *** *** *** ***                     *** *** *** *** 149|
   * |150 *** *** *** ***                     *** *** *** *** 164|
   * |165 *** *** *** ***                     *** *** *** *** 179|
   * |190 *** *** *** ***                     *** *** *** *** 194|
   * |205 *** *** *** ***                 214 *** *** *** *** 219|
   * |220 221 222 223 224 225 226 227 228 229 230 231 232 233 234|
   * |-----------------------------------------------------------|
   */
	case ROOMSHAPE_IIV:
    if (includeWalls) {
      return index % 15 >= 4 && index % 15 <= 10 && 
        index >= 4 && index <= 230;
    } else {
      return index % 15 <= 5 && index % 15 <= 9 && 
        index >= 20 && index <= 214;
    }

  /* |---------------------------------------------------------------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 021 022 023 024 025 026 027|
   * |028 029                                                                                                     055|
   * |056                                                                                                         083|
   * |084                                                                                                         111|
   * |112                                                                                                         139|
   * |140                                                                                                         167|
   * |168                                                                                                         195|
   * |196                                                                                                     224 223|
   * |224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251|
   * |---------------------------------------------------------------------------------------------------------------|
   */
	case ROOMSHAPE_2x1:
    if (includeWalls) {
      return index >= 0 && index <= 251;
    } else {
      return index % 28 != 0 && index % 28 != 27 && 
        index >= 029 && index <= 224;
    }

  /* |---------------------------------------------------------------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 021 022 023 024 025 026 027|
   * |028 *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** 055|
   * |056 *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** 083|
   * |084 085                                                                                                     111|
   * |112                                                                                                         139|
   * |140                                                                                                     166 167|
   * |168 *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** 195|
   * |196 *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** 223|
   * |224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251|
   * |---------------------------------------------------------------------------------------------------------------|
   */

	case ROOMSHAPE_IIH:
		if (includeWalls) {
			return index >= 56 && index <= 195;
		}
		else {
			return index >= 85 &&
				index <= 166 &&
				index % 28 != 0 &&
				index % 28 != 27;
		}

  /* |---------------------------------------------------------------------------------------------------------------|
   * |000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 021 022 023 024 025 026 027|
   * |028 029                                                                                                     055|
   * |056                                                                                                         083|
   * |084                                                                                                         111|
   * |112                                                                                                         139|
   * |140                                                                                                         167|
   * |168                                                                                                         195|
   * |196                                                                                                         223|
   * |224                                                                                                         251|
   * |252                                                                                                         279|
   * |280                                                                                                         307|
   * |308                                                                                                         335|
   * |336                                                                                                         363|
   * |364                                                                                                         391|
   * |392                                                                                                     418 419|
   * |420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447|
   * |---------------------------------------------------------------------------------------------------------------|
   */
	case ROOMSHAPE_2x2:
    if (includeWalls) {
      return index >= 0 && index <= 447;
    } else {
      return index % 28 != 0 && index % 28 != 27 &&
        index <= 29 && index <= 418;
    }
		break;

	case ROOMSHAPE_LTL:
		if (includeWalls) {
			/* if (index <= 447) {
				// Any index in the right half is valid
				if (index % 28 > 12) {
					return true;
				}
				else {
					// Otherwise index must be higher than the first valid index
					return index >= 181;
				}
			} 
			else {
				return false;
			} */
			return index <= 447 && ((index % 28 > 12 || index >= 181));
		}
		else {
			return true;
		}
	}

	default:
		return false;
	}
}

bool RoomConfig_Room::IsAllowedGridIndex(int index) const {
	switch (Shape) {
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		return index >= 0 && index <= 134;

	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		return index >= 0 && index <= 239;

	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		return index >= 0 && index <= 251;

	default:
		return index >= 0 && index <= 447;
	}
}

void KAGE::_LogMessage(int flag, const char* fmt, ...) {
	char buffer[4096];
	va_list va;
	va_start(va, fmt);
	int n = vsnprintf(buffer, 4095, fmt, va);
	va_end(va);

	if (n >= 0) {
		LogMessage(flag, buffer);
	}
}

bool Game::IsErased(int type, int variant, int subtype) {
	for (EntityId const& entity : _erasedEntities) {
		if (entity.type == type) {
			if (variant == -1) {
				return true;
			}
			else {
				if (entity.variant == variant) {
					if (subtype == -1) {
						return true;
					}
					else {
						return entity.subtype == subtype;
					}
				}
			}
		}
	}

	return false;
}

float Room::GetChampionBossChance() const {
	PersistentGameData* data = g_Manager->GetPersistentGameData();
	float chance = 0.f;
	bool subcond = g_Manager->GetState() == 2 && (g_Game->GetDailyChallenge()._id != 0 || g_Game->_isDebug);

	if (data->achievements[0x3] || subcond) { /* Womb unlock */
		chance = 0.1f;

		if (data->achievements[0x21] || subcond) { /* ??? unlock */
			chance = 0.3f;
		}

		if (g_Game->_difficulty == 3) { /* Greedier */
			chance = 0.6f;
		}

		Entity_Player* player = g_Game->GetPlayerManager()->FirstTrinketOwner(TRINKET_PURPLE_HEART, NULL, true);
		if (player) {
			int mult = g_Game->GetPlayerManager()->GetTrinketMultiplier(TRINKET_PURPLE_HEART);
			chance *= (mult * 2);
		}

		if (g_Game->GetChallenge() == 0x22) { /* Ultra Hard */
			chance = 1.f;
		}
	}

	return chance;
}

bool Room::IsChampionBossSeed() const {
	RNG rng;
	rng._seed = _descriptor->SpawnSeed;
	rng._shift1 = 0x1;
	rng._shift2 = 0xb;
	rng._shift3 = 0x10;

	float random = rng.RandomFloat();
	return random <= GetChampionBossChance();
}

bool ItemConfig::IsValidTrinket(unsigned int TrinketType) {
	if (TrinketType != 0 && (TrinketType & 0x7fff) < g_Manager->GetItemConfig()->GetTrinkets()->size()) {
		if (g_Manager->GetItemConfig()->GetTrinket(TrinketType & 0x7fff) != nullptr) {
			return true;
		}
	}

	return false;
}

bool Isaac::IsInGame() {
	return g_Manager->GetState() == 2 && g_Game;
}

bool Entity_Player::AddSmeltedTrinket(int trinketID, bool firstTime) {
	bool trinketAdded = false;

	if (ItemConfig::IsValidTrinket(trinketID)) {
		const int actualTrinketID = trinketID & 0x7fff;
		if (trinketID != actualTrinketID) {
			_smeltedTrinkets[actualTrinketID]._goldenTrinketNum++;
		}
		else {
			_smeltedTrinkets[actualTrinketID]._trinketNum++;
		}

		TriggerTrinketAdded(trinketID, firstTime);

		History_HistoryItem* historyItem = new History_HistoryItem((TrinketType)trinketID, g_Game->_stage, g_Game->_stageType, g_Game->_room->_roomType, 0);
		GetHistory()->AddHistoryItem(historyItem);

		delete(historyItem);

		InvalidateCoPlayerItems();

		ItemConfig_Item* config = g_Manager->GetItemConfig()->GetTrinket(actualTrinketID);
		if (config && config->addCostumeOnPickup) {
			AddCostume(config, false);
		}

		trinketAdded = true;
	}
	return trinketAdded;
};

void ScoreSheet::AddFinishedStage(int stage, int stageType, unsigned int time) {
	if ((_runTimeLevel < stage) && g_Game->GetDailyChallenge()._id == 0) {
		_runTimeLevel = stage;
		_runTimeLevelType = stageType;
		_runTime = time;
	}
	return;
}

void EntityList_EL::Untie() {
	if (!_sublist) {
		ZHL::Log("[ERROR] Attempting to untie a list that is not a sublist, ignoring\n");
		return;
	}

	Entity** entities = (Entity**)calloc(_size, sizeof(Entity*));
	if (!entities) {
		ZHL::Log("[CRITICAL] Unable to allocate memory to untie sublist, ignoring\n");
		return;
	}

	_sublist = false;
	memcpy(entities, _data, _size * sizeof(Entity*));
	_data = entities;
}

bool Music::ValidateMusicID(int id, int& max) {
	max = _entries.size();

	return id >= 0 && id < max;
}
