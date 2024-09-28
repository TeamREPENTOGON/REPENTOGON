#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "processthreadsapi.h"
#include "../Version.h"
#include "../MiscFunctions.h"
#include "minidumpapiset.h"

bool exception_message_shown = false;
//HOOK_GLOBAL_PRIORITY(RepCaughtException, 9999, (int x)->void,__cdecl) {
void ___todo_exception_handler(int x){	//placeholder

	char message[1024]="Something went wrong in the vanilla part of the game's code! Lua traceback unavailable!";
	char buffer[1024];
//	super(x);
	if (!exception_message_shown) {
		if (g_LuaEngine != nullptr) {
			lua_Debug debuginfo;
			int stacklevel = 0;
			int is_valid_stack = lua_getstack(g_LuaEngine->_state, stacklevel, &debuginfo);
			snprintf(message, 1023, "Something went wrong in the vanilla part of the game's code!\n"
				"Lua traceback:\n");
			while (is_valid_stack) {
				lua_getinfo(g_LuaEngine->_state, "Sln", &debuginfo);
				snprintf(buffer, 1023, "%s(%d): %s\n", debuginfo.short_src, debuginfo.currentline, debuginfo.name);
				strncat_s(message, 1023, buffer, 1023);
				stacklevel += 1;
				is_valid_stack = lua_getstack(g_LuaEngine->_state, stacklevel, &debuginfo);
			};
		};
		exception_message_shown = true;

		typedef BOOL (*MiniDumpWriteDump)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);

		HMODULE dbghelp = LoadLibraryA("dbghelp");

		if (dbghelp == 0x0) {
			return;
		};

		MiniDumpWriteDump gen_minidump = (MiniDumpWriteDump)GetProcAddress(dbghelp, "MiniDumpWriteDump");
		if (gen_minidump == 0x0) {
			return;
		};
		SYSTEMTIME time_holder;
		GetLocalTime(&time_holder);

		DWORD cur_pid = GetCurrentProcessId();
		DWORD cur_thread = GetCurrentThreadId();

		char dump_name_fmt[] = "rgon-v%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp";
		char dump_name[1024]="";
		snprintf(dump_name,1023,dump_name_fmt,CMAKE_REPENTOGON_VERSION,time_holder.wYear,time_holder.wMonth,time_holder.wDay,time_holder.wHour,time_holder.wMinute,time_holder.wSecond,cur_pid,cur_thread);

		std::string dump_path;
		dump_path = REPENTOGON::GetRepentogonDataPath();
		dump_path += "\\..\\";
		dump_path += dump_name;
		//todo: x is some value from which exception info should be retreivable, not trivially accessible though!
		HANDLE file_handle=CreateFileA(dump_path.c_str(), 0x40000000, 1, (LPSECURITY_ATTRIBUTES)0x0, 0x2, 0x80, 0x0);
		(MiniDumpWriteDump)gen_minidump(GetCurrentProcess(), cur_pid, file_handle, _MINIDUMP_TYPE::MiniDumpWithDataSegs, nullptr,nullptr, nullptr);
		CloseHandle(file_handle);

		MessageBoxA(nullptr, message, "A super-basic crash handler!", MB_OK);

		DWORD exitcode;
		ExitProcess(GetExitCodeProcess(GetCurrentProcess(),&exitcode));
		return;
	};
};
