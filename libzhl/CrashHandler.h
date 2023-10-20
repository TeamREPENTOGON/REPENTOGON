#pragma once

#include "libzhl.h"

extern bool SymbolHandlerLoaded;
extern bool SymbolHandlerInitialized;

void InitializeSymbolHandler();
void TerminateSymbolHandler();

namespace ZHL {
	LIBZHL_API void SetExceptionHandlerEnabled(bool value);
}