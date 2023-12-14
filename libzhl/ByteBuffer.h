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
	ByteBuffer& AddString(const char* s);
	ByteBuffer& AddZeroes(uint32_t n);
	ByteBuffer& AddAny(const char* addr, size_t n);
	ByteBuffer& AddByteBuffer(ByteBuffer const& other);

	size_t GetSize() const;
	char* GetData() const;
	std::string ToString() const;

	void Dump(FILE* f);

private:
	void CheckAndResize(size_t s);

	std::unique_ptr<char[]> _data;
	size_t _capacity, _size;
};