#include <Windows.h>
#include <DbgHelp.h>
#include <ehdata_forceinclude.h>
#include <stdio.h>

#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "HookSystem_private.h"
#include "IsaacRepentance.h"
#include "Log.h"

#include <fstream>
#include <filesystem>
#include <sstream>

typedef ASMPatch::ByteBuffer Signature;

static bool SymbolHandlerLoaded = false;
static bool SymbolHandlerInitialized = false;

static void InitializeSymbolHandler() {
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

enum SignatureCheckStatus {
	SIG_CHECK_UNIQUE,
	SIG_CHECK_MULTIPLE,
	SIG_CHECK_NONE,
};

static void ValidateSignature(size_t start, size_t end, unsigned char* sig, const unsigned char** p, size_t oldSize, const unsigned char* limit, std::list<const unsigned char*>& matches, ZHL::Logger* logger) {
	// logger->Log("[DEBUG] Validating signature from %p. Start = %u, end = %u, limit = %p\n", *p, start, end, limit);
	const unsigned char* ptr = *p;
	*p += oldSize;
	bool ok = true;
	for (size_t i = start; i < end && (!limit || *p != limit); ++i) {
		if (sig[i] != '?' && **p != sig[i]) {
			ok = false;
			break;
		}

		++*p;
	}

	if (ok) {
		matches.push_back(ptr);
	}
	else {
		++* p;
	}
}

static SignatureCheckStatus CheckSignature(const unsigned char* start, const unsigned char* end, Signature const& signature, std::list<const unsigned char*>& matches, size_t oldSize, ZHL::Logger* logger) {
	// logger->Log("[DEBUG] Checking signature between %p and %p\n", start, end);
	const unsigned char* p = start;
	unsigned char* sig = (unsigned char*)signature.GetData();
	size_t size = signature.GetSize();
	bool initial = matches.empty();

	// Initial search
	if (initial) {
		while (p != end) {
			ValidateSignature(0, size, sig, &p, 0, end, matches, logger);
		}
	}
	else {
		auto iter = matches.begin();
		std::list<const unsigned char*> newMatches;
		while (iter != matches.end()) {
			const unsigned char* ptr = *iter;
			ValidateSignature(oldSize, size, sig, &ptr, oldSize, NULL, newMatches, logger);
			++iter;
		}

		matches = std::move(newMatches);
	}

	// for (const unsigned char* match : matches) {
	// 	logger->Log("[DEBUG] Got match at %p\n", match);
	// }

	if (matches.size() == 0) {
		return SIG_CHECK_NONE;
	}
	else if (matches.size() == 1) {
		return SIG_CHECK_UNIQUE;
	}
	else {
		return SIG_CHECK_MULTIPLE;
	}
}

static void ReadRaw(Signature& s, unsigned char** raw, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		s.AddAny((const char*)*raw, 1);
		++* raw;
	}
}

static void WildcardRaw(Signature& s, unsigned char** raw, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		s.AddString("?");
		++* raw;
	}
}

// Add all the bytes preceeding the opcode and the opcode itself to the signature.
// In x86 format, this corresponds to legacy prefixes + opcodes with prefixes.
// Return true if everything went well, false if an error was encountered.
static bool AddInstructionHeaderToSignature(Signature& signature, ZydisDisassembledInstruction const& instruction, 
	unsigned char** raw, ZHL::Logger* logger) {
	if (instruction.info.encoding == ZydisInstructionEncoding::ZYDIS_INSTRUCTION_ENCODING_LEGACY) {
		// Legacy instruction contain a possibly mandatory prefix (SSE etc.)
		// followed by the opcode. The opcode can be one, two or three bytes, 
		// depending on the map used.
		ReadRaw(signature, raw, instruction.info.raw.prefix_count);

		// The number of bytes in the opcode depends on the mapping used.
		ZyanU8 opcodeBytes = 0;
		switch (instruction.info.opcode_map) {
		case ZydisOpcodeMap::ZYDIS_OPCODE_MAP_DEFAULT:
			opcodeBytes = 1;
			break;

		case ZydisOpcodeMap::ZYDIS_OPCODE_MAP_0F:
			opcodeBytes = 2;
			break;

		case ZydisOpcodeMap::ZYDIS_OPCODE_MAP_0F38:
		case ZydisOpcodeMap::ZYDIS_OPCODE_MAP_0F3A:
			opcodeBytes = 3;
			break;

		default:
			logger->Log("[ERROR] Unknown opcode map %d used on legacy opcode encoding\n", instruction.info.opcode_map);
			return false;
		}

		ReadRaw(signature, raw, opcodeBytes);

		return true;
	}
	else if (instruction.info.encoding == ZydisInstructionEncoding::ZYDIS_INSTRUCTION_ENCODING_VEX || 
		instruction.info.encoding == ZydisInstructionEncoding::ZYDIS_INSTRUCTION_ENCODING_XOP) {
		// VEX instructions start with a two or three bytes prefix, followed by the opcode.
		// The opcode itself is always one byte in this situation.
		// XOP instructions work similarly (though we shouldn't encounter them in Isaac).
		// The first byte is the prefix that distinguishes between 2 bytes VEX, 3 bytes VEX and XOP.

		unsigned char prefix = **raw;
		ZyanU8 opcodeBytes = 0;
		switch (prefix) {
		case '\xC4':
		case '\x08':
			opcodeBytes = 4;
			break;

		case '\xC5':
			opcodeBytes = 3;
			break;

		default:
			logger->Log("[ERROR] Unknown VEX/XOP prefix %#hhx in decoded instruction\n", prefix);
			return false;
		}

		ReadRaw(signature, raw, opcodeBytes);

		return true;
	}
	else {
		logger->Log("[ERROR] Unknown instruction encoding %d encountered\n", instruction.info.encoding);
		return false;
	}
}

