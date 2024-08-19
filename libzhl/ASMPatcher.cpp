#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <bitset>
#include <functional>
#include <vector>
#include <sstream>
#include <stdexcept>

#include <Windows.h>
#include <Zydis/Zydis.h>

#include "ASMPatcher.hpp"
#include "ByteBuffer.h"
#include "Log.h"
#include "SigScan.h"

// C4309: truncation of constant value
#pragma warning(disable : 4309)

#ifdef max
#undef max
#endif

ASMPatcher::ASMPatcher() {
	_patching.store(false, std::memory_order_relaxed);
}

ASMPatcher::~ASMPatcher() {

}

ASMPatcher& ASMPatcher::instance() {
	static ASMPatcher patcher;
	return patcher;
}

void* ASMPatcher::AttemptPatchFunction(const char*, const char*, const char*, const char*) {
	throw std::runtime_error("Not implemented");
	return nullptr;
}

void* ASMPatcher::PatchFromSig(const char* sig, const char* with) {
	SigScan scan(sig);
	bool found = scan.Scan();

	if (!found) {
		return nullptr;
	}

	return PatchAt(scan.GetAddress(), with);
}

void* ASMPatcher::PatchAt(void* at, ASMPatch* with) {
	void* targetPage = GetAllocPage(with->Length(), true);
	std::unique_ptr<char[]> text = with->ToASM(targetPage);
	return Patch(at, targetPage, text.get(), with->Length());
}

void* ASMPatcher::PatchAt(void* at, const char* with, size_t len) {
	void* targetPage = GetAllocPage(with, true);
	return Patch(at, targetPage, with, len);
}

void ASMPatcher::FlatPatch(void* at, ASMPatch* with, bool nopRest) {
	std::unique_ptr<char[]> text = with->ToASM(at);
	FlatPatch(at, text.get(), with->Length(), nopRest);
}

void ASMPatcher::FlatPatch(void* at, const char* with, size_t len, bool nopRest) {
	ZHL::Logger logger(true);
	logger.Log("Flat patching at %p\n", at);

	bool expected = false;
	if (!_patching.compare_exchange_strong(expected, true, std::memory_order_acq_rel, std::memory_order_acquire)) {
		logger.Log("[FATAL] Parallel patching is not allowed\n");
		throw std::runtime_error("Parallel patching is not allowed");
	}

	ProtectionGuard guard(at, PAGE_EXECUTE_READWRITE);

	if (nopRest) {
		int32_t remaining = len;
		ZyanU64 base = (ZyanU64)at;
		unsigned char* basePtr = (unsigned char*)at;
		while (remaining > 0) {
			ZydisDisassembledInstruction instruction;
			ZyanStatus status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LEGACY_32, (ZyanU64)base, basePtr, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
			if (!ZYAN_SUCCESS(status)) {
				std::ostringstream err;
				err << "Unable to properly decode instruction at " << std::hex << at << "(Zyan status = " << status << "), base = " << base << ", basePtr = " << basePtr << ")" << std::endl;
				logger.Log("[FATAL] %s\n", err.str().c_str());
				throw std::runtime_error(err.str());
			}

			ZyanU8 length = instruction.info.length;
			remaining -= length;
			basePtr += length;
			base += length;
		}

		// The patch breaks the last instruction: nop its remaining bytes
		if (remaining < 0) {
			memset(basePtr + remaining, 0x90, -remaining);
		}
	}

	memcpy(at, with, len);

	expected = true;
	if (!_patching.compare_exchange_strong(expected, false, std::memory_order_acq_rel, std::memory_order_acquire)) {
		logger.Log("[FATAL] Parallel patching is not allowed\n");
		throw std::runtime_error("Parallel patching is not allowed");
	}

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}

