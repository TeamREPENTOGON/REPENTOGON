#include "IsaacRepentance.h"
#include "Log.h"

bool Room::IsValidRailType(int rail) {
	if (rail < 0 || rail > RailType::RAIL_NONE) {
		return false;
	}
	else {
		return rail <= RailType::RAIL_CROSSROAD ||
			rail == RailType::RAIL_HORIZONTAL ||
			rail == RailType::RAIL_VERTICAL ||
			rail == RailType::RAIL_DOWN_TO_RIGHT ||
			rail == RailType::RAIL_DOWN_TO_LEFT ||
			rail == RailType::RAIL_UP_TO_RIGHT ||
			rail == RailType::RAIL_UP_TO_LEFT ||
			rail == RailType::RAIL_CROSSROAD ||
			rail == RailType::RAIL_END_LEFT ||
			rail == RailType::RAIL_END_RIGHT ||
			rail == RailType::RAIL_END_UP ||
			rail == RailType::RAIL_END_DOWN ||
			rail == RailType::RAIL_HORIZONTAL_CART_LEFT ||
			rail == RailType::RAIL_HORIZONTAL_CART_RIGHT ||
			rail == RailType::RAIL_VERTICAL_CART_UP ||
			rail == RailType::RAIL_VERTICAL_CART_DOWN ||
			rail == RailType::RAIL_MINESHAFT_HORIZONTAL_1 ||
			rail == RailType::RAIL_MINESHAFT_HORIZONTAL_2 ||
			rail == RailType::RAIL_MINESHAFT_HORIZONTAL_3 ||
			rail == RailType::RAIL_MINESHAFT_VERTICAL_1 ||
			rail == RailType::RAIL_MINESHAFT_VERTICAL_2 ||
			rail == RailType::RAIL_MINESHAFT_VERTICAL_3 ||
			rail == RailType::RAIL_MINESHAFT_DOWN_TO_RIGHT_1 ||
			rail == RailType::RAIL_MINESHAFT_DOWN_TO_RIGHT_2 ||
			rail == RailType::RAIL_MINESHAFT_DOWN_TO_LEFT_1 ||
			rail == RailType::RAIL_MINESHAFT_DOWN_TO_LEFT_2 ||
			rail == RailType::RAIL_MINESHAFT_UP_TO_RIGHT_1 ||
			rail == RailType::RAIL_MINESHAFT_UP_TO_RIGHT_2 ||
			rail == RailType::RAIL_MINESHAFT_UP_TO_LEFT_1 ||
			rail == RailType::RAIL_MINESHAFT_UP_TO_LEFT_2 ||
			rail == RailType::RAIL_DOOR_LEFT ||
			rail == RailType::RAIL_DOOR_BOTTOM ||
			rail == RailType::RAIL_DOOR_RIGHT ||
			rail == RailType::RAIL_DOOR_TOP ||
			rail == RailType::RAIL_NONE;
	}
}