static bool HasMoffs(ZydisDisassembledInstruction const& instruction) {
	ZyanU8 opcode = instruction.info.opcode;
	return opcode == '\xA0' || opcode == '\xA1' || opcode == '\xA2' || opcode == '\xA3';
}

static void AddInstructionOperandsToSignature(Signature& signature, ZydisDisassembledInstruction const& instruction,
	unsigned char** raw, unsigned char* moduleStart, size_t moduleSize, ZHL::Logger* logger) {
	ZyanU64 moduleAddr = (ZyanU64)moduleStart;

	// Read the Mod R/M and SIB bytes first.
	unsigned char* modrm = NULL;
	if (instruction.info.attributes & ZYDIS_ATTRIB_HAS_MODRM) {
		modrm = *raw;
		ReadRaw(signature, raw, 1);
	}

	unsigned char* sib = NULL;
	if (instruction.info.attributes & ZYDIS_ATTRIB_HAS_SIB) {
		sib = *raw;
		ReadRaw(signature, raw, 1);
	}

	// Read displacement. Displacement is present only in some configurations of 
	// ModR/M + SIB, and on specific instructions (movs from/to (e)A(X/H), specific opcodes).
	bool hasDisplacement = (modrm && instruction.info.raw.modrm.mod != 3) || HasMoffs(instruction);
	if (hasDisplacement) {
		// Displacement needs to be wildcarded when RM references the BP register and mod is 00 and the 
		// displacement falls into the address space of the proces. It means we may be attempting to 
		// read from / write to an absolute address.
		if (instruction.info.raw.modrm.mod == 0 && instruction.info.raw.modrm.rm == 5 /* BP */ &&
			instruction.info.raw.disp.value >= moduleAddr && instruction.info.raw.disp.value < moduleAddr + moduleSize) {
			// Mod = 0 && RM = 5 is always a 32 (4 * 8) bits displacement in protected mode
			WildcardRaw(signature, raw, 4);
		}
		else {
			ZyanU8 displacement = instruction.info.raw.disp.size / 8;
			// Raw reading so we don't explicitly convert from ZyanI64 to one / four bytes string
			ReadRaw(signature, raw, displacement);
		}
	}

	// Read immediates
	for (ZyanU8 i = 0; i < instruction.info.operand_count; ++i) {
		ZydisDecodedOperand const& operand = instruction.operands[i];
		if (operand.type == ZydisOperandType::ZYDIS_OPERAND_TYPE_IMMEDIATE) {
			// Addresses will always be written as 32 bits values in memory.
			// 
			// Heuristic: if the immediate is a value that falls anywhere
			// in the image of the process (_any_ region in the process), and its 
			// size is 32 bits, then it is an absolute address. Wildcard it.
			//
			// The heuristic relies on the fact that most immediate values are "low"
			// and are less likely to collide with the address space of the process.
			// Therefore, the higher the load address of the process, the more robust
			// this heuristic becomes.

			// Assume the immediate is an address, and read it as unsigned because signedness
			// is irrelevant for addresses.
			ZyanU64 value = operand.imm.value.u;
			//logger->Log(" (operand value = %#llx, low = %#llx, high = %#llx, size = %#x, size = %d, signed = %d) ", value, moduleAddr, moduleAddr + moduleSize, moduleSize, operand.size, operand.imm.is_signed);
			// First comparison is inclusive because the address can be the first instruction of 
			// the module. Second comparison is strict because the address cannot be the last 
			// address of the module (it would end up outside the module).
			if (value >= moduleAddr && value < moduleAddr + moduleSize && operand.size == 32) {
				ZyanU8 nWildcards = operand.size / 8;
				WildcardRaw(signature, raw, nWildcards);
			}
			else {
				// Raw reading so we don't explicitly convert from ZyanI64 to one / four bytes string
				ReadRaw(signature, raw, operand.size / 8);
			}
		}
	}
}

