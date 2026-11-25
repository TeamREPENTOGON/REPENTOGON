#include "IsaacRepentance.h"
#include "libzhl.h"
#include "LuaEntityBridge.h"

static std::vector<Entity*> s_entityResults;

void StoreEntityResults(Entity** data, unsigned int size) {
	s_entityResults.assign(data, data + size);
}

MOD_EXPORT unsigned int L_Entity_GetResultsCount() {
	return (unsigned int)s_entityResults.size();
}

MOD_EXPORT Entity* L_Entity_GetResult(unsigned int index) {
	return s_entityResults[index];
}
