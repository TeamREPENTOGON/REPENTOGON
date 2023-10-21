#include <Windows.h>
#include <DbgHelp.h>
#include <ehdata_forceinclude.h>

#include "CrashHandler.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "Log.h"
#include "Signature.h"

bool SymbolHandlerLoaded = false;
bool SymbolHandlerInitialized = false;
bool ExceptionHandlerEnabled = true;

void ZHL::SetExceptionHandlerEnabled(bool value) {
	ExceptionHandlerEnabled = value;

	ZHL::Logger logger(true);
	logger.Log("[INFO] Stack tracing has been %s\n", value ? "enabled; exceptions will be logged." : "disabled; exceptions will not be logged.");
	return;
}

void InitializeSymbolHandler() {
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	HANDLE process = GetCurrentProcess();

	if (!SymInitialize(process, NULL, TRUE)) {
		DWORD error = GetLastError();
		ZHL::Log("[ERROR] Unable to initialize symbol handler: %d\n", error);
		return;
	}

	SymbolHandlerLoaded = true;
	return;
}

void TerminateSymbolHandler() {
	if (!SymbolHandlerLoaded) {
		// throw std::runtime_error("Symbol handler already unloaded");
		return;
	}

	SymbolHandlerLoaded = false;
	SymCleanup(GetCurrentProcess());
}


void __declspec(noreturn) CxxThrowException_Hook(void* except, ThrowInfo* info, void(__stdcall* super)(void*, ThrowInfo*)) {
	if (ExceptionHandlerEnabled) {
		ZHL::Logger logger(true);
		logger.Log("Exception thrown. Exception object at %p, exception data at %p\n", except, info);

		if (!SymbolHandlerInitialized) {
			InitializeSymbolHandler();
			SymbolHandlerInitialized = true;
		}

		CONTEXT context;
		RtlCaptureContext(&context);

		STACKFRAME64 frame;
		memset(&frame, 0, sizeof(frame));
		frame.AddrPC.Offset = context.Eip;
		frame.AddrPC.Mode = AddrModeFlat;

		frame.AddrFrame.Offset = context.Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;

		frame.AddrStack.Offset = context.Esp;
		frame.AddrStack.Mode = AddrModeFlat;

		int i = 0;
		logger.Log("### BEGIN STACK TRACE ###\n");
		while (StackWalk64(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(),
			&frame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {

			std::string symbolName;
			DWORD64 symbolAddr;
			std::string symbolModule;

			std::optional<ByteBuffer> signature = std::nullopt;

			if (SymbolHandlerLoaded) {
				DWORD64 displacement;
				IMAGEHLP_SYMBOL64* symbol = (IMAGEHLP_SYMBOL64*)malloc(sizeof(IMAGEHLP_SYMBOL64) + 1024);
				if (!symbol)
					break;
				memset(symbol, 0, sizeof(*symbol) + 1024);
				symbol->SizeOfStruct = sizeof(*symbol);
				symbol->MaxNameLength = 1024;
				SymGetSymFromAddr64(GetCurrentProcess(), frame.AddrPC.Offset, &displacement, symbol);
				if (strlen(symbol->Name) == 0) {
					symbolName = "???";
				}
				else {
					symbolName = symbol->Name;
				}

				IMAGEHLP_MODULE64 module;
				memset(&module, 0, sizeof(module));
				module.SizeOfStruct = sizeof(module);

				symbolAddr = symbol->Address;
				if (!SymGetModuleInfo64(GetCurrentProcess(), symbol->Address, &module)) {
					symbolModule = "??? (probably a runtime trampoline)";
				}
				else {
					symbolModule = module.ModuleName;

					if (!strcmp(module.ModuleName, "isaac-ng")) {
						// logger.Log("Getting signature for function at %#x in module %s\n", symbol->Address, module.ModuleName);
						Signature sig((void*)symbolAddr, NULL, &logger);
						signature = sig.Make();
					}
				}

				free(symbol);
			}
			else {
				symbolName = "<???>";
				symbolAddr = 0;
				symbolModule = "<???>";
			}

			logger.Log("#%d: %s (%#llx) in %s, PC = %#llx, Frame = %#llx, Signature = ", i, symbolName.c_str(), symbolAddr, symbolModule.c_str(), frame.AddrPC.Offset, frame.AddrFrame.Offset);
			if (signature) {
				logger.Log("\"");
				logger.LogByteBuffer(*signature);
				logger.Log("\"");
			}
			else {
				logger.Log("nil");
			}
			logger.Log("\n");
			++i;
		}

		logger.Log("### END STACK TRACE ###\n");

	}

	super(except, info);
}

HOOK_GLOBAL(VCRuntime::_CxxThrowException, (void* except, ThrowInfo* info) -> void, __stdcall) {
	CxxThrowException_Hook(except, info, super);
}

#ifdef DEBUG
HOOK_GLOBAL(VCRuntime::_CxxThrowExceptionD, (void* except, ThrowInfo* info) -> void, __stdcall) {
	CxxThrowException_Hook(except, info, super);
}
#endif