void* ASMPatcher::Patch(void* at, void* targetPage, const char* with, size_t len) {
	ZHL::Logger logger;
	logger.Log("Patching at %p\n", at);
	bool expected = false;
	if (!_patching.compare_exchange_strong(expected, true, std::memory_order_acq_rel, std::memory_order_acquire)) {
		throw std::runtime_error("Parallel patching is not allowed");
		return nullptr;
	}

	// void* targetPage = GetAllocPage(with, true);
	ProtectionGuard inputGuard(at, PAGE_EXECUTE_READWRITE);
	ProtectionGuard outputGuard(targetPage, PAGE_READWRITE, PAGE_EXECUTE_READWRITE);

	ZydisDisassembledInstruction instruction;
	ZyanStatus status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LEGACY_32, (ZyanU64)at, at, X86_LONGEST_INSTRUCTION_NBYTES, &instruction);
	if (!ZYAN_SUCCESS(status)) {
		throw std::runtime_error("Unable to properly decode instruction at which you intend to patch");
	}

	// Jump fits in neatly
	if (instruction.info.length == 5) {
		logger.Log("Nothing to rewrite\n");
	}
	// Jump does not override whole instruction, nop extra bytes
	else if (instruction.info.length > 5) {
		logger.Log("Instruction too long, noping the end\nStart at %p, for %d bytes\n", (char*)at + 5, instruction.info.length - 5);
		memset((char*)at + 5, 0x90, instruction.info.length - 5);
	} 
	// Jump overrides other instructions, nop them properly
	else {
		logger.Log("Instruction too short, multiple nops required\n");
		// From the address at which we write the jump, decode every instruction until we've moved at least five bytes.
		// At this point, the first instruction after the five bytes mark is the next valid instruction.
		// Rewrite the first five bytes as the jump, and nop the rest.
		//
		// In short: search for the next valid instruction after the five bytes mark.
		// Nope everything from five bytes to the beginning of the next instruction.
		size_t count = instruction.info.length;
		char* start = (char*)at + instruction.info.length;

		while (count < 5) {
			ZydisDisassembledInstruction ins;
			ZyanStatus status = ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LEGACY_32, (ZyanU64)start, start, X86_LONGEST_INSTRUCTION_NBYTES, &ins);
			if (!ZYAN_SUCCESS(status)) {
				throw std::runtime_error("Unable to properly decode instruction while trying to instructions that would be overriden by jump");
			}
			start = start + ins.info.length;
			count += ins.info.length;
		}

		// Assertion: the instruction designated by "start" is out of the 5 bytes range
		// Count is the number of bytes between "at" and "start"
		// Nop out all but the first five of these bytes.
		logger.Log("Write %d nops at %p\n", count - 5, (char*)at + 5);
		memset((char*)at + 5, 0x90, count - 5);
	}

	EncodeAndWriteJump(at, targetPage);
	size_t textLen = len;
	if (textLen == 0) {
		textLen = strlen(with);
	}
	memcpy(targetPage, with, textLen);
	_firstAvailable = (char*)_firstAvailable + textLen;
	_bytesRemaining -= textLen;

	expected = true;
	if (!_patching.compare_exchange_strong(expected, false, std::memory_order_acq_rel, std::memory_order_acquire)) {
		throw std::runtime_error("Parallel patching is not allowed");
		return nullptr;
	}

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);

	return targetPage;
}

void* ASMPatcher::AllocPage() {
	void* page = VirtualAlloc(NULL, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!page) {
		DWORD error = GetLastError();
		std::ostringstream err;
		err << "Ran out of memory while trying to reserve memory pages for ASM patch. GetLastError() = " << error << std::endl;
		throw std::runtime_error(err.str());
	}

	MEMORY_BASIC_INFORMATION info;
	GetMemoryInformation(page, &info);

	_firstAvailable = info.BaseAddress;
	_bytesRemaining = info.RegionSize;
	_pages.push_back(page);
	return page;
}

void* ASMPatcher::GetAllocPage(const char* text, bool withExecute) {
	return GetAllocPage(strlen(text), withExecute);
}

void* ASMPatcher::GetAllocPage(size_t n, bool withExecute) {
	void* page;
	if (_bytesRemaining < n) {
		page = AllocPage();
	} else {
		// page = _pages.back();
		page = _firstAvailable;
	}

	DWORD protection;
	if (withExecute) {
		protection = PAGE_EXECUTE_READWRITE;
	} else {
		protection = PAGE_READWRITE;
	}

	SetProtection(page, protection);

	return page;
}

DWORD ASMPatcher::SetProtection(void* page, DWORD protect) {
	MEMORY_BASIC_INFORMATION info;
	GetMemoryInformation(page, &info);
	return Protect(&info, protect);
}

void ASMPatcher::GetMemoryInformation(void* page, PMEMORY_BASIC_INFORMATION infos) {
	if (!infos) {
		throw std::runtime_error("NULL MEMORY_BASIC_INFORMATION passed to ASMPatcher::GetMemoryInformation");
	}

	SIZE_T query = VirtualQuery(page, infos, sizeof(*infos));
	if (query == 0) {
		DWORD error = GetLastError();
		std::ostringstream err;
		err << "Error while querying page information at " << page << ". GetLastError() = " << error << std::endl;
		throw std::runtime_error(err.str());
	}
}

DWORD ASMPatcher::Protect(PMEMORY_BASIC_INFORMATION const infos, DWORD protect) {
	DWORD oldProtect;
	BOOL result = VirtualProtect(infos->BaseAddress, infos->RegionSize, protect, &oldProtect);
	if (!result) {
		DWORD error = GetLastError();
		std::ostringstream err;
		err << "Error while setting protection on region of " << infos->RegionSize << " bytes at " << infos->BaseAddress << std::endl;
		throw std::runtime_error(err.str());
	}

	return oldProtect;
}

