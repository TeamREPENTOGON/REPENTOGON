#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../LuaInterfaces/_Internals.h"

HOOK_STATIC(Manager, Init, () -> void, __stdcall)
{
    super();
    LuaInternals::detail::RaiseInitError();
}