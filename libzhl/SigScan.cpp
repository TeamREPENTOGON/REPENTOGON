#include "SigScan.h"
#include <string>
#include <Windows.h>
#include <fstream>

#include "hde.h"

#include "ImageHlp.h"

unsigned char *SigScan::s_pBase = 0;
size_t SigScan::s_iBaseLen = 0;
unsigned char *SigScan::s_pLastStartAddress = 0;
unsigned char *SigScan::s_pLastAddress = 0;
std::list<SigScan::Match> SigScan::s_lastMatches;
size_t SigScan::s_sigCounter = -1;

namespace SigCache {
	bool IsLoaded = false;
	bool IsIndirectMode = false;
	std::vector<SigCacheEntry> _entries = {};
	std::ofstream _writebuffer;
	std::string _sigcachepath="signatures.log";

	void GenFilename() {
		DWORD checksum=0;
		DWORD header_sum = 0;

		TCHAR default_path_buffer[260];
		TCHAR* exe_path=default_path_buffer;
		size_t exe_path_size = sizeof(default_path_buffer);

		DWORD out=GetModuleFileName(nullptr, exe_path, exe_path_size);
		if (out == 0x0) {
			return;
		};

		while (GetLastError() == ERROR_INSUFFICIENT_BUFFER){
			if (exe_path != default_path_buffer) {
				delete[] exe_path;
			};
			exe_path_size *= 2;
			exe_path = new TCHAR[exe_path_size];
			if (exe_path == 0x0) {
				return;			//if we hit this we are doomed either way
			};
			DWORD out=GetModuleFileName(nullptr, exe_path, exe_path_size);
			if (out == 0x0) {
				if (exe_path != default_path_buffer) {
					delete[] exe_path;
				};
				return;
			};
		};

		MapFileAndCheckSum(exe_path,&header_sum,&checksum);
		char path_buffer[100];
		(void)snprintf(path_buffer, sizeof(path_buffer),"sig_%08x.log",checksum);
		_sigcachepath = path_buffer;
		if (exe_path != default_path_buffer) {
			delete[] exe_path;
		};
		return;
	};

	size_t FindCacheEntryBySig(size_t sighash) {
		for (size_t i = 0; i < _entries.size(); i++) {
			if (_entries[i]._sighash == sighash) {
				return _entries[i]._address;
			};
		};
		return 0;
	};

	void FlushCache() {
		if (_writebuffer.is_open()) {
			_writebuffer.flush();
		};
	};

	void ResetSigFile() {
		_writebuffer.close();
		std::ofstream siglist(_sigcachepath, std::ios::out | std::ios::trunc);
		if (siglist.is_open()){
			siglist << "1 1337" << std::endl;	//todo: write exe hash here
			siglist.close();
		};
		_writebuffer.open(_sigcachepath, std::ios::app);
	};

	void InvalidateCache(size_t offset) {
		ResetSigFile();
		for (size_t i = 0; i < offset; i++) {		//salvage everything that was correct
			SigCacheEntry& entry = _entries[i];
			WriteCacheEntry(entry._sighash,entry._address);
		};
		IsIndirectMode = true;
	};

	void LoadCache() {
		_entries.clear();
		_entries.reserve(100);
		GenFilename();
		std::ifstream siglist(_sigcachepath);
		_writebuffer.open(_sigcachepath, std::ios::app);
		std::string line;
		size_t sigsize = 0;
		uint32_t sighash;
		HMODULE addr;
		HMODULE baseaddr = GetModuleHandleA(nullptr);
		size_t version_hash=0;
		size_t exe_hash = 1337;	//placeholder
		size_t file_version=1;
		if (siglist.is_open()) {
			while (getline(siglist, line)) {
				if (!version_hash) {
					(void)sscanf(line.c_str(), "%zu %zu", (&file_version), (&version_hash));
					if (file_version != 1) {
						siglist.close();
						break;	//entries.size will be 0, will run resetsigfile
					}
					else {
						continue;
					}
				}
				(void)sscanf(line.c_str(), "%x %x\n", (uint32_t*)(&sighash), (uint32_t*)(&addr));
				SigCacheEntry entry;
				entry._sighash = sighash;
				entry._address = ((size_t)addr + (size_t)baseaddr);
				_entries.push_back(entry);
			};
			siglist.close();
		};

		if (_entries.size() == 0) {
			printf("[ZHL::SigCache] Signature cache not found or is invalid. Expect longer load time...\n");
			ResetSigFile();
		}

		IsLoaded = true;
	};
	void WriteCacheEntry(size_t sighash, size_t address) {
		std::string line;
		std::string buffer(80, ' ');
		size_t sigsize = 0;
		size_t baseaddr = (uint32_t)GetModuleHandleA(nullptr);	//todo: take m_dist instead, get rid of baseaddr here
		if (_writebuffer.is_open()) {
			snprintf(&(buffer[0]), 80, "%x %x", sighash, (uint32_t)(address - baseaddr));
			buffer.resize(strnlen_s(&(buffer[0]), 80));
			_writebuffer << buffer << std::endl;
		};
	};
};

