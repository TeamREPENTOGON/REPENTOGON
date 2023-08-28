#pragma once

#include <cstdarg>

#include "libzhl.h"

namespace ZHL {
	template<typename T>
	void Throw(const char* text, ...) {
		va_list va;
		va_start(va, text);
		char buffer[4096];
		vsprintf(buffer, text, va);
		throw T(buffer);
	}
}