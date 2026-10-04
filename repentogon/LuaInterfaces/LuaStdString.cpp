#include "IsaacRepentance.h"
#include "LuaCore.h"

MOD_EXPORT const char* L_StdString_CStr(const std::string* str) {
	return str->c_str();
}
