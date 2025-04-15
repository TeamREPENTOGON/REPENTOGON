#pragma once

#include <vector>
#include <list>
#include <fstream>
#include <variant>

#include "libzhl.h"

namespace SigCache {
	void FlushCache();
	size_t FindCacheEntryBySig(size_t sighash);
	void LoadCache();
	
	void WriteCacheEntry(size_t sighash, size_t address);
	void InvalidateCache(size_t offset);
	void ResetSigFile();

	struct SigCacheEntry {
		size_t _sighash;
		size_t _address;
	};

	extern std::ofstream _writebuffer;
	extern std::vector<SigCacheEntry> _entries;
	extern std::string _sigcachepath;
	extern bool IsLoaded;
	extern bool IsIndirectMode;
};

struct LIBZHL_API SigScanEntry
{
	std::string name;
	std::string signature;
	bool isFunction = false;
	std::vector<void*> locations;
};

class LIBZHL_API SigScan
{
public:
	class Match
	{
	public:
		Match(short b, short n) : address(0), begin(b), length(n) {}
		~Match() {}

		void *address;
		short begin;
		short length;
	};

	typedef void (*Callback)(SigScan *scan);

private:
	static unsigned char *s_pBase;
	static size_t s_iBaseLen;
	static unsigned char *s_pLastStartAddress;
	static unsigned char *s_pLastAddress;
	static std::list<Match> s_lastMatches;
	static size_t s_sigCounter;

private:
	size_t m_iLength;
	unsigned char *m_sig;
	size_t m_sighash;
	unsigned char *m_mask;
	std::list<Match> m_matches;

	bool m_bNoReturnSeek;
	bool m_bStartFromLastAddress;

	std::variant<void*, std::vector<void*>> m_scanResult;
	int m_dist;

public:
	SigScan(const char *sig);
	~SigScan();

	static void FlushCache();
	bool Scan(bool useCache = true, bool allowRepetitions = false,
		Callback callback = NULL);

	void* GetAddress() const;
	std::vector<void*> GetAddresses() const;

	template <class T> T GetAddress() const
	{
		return reinterpret_cast<T>(GetAddress());
	}

	int GetDistance() const {return m_dist;}

	int GetMatchCount() const {return m_matches.size();}
	const Match &GetMatch(int i = 0) const
	{
		auto it = m_matches.cbegin();
		for( ; i && it != m_matches.cend() ; ++it, --i);
		return *it;
	}

public:
	static void *GetBaseAddress() {return (void*)s_pBase;}
	static size_t GetBaseLength() {return s_iBaseLen;}
	static void Init();
};
