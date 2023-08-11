#include <cstdarg>
#include <cstdio>

#include "Log.h"

#ifndef ZHL_LOG_FILE
#define ZHL_LOG_FILE "repentogon.log"
#endif

void ZHL::Log(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	
	FILE* f = fopen(ZHL_LOG_FILE, "a");
	vfprintf(f, fmt, va);
	fclose(f);

	va_end(va);
}

namespace ZHL {
	Logger::Logger() {
		_f = fopen(ZHL_LOG_FILE, "a");
	}

	Logger::~Logger() {
		if (_f)
			fclose(_f);
	}

	FILE* Logger::GetFile() {
		return _f;
	}

	void Logger::Log(const char* fmt, ...) {
		va_list va;
		va_start(va, fmt);
		vfprintf(_f, fmt, va);
		va_end(va);
	}
}