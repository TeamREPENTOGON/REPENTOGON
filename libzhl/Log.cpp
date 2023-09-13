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
	Logger::Logger(bool forceFlush) {
		_f = fopen(ZHL_LOG_FILE, "a");
		_forceFlush = forceFlush;
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

		if (_forceFlush)
			fflush(_f);
	}

	void Logger::LogByteBuffer(const ASMPatch::ByteBuffer& buffer) {
		const unsigned char* data = (const unsigned char*)buffer.GetData();
		size_t size = buffer.GetSize();

		// Log("Byte buffer size = %llu\n", size);

		// std::unique_ptr<char[]> ptr(new char[size + 1]);
		// ptr[size] = '\0';

		// for (size_t i = 0; i < size; ++i) {
			// sprintf(ptr.get() + i, "%.2hhx ", data[i]);
		// }

		for (size_t i = 0; i < size; ++i) {
			fprintf(_f, "%.2hhx ", data[i]);
		}
	}
}