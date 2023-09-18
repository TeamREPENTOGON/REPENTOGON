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

	void Logger::LogByteBuffer(const ByteBuffer& buffer) {
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

	void Logger::DumpLuaStack(lua_State* L) {
		int n = lua_gettop(L);
		Log("Dumping Lua stack of %d elements\n", n);
		for (int i = n; i > 0; --i) {
			DumpLuaStack(L, i);
		}
	}

	void Logger::DumpLuaStack(lua_State* L, int pos) {
		Log("Lua stack at index %d: ", pos);
		switch (lua_type(L, pos)) {
		case LUA_TNIL:
			Log("nil\n");
			break;

		case LUA_TBOOLEAN:
			Log("bool: %c\n", lua_toboolean(L, pos));
			break;

		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA:
			Log("userdata: %p\n", lua_touserdata(L, pos));
			break;

		case LUA_TNUMBER:
			Log("number: %lld\n", lua_tonumber(L, pos));
			break;

		case LUA_TSTRING:
			Log("string: %s\n", lua_tostring(L, pos));
			break;

		case LUA_TTABLE:
			Log("table\n");
			break;

		case LUA_TFUNCTION:
			Log("thread\n");
			break;

		default:
			Log("unknown\n");
			break;
		}
	}
}