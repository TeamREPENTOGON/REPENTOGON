#include <Windows.h>

static DWORD SetPageMemoryRW(void* addr, MEMORY_BASIC_INFORMATION* info) {
    VirtualQuery(addr, info, sizeof(*info));
    DWORD old_protect;
    VirtualProtect(info->BaseAddress, info->RegionSize, PAGE_READWRITE, &old_protect);
    return old_protect;
}