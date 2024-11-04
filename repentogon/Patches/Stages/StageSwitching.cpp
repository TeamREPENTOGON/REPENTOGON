#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageManager.h"
#include "../StagesStuff.h"

static bool IsSecondFloor(int stageid) {
	// God I wish there was an easier way to do this
	return (stageid == 2) || (stageid == 4) || (stageid == 6) || (stageid == 8) 
		|| (g_Game->_curses & (1 << 1)); //has curse XL
}

extern int toint(const string& str);
extern tuple<int, int> GetSetStage(int stageid, bool secondfloor);

static tuple<int, int> ConsoleStageIdToTuple(const string& input) {
	std::string numberPart;
	int letterValue = 0;
	for (char c : input) {
		if (isdigit(c)) {
			numberPart += c;
		}
		else {
			if (isalpha(c)) {
				int x = toint(numberPart);
				int y = c - 'a' + 1;
				if (y >= 3)
					y += 1;
				return { x , y };
			}
		}
	}
	int y = toint(numberPart);
	return { y ,0 };
}

static int GetStbTypeFromTuple(tuple<int, int> id) {
	auto it = std::find_if(std::begin(stageidtotuple), std::end(stageidtotuple),
		[&id](auto&& p) { return p.second == id; });

	if (it == std::end(stageidtotuple))
		return -1;

	return it->first;
}

int lastconfigId = 0;
int overloadLevelStage = 0;
int overloadStageType = 0;

extern std::vector<std::string> ParseCommand2(const std::string& command, int size);

HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player)-> void) {
	if (in.rfind("stage", 0) == 0) {
		ZHL::Logger logger(true);
		std::vector<std::string> cmdlets = ParseCommand2(in, 2);
		tuple<int, int> id = ConsoleStageIdToTuple(cmdlets[1]);

		logger.Log("recieved stage %s, calculated id %d, %d, of which we have %d of\n", cmdlets[1].c_str(), get<0>(id), get<1>(id), XMLStuff.StageData->bystagealt.count(id));

		// do we have a stage registered under that id?
		if (XMLStuff.StageData->bystagealt.count(id) > 0) {
			overloadLevelStage = get<0>(id);
			overloadStageType = get<1>(id);
			in = "stage 1";
		}
	}
	super(in, out, player);
}


HOOK_METHOD(Level, SetStage, (int levelStage, int stageType)-> void) {
	StageManager& stageManager = StageManager::GetInstance();
	ZHL::Logger logger(true);

	if (overloadLevelStage > 0)
		levelStage = overloadLevelStage;
		
	if (overloadStageType > 0)
		stageType = overloadStageType;

	tuple<int, int> idx = { levelStage, stageType };
	logger.Log("[INFO] node count for %d, %d : %d\n", levelStage, stageType, XMLStuff.StageData->bystagealt.count(idx));
	if (XMLStuff.StageData->bystagealt.count(idx) > 0) {
		XMLAttributes& targetstage = XMLAttributes(XMLStuff.StageData->nodes[XMLStuff.StageData->bystagealt[idx]]);
		int nodeId = toint((targetstage)["id"]);
		int configId = toint((targetstage)["basestage"]);

		std::string stage = to_string(levelStage);
		std::string stage2 = to_string(stageType);
		std::string stage3 = to_string(nodeId);
		std::string stage4 = to_string(configId);

		logger.Log("[INFO] SetStage: level %d, stage %d, node %d, config %d\n", levelStage, stageType, nodeId, configId);

		stageManager.LoadStage(configId, nodeId);

		tuple<int, int> superArgs = GetSetStage(configId, IsSecondFloor(levelStage));
		levelStage = get<0>(superArgs);
		stageType = get<1>(superArgs);

		logger.Log("[INFO] SetStage: got set stage %d, %d, second floor %s\n", levelStage, stageType, IsSecondFloor(levelStage) ? "true" : "false");

		logger.Log("done\n");
	}
	// dark home has special behavior, because of course it does...
	else if (levelStage != 13 || stageType != 1)
	{
		logger.Log("[ERROR] SetStage: found no node for level %d, stage %d!\n", levelStage, stageType);
		return;
	}

	// last ditch effort to avoid disaster
	if (levelStage == 0) {
		logger.Log("[ERROR] SetStage: attempted to set stage to 0! defaulting to basement\n");
		levelStage = 1;
		stageType = 0;
		
	}	

	super(levelStage, stageType);
	
	overloadLevelStage = 0;
	overloadStageType = 0;
}

HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int stage, unsigned int mode) -> bool) {
	if (stage == 36) {
		KAGE::LogMessage(1, "[RoomConfig] LoadStageBinary: force setting stage 36 to backwards\n");

		std::string filepath = this->_stages[stage]._rooms[mode]._filepath;
		std::string name = this->_stages[stage]._displayName;
		this->_stages[stage]._displayName = "(backwards)";
		this->_stages[stage]._rooms[mode]._filepath = "rooms/36.backwards.xml";

		bool res = super(stage, mode);

		this->_stages[stage]._displayName = name;
		this->_stages[stage]._rooms[mode]._filepath = filepath;

		return res;
	}
	return super(stage, mode);
}