bool RoomConfig_Room::IsValidGridIndex(int index, bool includeWalls) const {
	if (index < 0) {
		return false;
	}

	/* Help regarding drawings:
	 *  - Walls are represented with xxx.
	 *  - In a room with out-of-bounds part, out-of-bounds parts are 
	 *    represented with ***.
	 *  - Numbers represent the grid index of in-bounds non-walls grids.
	 */
	switch (Shape) {
    /* |-----------------------------------------------------------|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |xxx 016                                                 xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                 118 xxx|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
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
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |xxx 046                                                 xxx|
     * |xxx                                                     xxx|
     * |xxx                                                 088 xxx|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
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
     * |*** *** *** *** xxx xxx xxx xxx xxx xxx xxx *** *** *** ***|
     * |*** *** *** *** xxx 020                 xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                 114 xxx *** *** *** ***|
     * |*** *** *** *** xxx xxx xxx xxx xxx xxx xxx *** *** *** ***|
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
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |xxx 016                                                 xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                     xxx|
     * |xxx                                                 218 xxx|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |-----------------------------------------------------------|
     */
	case ROOMSHAPE_1x2:
		if (includeWalls) {
		  return index >= 0 && index <= 234;
		} else {
		  return index % 15 != 0 && index % 15 != 14 &&
			index >= 16 && index <= 218;
		}

    /* |-----------------------------------------------------------|
     * |*** *** *** *** xxx xxx xxx xxx xxx xxx xxx *** *** *** ***|
     * |*** *** *** *** xxx 020                 xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                     xxx *** *** *** ***|
     * |*** *** *** *** xxx                 214 xxx *** *** *** ***|
     * |*** *** *** *** xxx xxx xxx xxx xxx xxx xxx *** *** *** ***|
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
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |xxx 029                                                                                                     xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                     224 xxx|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |---------------------------------------------------------------------------------------------------------------|
     */
	case ROOMSHAPE_2x1:
		if (includeWalls) {
		  return index >= 0 && index <= 251;
		} else {
		  return index % 28 != 0 && index % 28 != 27 && 
			index >= 29 && index <= 224;
		}

    /* |---------------------------------------------------------------------------------------------------------------|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |xxx 085                                                                                                     xxx|
     * |xxx                                                                                                         xxx|
     * |xxx                                                                                                     166 xxx|
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
     * |*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***|
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
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
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
     * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
     * |---------------------------------------------------------------------------------------------------------------|
     */
	case ROOMSHAPE_2x2:
		if (includeWalls) {
		  return index >= 0 && index <= 447;
		} else {
		  return index % 28 != 0 && index % 28 != 27 &&
			index >= 29 && index <= 418;
		}

    /* |---------------------------------------------------------------------------------------------------------------|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx 042                                             054 xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx 210                                             222 xxx|
	 * |xxx 225                                                                                                     xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                     418 xxx|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |---------------------------------------------------------------------------------------------------------------|
     */
	case ROOMSHAPE_LTL:
		if (includeWalls) {
			if (index >= 196) {
				return index <= 447;
			}
			else if (index >= 0) {
				return index % 28 >= 13;
			}
			else {
				return false;
			}
		}
		else {
			if (index >= 196) {
				return index >= 225 && index <= 418 &&
					index % 28 != 0 && index % 28 != 27;
			}
			else if (index >= 0) {
				return index >= 42 && index <= 222 &&
					index % 28 >= 14 && index % 28 <= 26;
			}
			else {
				return false;
			}
		}
	}

	/* |---------------------------------------------------------------------------------------------------------------|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx 029                                             041 xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx 197                                             209 xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |xxx 225                                                                                                     xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                     418 xxx|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |---------------------------------------------------------------------------------------------------------------|
	 */
	case ROOMSHAPE_LTR:
		if (includeWalls) {
			if (index >= 196) {
				return index <= 447;
			}
			else if (index >= 0) {
				return index % 28 <= 14;
			}
			else {
				return false;
			}
		}
		else {
			if (index >= 225) {
				return index <= 418;
			}
			else if (index >= 0) {
				return index >= 29 && index <= 209 &&
					index % 28 >= 1 && index % 28 <= 13;
			}
			else {
				return false;
			}
		}

	/* |---------------------------------------------------------------------------------------------------------------|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |xxx 029                                                                                                     xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx 238                                             250 xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx                                                     xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx 406                                             418 xxx|
	 * |*** *** *** *** *** *** *** *** *** *** *** *** *** xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |---------------------------------------------------------------------------------------------------------------|
	 */
	case ROOMSHAPE_LBL:
		if (includeWalls) {
			if (index >= 265) {
				return index <= 447 && index % 28 >= 13;
			}
			else if (index >= 0) {
				return index <= 251;
			}
			else {
				return false;
			}
		}
		else {
			if (index >= 238) {
				return index <= 418 && index % 28 >= 14 && index % 28 <= 26;
			}
			else if (index >= 0) {
				return index >= 29 && index <= 222 && 
					index % 28 != 0 && index % 28 != 27;
			}
			else {
				return false;
			}
		}

	/* |---------------------------------------------------------------------------------------------------------------|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |xxx 029                                                                                                     xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                         xxx|
	 * |xxx                                                                                                     222 xxx|
	 * |xxx 225                                             237 xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx                                                     xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx 393                                             405 xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx *** *** *** *** *** *** *** *** *** *** *** *** ***|
	 * |---------------------------------------------------------------------------------------------------------------|
	 */
	case ROOMSHAPE_LBR:
		if (includeWalls) {
			if (index >= 252) {
				return index <= 447 && index % 27 <= 14;
			}
			else if (index >= 0) {
				return index <= 251;
			}
			else {
				return false;
			}
		}
		else {
			if (index >= 253) {
				return index <= 405 && 
					index % 28 != 0 && index % 28 <= 13;
			}
			else if (index >= 0) {
				return index <= 222 &&
					index % 28 != 0 && index % 28 != 27;
			}
			else {
				return false;
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