//=====================================================================

void SigScan::FlushCache() {
	SigCache::FlushCache();
};

SigScan::SigScan(const char *sig) : m_scanResult(nullptr)
{
	m_bStartFromLastAddress = false;
	m_bNoReturnSeek = false;
	m_dist = 0;
	m_sighash = 0x0;	//init
	if (!SigCache::IsLoaded) {
		SigCache::LoadCache();
	}
	// Default signature if nothing was specified
	if(!sig || !sig[0])
	{
		if(s_lastMatches.empty())
		{
			// Look for the first function after the last scanned function
			sig = ".558bec";
		}
		else
		{
			// Get remaining matches from the last scan
			s_lastMatches.pop_front();
			m_matches = s_lastMatches;
			m_iLength = 0;
			m_sig = NULL;
			m_mask = NULL;
			return;
		}
	}

	int len = 0;
	for(const char *p = sig ; *p ; ++p)
	{
		char c = *p;
		len += ((c>='a' && c<='f') || (c>='A' && c<='F') || (c>='0' && c<='9') || c=='?');
	}

	m_iLength = len / 2;
	m_sig = new unsigned char[m_iLength];
	m_mask = new unsigned char[m_iLength];

	unsigned char *ps = m_sig;
	unsigned char *pm = m_mask;

	bool b = false;

	short matchStart = -1;
	int i = 0;

	for(const char *p = sig ; *p ; ++p)
	{
		int c = *p;
		if(c == '.')
		{
			m_bStartFromLastAddress = true;
			continue;
		}

		if(c == '!')
		{
			m_bNoReturnSeek = true;
			continue;
		}

		if(c == '(')
		{
			matchStart = i;
			continue;
		}
		else if(c == ')')
		{
			m_matches.push_back(Match(matchStart, i - matchStart));

			matchStart = -1;
			continue;
		}

		if(c >= '0' && c <= '9') c -= '0';
		else if(c >= 'a' && c <= 'f') c -= ('a' - 10);
		else if(c >= 'A' && c <= 'F') c -= ('A' - 10);
		else c = -1;
		
		++p;

		int d = *p;
		if(!d) break;
		else if(d >= '0' && d <= '9') d -= '0';
		else if(d >= 'a' && d <= 'f') d -= ('a' - 10);
		else if(d >= 'A' && d <= 'F') d -= ('A' - 10);
		else d = -1;

		if(c >= 0 && d >= 0)
		{
			*(ps++) = (c << 4) | d;
			*(pm++) = 1;
		}
		else
		{
			*(ps++) = 0;
			*(pm++) = 0;
		}
		++i;
	}
	std::string_view sview = std::string_view((char*)m_sig, m_iLength);
	m_sighash = std::hash<std::string_view>{} (sview);
}

//=====================================================================

SigScan::~SigScan()
{
	if(m_sig)
		delete[] m_sig;
	if(m_mask)
		delete[] m_mask;
}

void* SigScan::GetAddress() const
{
	if (std::holds_alternative<void*>(m_scanResult))
		return std::get<void*>(m_scanResult);
	else
	{
		std::vector<void*> const& results =
			std::get<std::vector<void*>>(m_scanResult);
		if (results.size() == 0)
			return nullptr;
		else
			return results.front();
	}
}

