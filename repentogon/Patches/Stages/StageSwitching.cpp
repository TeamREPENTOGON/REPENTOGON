#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageManager.h"
#include "../StagesStuff.h"

static bool IsOnSecondFloor() {
	int stageid = g_Game->_stage;
	// God I wish there was an easier way to do this
	return (stageid == 2) || (stageid == 4) || (stageid == 6) || (stageid == 8) 
		|| (g_Game->_curses & (1 << 1)); //has curse XL
}

extern int toint(const string& str);

static tuple<int, int> GetSetStage(int stageId, bool secondFloor) {
	if (stageidtotuple.empty()) {
		initstagetotuple();
	}
	if (stageidtotuple.count(stageId) > 0) {
		tuple<int, int> ret = stageidtotuple[stageId];
		if (secondFloor && (get<0>(ret) < 9)) {
			ret = { get<0>(ret) + 1,get<1>(ret) };
		}
		return ret;
	}
	else {
		return { stageId, 0 };
	}
}

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
				return { x ,c - 'a' + 1 };
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
	int baseStb = GetStbTypeFromTuple(idx);
	XMLAttributes& targetstage = baseStb != -1 ? XMLAttributes(XMLStuff.StageData->GetNodeById(baseStb)) : XMLAttributes(XMLStuff.StageData->nodes[XMLStuff.StageData->bystagealt[idx]]);
	if (XMLStuff.StageData->bystagealt.count(idx) > 0) {
		int nodeId = toint((targetstage)["id"]);
		int configId = toint((targetstage)["basestage"]);

		logger.Log("SetStage: level %d, stage %d, node %d, config %d\n", levelStage, stageType, nodeId, configId);

		stageManager.LoadStage(configId, nodeId);

		tuple<int, int> superArgs = GetSetStage(configId, IsOnSecondFloor());
		levelStage = get<0>(superArgs);
		stageType = get<1>(superArgs);

		logger.Log("done\n");
	}

	super(levelStage, stageType);
	
	overloadLevelStage = 0;
	overloadStageType = 0;
}