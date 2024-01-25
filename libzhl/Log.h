#pragma once

#include <LuaJIT/src/lua.hpp>

#include "libzhl.h"
#include "ByteBuffer.h"

namespace ZHL {
	LIBZHL_API void Log(const char* fmt, ...);
	LIBZHL_API void ClearLogFile();

	class LIBZHL_API Logger {
	public:
		Logger(bool forceFlush = false);
		~Logger();

		void Log(const char* fmt, ...);
		void LogByteBuffer(ByteBuffer const& buffer);
		FILE* GetFile();
		void DumpLuaStack(lua_State* L);
		void DumpLuaStack(lua_State* L, int pos);

	private:
		FILE* _f;
		bool _forceFlush;
	};
}