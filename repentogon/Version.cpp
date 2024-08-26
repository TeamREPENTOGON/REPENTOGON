#ifndef CMAKE_REPENTOGON_VERSION
#pragma message("CMAKE_REPENTOGON_VERSION undefined, build process unsafe")
#define CMAKE_REPENTOGON_VERSION "repentogon-nover"
#endif

extern "C" {
	__declspec(dllexport) const char* __REPENTOGON_VERSION = CMAKE_REPENTOGON_VERSION;
}