bool SigScan::Scan(bool useCache, bool allowRepetitions, Callback callback)
{
	const unsigned char *usig = m_sig;
	if (!m_iLength)
	{
		m_scanResult = s_pLastStartAddress;
		return true;
	}

	short itn = 0;

	unsigned char *pStart = s_pBase;
	s_sigCounter++;
	bool readfromcache = useCache && (s_sigCounter != -1 && s_sigCounter < SigCache::_entries.size());
	if (useCache && SigCache::IsIndirectMode)
	{
		readfromcache = false;
		size_t address = SigCache::FindCacheEntryBySig(m_sighash);
		if (address != 0)
			pStart = (unsigned char*)address;
	}

	if (readfromcache)
	{
		if (SigCache::_entries[s_sigCounter]._sighash != m_sighash)
		{
//			SigCache::ResetSigFile();
			printf("[ZHL::SigCache] Invalid signature %x instead of %x! Expect longer load time...\n---\nMismatch sig: ", SigCache::_entries[s_sigCounter]._sighash,m_sighash);
			const unsigned char* sig_print_iter = m_sig;
			
			for(size_t i=0;i<m_iLength;i++) {
				if (*(m_mask + i) == '\0') {
					printf("??");
					continue;
				};
				printf("%02x",*(m_sig+i));
			};
			printf("\n---\n");
			SigCache::InvalidateCache(s_sigCounter);
			readfromcache = false;
		}
		else
			pStart = (unsigned char*)SigCache::_entries[s_sigCounter]._address;
	}

	unsigned char *pEnd = s_pBase + s_iBaseLen - m_iLength;
	bool found = false;

	if(m_bStartFromLastAddress)
		pStart = (unsigned char*)s_pLastAddress;

	for( ; pStart <= pEnd ; ++pStart)
	{
		const unsigned char *p = pStart;
		const unsigned char *s = usig;
		const unsigned char *m = m_mask;
		size_t i;

		for(i=0 ; i<m_iLength && p < pEnd; ++i, ++p, ++s, ++m)
			if(*m && *s != *p) break;

		if(i == m_iLength)
		{
			if(m_bStartFromLastAddress)
				m_dist = pStart-s_pLastAddress;
			else
				m_dist = pStart-s_pBase;

			// Find out where the function ends
			s_pLastStartAddress = pStart;

			unsigned char* address = pStart;
			
			if(m_bNoReturnSeek)
			{
				s_pLastAddress = pStart + m_iLength;
			}
			else
			{
				hde32s s = {0};
				int n = 0;

				do
				{
					n = hde32_disasm(s_pLastAddress, &s);
					s_pLastAddress += n;
				} while(n && s.opcode != 0xc2 && s.opcode != 0xc3);

				if(!n)
				{
					// failed to find ret instruction for whatever reason
					s_pLastAddress = pStart + m_iLength;
				}
			}

			for(auto it = m_matches.begin() ; it != m_matches.end() ; ++it)
				it->address = address + it->begin;

			s_lastMatches = m_matches;
			if(callback) callback(this);
			if (useCache && !readfromcache) {
				SigCache::WriteCacheEntry(m_sighash, (size_t)address);
			};

			if (allowRepetitions)
			{
				if (!found)
				{
					std::vector<void*> matches;
					matches.push_back(address);
					m_scanResult = std::move(matches);
					found = true;
				}
				else
				{
					std::get<std::vector<void*>>(m_scanResult).push_back(address);
				}
			} 
			else
			{
				m_scanResult = pStart;
				return true;
			}
		}
	}

	s_pLastAddress = s_pBase;
	if ((!allowRepetitions || !found) && useCache && !readfromcache) {	//not sure about allowrepetitions
		SigCache::WriteCacheEntry(m_sighash, (size_t)s_pBase);	//handling for cases when address was not found
	};
	return allowRepetitions ? found : false;
}

std::vector<void*> SigScan::GetAddresses() const
{
	if (std::holds_alternative<void*>(m_scanResult))
	{
		std::vector<void*> result;
		result.push_back(std::get<void*>(m_scanResult));
		return result;
	} 
	else
		return std::get<std::vector<void*>>(m_scanResult);
}

//=====================================================================

void SigScan::Init()
{
	HMODULE hModule = GetModuleHandle(NULL);

	s_pBase = (unsigned char*)hModule;
	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER*)s_pBase;
	IMAGE_NT_HEADERS *pe = (IMAGE_NT_HEADERS*)(s_pBase + dos->e_lfanew);

	if(pe->Signature != IMAGE_NT_SIGNATURE)
	{
		s_pBase = 0;
		return;
	}
	
	s_iBaseLen = pe->OptionalHeader.SizeOfImage;
	s_pLastAddress = s_pBase;
}
