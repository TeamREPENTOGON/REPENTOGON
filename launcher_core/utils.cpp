#include "dsound/utils.h"
#include <cstdio>
#include <cstdarg>
#include <windows.h>
#include <psapi.h> // For MODULEINFO

#include <atomic>
#include <string>

int argc = 0;
char** argv = NULL;

static bool cliInitialized = false;

void InitCLI() {
    if (cliInitialized)
        return;

    argv = CommandLineToArgvA(GetCommandLineA(), &argc);
}

std::string GetExeVersion() {
    HMODULE hModule = GetModuleHandle(NULL);
    MODULEINFO modInfo;
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO))) {
        return "Fucked";
    }
    const char* baseAddress = (const char*)modInfo.lpBaseOfDll;
    size_t moduleSize = modInfo.SizeOfImage;
    const char* searchStr = "isaacv";

    for (size_t i = 0; i < moduleSize - strlen(searchStr); ++i) {
        const char* potentialMatch = baseAddress + i;
        if (strncmp(potentialMatch, searchStr, strlen(searchStr)) == 0) {
            const char* afterIsaacv = potentialMatch + strlen(searchStr);
            const char* dashPos = strchr(afterIsaacv, '-');

            if (dashPos) {
                return std::string(afterIsaacv, dashPos);
            }
        }
    }

    return "Fucked";
}

PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc)
{
    PCHAR* argv;
    PCHAR  _argv;
    ULONG   len;
    ULONG   argc;
    CHAR   a;
    ULONG   i, j;

    BOOLEAN  in_QM;
    BOOLEAN  in_TEXT;
    BOOLEAN  in_SPACE;

    len = strlen(CmdLine);
    i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
        i + (len+2)*sizeof(CHAR));

    _argv = (PCHAR)(((PUCHAR)argv)+i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while( a = CmdLine[i] ) {
        if(in_QM) {
            if(a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch(a) {
            case '\"':
                in_QM = TRUE;
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                in_SPACE = FALSE;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                if(in_TEXT) {
                    _argv[j] = '\0';
                    j++;
                }
                in_TEXT = FALSE;
                in_SPACE = TRUE;
                break;
            default:
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                _argv[j] = a;
                j++;
                in_SPACE = FALSE;
                break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}

bool HasCommandLineArgument(const char* arg)
{
    if (!argv)
        argv = CommandLineToArgvA(GetCommandLineA(), &argc);

    for (int i = 1; i < argc; ++i)
    {
        if (_stricmp(argv[i], arg) == 0) return true;
    }

    return false;
}
