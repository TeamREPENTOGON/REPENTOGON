#ifndef CMAKE_ZHL_VERSION
#pragma message("CMAKE_ZHL_VERSION undefined, build process unsafe")
#define CMAKE_ZHL_VERSION "zhl-nover"
#endif

extern "C" {
	__declspec(dllexport) const char* __ZHL_VERSION = CMAKE_ZHL_VERSION;
}