static bool IsUnconditionalJump(ZydisMnemonic mnemonic) {
	return mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JMP;
}

static bool IsConditionalJump(ZydisMnemonic mnemonic) {
	return mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JB ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JBE ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JL ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JLE ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNB ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNBE ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNL ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNLE ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNO ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNP ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNS ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JNZ ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JO ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JP ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JS ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JZ ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JECXZ ||
		mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_JCXZ;
}

static bool IsJump(ZydisMnemonic mnemonic) {
	return IsUnconditionalJump(mnemonic) || IsConditionalJump(mnemonic);
}

// Build a signature for the function at the given address. The signature will be unique
// in the context of the module from which the address comes, not necessarily unique
// in the context of the entire process.
// 
// This function must not raise any C++ exceptions, because there's already an exception
// ready to be processed on the stack (_CxxThrowException is suspended).
static std::optional<Signature> MakeSignature(DWORD64 address, ZHL::Logger* logger) {
	// Logging is fine because the Logger uses the C runtime instead of the C++
	// runtime. Errors will never raise C++ exceptions.
	ZHL::Logger l2(true);
	if (!logger) {
		logger = &l2;
	}

	// Conversion does not loose precision. address is in flat (virtual) mode and uintptr_t
	// is wide enough to represent any valid virtual address.
	unsigned char* ptr = (unsigned char*)(uintptr_t)address;

	HMODULE module = GetModuleHandle(NULL);
	if (!module) {
		logger->Log("[ERROR] Unable to make signature for function at %#llx, unable to get a handle to source module\n", address);
		return std::nullopt;
	}

	size_t moduleLength = 0;
	const unsigned char* textStart = NULL;
	size_t textSize = 0;
	{
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)module;
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((char*)module + dos->e_lfanew);
		moduleLength = nt->OptionalHeader.SizeOfImage;
		textStart = (const unsigned char*)module + nt->OptionalHeader.BaseOfCode;
		textSize = nt->OptionalHeader.SizeOfCode;
	}

	//logger->Log("[DEBUG] Module %p, length = %u\n", module, moduleLength);

	ZyanU64 zyanAddr = address;
	bool stop = false;
	std::list<const unsigned char*> matches;
	Signature signature;
	size_t signatureSize = 0;
	ZyanU64 farthestAddr = address;
	// Disassemble each instruction, one by one. Bytes that must be wildcarded are wildcarded
	// others are kept as-is.
	do {
		// logger->Log("Begin loop, buffer size = %u\n", signature.GetSize());
		ZydisDisassembledInstruction instruction;
		ZyanStatus status = ZydisDisassembleIntel(ZydisMachineMode::ZYDIS_MACHINE_MODE_LEGACY_32, zyanAddr, ptr, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
		if (!ZYAN_SUCCESS(status)) {
			logger->Log("[ERROR] Unable to make signature for function at %#llx, unable to decode instruction at %#llx\n", address, zyanAddr);
			return std::nullopt;
		}

		Signature instructionSig;

		ZydisMnemonic mnemonic = instruction.info.mnemonic;
		// If the function was detoured, the first bytes may be completely absurd.
		if (signatureSize == 0 && IsUnconditionalJump(mnemonic)) {
			logger->Log("[INFO] Function at %#llx was detoured, first bytes of the signature will be wrong\n", address);
		}

		if (signatureSize != 0 && IsJump(mnemonic)) {
			ZyanU64 target;
			status = ZydisCalcAbsoluteAddress(&instruction.info, instruction.operands, zyanAddr, &target);

			if (!ZYAN_SUCCESS(status)) {
				logger->Log("[ERROR] Unable to compute absolute address of jump while making signature\n");
				return std::nullopt;
			}

			// If the target of the jump falls in the text segment, move the limit of 
			// disassembly further, if needed.
			if (target >= (ZyanU64)textStart && target < (ZyanU64)textStart + textSize) {
				if (target > farthestAddr) {
					farthestAddr = target;
				}
			}
			else {
				// Otherwise, we're in an ASM patched function (probably), and cannot compute a 
				// signature because of that.
				logger->Log("[ERROR] Unable to compute signature because function is ASM patched\n");
				return std::nullopt;
			}
		}
		
		// Rebuild the instruction in the signature.
		unsigned char* copy = ptr;
		if (!AddInstructionHeaderToSignature(instructionSig, instruction, &copy, logger)) {
			return std::nullopt;
		}

		//logger->Log("[DEBUG] Signature: Header = ");
		//logger->LogByteBuffer(instructionSig);
		// logger->Log("After header, buffer size = %lu\n", signature.GetSize());

		AddInstructionOperandsToSignature(instructionSig, instruction, &copy, (unsigned char*)module, moduleLength, logger);
		//logger->Log(", Operands = ");
		//logger->LogByteBuffer(instructionSig);
		// logger->Log("After operands, buffer size = %lu\n", signature.GetSize());
		//logger->Log("\n");

		signature.AddByteBuffer(instructionSig);
		//logger->Log("[DEBUG] Full signature: ");
		//logger->LogByteBuffer(signature);
		//logger->Log("\n");

		SignatureCheckStatus sigStatus = CheckSignature(textStart, textStart + textSize, signature, matches, signatureSize, logger);
		if (sigStatus == SIG_CHECK_NONE) {
			logger->Log("[ERROR] Invalid signature: search found no matches\n");
			return std::nullopt;
		}
		else if (sigStatus == SIG_CHECK_UNIQUE) {
			return std::move(signature);
		}

		signatureSize = signature.GetSize();

		zyanAddr += instruction.info.length;
		ptr += instruction.info.length;

		if (instruction.info.mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_RET) {
			// Hard boundary on rets
			if (zyanAddr >= farthestAddr) {
				logger->Log("[WARN] Multiple matches for signature, but end of function reached\n");
				stop = true;
			}
		}
	} while (!stop);

	return std::move(signature);
}

