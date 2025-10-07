#pragma once

#include <memory>
#include <string>

#include "libzhl.h"

#define X86_LONGEST_INSTRUCTION_NBYTES 15

void LIBZHL_API EndianSwap(char* src, int a, int b);

class LIBZHL_API ByteBuffer {
public:
	ByteBuffer();

	// Deep copy. Costly but allows multiple independant copies.
	ByteBuffer(ByteBuffer const& other);
	ByteBuffer& operator=(const ByteBuffer& other);

	// Transfer ownership. Faster, but the source object is reset
	ByteBuffer(ByteBuffer&& other);
	ByteBuffer& operator=(ByteBuffer&& other);

	ByteBuffer& AddByte(unsigned char byte, size_t n = 1);
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, ByteBuffer&> AddInteger(T t, bool endianConvert = true) {
		T result = t;
		if (endianConvert) {
			if constexpr (sizeof(T) == 1)
				return AddAny(t, 1);
			else if constexpr (sizeof(T) == 2) {
				result = _byteswap_ushort(t);
			} else if constexpr (sizeof(T) == 4) {
				result = _byteswap_ulong(t);
			} else if constexpr (sizeof(T) == 8) {
				result = _byteswap_uint64(t);
			} else {
				static_assert(false, "AddInteger with endian conversion does not support integers of more than 64 bits");
			}
		}

		return AddAny(&result, sizeof(T));
	}
	ByteBuffer& AddString(const char* s);
	ByteBuffer& AddZeroes(uint32_t n);
	ByteBuffer& AddAny(void* addr, size_t n);
	ByteBuffer& AddByteBuffer(ByteBuffer const& other);
	ByteBuffer& AddPointer(void* ptr);

	size_t GetSize() const;
	char* GetData() const;
	std::string ToString() const;

	void Dump(FILE* f);

private:
	void CheckAndResize(size_t s);

	std::unique_ptr<char[]> _data;
	size_t _capacity, _size;
};