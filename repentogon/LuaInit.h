#pragma once

typedef void (*LuaCallback)(int callbackID, int Param, void** args, int numArgs, int* retType, void** ret);

namespace LuaKeys {
	// Callback Tracking for optimizations
	extern int runCallbackKey;
	extern int additiveCallbackKey;
	extern int preRenderCallbackKey;
	extern int entityTakeDmgCallbackKey;
}

// I know, I know. I have to have the prefix because I'll get typedef redefinition compile errors otherwise.
enum ReturnType {
	TYPE_NONE = 0,
	TYPE_STRING = 1,
	TYPE_NUMBER = 2,
	TYPE_BOOLEAN = 3,
};
extern LuaCallback L_RunCallback;