void __declspec(noreturn) CxxThrowException_Hook(void* except, ThrowInfo* info, void (__stdcall* super)(void*, ThrowInfo*)) {
	if (!SymbolHandlerInitialized) {
		InitializeSymbolHandler();
		SymbolHandlerInitialized = true;
	}

	ZHL::Logger logger(true);
	logger.Log("Exception thrown. Exception object at %p, exception data at %p\n", except, info);
	
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

		std::optional<Signature> signature = std::nullopt;

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
			if (!SymGetModuleInfo64(GetCurrentProcess(), symbol->Address, &module)) {
				symbolModule = "??? (probably a runtime trampoline)";
			}
			else {
				symbolModule = module.ModuleName;

				if (!strcmp(module.ModuleName, "isaac-ng")) {
					// logger.Log("Getting signature for function at %#x in module %s\n", symbol->Address, module.ModuleName);
					signature = MakeSignature(symbol->Address, &logger);
				}
			}
			symbolAddr = symbol->Address;

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

	super(except, info);
}

HOOK_GLOBAL(VCRuntime::_CxxThrowException, (void* except, ThrowInfo* info) -> void, __stdcall) {
	CxxThrowException_Hook(except, info, super);
}

/* HOOK_GLOBAL(VCRuntime::_CxxThrowExceptionD, (void* except, ThrowInfo* info) -> void, __stdcall) {
	CxxThrowException_Hook(except, info, super);
} */

static void TerminateSymbolHandler() {
	if (!SymbolHandlerLoaded) {
		// throw std::runtime_error("Symbol handler already unloaded");
		return;
	}

	SymbolHandlerLoaded = false;
	SymCleanup(GetCurrentProcess());
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		InitializeSymbolHandler();

#ifdef ZHL_LOG_FILE
		std::ofstream f(ZHL_LOG_FILE, std::ios_base::app);
		f.close();

		std::filesystem::path log_file(ZHL_LOG_FILE);
		if (!std::filesystem::is_regular_file(log_file)) {
			std::ostringstream err;
			err << "The specified ZHL_LOG_FILE " << ZHL_LOG_FILE << " is not a regular file";
			MessageBox(0, err.str().c_str(), "Error", MB_ICONERROR);
			ExitProcess(1);
		}
#endif
		if(!FunctionDefinition::Init())
		{
			auto const& missing = Definition::GetMissing();
			if (missing.size() == 1) {
				MessageBox(0, FunctionDefinition::GetLastError(), "Error", MB_ICONERROR);
			}
			else {
				std::ofstream out("libzhl.log");
				for (auto const& [isFunction, name] : missing) {
					out << name << " (";
					if (isFunction) {
						out << "function";
					}
					else {
						out << "variable";
					}
					out << ")" << std::endl;
				}
				MessageBox(0, "Multiple function / variables were not found", "Error", MB_ICONERROR);
			}
			ExitProcess(1);
		}

		ASMPatch::_Init();
		ASMPatch::SavedRegisters::_Init();
	}
	else {
		TerminateSymbolHandler();
	}

	
	return TRUE;
}
