#include "Version.h"

extern "C" {
	__declspec(dllexport) const char* __REPENTOGON_VERSION = CMAKE_REPENTOGON_VERSION;
}
