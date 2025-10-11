#pragma once

#include <array>
#include <atomic>
#include <bitset>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

#include <Windows.h>

#include <Zydis/Zydis.h>

#include "ByteBuffer.h"
#include "libzhl.h"
#include "x86_defines.h"

extern "C" {
	__declspec(dllexport) int InitZHL(void (__stdcall*ptr)(), void**, char*);
}

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
	friend class ASMPatch;

	static constexpr const size_t PAGE_SIZE = 4096;
	~ASMPatcher();

	ASMPatcher(const ASMPatcher&) = delete;
	ASMPatcher(ASMPatcher&&) = delete;
	ASMPatcher& operator=(ASMPatcher const&) = delete;
	ASMPatcher& operator=(ASMPatcher&&) = delete;

	static ASMPatcher& instance();

	void* AttemptPatchFunction(const char* beginSig, const char* endSig, const char* patchSig, const char* with);
	void* PatchAt(const char* signature, const char* name, ASMPatch* with);
	void* PatchAt(const char* signature, const char* name, const char* with, size_t len = 0);
	void* PatchAt(void* at, const char* with, size_t len = 0);
	void* PatchAt(void* at, ASMPatch* with, size_t* len = NULL);
	void* Patch(void* at, void* page, const char* with, size_t len);

	void FlatPatch(void* at, ASMPatch* with, bool nopRest = false);
	void FlatPatch(void* at, const char* with, size_t len = 0, bool nopRest = false);
	bool FlatPatch(const char* signature, const char* name, ASMPatch* with, bool nopRest = false);
	bool FlatPatch(const char* signature, const char* name, const char* with, size_t len = 0, bool nopRest = false);

	static ptrdiff_t JumpOffset(const void* next, const void* target);
	static std::unique_ptr<char[]> EncodeJump(const void* at, const void* target);
	static void* EncodeAndWriteJump(void* at, const void* target);
	static std::unique_ptr<char[]> EncodeCondJump(CondJumps cond, const void* at, const void* target);

	static void SetExecutable(char* text);

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
private:
	class CModRM {
	public:
		char Value() const {
			char res = 0;
			for (int i = 0; i < 8; ++i) {
				res |= (_bits[i] << i);
			}
			return res;
		}

		char Mod() const {
			return (char) (_bits >> 6).to_ulong();
		}

		char Reg() const {
			return (char) ((_bits >> 3) & std::bitset<8>(0x7)).to_ulong();
		}

		char RM() const {
			return (char) (_bits & std::bitset<8>(0x7)).to_ulong();
		}

		void Mod(char value) {
			_bits |= std::bitset<8>((value & 0x3) << 6);
		}

		void Reg(char value) {
			_bits |= std::bitset<8>((value & 0x7) << 3);
		}

		void RM(char value) {
			_bits |= std::bitset<8>(value & 0x7);
		}

	private:
		std::bitset<8> _bits;
	};

