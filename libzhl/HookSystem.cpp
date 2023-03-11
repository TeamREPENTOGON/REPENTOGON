#include "HookSystem_private.h"
#include "mologie_detours.h"
#include "SigScan.h"
#include <unordered_map>
#include <map>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void ZHL::Init()
{
	static bool initialized = false;
	if(initialized) return;

	if(!FunctionHook_private::Init())
	{
		MessageBox(0, FunctionHook_private::GetLastError(), "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	initialized = true;
}

//#define HOOK_LOG

static void Log(const char *format, ...)
{
	static FILE *hookLog = 0;
	if(!hookLog) fopen_s(&hookLog, "zhl.log", "w");

	va_list va;
	va_start(va, format);
	vfprintf(hookLog, format, va);
	fflush(hookLog);
	va_end(va);
}

template <size_t Size> static const char *ConvertToUniqueName(char (&dst)[Size], const char *name, const char *type)
{
	char tmp[128];
	strcpy_s(tmp, type);
	
	const char *p = tmp;
	if(p[0] == '.')
	{
		++p;
		if(p[0] == 'P' && p[1] == '8')
		{
			p += 2;
			while(p[0] && (p[0] != '@' || p[1] != '@')) ++p;
		}
	}

	sprintf_s(dst, "%s%s", name, p);
	return dst;
}

//================================================================================
// Definition

static std::vector<Definition*> &Defs()
{
	static std::vector<Definition*> defs;
	return defs;
}

static std::unordered_map<std::string, Definition*> &DefsByName()
{
	static std::unordered_map<std::string, Definition*> defsByName;
	return defsByName;
}

static char g_defLastError[1024] = {0};

const char *Definition::GetLastError() {return g_defLastError;}

void FunctionDefinition::SetName(const char *name, const char *type)
{
	ConvertToUniqueName(_name, name, type);
	strcpy_s(_shortName, name);
}

int Definition::Init()
{
	SigScan::Init();

	for(auto it = Defs().begin() ; it != Defs().end() ; ++it)
	{
		if(!(*it)->Load()) return 0;
	}
	return 1;
}

Definition *Definition::Find(const char *name)
{
	auto it = DefsByName().find(name);
	if(it != DefsByName().end())
		return (*it).second;
	else
		return NULL;
}

void Definition::Add(const char *name, Definition *def)
{
	Defs().push_back(def);
	DefsByName().insert(std::pair<std::string, Definition*>(name, def));
}

//================================================================================
// VariableDefinition

VariableDefinition::VariableDefinition(const char *name, const char *sig, void *outvar) :
	_name(name),
	_sig(sig),
	_outVar(outvar)
{
	Add(_name, this);
}

int VariableDefinition::Load()
{
	SigScan sig(_sig);
	if(!sig.Scan())
	{
		sprintf_s(g_defLastError, "Failed to find value for variable %s, address could not be found", _name);
		return 0;
	}

	if(sig.GetMatchCount() == 0)
	{
		sprintf_s(g_defLastError, "Failed to find address for variable %s, no capture in input signature", _name);
		return 0;
	}

	const SigScan::Match &m = sig.GetMatch();
	memcpy(_outVar, m.address, m.length);

	Log("Found value for %s:", _name);
	for(int i=0 ; i<m.length ; ++i) Log(" %02x", ((unsigned char*)_outVar)[i]);
	Log("\n");

	return 1;
}

//================================================================================
// FunctionDefinition

FunctionDefinition::FunctionDefinition(const char *name, const type_info &type, const char *sig, const short *argdata, int nArgs, unsigned int flags, void **outfunc) :
	_sig(sig),
	_argdata(argdata),
	_nArgs(nArgs),
	_flags(flags),
	_outFunc(outfunc)
{
	SetName(name, type.raw_name());
	Add(_name, this);
}

int FunctionDefinition::Load()
{
	SigScan sig(_sig);
	if(!sig.Scan())
	{
		sprintf_s(g_defLastError, "Failed to find address for function %s", _name);
		return 0;
	}

	_address = sig.GetAddress<void*>();
	*_outFunc = _address;
	Log("Found address for %s: %08x, dist %d\n", _name, (unsigned int)_address, sig.GetDistance());

	return 1;
}

//================================================================================
// FunctionHook

#ifdef HOOK_LOG
static FILE *g_hookLog = NULL;
#endif

static std::multimap<int, FunctionHook_private*, std::greater<int>> &FuncHooks()
{
	static std::multimap<int, FunctionHook_private*, std::greater<int>> funcHooks;
	return funcHooks;
}

static char g_hookLastError[1024] = {0};

const char *FunctionHook_private::GetLastError() {return g_hookLastError;}

void FunctionHook_private::SetName(const char *name, const char *type)
{
	ConvertToUniqueName(_name, name, type);
	strcpy_s(_shortName, name);
}

int FunctionHook_private::Init()
{
	for(auto it = FuncHooks().begin() ; it != FuncHooks().end() ; ++it)
	{
		if(!it->second->Install()) return 0;
	}
	return 1;
}

void FunctionHook_private::Add(FunctionHook_private *hook)
{
	FuncHooks().insert(std::pair<int, FunctionHook_private*>(hook->_priority, hook));
}

#define P(x) *(ptr++) = (x)
#define PS(x) *(((unsigned short*&)ptr)++) = (x)
#define PL(x) *(((unsigned int*&)ptr)++) = (x)

FunctionHook_private::~FunctionHook_private()
{
	if(_detour)
		delete (MologieDetours::Detour<void*>*)_detour;
}

int FunctionHook_private::Install()
{
	FunctionDefinition *def = dynamic_cast<FunctionDefinition*>(Definition::Find(_name)); // function definition
	if(!def)
	{
		sprintf_s(g_hookLastError, "Failed to install hook for %s: Function not found", _name);
		return 0;
	}

	struct ArgData
	{
		char r;
		char s;
	};

	const ArgData *argd = (const ArgData*)def->GetArgData();
	int argc = def->GetArgCount();
	unsigned char *ptr;
	int stackPos;
	int k;
	DWORD oldProtect;
	
	//==================================================
	// Internal hook
	// Converts userpurge to thiscall to call the user
	// defined hook
	ptr = _internalHook;

	// Prologue
	P(0x55);					// push ebp
	P(0x89); P(0xe5);			// mov ebp, esp

	// Compute stack size
	stackPos = 8;
	for(int i=0 ; i<argc ; ++i)
	{
		if(argd[i].r < 0)
			stackPos += 4 * argd[i].s;
	}

	// Push general purpose registers
	if(def->IsVoid() || !def->IsLongLong())
		P(0x52);	// push edx
	if(def->IsVoid())
		P(0x50);	// push eax
	P(0x51);	// push ecx
	P(0x53);	// push ebx
	P(0x56);	// push esi
	P(0x57);	// push edi

	// Copy arguments to their appropriate location
	k = stackPos;
	for(int i=argc-1 ; i>=0 ; --i)
	{
		if(def->IsThiscall() && i == 0)
		{
			// special case: this (shouldn't be pushed to the stack but copied to ecx)
			if(argd[i].r >= 0)
			{
				if(argd[i].r != 1) // no need to do mov ecx, ecx
				{
					P(0x89); P(0xc1 | ((argd[i].r & 7) << 3));	// mov ecx, XXX
				}
			}
			else
			{
				k -= 4;
				P(0x8b); P(0x4d); P(k);							// mov ecx, [ebp+8+4*X]
			}
		}
		else
		{
			if(argd[i].r >= 0)
			{
				P(0x50 + argd[i].r);							// push XXX
			}
			else
			{
				for(int j=0 ; j<argd[i].s ; ++j)
				{
					k -= 4;
					P(0xff); P(0x75); P(k);						// push [ebp+8+4*X]
				}
			}
		}
	}

	// Call the hook
	P(0xE8); PL((unsigned int)_hook - (unsigned int)ptr - 4);	// call _hook

	// Restore all saved registers
	P(0x5f);	// pop edi
	P(0x5e);	// pop esi
	P(0x5b);	// pop ebx
	P(0x59);	// pop ecx
	if(def->IsVoid())
		P(0x58);	// pop eax
	if(def->IsVoid() || !def->IsLongLong())
		P(0x5a);	// pop edx

	// Epilogue
	P(0x89); P(0xec);				// mov esp, ebp
	P(0x5d);						// pop ebp
	if(stackPos > 8 && !def->NeedsCallerCleanup())
	{
		P(0xc2); PS(stackPos - 8);	// ret 4*N
	}
	else
		P(0xc3);					// ret

	_hSize = ptr - _internalHook;
	VirtualProtect(_internalHook, _hSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	// Install the hook with MologieDetours
	try
	{
		_detour = new MologieDetours::Detour<void*>(def->GetAddress(), _internalHook);
	}
	catch(MologieDetours::DetourException &e)
	{
		sprintf_s(g_hookLastError, "Failed to install hook for %s: %s", _name, e.what());
		return 0;
	}
	void *original = ((MologieDetours::Detour<void*>*)_detour)->GetOriginalFunction();

	//==================================================
	// Internal super
	// Converts thiscall to userpurge to call the
	// original function from the user defined hook
	ptr = _internalSuper;

	// Prologue
	P(0x55);					// push ebp
	P(0x89); P(0xe5);			// mov ebp, esp

	// Push general purpose registers
	if(def->IsVoid() || !def->IsLongLong())
		P(0x52);	// push edx
	if(def->IsVoid())
		P(0x50);	// push eax
	P(0x51);	// push ecx
	P(0x53);	// push ebx
	P(0x56);	// push esi
	P(0x57);	// push edi

	// Copy arguments to their appropriate location
	stackPos = 8;
	for(int i=0 ; i<argc ; ++i)
	{
		if(!def->IsThiscall() || i != 0)
			stackPos += 4 * argd[i].s;
	}

	// Stack arguments first
	int sizePushed = 0;
	k = stackPos;
	for(int i=argc-1 ; i>=0 ; --i)
	{
		if(def->IsThiscall() && i == 0)
		{
			if(argd[i].r < 0)
			{
				// special case: this (shouldn't be taken from the stack but copied from ecx)
				P(0x51);								// push ecx
				sizePushed += 4;
			}
		}
		else
		{
			if(argd[i].r < 0)
			{
				for(int j=0 ; j<argd[i].s ; ++j)
				{
					k -= 4;
					P(0xff); P(0x75); P(k);				// push [ebp+8+4*X]
					sizePushed += 4;
				}
			}
			else
				k -= 4 * argd[i].s;
		}
	}

	// Now register based arguments
	k = 8;
	for(int i=0 ; i<argc ; ++i)
	{
		if(def->IsThiscall() && i == 0)
		{
			// special case: this (shouldn't be taken from the stack but copied from ecx)
			if(argd[i].r >= 0 && argd[i].r != 1) // no need to do mov ecx, ecx
			{
				P(0x89); P(0xc8 | (argd[i].r & 7));		// mov XXX, ecx
			}
		}
		else
		{
			if(argd[i].r >= 0)
			{
				P(0x8b); P(0x45 | ((argd[i].r & 7)<<3));
				P(k);							// mov XXX, [ebp+8+4*X]
			}
			k += 4 * argd[i].s;
		}
	}
	
	// Call the original function
	P(0xE8); PL((unsigned int)original - (unsigned int)ptr - 4);	// call original
	
	// If the function requires caller cleanup, increment the stack pointer here
	if(def->NeedsCallerCleanup())
	{
		if(sizePushed < 128)	{ P(0x83); P(0xc4); P(sizePushed); }	// add esp, N8
		else					{ P(0x81); P(0xc4); PL(sizePushed); }	// add esp, N32
	}

	// Pop general purpose registers
	P(0x5f);	// pop edi
	P(0x5e);	// pop esi
	P(0x5b);	// pop ebx
	P(0x59);	// pop ecx
	if(def->IsVoid())
		P(0x58);	// pop eax
	if(def->IsVoid() || !def->IsLongLong())
		P(0x5a);	// pop edx

	// Epilogue
	P(0x89); P(0xec);				// mov esp, ebp
	P(0x5d);						// pop ebp
	if(stackPos > 8)
	{
		P(0xc2); PS(stackPos - 8);	// ret 4*N
	}
	else
		P(0xc3);					// ret
	
	_sSize = ptr - _internalSuper;
	VirtualProtect(_internalSuper, _sSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	// Set the external reference to internalSuper so it can be used inside the user defined hook
	*_outInternalSuper = _internalSuper;
	
	Log("Successfully hooked function %s\n", _name);
	
#ifdef HOOK_LOG
	if(!g_hookLog) g_hookLog = fopen("hooks.log", "w");

	Log("%s\ninternalHook:\n", _name);
	for(unsigned int i=0 ; i<_hSize ; ++i)
		Log("%02x ", _internalHook[i]);

	Log("\ninternalSuper:\n", _name);
	
	for(unsigned int i=0 ; i<_sSize ; ++i)
		Log("%02x ", _internalSuper[i]);
	
	Log("\n\n");
#endif

	return 1;
}

//================================================================================
// FunctionHook

void FunctionHook::Add(const char *name, const type_info &type, void *hook, void **outInternalSuper, int priority)
{
	new FunctionHook_private(name, type, hook, outInternalSuper, priority);
}
