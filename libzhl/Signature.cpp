#include <list>

#include <Windows.h>
#include <DbgHelp.h>

#include "Log.h"
#include "Signature.h"

static void ValidateSignature(size_t start, size_t end, unsigned char* sig, const unsigned char** p, size_t oldSize, 
	const unsigned char* limit, std::list<const unsigned char*>& matches) {
	// logger->Log("[DEBUG] Validating signature from %p. Start = %u, end = %u, limit = %p\n", *p, start, end, limit);
	const unsigned char* ptr = *p;
	*p += oldSize;
	bool ok = true;
	for (size_t i = start; i < end && (!limit || *p != limit); ++i) {
		if (sig[i] != '?' && **p != sig[i]) {
			ok = false;
			break;
		}

		++* p;
	}

	if (ok) {
		matches.push_back(ptr);
	}
	else {
		++* p;
	}
}

Signature::SignatureCheckStatus Signature::CheckSignature(const unsigned char* start, 
	const unsigned char* end, std::list<const unsigned char*>& matches, 
	size_t oldSize) {
	// logger->Log("[DEBUG] Checking signature between %p and %p\n", start, end);
	const unsigned char* p = start;
	unsigned char* sig = (unsigned char*)_bytes.GetData();
	size_t size = _bytes.GetSize();
	bool initial = matches.empty();

	// Initial search
	if (initial) {
		while (p != end) {
			ValidateSignature(0, size, sig, &p, 0, end, matches);
		}
	}
	else {
		auto iter = matches.begin();
		std::list<const unsigned char*> newMatches;
		while (iter != matches.end()) {
			const unsigned char* ptr = *iter;
			ValidateSignature(oldSize, size, sig, &ptr, oldSize, NULL, newMatches);
			++iter;
		}

		matches = std::move(newMatches);
	}

	// for (const unsigned char* match : matches) {
	// 	logger->Log("[DEBUG] Got match at %p\n", match);
	// }

	if (matches.size() == 0) {
		return SignatureCheckStatus::SIG_CHECK_NONE;
	}
	else if (matches.size() == 1) {
		return SignatureCheckStatus::SIG_CHECK_UNIQUE;
	}
	else {
		return SignatureCheckStatus::SIG_CHECK_MULTIPLE;
	}
}

void Signature::ReadRaw(unsigned char** raw, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		_bytes.AddAny((const char*)*raw, 1);
		++* raw;
	}
}

void Signature::WildcardRaw(unsigned char** raw, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		_bytes.AddString("?");
		++* raw;
	}
}

// Add all the bytes preceeding the opcode and the opcode itself to the signature.
// In x86 format, this corresponds to legacy prefixes + opcodes with prefixes.
// Return true if everything went well, false if an error was encountered.
bool Signature::AddInstructionHeaderToSignature(ZydisDisassembledInstruction const& instruction,
	unsigned char** raw) {
	if (instruction.info.encoding == ZydisInstructionEncoding::ZYDIS_INSTRUCTION_ENCODING_LEGACY) {
		// Legacy instruction contain a possibly mandatory prefix (SSE etc.)
		// followed by the opcode. The opcode can be one, two or three bytes, 
		// depending on the map used.
		ReadRaw(raw, instruction.info.raw.prefix_count);

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
			_logger->Log("[ERROR] Unknown opcode map %d used on legacy opcode encoding\n", instruction.info.opcode_map);
			return false;
		}

		ReadRaw(raw, opcodeBytes);

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
			_logger->Log("[ERROR] Unknown VEX/XOP prefix %#hhx in decoded instruction\n", prefix);
			return false;
		}

		ReadRaw(raw, opcodeBytes);

		return true;
	}
	else {
		_logger->Log("[ERROR] Unknown instruction encoding %d encountered\n", instruction.info.encoding);
		return false;
	}
}

static bool HasMoffs(ZydisDisassembledInstruction const& instruction) {
	ZyanU8 opcode = instruction.info.opcode;
	return opcode == '\xA0' || opcode == '\xA1' || opcode == '\xA2' || opcode == '\xA3';
}