public:
	enum class Registers;

	/* Encode an operand for an instruction expecting an r/mXX operand.
	 * The returned ByteBuffer provides the values of the R/M byte, the
	 * SIB byte (if applicable) and the displacement.
	 *
	 * The function attempts to optimize the generation by privileging shorter
	 * forms of the encoding when possible. displacement values in range
	 * [-127, 127] prefer the 1 byte displacement encoding while other values
	 * use the 4 bytes displacement encoding.
	 *
	 * An r/mXX operand is, in Intel syntax, of the form [base (+index * scale)? + displacement].
	 * Examples:
	 *	push [eax + 0x12] (base = eax, index = nullopt, scale = n/a, displacement = 0x12)
	 *  push [eax + ebx * 2 + 0x31] (base = eax, index = ebx, scale = 2, displacement = 0x31)
	 *
	 * Legal values for scale are 1, 2, 4 and 8. A value of 0 is ignored, alongside
	 * any provided index. Other values raise an exception.
	 *
	 * If the encoding of the Mod R/M byte results in a use of the SIB byte, and
	 * index is set to ESP, then an exception is raised. While this would not
	 * result in an invalid encoding, it would result in an unexpected (for the
	 * non R/M gurus) operation.
	 */
	static ByteBuffer EncodeRMOperand(Registers base, int32_t displacement,
		std::optional<Registers> index, uint8_t scale, uint8_t reg_op,
		bool allowMod0Disp0 = false);

	ASMPatch();
	ASMPatch(ByteBuffer const& buffer);
	ASMPatch(const ASMPatch& other) = delete;
	ASMPatch& operator=(const ASMPatch& other) = delete;

	class LIBZHL_API SavedRegisters {
	public:
		enum Registers {
			EAX = 1 << 0,
			EBX = 1 << 1,
			ECX = 1 << 2,
			EDX = 1 << 3,
			ESI = 1 << 4,
			EDI = 1 << 5,
			ESP = 1 << 6,
			EBP = 1 << 7,
			GP_REGISTERS_STACKLESS = EAX | EBX | ECX | EDX | ESI | EDI,
			GP_REGISTERS = GP_REGISTERS_STACKLESS | ESP | EBP,
			XMM0 = 1 << 8,
			XMM1 = 1 << 9,
			XMM2 = 1 << 10,
			XMM3 = 1 << 11,
			XMM4 = 1 << 12,
			XMM5 = 1 << 13,
			XMM6 = 1 << 14,
			XMM7 = 1 << 15,
			XMM_REGISTERS = XMM0 | XMM1 | XMM2 | XMM3 | XMM4 | XMM5 | XMM6 | XMM7,
			ALL = GP_REGISTERS | XMM_REGISTERS
		};

		SavedRegisters(uint32_t mask, bool shouldRestore);
		~SavedRegisters();

	private:
		friend ASMPatch;
		friend __declspec(dllexport) int InitZHL(void (__stdcall*ptr)(), void**, char*);

		void Restore();
		uint32_t GetMask() const;

		bool _shouldRestore;
		bool _restored = false;
		uint32_t _mask;

		static void _Init();
		static std::map<uint32_t, ByteBuffer> _RegisterPushMap;
		static std::map<uint32_t, ByteBuffer> _RegisterPopMap;
		static std::array<uint32_t, 16> _RegisterOrder;
	};

	enum class Registers {
		EAX,
		EBX,
		ECX,
		EDX,
		ESI,
		EDI,
		ESP,
		EBP
	};

	enum class XMMRegisters {
		XMM0,
		XMM1,
		XMM2,
		XMM3,
		XMM4,
		XMM5,
		XMM6,
		XMM7
	};

	class LIBZHL_API StackSpace {
	public:
		template<typename T>
		static StackSpace Make(bool shouldFree = true) {
			return StackSpace(sizeof(T), shouldFree);
		}

	private:
		template<typename T>
		friend static StackSpace Make(bool);

		StackSpace(size_t s, bool shouldFree = true) : _size(s) {

		}

	public:
		~StackSpace() {
			if (_shouldFree && !_freed) {
				// throw std::runtime_error("Stack space not freed");
				abort();
			}
		}

		size_t Size() const { return _size;  }

		void Free() {
			if (_freed) {
				throw std::runtime_error("Stack space deallocated multiple times");
			}

			_freed = true;
		}

	private:
		size_t _size;
		bool _shouldFree;
		bool _freed = false;
	};

	enum SIBScale {
		SCALE_NONE,
		SCALE_1,
		SCALE_2,
		SCALE_4,
		SCALE_8
	};

	struct LIBZHL_API RMOperand {
		SIBScale					scale = SCALE_NONE;
		std::optional<Registers>	index = std::nullopt;
		Registers					base  = Registers::EAX;
		int32_t						disp  = 0;

		RMOperand() = delete;
		RMOperand(Registers base, int32_t disp, SIBScale scale = SCALE_NONE,
			std::optional<Registers> index = std::nullopt);
	};

	/* Add arbitrary bytes to the patch. These bytes cannot contain a null character
	 * as it will cause the copy of the bytes into the patch to stop early.
	 */
	ASMPatch& AddBytes(std::string const& str);
	/* Add arbtirary bytes to the patch. These bytes can contain null characters. */
	ASMPatch& AddBytes(ByteBuffer const& str);
	ASMPatch& AddZeroes(uint32_t n = 1);
	ASMPatch& AddRelativeJump(void* target);
	ASMPatch& AddConditionalRelativeJump(ASMPatcher::CondJumps cond, void* target);
	/* Move a value from memory into a register. Memory access is performed through
	 * a register. This allows moves of the form "mov dst, [src + offset]".
	 *
	 * Do not use with a src register of ESP or EBP as they use the SIB byte which is not
	 * supported yet.
	 */
	ASMPatch& MoveFromMemory(Registers src, int32_t offset, Registers dst);
	/* Move a value from a register into memory. Memory access is performed through
	 * a register. This allows moves of the form "mov dst, [src + offset]".
	 *
	 * Do not use with a src register of ESP or EBP as they use the SIB byte which is not
	 * supported yet.
	 */
	ASMPatch& MoveToMemory(Registers src, int32_t offset, Registers dst);

	/* Move an immediate into a register. */
	ASMPatch& MoveImmediate(Registers dst, int32_t immediate);

	/* Copy the value of one register into another.
	 *
	 * This function will only work with the low 32-bits of registers. For
	 * instance, MoveImmediate(XMM0, EAX) will copy the value of EAX in the
	 * low 32 bits of XMM0 (MM0).
	 *
	 * Please remember that moving to an (X)MM register zeroes the bits that
	 * are unaffected.
	 */
	ASMPatch& CopyRegister(std::variant<Registers, XMMRegisters> const& dst,
		std::variant<Registers, XMMRegisters> const& src);

	/* Copy the value of one register into another.
	 *
	 * This function will only with the low 8-bits of registers. As a consequence,
	 * you cannot use to move to/from (X)MM registers. You can only use with
	 * general purpose registers.
	 */
	ASMPatch& CopyRegister8(Registers dst, Registers src);

	/* Allocate space on the stack for the return value of a function that returns
	 * a complex type. */
	ASMPatch& AllocateReturn(StackSpace& space) {
		size_t s = space.Size();
		ByteBuffer bytes;
		// Use sub /5
		char opcode = '\x81';
		unsigned char modrm = 0b11101100; // Mod = 11, Reg = 101 (/5), RM = 100 (ESP)
		ByteBuffer buffer;
		buffer.AddInteger((int32_t)s);
		bytes.AddByte(opcode).AddByte(modrm).AddByteBuffer(buffer);
		return AddBytes(bytes);
	}

	/* Free space on the stack for the return value of a function that returns
	 * a complex type. */
	ASMPatch& FreeReturn(StackSpace& space) {
		size_t s = space.Size();
		ByteBuffer bytes;
		// Use add /5
		char opcode = '\x81';
		unsigned char modrm = 0b11000100; // Mod = 11, Reg = 000 (/0), RM = 100 (ESP)
		ByteBuffer buffer;
		buffer.AddInteger((int32_t)s);
		bytes.AddByte(opcode).AddByte(modrm).AddByteBuffer(buffer);
		space.Free();
		return AddBytes(bytes);
	}

	/* Encode the lea instruction using the index/base + displacement format.
	 * Usual restrictions to the encoding an r/m operand apply.
	 * Additionally, this implementation cannot be used for operations such as
	 * `lea reg, [reg]`. For such operations, prefer the CopyRegister function.
	 *
	 * For the meaning of base, index and scale, please refer to the
	 * EncodeRMOperand function.
	 *
	 * If you wish to encode something like lea r32, [m64], use the overload below.
	 *
	 * Example: to encode lea ebx, [eax + ecx * 2 + 0xA], call :
	 * LoadEffectiveAddress(EAX, 0xA, EBX, ECX, 2);
	 */
	ASMPatch& LoadEffectiveAddress(Registers base, int32_t offset, Registers dst,
		std::optional<Registers> index = std::nullopt, uint8_t scale = 0);
	/* Load an effective address into the given register. This effectively moves
	 * the value inside the register.
	 *
	 * Use this function when you want the very specific encoding of the Mod R/M
	 * such that the effective address is addr.
	 */
	ASMPatch& LoadEffectiveAddress(Registers dst, int32_t addr);
	ASMPatch& PreserveRegisters(SavedRegisters& registers);
	ASMPatch& RestoreRegisters(SavedRegisters& registers);
	ASMPatch& Push(Registers reg);
	ASMPatch& Push(int8_t imm8);
	ASMPatch& Push(int32_t imm32);
	// Push [reg - imm32].
	ASMPatch& Push(Registers ref, int32_t imm32);
	ASMPatch& Push(RMOperand rm);
	ASMPatch& Pop(Registers reg);
	/* Add a call instruction. The jump is relative and uses the 0xE8 opcode for the
	 * call instruction. Registers are not preserved: you must take care of that yourself.
	 *
	 * addr is the address of the function to call.
	 */
	ASMPatch& AddInternalCall(void* addr);
	ASMPatch& AddDLLCall(void* addr);

	size_t Length() const;
	std::unique_ptr<char[]> ToASM(void* at) const;

private:
	class LIBZHL_API ASMNode {
	public:
		virtual std::unique_ptr<char[]> ToASM(void* at) const = 0;
		virtual size_t Length() const = 0;
	};

	class LIBZHL_API ASMBytesAny : public ASMNode {
	public:
		ASMBytesAny(ByteBuffer const& bytes);
		std::unique_ptr<char[]> ToASM(void*) const override;
		size_t Length() const override;

	private:
		ByteBuffer _bytes;
	};

	class LIBZHL_API ASMBytes : public ASMNode {
	public:
		ASMBytes(const char* bytes);
		std::unique_ptr<char[]> ToASM(void*) const override;
		size_t Length() const override;

	private:
		std::unique_ptr<char[]> _bytes;
	};

	class LIBZHL_API ASMZeroes : public ASMNode {
	public:
		ASMZeroes(uint32_t n);
		std::unique_ptr<char[]> ToASM(void* at) const override;
		size_t Length() const override;

	private:
		uint32_t _n;
	};

	class LIBZHL_API ASMJump : public ASMNode {
	public:
		ASMJump(void* target);
		std::unique_ptr<char[]> ToASM(void* at) const override;
		size_t Length() const override;

	private:
		void* _target;
	};

	class LIBZHL_API ASMCondJump : public ASMNode {
	public:
		ASMCondJump(ASMPatcher::CondJumps cond, void* target);
		std::unique_ptr<char[]> ToASM(void* at) const override;
		size_t Length() const override;

	private:
		ASMPatcher::CondJumps _cond;
		void* _target;
	};

	class LIBZHL_API ASMInternalCall : public ASMNode {
	public:
		ASMInternalCall(void* target);
		std::unique_ptr<char[]> ToASM(void* at) const override;
		size_t Length() const override;

	private:
		void* _target;
	};

	friend __declspec(dllexport) int InitZHL(void (__stdcall*ptr)(), void**, char*);
	static void _Init();

	uint8_t RegisterTox86(Registers reg);
	uint8_t RegisterTox86(XMMRegisters reg);

	// friend void* ASMPatcher::PatchAt(void*, const char*);

	std::bitset<8> ModRM(Registers reg, int32_t value);
	std::bitset<8> ModRM(std::variant<Registers, XMMRegisters> const& src,
		std::variant<Registers, XMMRegisters> const& dst, bool isRegDst);
	std::vector<std::unique_ptr<ASMNode>> _nodes;
	static std::map<ASMPatch::Registers, std::bitset<8>> _ModRM;
	static uint32_t RegisterToBackupRegister(Registers reg);
	size_t _size = 0;

	uint8_t SIBScaleToInt(SIBScale scale);
};

/// Box a type, preventing Visual Studio from moving a value of such a type in a register.
/// Only works on fundamental types (char, int, float, pointers).
template<typename T>
struct Box {
	std::enable_if_t<std::disjunction_v<std::is_fundamental<T>, std::is_pointer<T>>, T> _t;

	T& Get() {
		return _t;
	}

	T const& Get() const {
		return _t;
	}

	T& operator*() {
		return _t;
	}

	T const& operator*() const {
		return _t;
	}
};

#define sASMPatcher ASMPatcher::instance()