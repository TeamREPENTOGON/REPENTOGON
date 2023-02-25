#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

#include <Windows.h>

#include <Zydis/Zydis.h>

#include "libzhl.h"

#define X86_LONGEST_INSTRUCTION_NBYTES 15

class LIBZHL_API ASMPatcher
{
public:
	enum class CondJumps {
		JO,
		JNO,
		JB, JNAE = JB, JC = JB,
		JNB, JAE = JNB, JNC = JNB,
		JZ, JE = JZ,
		JNZ, JNE = JNZ,
		JBE, JNA = JBE,
		JNBE, JA = JNBE,
		JS,
		JNS,
		JP, JPE = JP,
		JNP, JPO = JNP,
		JL, JNGE = JL,
		JNL, JGE = JNL,
		JLE, JNG = JLE,
		JNLE, JG = JNLE
	};

	class ProtectionGuard {
	public:
		ProtectionGuard(void* page, DWORD in, DWORD out = 0);
		~ProtectionGuard();

	private:
		void* _page;
		DWORD _in;
		DWORD _out;
	};

	friend class ProtectionGuard;

	static constexpr const size_t PAGE_SIZE = 4096;
	~ASMPatcher();

	ASMPatcher(const ASMPatcher&) = delete;
	ASMPatcher(ASMPatcher&&) = delete;
	ASMPatcher& operator=(ASMPatcher const&) = delete;
	ASMPatcher& operator=(ASMPatcher&&) = delete;

	static ASMPatcher& instance();

	void* AttemptPatchFunction(const char* beginSig, const char* endSig, const char* patchSig, const char* with);
	void* PatchFromSig(const char* sig, const char* with);
	void* PatchAt(void* at, const char* with);

	static ptrdiff_t JumpOffset(const void* next, const void* target);
	static std::unique_ptr<char[]> EncodeJump(const void* at, const void* target);
	static void* EncodeAndWriteJump(void* at, const void* target);
	static std::unique_ptr<char[]> EncodeCondJump(CondJumps cond, const void* at, const void* target);

	template<typename T>
	static std::enable_if_t<std::is_integral_v<T>> EndianConvert(T* t) {
		auto swap = [](uint8_t& a, uint8_t& b) -> void {
			uint8_t c = a;
			a = b;
			b = c;
		};

		uint8_t* ptr = (uint8_t*)t;
		if constexpr (sizeof(T) == 1) {
			return;
		} else if constexpr (sizeof(T) == 2) {
			swap(ptr[0], ptr[1]);
		}
		else if constexpr (sizeof(T) == 4) {
			swap(ptr[0], ptr[3]);
			swap(ptr[1], ptr[2]);
		}
	}

private:
	ASMPatcher();

	// Page will always be allocated with read and write, but never execute.
	// This function CANNOT return NULL.
	void* AllocPage();

	// Return either an existing page, or a new page if writing the patch would 
	// go out of the bounds of the last allocated page. 
	// The returned page will always have at least read and write privileges.
	// If withExecute is set to false, the page will not have execute permission 
	// regardless of its original protection. If withExecute is set to true, 
	// the page will have execute permission, regardless of its original protection.
	// By construction, both these functions CANNOT return NULL.
	void* GetAllocPage(const char* text, bool withExecute);
	void* GetAllocPage(size_t n, bool withExecute);

	static DWORD SetProtection(void* page, DWORD protect);
	static void GetMemoryInformation(void* page, PMEMORY_BASIC_INFORMATION infos);
	static DWORD Protect(PMEMORY_BASIC_INFORMATION const infos, DWORD protect);

	// __declspec(noreturn) void Error(const char* fmt, ...);

	std::vector<void*> _pages;
	void* _firstAvailable = nullptr;
	size_t _bytesRemaining = 0;

	std::atomic<bool> _patching;
};

class LIBZHL_API ASMPatch
{
public:
	ASMPatch();
	ASMPatch(const ASMPatch& other) = delete;
	ASMPatch& operator=(const ASMPatch& other) = delete;

	ASMPatch& AddBytes(const char* bytes);
	ASMPatch& AddRelativeJump(ptrdiff_t distance);
	ASMPatch& AddConditionalRelativeJump(ASMPatcher::CondJumps cond, ptrdiff_t distance);
	ASMPatch& AddInternalCall(void* addr);
	ASMPatch& AddDLLCall(void* addr);

	size_t Length() const;
	std::unique_ptr<char[]> ToASM(const char* patchBase, const char* writeAt) const;

private:
	class LIBZHL_API ASMNode {
	public:
		virtual std::unique_ptr<char[]> ToASM(const char* base, const char* at) const = 0;
		virtual size_t Length() const = 0;
	};

	class LIBZHL_API ASMBytes : public ASMNode {
	public:
		ASMBytes(const char* bytes);
		std::unique_ptr<char[]> ToASM(const char*, const char*) const override;
		size_t Length() const override;

	private:
		std::unique_ptr<char[]> _bytes;
	};

	class LIBZHL_API ASMJump : public ASMNode {
	public:
		ASMJump(ptrdiff_t dist);
		std::unique_ptr<char[]> ToASM(const char* base, const char* at) const override;
		size_t Length() const override;

	private:
		ptrdiff_t _dist;
	};

	class LIBZHL_API ASMCondJump : public ASMNode {
	public:
		ASMCondJump(ASMPatcher::CondJumps cond, ptrdiff_t dist);
		std::unique_ptr<char[]> ToASM(const char* base, const char* at) const override;
		size_t Length() const override;

	private:
		ASMPatcher::CondJumps _cond;
		ptrdiff_t _dist;
	};

	// friend void* ASMPatcher::PatchAt(void*, const char*);

	std::vector<std::unique_ptr<ASMNode>> _nodes;
	size_t _size = 0;
};

#define sASMPatcher ASMPatcher::instance()