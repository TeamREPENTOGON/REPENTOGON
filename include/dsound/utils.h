#pragma once

#include <Windows.h>

extern int argc;
extern char** argv;

void InitCLI();
bool HasCommandLineArgument(const char* arg);
PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc);