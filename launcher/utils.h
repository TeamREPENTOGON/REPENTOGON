#pragma once

#include <Windows.h>

PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc);


static int argc = 0;
static char** argv = NULL;

static bool HasCommandLineArgument(const char* arg)
{
	if (!argv)
		argv = CommandLineToArgvA(GetCommandLineA(), &argc);

	for (int i = 1; i < argc; ++i)
	{
		if (_stricmp(argv[i], arg) == 0) return true;
	}

	return false;
}