ASMPatcher::ProtectionGuard::ProtectionGuard(void* page, DWORD in, DWORD out) : _page(page), _in(in), _out(out) {
	DWORD original = ASMPatcher::SetProtection(page, in);
	if (!out) {
		_out = original;
	}
}

ASMPatcher::ProtectionGuard::~ProtectionGuard() {
	ASMPatcher::SetProtection(_page, _out);
}

ptrdiff_t ASMPatcher::JumpOffset(const void* next, const void* target) {
	// EIP + result = target
	// => result = target - eip
	return (intptr_t)target - (intptr_t)next;
}

std::unique_ptr<char[]> ASMPatcher::EncodeJump(const void* at, const void* target) {
	void* next = (char*)at + 5;
	ptrdiff_t offset = JumpOffset(next, target);
	char* buffer = new char[5];
	*buffer = 0xE9;
	memcpy(buffer + 1, &offset, sizeof(ptrdiff_t));
	return std::unique_ptr<char[]>(buffer);
}

std::unique_ptr<char[]> ASMPatcher::EncodeCondJump(CondJumps cond, const void* at, const void* target) {
	void* next = (char*)at + 6;
	ptrdiff_t offset = JumpOffset(next, target);
	char* buffer = new char[6];
	buffer[0] = 0x0F;
	char& res = buffer[1];

	switch (cond) {
	case CondJumps::JO:
		res = 0x80;
		break;

	case CondJumps::JNO:
		res = 0x81;
		break;

	case CondJumps::JB:
		res = 0x82;
		break;

	case CondJumps::JNB:
		res = 0x83;
		break;

	case CondJumps::JZ:
		res = 0x84;
		break;

	case CondJumps::JNZ:
		res = 0x85;
		break;

	case CondJumps::JBE:
		res = 0x86;
		break;

	case CondJumps::JNBE:
		res = 0x87;
		break;

	case CondJumps::JS:
		res = 0x88;
		break;

	case CondJumps::JNS:
		res = 0x89;
		break;

	case CondJumps::JP:
		res = 0x8A;
		break;

	case CondJumps::JNP:
		res = 0x8B;
		break;

	case CondJumps::JL:
		res = 0x8C;
		break;

	case CondJumps::JNL:
		res = 0x8D;
		break;

	case CondJumps::JLE:
		res = 0x8E;
		break;

	case CondJumps::JNLE:
		res = 0x8F;
		break;

	default:
		throw std::runtime_error("Invalid conditional jump opcode");
		break;
	}

	memcpy(buffer + 2, &offset, sizeof(offset));
	return std::unique_ptr<char[]>(buffer);
}

std::map<ASMPatch::Registers, std::bitset<8>> ASMPatch::_ModRM;

ASMPatch::ASMPatch() {

}

ASMPatch::ASMPatch(const ByteBuffer& buffer) {
	AddBytes(buffer);
}

std::map<uint32_t, ByteBuffer> ASMPatch::SavedRegisters::_RegisterPushMap;
std::map<uint32_t, ByteBuffer> ASMPatch::SavedRegisters::_RegisterPopMap;
std::array<uint32_t, 16> ASMPatch::SavedRegisters::_RegisterOrder;

ASMPatch::SavedRegisters::SavedRegisters(uint32_t mask, bool shouldRestore) {
	_mask = mask;
	_shouldRestore = shouldRestore;
}

ASMPatch::SavedRegisters::~SavedRegisters() {
	if (!_restored && _shouldRestore) {
		ZHL::Log("Backed up registers were not restored in ASMPatch");
		abort();
	}
}

uint32_t ASMPatch::SavedRegisters::GetMask() const {
	return _mask;
}

void ASMPatch::SavedRegisters::Restore() {
	if (_restored) {
		throw std::runtime_error("Backed up registers restored multiple times in ASMPatch");
	}

	_restored = true;
}

