#pragma once

#include <vector>
#include <list>

class SigScan
{
public:
	class Match
	{
	public:
		Match(short b, short n) : address(0), begin(b), length(n) {}
		~Match() {}

		unsigned char *address;
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

private:
	size_t m_iLength;
	unsigned char *m_sig;
	unsigned char *m_mask;
	std::list<Match> m_matches;

	bool m_bNoReturnSeek;
	bool m_bStartFromLastAddress;

	unsigned char *m_pAddress;
	int m_dist;

public:
	SigScan(const char *sig);
	~SigScan();

	bool Scan(Callback callback = NULL);

	void *GetAddress() const {return m_pAddress;}
	template <class T> T GetAddress() const
	{
		return reinterpret_cast<T>(m_pAddress);
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
