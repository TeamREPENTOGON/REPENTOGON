#include <sstream>

#include "ASMPatcher.hpp"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "SigScan.h"
#include "Delirium.h"

static void __stdcall PreTransformationCallback(Box<Entity_NPC*> deliriumBox) {
	lua_State* L = g_LuaEngine->_state;
	Entity_NPC* delirium = deliriumBox.Get();
	
	lua_getglobal(L, "DeliriumTransformation");
	lua::luabridge::UserdataPtr::push(L, delirium, delirium::DeliriumMetatable);
	lua_pcall(L, 1, 1, 0);

	bool value = lua_toboolean(L, -1);
	std::ostringstream str;
	str << "Isaac.ConsoleOutput(" << std::boolalpha << value << std::noboolalpha << ")";
	luaL_dostring(L, str.str().c_str());
}

namespace delirium {
	const char* DeliriumMetatable = "DeliriumMT";

	void PatchSkipFrames() {
		SigScan scanner("e85e5b5500");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		/* Override the call to the function that computes how many frames 
		 * must be skipped. Instead, set eax to 0 so no frames are skipped, 
		 * ever.
		 */
		ByteBuffer buffer;
		buffer.AddString("\x33\xC0"); // xor eax, eax
		ASMPatch patch(buffer);

		sASMPatcher.FlatPatch(addr, &patch);
	}

	void AddPreTransformationCallback() {
		SigScan scanner("c683230b000010b010");
		scanner.Scan();
		void* addr = scanner.GetAddress();

		ASMPatch patch;
		patch.AddBytes("\x53"); // push ebx
		patch.AddInternalCall(PreTransformationCallback);
		patch.AddRelativeJump((char*)addr + 0x7);
		sASMPatcher.PatchAt(addr, &patch);
	}

	void AddTransformationCallback() {

	}
}