void ASMPatch::SavedRegisters::_Init() {
	using Reg = Registers;

	_RegisterPushMap[Reg::EAX] = std::move(ByteBuffer().AddString("\x50"));
	_RegisterPushMap[Reg::ECX] = std::move(ByteBuffer().AddString("\x51"));
	_RegisterPushMap[Reg::EDX] = std::move(ByteBuffer().AddString("\x52"));
	_RegisterPushMap[Reg::EBX] = std::move(ByteBuffer().AddString("\x53"));
	_RegisterPushMap[Reg::ESP] = std::move(ByteBuffer().AddString("\x54"));
	_RegisterPushMap[Reg::EBP] = std::move(ByteBuffer().AddString("\x55"));
	_RegisterPushMap[Reg::ESI] = std::move(ByteBuffer().AddString("\x56"));
	_RegisterPushMap[Reg::EDI] = std::move(ByteBuffer().AddString("\x57"));
	// For XMM registers, first make place on the stack, because
	// x86 doesn't allow direct push of an XMM value onto the stack.
	// Room is made first, then the value is moved from the register
	// to the stack. 
	// movupd is used because there is no guarantee that the stack will 
	// be aligned on a 16-byte boundary (as is expected by movapd).
	_RegisterPushMap[Reg::XMM0] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x04\x24")); // sub esp, 16 (0x10); movupd [esp], xmm0
	_RegisterPushMap[Reg::XMM1] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x0C\x24")); // sub esp, 16 (0x10); movupd [esp], xmm1
	_RegisterPushMap[Reg::XMM2] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x14\x24")); // sub esp, 16 (0x10); movupd [esp], xmm2
	_RegisterPushMap[Reg::XMM3] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x1C\x24")); // sub esp, 16 (0x10); movupd [esp], xmm3
	_RegisterPushMap[Reg::XMM4] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x24\x24")); // sub esp, 16 (0x10); movupd [esp], xmm4
	_RegisterPushMap[Reg::XMM5] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x2C\x24")); // sub esp, 16 (0x10); movupd [esp], xmm5
	_RegisterPushMap[Reg::XMM6] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x34\x24")); // sub esp, 16 (0x10); movupd [esp], xmm6
	_RegisterPushMap[Reg::XMM7] = std::move(ByteBuffer().AddString("\x83\xEC\x10").AddString("\x66\x0F\x11\x3C\x24")); // sub esp, 16 (0x10); movupd [esp], xmm7

	_RegisterPopMap[Reg::EAX] = std::move(ByteBuffer().AddString("\x58"));
	_RegisterPopMap[Reg::ECX] = std::move(ByteBuffer().AddString("\x59"));
	_RegisterPopMap[Reg::EDX] = std::move(ByteBuffer().AddString("\x5a"));
	_RegisterPopMap[Reg::EBX] = std::move(ByteBuffer().AddString("\x5b"));
	_RegisterPopMap[Reg::ESP] = std::move(ByteBuffer().AddString("\x5c"));
	_RegisterPopMap[Reg::EBP] = std::move(ByteBuffer().AddString("\x5d"));
	_RegisterPopMap[Reg::ESI] = std::move(ByteBuffer().AddString("\x5e"));
	_RegisterPopMap[Reg::EDI] = std::move(ByteBuffer().AddString("\x5f"));
	// Similar reasoning as above: we cannot pop from the stack into an 
	// XMM register, so first we move the value from the stack to the 
	// register, then we free the space used on the stack.
	// movupd used because no guarantee of alignment.
	_RegisterPopMap[Reg::XMM0] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x04\x24\x83\xC4\x10")); // movupd xmm0, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM1] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x0C\x24\x83\xC4\x10")); // movupd xmm1, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM2] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x14\x24\x83\xC4\x10")); // movupd xmm2, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM3] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x1C\x24\x83\xC4\x10")); // movupd xmm3, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM4] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x24\x24\x83\xC4\x10")); // movupd xmm4, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM5] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x2C\x24\x83\xC4\x10")); // movupd xmm5, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM6] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x34\x24\x83\xC4\x10")); // movupd xmm6, [esp]; add esp, 128
	_RegisterPopMap[Reg::XMM7] = std::move(ByteBuffer().AddString("\x66\x0F\x10\x3C\x24\x83\xC4\x10")); // movupd xmm7, [esp]; add esp, 128

	_RegisterOrder = std::array<uint32_t, 16>({ Reg::EAX, Reg::EBX, Reg::ECX, Reg::EDX, Reg::ESP, Reg::EBP, Reg::ESI, Reg::EDI, 
		Reg::XMM0, Reg::XMM1, Reg::XMM2, Reg::XMM3, Reg::XMM4, Reg::XMM5, Reg::XMM6, Reg::XMM7 });
}

void* ASMPatcher::EncodeAndWriteJump(void* at, const void* target) {
	std::unique_ptr<char[]> encoded = EncodeJump(at, target);
	memcpy(at, encoded.get(), 5);
	return encoded.get() + 5;
}