void Signature::AddInstructionOperandsToSignature(ZydisDisassembledInstruction const& instruction,
	unsigned char** raw) {
	ZyanU64 moduleAddr = (ZyanU64)_moduleAddr;

	// Read the Mod R/M and SIB bytes first.
	unsigned char* modrm = NULL;
	if (instruction.info.attributes & ZYDIS_ATTRIB_HAS_MODRM) {
		modrm = *raw;
		ReadRaw(raw, 1);
	}

	unsigned char* sib = NULL;
	if (instruction.info.attributes & ZYDIS_ATTRIB_HAS_SIB) {
		sib = *raw;
		ReadRaw(raw, 1);
	}

	// Read displacement. Displacement is present only in some configurations of 
	// ModR/M + SIB, and on specific instructions (movs from/to (e)A(X/H), specific opcodes).
	bool hasDisplacement = (modrm && instruction.info.raw.modrm.mod != 3) || HasMoffs(instruction);
	if (hasDisplacement) {
		// Displacement needs to be wildcarded when RM references the BP register and mod is 00 and the 
		// displacement falls into the address space of the proces. It means we may be attempting to 
		// read from / write to an absolute address.
		if (instruction.info.raw.modrm.mod == 0 && instruction.info.raw.modrm.rm == 5 /* BP */ &&
			instruction.info.raw.disp.value >= moduleAddr && instruction.info.raw.disp.value < moduleAddr + _moduleSize) {
			// Mod = 0 && RM = 5 is always a 32 (4 * 8) bits displacement in protected mode
			WildcardRaw(raw, 4);
		}
		else {
			ZyanU8 displacement = instruction.info.raw.disp.size / 8;
			// Raw reading so we don't explicitly convert from ZyanI64 to one / four bytes string
			ReadRaw(raw, displacement);
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
			if (value >= moduleAddr && value < moduleAddr + _moduleSize && operand.size == 32) {
				ZyanU8 nWildcards = operand.size / 8;
				WildcardRaw(raw, nWildcards);
			}
			else {
				// Raw reading so we don't explicitly convert from ZyanI64 to one / four bytes string
				ReadRaw(raw, operand.size / 8);
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

std::optional<ByteBuffer> Signature::Make() {
	// Logging is fine because the Logger uses the C runtime instead of the C++
	// runtime. Errors will never raise C++ exceptions.
	ZHL::Logger l2(true);
	if (!_logger) {
		_logger = &l2;
	}

	// Conversion does not loose precision. address is in flat (virtual) mode and uintptr_t
	// is wide enough to represent any valid virtual address.
	unsigned char* ptr = (unsigned char*)_addr;
	ZyanU64 zyanAddr = (ZyanU64)_addr;

	//logger->Log("[DEBUG] Module %p, length = %u\n", module, moduleLength);

	bool stop = false;
	std::list<const unsigned char*> matches;
	size_t signatureSize = 0;
	ZyanU64 farthestAddr = zyanAddr;
	// Disassemble each instruction, one by one. Bytes that must be wildcarded are wildcarded
	// others are kept as-is.
	do {
		// logger->Log("Begin loop, buffer size = %u\n", signature.GetSize());
		ZydisDisassembledInstruction instruction;
		ZyanStatus status = ZydisDisassembleIntel(ZydisMachineMode::ZYDIS_MACHINE_MODE_LEGACY_32, zyanAddr, ptr, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
		if (!ZYAN_SUCCESS(status)) {
			_logger->Log("[ERROR] Unable to make signature for function at %#llx, unable to decode instruction at %#llx\n", zyanAddr, zyanAddr);
			return std::nullopt;
		}

		ZydisMnemonic mnemonic = instruction.info.mnemonic;
		// If the function was detoured, the first bytes may be completely absurd.
		if (signatureSize == 0 && IsUnconditionalJump(mnemonic)) {
			_logger->Log("[INFO] Function at %#llx was detoured, first bytes of the signature will be wrong\n", zyanAddr);
		}

		if (signatureSize != 0 && IsJump(mnemonic)) {
			ZyanU64 target;
			status = ZydisCalcAbsoluteAddress(&instruction.info, instruction.operands, zyanAddr, &target);

			if (!ZYAN_SUCCESS(status)) {
				_logger->Log("[ERROR] Unable to compute absolute address of jump while making signature\n");
				return std::nullopt;
			}

			// If the target of the jump falls in the text segment, move the limit of 
			// disassembly further, if needed.
			if (target >= (ZyanU64)_moduleTextAddr && target < (ZyanU64)_moduleTextAddr + _moduleTextSize) {
				if (target > farthestAddr) {
					farthestAddr = target;
				}
			}
			else {
				// Otherwise, we're in an ASM patched function (probably), and cannot compute a 
				// signature because of that.
				_logger->Log("[ERROR] Unable to compute signature because function is ASM patched\n");
				return std::nullopt;
			}
		}

		// Rebuild the instruction in the signature.
		unsigned char* copy = ptr;
		if (!AddInstructionHeaderToSignature(instruction, &copy)) {
			return std::nullopt;
		}

		AddInstructionOperandsToSignature(instruction, &copy);

		SignatureCheckStatus sigStatus = CheckSignature((unsigned char*)_moduleTextAddr, (unsigned char*)_moduleTextAddr + _moduleTextSize, matches, signatureSize);
		if (sigStatus == SignatureCheckStatus::SIG_CHECK_NONE) {
			_logger->Log("[ERROR] Invalid signature: search found no matches\n");
			return std::nullopt;
		}
		else if (sigStatus == SignatureCheckStatus::SIG_CHECK_UNIQUE) {
			return _bytes;
		}

		signatureSize = _bytes.GetSize();

		zyanAddr += instruction.info.length;
		ptr += instruction.info.length;

		if (instruction.info.mnemonic == ZydisMnemonic::ZYDIS_MNEMONIC_RET) {
			// Hard boundary on rets
			if (zyanAddr >= farthestAddr) {
				_logger->Log("[WARN] Multiple matches for signature, but end of function reached\n");
				stop = true;
			}
		}
	} while (!stop);

	return _bytes;
}

// Build a signature for the function at the given address. The signature will be unique
// in the context of the module from which the address comes, not necessarily unique
// in the context of the entire process.
// 
// This function must not raise any C++ exceptions, because there's already an exception
// ready to be processed on the stack (_CxxThrowException is suspended).
Signature::Signature(void* addr, const char* moduleName, ZHL::Logger* logger) : _addr(addr), _logger(logger) {
	HMODULE module = GetModuleHandle(moduleName);
	if (!module) {
		logger->Log("[ERROR] Unable to make signature for function at %p, unable to get a handle to source module\n", addr);
		return;
	}

	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)module;
	IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((char*)module + dos->e_lfanew);
	_moduleSize = nt->OptionalHeader.SizeOfImage;
	_moduleTextAddr = (void*)((const unsigned char*)module + nt->OptionalHeader.BaseOfCode);
	_moduleTextSize = nt->OptionalHeader.SizeOfCode;
	_moduleAddr = module;
}