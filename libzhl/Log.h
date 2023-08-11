#pragma once

#include "libzhl.h"

namespace ZHL {
	LIBZHL_API void Log(const char* fmt, ...);

	class LIBZHL_API Logger {
	public:
		Logger();
		~Logger();

		void Log(const char* fmt, ...);
		FILE* GetFile();

	private:
		FILE* _f;
	};
}