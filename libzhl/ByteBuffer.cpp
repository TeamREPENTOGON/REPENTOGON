#include <stdexcept>

#include "ByteBuffer.h"

void EndianSwap(char* src, int a, int b) {
	char copy = src[a];
	src[a] = src[b];
	src[b] = copy;
}



ByteBuffer::ByteBuffer() {
	_capacity = X86_LONGEST_INSTRUCTION_NBYTES;
	_data.reset(new char[_capacity]);
	_size = 0;
}

ByteBuffer::ByteBuffer(ByteBuffer const& other) {
	*this = other;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer const& other) {
	if (this == &other) {
		return *this;
	}

	_size = other._size;
	_capacity = other._capacity;
	_data.reset(new char[_size]);
	memcpy(_data.get(), other._data.get(), _size);

	return *this;
}

ByteBuffer::ByteBuffer(ByteBuffer&& other) {
	*this = std::move(other);
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) {
	if (this == &other) {
		return *this;
	}

	_capacity = other._capacity;
	_size = other._size;
	_data = std::move(other._data);

	other._capacity = other._size = 0;

	return *this;
}

ByteBuffer& ByteBuffer::AddString(const char* s) {
	size_t len = strlen(s);
	CheckAndResize(len);
	memcpy(_data.get() + _size, s, len);
	_size += len;

	return *this;
}

ByteBuffer& ByteBuffer::AddZeroes(uint32_t n) {
	if (n == 0) {
		throw std::runtime_error("Adding 0 zeroes to ByteBuffer");
	}

	CheckAndResize(n);
	memset(_data.get() + _size, 0, n);
	_size += n;

	return *this;
}

ByteBuffer& ByteBuffer::AddAny(const char* addr, size_t n) {
	if (n == 0) {
		throw std::runtime_error("Adding 0 length byte array to ByteBuffer");
	}

	CheckAndResize(n);
	memcpy(_data.get() + _size, addr, n);
	_size += n;

	return *this;
}

ByteBuffer& ByteBuffer::AddByteBuffer(ByteBuffer const& other) {
	CheckAndResize(other._size);
	memcpy(_data.get() + _size, other.GetData(), other._size);
	_size += other._size;

	return *this;
}

void ByteBuffer::CheckAndResize(size_t s) {
	if (_size + s >= _capacity) {
		size_t newCapacity = std::max(_capacity * 2, _capacity + s);
		char* content = new char[newCapacity];
		_capacity = newCapacity;
		memcpy(content, _data.get(), _size);
		_data.reset(content);
	}
}

char* ByteBuffer::GetData() const {
	return _data.get();
}

size_t ByteBuffer::GetSize() const {
	return _size;
}

void ByteBuffer::Dump(FILE* f) {
	for (size_t i = 0; i < _size; ++i) {
		fprintf(f, "%.2hhx", _data.get()[i]);
	}
}
