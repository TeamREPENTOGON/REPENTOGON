#pragma once

#include "libzhl.h"
#include "ByteBuffer.h"

namespace ZHL {
	LIBZHL_API void Log(const char* fmt, ...);

	class LIBZHL_API Logger {
	public:
		Logger(bool forceFlush = false);
		~Logger();

		void Log(const char* fmt, ...);
		void LogByteBuffer(ByteBuffer const& buffer);
		FILE* GetFile();

	private:
		FILE* _f;
		bool _forceFlush;
	};
}