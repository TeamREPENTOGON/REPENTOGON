#include "libzhl.h"
#include "HookSystem.h"
#include "Version.h"
#include <Windows.h>
#include <stdio.h>

/********************************************************************************
HOOKING

The following macros are used to declare function hooks:
	HOOK_METHOD
	HOOK_STATIC
	HOOK_GLOBAL


The syntax of HOOK_METHOD and HOOK_STATIC is as follows:
	HOOK_METHOD(class name, function name, (arguments) -> return type)
	{
		function body
	}

All hookable class methods are preceded by LIBZHL_API in Isaac.h
If the method is static, use HOOK_STATIC, otherwise use HOOK_METHOD

Writing a hook for a non static class method works pretty much like writing a method
for that class, meaning that the "this" pointer is available and you can access class
members directly without using . or ->

All hooks give access to a special function named super(), which calls the original function


The syntax of HOOK_GLOBAL is pretty much the same, just without a class name:
	HOOK_GLOBAL(function name, (arguments) -> return type)
	{
		function body
	}

It is used to hook into global functions that don't belong to any class

super() doesn't necessarily need to receive the original arguments passed to the hooked function, in fact you can even decide
not to call it all if you want to completely overwrite the behavior of the function in certain cases


When more than one hook are attached to the same function, the last created hook will be executed first, and calling super() in that hook
will call the previous hook instead of the original function:

	Hook2
		code before super()
		Hook1
			code before super()
			original code
			code after super()
		end of Hook 1
		code after super()
	end of Hook 2

You can control the order in which hooks are called by giving them a priority number using the following macros:
	HOOK_METHOD_PRIORITY(class name, function name, priority, (arguments) -> return type)
	HOOK_STATIC_PRIORITY(class name, function name, priority, (arguments) -> return type)
	HOOK_GLOBAL_PRIORITY(function name, priority, (arguments) -> return type)

Hooks with a lower priority number will run first (yeah it's kinda backwards, sorry about that), while hooks with
a high priority number will run last, right before the original function is called
The default priority is 0, and priority numbers can be negative

********************************************************************************/

// This small function loads all the hooks and must be present in every mod

static char titlebar[128];
MOD_EXPORT int ModInit(int argc, char **argv)
{
	ZHL::Init();
	sprintf(titlebar, "The Binding of Isaac: Repentance (+ REPENTOGON v%d.%d.%d)", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	SetWindowTextA(GetActiveWindow(), titlebar);
	printf(":REPENTOGON:\n");
	return 0;
}
