#include "IsaacRepentance.h"
#include "LuaCore.h"

MOD_EXPORT const char* L_StdString_CStr(const std::string* str) {
	return str->c_str();
}

MOD_EXPORT void L_StdString_Assign(std::string* str, const char* value) {
	*str = value;
}
