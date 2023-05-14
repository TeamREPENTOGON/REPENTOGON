#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <sstream>
#include <stdexcept>

#include <Windows.h>
#include <Zydis/Zydis.h>

#include "ASMPatcher.hpp"
#include "SigScan.h"

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

void* ASMPatcher::Patch(void* at, void* targetPage, const char* with, size_t len) {
	FILE* f = fopen("repentogon.log", "a");
	fprintf(f, "Patching at %p\n", at);
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
		fprintf(f, "Nothing to rewrite\n");
	}
	// Jump does not override whole instruction, nop extra bytes
	else if (instruction.info.length > 5) {
		fprintf(f, "Instruction too long, noping the end\nStart at %p, for %d bytes\n", (char*)at + 5, instruction.info.length - 5);
		memset((char*)at + 5, 0x90, instruction.info.length - 5);
	} 
	// Jump overrides other instructions, nop them properly
	else {
		fprintf(f, "Instruction too short, multiple nops required\n");
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
		fprintf(f, "Write %d nops at %p\n", count - 5, (char*)at + 5);
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

	fclose(f);
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

ASMPatch::ASMPatch() {

}

void* ASMPatcher::EncodeAndWriteJump(void* at, const void* target) {
	std::unique_ptr<char[]> encoded = EncodeJump(at, target);
	memcpy(at, encoded.get(), 5);
	return encoded.get() + 5;
}

ASMPatch& ASMPatch::AddBytes(const char* bytes) {
	std::unique_ptr<ASMNode> ptr(new ASMBytes(bytes));
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

/* void ASMPatcher::Error(const char* fmt, ...) {
	char buffer[4096];

	va_list va;
	va_start(va, fmt);
	vsnprintf(buffer, 4096, fmt, va);
	va_end(va);

	throw std::runtime_error(buffer);
} */