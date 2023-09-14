#pragma once

#include <optional>
#include <Zydis/Zydis.h>

#include "libzhl.h"

#include "ByteBuffer.h"
#include "Log.h"

class LIBZHL_API Signature {
public:
	Signature(void* addr, const char* moduleName, ZHL::Logger* logger);

	std::optional<ByteBuffer> Make();
	ByteBuffer const& GetBytes() const;

private:
	enum class SignatureCheckStatus {
		SIG_CHECK_UNIQUE,
		SIG_CHECK_MULTIPLE,
		SIG_CHECK_NONE,
	};

	void ReadRaw(unsigned char** raw, size_t n);
	void WildcardRaw(unsigned char** raw, size_t n);
	bool Signature::AddInstructionHeaderToSignature(
		ZydisDisassembledInstruction const& instruction,
		unsigned char** raw);
	SignatureCheckStatus CheckSignature(const unsigned char* start,
		const unsigned char* end, std::list<const unsigned char*>& matches,	size_t oldSize);
	void AddInstructionOperandsToSignature(ZydisDisassembledInstruction const& instruction,
		unsigned char** raw);

	ByteBuffer _bytes;
	void* _moduleTextAddr;
	size_t _moduleTextSize;
	size_t _moduleSize;
	void* _moduleAddr;
	ZHL::Logger* _logger;
	void* _addr;
};