ASMPatch& ASMPatch::AddBytes(std::string const& bytes) {
	std::unique_ptr<ASMNode> ptr(new ASMBytes(bytes.c_str()));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::AddBytes(ByteBuffer const& bytes) {
	std::unique_ptr<ASMNode> ptr(new ASMBytesAny(bytes));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::AddRelativeJump(void* target) {
	std::unique_ptr<ASMNode> ptr(new ASMJump(target));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::AddConditionalRelativeJump(ASMPatcher::CondJumps cond, void* target) {
	std::unique_ptr<ASMNode> ptr(new ASMCondJump(cond, target));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::AddInternalCall(void* addr) {
	std::unique_ptr<ASMNode> ptr(new ASMInternalCall(addr));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::AddZeroes(uint32_t n) {
	if (n == 0) {
		throw std::runtime_error("Cannot add 0 zeroes to an ASMPatch");
	}

	std::unique_ptr<ASMNode> ptr(new ASMZeroes(n));
	_size += ptr->Length();
	_nodes.push_back(std::move(ptr));
	return *this;
}

ASMPatch& ASMPatch::PreserveRegisters(SavedRegisters& registers) {
	uint32_t mask = registers.GetMask();

	for (auto iter = SavedRegisters::_RegisterOrder.begin(); iter != SavedRegisters::_RegisterOrder.end(); ++iter) {
		if (mask & *iter) {
			AddBytes(SavedRegisters::_RegisterPushMap[*iter]);
		}
	}

	return *this;
}

ASMPatch& ASMPatch::RestoreRegisters(SavedRegisters& registers) {
	registers.Restore();

	std::ostringstream stream;
	uint32_t mask = registers.GetMask();
	using Reg = SavedRegisters::Registers;

	for (auto iter = SavedRegisters::_RegisterOrder.rbegin(); iter != SavedRegisters::_RegisterOrder.rend(); ++iter) {
		if (mask & *iter) {
			AddBytes(SavedRegisters::_RegisterPopMap[*iter]);
		}
	}

	return *this;
}

void ASMPatch::_Init() {
	using Reg = ASMPatch::Registers;

	_ModRM[Reg::EAX] = 0b00000000;
	_ModRM[Reg::ECX] = 0b00000001;
	_ModRM[Reg::EDX] = 0b00000010;
	_ModRM[Reg::EBX] = 0b00000011;
	_ModRM[Reg::ESP] = 0b00000100;
	_ModRM[Reg::EBP] = 0b00000101;
	_ModRM[Reg::ESI] = 0b00000110;
	_ModRM[Reg::EDI] = 0b00000111;
}

ASMPatch& ASMPatch::MoveFromMemory(ASMPatch::Registers src, int32_t offset, ASMPatch::Registers dst) {
	std::ostringstream stream;
	ByteBuffer offsetHex;
	// 7-6 : how many bytes of displacement (1 or 4), basically how many bytes after the opcode
	// 5-4-3 : destination register
	// 2-1-0 : source register
	std::bitset<8> modrmBits;
	// 32 bits opcode
	if (offset < -127 || offset > 127) {
		offsetHex = ToHexString(offset, false);
		modrmBits[7] = true;
		modrmBits[6] = false;
	}
	else {
		// 8 bits opcode
		offsetHex = ToHexString((int8_t)offset);
		modrmBits[7] = false;
		modrmBits[6] = true;
	}

	stream << "\x8B";
	// Mod R/M part, I hate this
		
	std::bitset<8> source = _ModRM[src];
	std::bitset<8> destination = _ModRM[dst] << 3;

	modrmBits |= (source | destination);

	uint8_t modRmVal = (uint8_t)modrmBits.to_ulong();
	ByteBuffer result;
	result.AddString("\x8B").AddByteBuffer(ToHexString(modRmVal)).AddByteBuffer(offsetHex);
	return AddBytes(result);
}

ASMPatch& ASMPatch::MoveToMemory(ASMPatch::Registers src, int32_t offset, ASMPatch::Registers dst) {
	ByteBuffer offsetHex;
	// 7-6 : how many bytes of displacement (1 or 4), basically how many bytes after the opcode
	// 5-4-3 : source register
	// 2-1-0 : destination register
	std::bitset<8> modrmBits;
	// 32 bits opcode
	if (offset < -127 || offset > 127) {
		offsetHex = ToHexString(offset, false);
		modrmBits[7] = true;
		modrmBits[6] = false;
	}
	else {
		// 8 bits opcode
		offsetHex = ToHexString((int8_t)offset);
		modrmBits[7] = false;
		modrmBits[6] = true;
	}

	// Mod R/M part, I hate this

	std::bitset<8> source = _ModRM[src] << 3;
	std::bitset<8> destination = _ModRM[dst];

	modrmBits |= (source | destination);

	uint8_t modRmVal = (uint8_t)modrmBits.to_ulong();
	ByteBuffer result;
	result.AddString("\x89").AddByteBuffer(ToHexString(modRmVal)).AddByteBuffer(offsetHex);
	return AddBytes(result);
}

ASMPatch& ASMPatch::MoveImmediate(ASMPatch::Registers dst, int32_t immediate) {
	char opcode = '\xB8';
	ByteBuffer offset;
	if (immediate >= -127 && immediate <= 127) {
		opcode = '\xB0';
		offset = ToHexString((int8_t)immediate);
	}
	else {
		offset = ToHexString(immediate, false);
	}

	opcode += RegisterTox86(dst);
	ByteBuffer instruction;
	instruction.AddByte(opcode);
	instruction.AddByteBuffer(offset);

	return AddBytes(instruction);
}

ASMPatch& ASMPatch::CopyRegister(std::variant<Registers, XMMRegisters> const& dst, 
	std::variant<Registers, XMMRegisters> const& src) {
	ByteBuffer bytes;
	XMMRegisters const* xmmDst = std::get_if<XMMRegisters>(&dst);
	XMMRegisters const* xmmSrc = std::get_if<XMMRegisters>(&src);
	/* Whether the reg field of the ModR/M byte is destination or not. */
	bool isRegDest = true;

	// Add opcode
	if (xmmDst || xmmSrc) {
		if (xmmDst) {
			if (xmmSrc) {
				// movss /r: 0xF3 0x0F 0x10 xmm, xmm/m32
				bytes.AddString("\xF3\x0F\x10");
			}
			else {
				// movd /r: 0x66 0x0F 0x6E mm, r/m32
				bytes.AddString("\66\x0F\x6E");
			}
		}
		else {
			// movd /r: 0x66 0x0F 0x7E r/m32, mm
			bytes.AddString("\x66\x0F\x7E");
			isRegDest = false;
		}
	}
	else {
		bytes.AddByte(0x8B);		
	}

	// Add R/M
	std::bitset<8> rm = ModRM(src, dst, isRegDest);
  #pragma warning(suppress : 4244)
	bytes.AddByte(rm.to_ulong());
	return AddBytes(bytes);
}

ASMPatch& ASMPatch::CopyRegister8(ASMPatch::Registers dst, ASMPatch::Registers src) {
	ByteBuffer bytes;
	// mov /r: 0x8A r8, r/m8
	bytes.AddByte(0x8A);
	std::bitset<8> rm = ModRM(src, dst, true);
  #pragma warning(suppress : 4244)
	bytes.AddByte(rm.to_ulong());
	return AddBytes(bytes);
}

/* static bool IsRegister(ASMPatch::LeaOperand const& operand) {
	return std::holds_alternative<ASMPatch::Registers>(operand);
}

static ASMPatch::LeaParameter const& ValidateLeaParameter(ASMPatch::LeaParameter const& param) {
	ASMPatch::LeaOps op;
	ASMPatch::LeaOperand operand;

	std::tie(op, operand) = param;
	if (op == ASMPatch::LeaOps::MULT) {
		if (IsRegister(operand)) {
			throw std::runtime_error("Operand of mult cannot be a register");
		}
		else {
			int32_t value = std::get<int32_t>(operand);
			if (value != 1 && value != 2 && value != 4 && value != 8) {
				std::ostringstream stream;
				stream << "Invalid SCALE value for MULT operand of lea: " << value;
				throw std::runtime_error(stream.str());
			}
		}
	}

	return param;
}

ASMPatch& ASMPatch::LoadEffectiveAddress(Registers dst, Registers src, std::optional<LeaParameter> firstParam, std::optional<LeaParameter> secondParam) {
	if (!firstParam && secondParam) {
		firstParam = secondParam;
		secondParam = std::nullopt;
	}

	LeaOps firstOp, secondOp;
	LeaOperand firstOperand, secondOperand;
	bool needSIB = false;
	ByteBuffer SIB;

	if (firstParam) {
		std::tie(firstOp, firstOperand) = ValidateLeaParameter(*firstParam);
	}

	if (secondParam) {
		std::tie(secondOp, secondOperand) = ValidateLeaParameter(*secondParam);
	}

	if (firstParam && secondParam) {
		if (firstOp == secondOp && firstOp == LeaOps::MULT) {
			throw std::runtime_error("Cannot use multiple multiplications in lea");
		}
	}
} */

ASMPatch& ASMPatch::LoadEffectiveAddress(Registers src, int32_t offset, Registers dst) {
	ByteBuffer result, hexOffset;
	result.AddString("\x8D"); // lea opcode

	// 7-6: mod
	// 5-3: dest
	// 2-0: src
	std::bitset<8> modrmBits = 0b00000000;
	if (offset < -127 || offset > 127) {
		modrmBits[7] = true;
		hexOffset = ToHexString(offset, false);
	}
	else {
		modrmBits[7] = false;
		hexOffset = ToHexString((int8_t)offset);
	}

	modrmBits[6] = !modrmBits[7];

	std::bitset<8> destBits = _ModRM[dst] << 3, srcBits = _ModRM[src];

	modrmBits |= (destBits | srcBits);
	result.AddByteBuffer(ToHexString((int8_t)modrmBits.to_ulong()))
		.AddByteBuffer(hexOffset);

	return AddBytes(result);
}

ASMPatch& ASMPatch::Push(ASMPatch::Registers reg) {
	uint32_t mask = RegisterToBackupRegister(reg);
	return AddBytes(ASMPatch::SavedRegisters::_RegisterPushMap[mask]);
}

ASMPatch& ASMPatch::Push(int8_t imm8) {
	ByteBuffer buffer;
	buffer.AddString("\x6A");
	buffer.AddByteBuffer(ASMPatch::ToHexString(imm8));
	return AddBytes(buffer);
}

ASMPatch& ASMPatch::Push(int32_t imm32) {
	ByteBuffer buffer;
	buffer.AddString("\x68");
	buffer.AddByteBuffer(ASMPatch::ToHexString(imm32, false));
	return AddBytes(buffer);
}

ASMPatch& ASMPatch::Push(ASMPatch::Registers reg, int32_t imm32) {
	ByteBuffer buffer, immBuffer;
	buffer.AddString("\xFF");
	std::bitset<8> modrm;

	// ModRM displacement 
	if (imm32 < -127 || imm32 > 127) {
		immBuffer = ASMPatch::ToHexString(imm32, false);
		modrm[7] = true;
	}
	else {
		immBuffer = ASMPatch::ToHexString((int8_t)imm32);
		modrm[7] = false;
	}

	modrm[6] = !modrm[7];
	
	// ModRM Push specific identifier
	modrm[5] = modrm[4] = true;
	modrm[3] = false;

	// ModRM register
	modrm |= _ModRM[reg];

	buffer.AddByteBuffer(ToHexString((int8_t)modrm.to_ulong())).AddByteBuffer(immBuffer);
	return AddBytes(buffer);
}

ASMPatch& ASMPatch::Pop(ASMPatch::Registers reg) {
	uint32_t mask = RegisterToBackupRegister(reg);
	return AddBytes(ASMPatch::SavedRegisters::_RegisterPopMap[mask]);
}

uint32_t ASMPatch::RegisterToBackupRegister(ASMPatch::Registers reg) {
	using GPReg = ASMPatch::Registers;
	using Reg = ASMPatch::SavedRegisters::Registers;

	switch (reg) {
	case GPReg::EAX:
		return Reg::EAX;

	case GPReg::EBX:
		return Reg::EBX;

	case GPReg::ECX:
		return Reg::ECX;

	case GPReg::EDX:
		return Reg::EDX;

	case GPReg::ESP:
		return Reg::ESP;

	case GPReg::EBP:
		return Reg::EBP;

	case GPReg::ESI:
		return Reg::ESI;

	case GPReg::EDI:
		return Reg::EDI;

	default:
		throw std::runtime_error("Invalid register");
	}
}

ASMPatch::ASMBytes::ASMBytes(const char* bytes) : _bytes()  {
	char* buffer = new char[strlen(bytes) + 1];
	strcpy(buffer, bytes);
	_bytes.reset(buffer);
}

std::unique_ptr<char[]> ASMPatch::ASMBytes::ToASM(void*) const {
	std::unique_ptr<char[]> result(new char[Length() + 1]);
	strcpy(result.get(), _bytes.get());
	return result;
}

size_t ASMPatch::ASMBytes::Length() const {
	return strlen(_bytes.get());
}

ASMPatch::ASMJump::ASMJump(void* target) : _target(target) {

}

std::unique_ptr<char[]> ASMPatch::ASMJump::ToASM(void* at) const {
	std::unique_ptr<char[]> jump = ASMPatcher::EncodeJump(at, _target);
	return jump;
}

ASMPatch::ASMZeroes::ASMZeroes(uint32_t n) : _n(n) {

}

std::unique_ptr<char[]> ASMPatch::ASMZeroes::ToASM(void*) const {
	std::unique_ptr<char[]> content(new char[_n]);
	memset(content.get(), 0, _n);
	return content;
}

size_t ASMPatch::ASMZeroes::Length() const {
	return _n;
}

size_t ASMPatch::ASMJump::Length() const {
	return 5;
}

ASMPatch::ASMCondJump::ASMCondJump(ASMPatcher::CondJumps cond, void* target) : _cond(cond), _target(target) {

}

std::unique_ptr<char[]> ASMPatch::ASMCondJump::ToASM(void* at) const {
	std::unique_ptr<char[]> jump = ASMPatcher::EncodeCondJump(_cond, at, _target);
	return jump;
}

size_t ASMPatch::ASMCondJump::Length() const {
	return 6;
}

ASMPatch::ASMInternalCall::ASMInternalCall(void* target) : _target(target) {

}

std::unique_ptr<char[]> ASMPatch::ASMInternalCall::ToASM(void* at) const {
	std::unique_ptr<char[]> call(new char[5]);
	call[0] = '\xE8';
	ptrdiff_t diff = ASMPatcher::JumpOffset((char*)at + 5, _target);
	memcpy(call.get() + 1, &diff, sizeof(ptrdiff_t));
	return call;
}

size_t ASMPatch::ASMInternalCall::Length() const {
	return 5;
}

ASMPatch::ASMBytesAny::ASMBytesAny(ByteBuffer const& bytes) : _bytes(bytes) {

}

std::unique_ptr<char[]> ASMPatch::ASMBytesAny::ToASM(void*) const {
	char* data = _bytes.GetData();
	size_t size = _bytes.GetSize();
	std::unique_ptr<char[]> bytes(new char[size]);
	memcpy(bytes.get(), data, size);
	return bytes;
}

size_t ASMPatch::ASMBytesAny::Length() const {
	return _bytes.GetSize();
}

std::unique_ptr<char[]> ASMPatch::ToASM(void* at) const {
	std::unique_ptr<char[]> result(new char[_size + 1]);
	size_t pos = 0;
	void* begin = at;

	for (std::unique_ptr<ASMNode> const& ptr : _nodes) {
		// memcpy because strings are not zero terminated (or contain zeroes)
		std::unique_ptr<char[]> res = ptr->ToASM(begin);
		memcpy(result.get() + pos, res.get(), ptr->Length());
		pos += ptr->Length();
		begin = (char*)at + pos;
	}

	result.get()[_size] = '\0';
	return result;
}

size_t ASMPatch::Length() const {
	return _size;
}

ByteBuffer ASMPatch::ToHexString(int8_t x) {
	ByteBuffer result;
	char value;
	memcpy(&value, &x, 1);
	result.AddAny(&value, 1);
	return result;
}

ByteBuffer ASMPatch::ToHexString(int16_t x, bool endianConvert) {
	char buffer[2];
	memcpy(buffer, &x, 2);

	if (endianConvert) {
		// Flip 0 and 1
		EndianSwap(buffer, 0, 1);
	}

	ByteBuffer result;
	result.AddAny(buffer, 2);
	return result;
}

ByteBuffer ASMPatch::ToHexString(int32_t x, bool endianConvert) {
	char buffer[4];
	memcpy(buffer, &x, 4);

	if (endianConvert) {
		EndianSwap(buffer, 0, 3);
		EndianSwap(buffer, 1, 2);
	}

	ByteBuffer result;
	result.AddAny(buffer, 4);
	return result;
}

/* void ASMPatcher::Error(const char* fmt, ...) {
	char buffer[4096];

	va_list va;
	va_start(va, fmt);
	vsnprintf(buffer, 4096, fmt, va);
	va_end(va);

	throw std::runtime_error(buffer);
} */

uint8_t ASMPatch::RegisterTox86(ASMPatch::Registers reg) {
	switch (reg) {
	case Registers::EAX:
		return 0b000;

	case Registers::EBX:
		return 0b011;

	case Registers::ECX:
		return 0b001;

	case Registers::EDX:
		return 0b010;

	case Registers::ESP:
		return 0b100;

	case Registers::EBP:
		return 0b101;

	case Registers::ESI:
		return 0b110;

	case Registers::EDI:
		return 0b111;

	default:
		throw std::runtime_error("Invalid GP register");
		return -1;
	}
}

uint8_t ASMPatch::RegisterTox86(XMMRegisters reg) {
	switch (reg) {
	case XMMRegisters::XMM0:
		return 0b000;

	case XMMRegisters::XMM1:
		return 0b001;

	case XMMRegisters::XMM2:
		return 0b010;

	case XMMRegisters::XMM3:
		return 0b011;

	case XMMRegisters::XMM4:
		return 0100;

	case XMMRegisters::XMM5:
		return 0b101;

	case XMMRegisters::XMM6:
		return 0b110;

	case XMMRegisters::XMM7:
		return 0b111;

	default:
		throw std::runtime_error("Invalid XMM register");
		return -1;
	}
}

std::bitset<8> ASMPatch::ModRM(std::variant<Registers, XMMRegisters> const& src, 
	std::variant<Registers, XMMRegisters> const& dst, bool isRegDst) {
	uint8_t rmDst = std::visit([=](auto x) { return RegisterTox86(x);  }, dst);
	uint8_t rmSrc = std::visit([=](auto x) { return RegisterTox86(x);  }, src);

	if (isRegDst) {
		return 0b11000000 | (rmDst << 3) | rmSrc;
	}
	else {
		return 0b11000000 | (rmSrc << 3) | rmDst;
	}
}