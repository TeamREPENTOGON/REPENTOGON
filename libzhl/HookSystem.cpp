#include "HookSystem_private.h"
#include "mologie_detours.h"
#include "SigScan.h"
#include <unordered_map>
#include <map>
#include <string>
#include <sstream>

#include "Zydis/Encoder.h"

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

// ArgData

bool HookSystem::ArgData::IsRegister() const {
	return _size == 0;
}

bool HookSystem::ArgData::IsGPRegister() const {
	return IsRegister() && std::holds_alternative<GPRegisters>(_register);
}

bool HookSystem::ArgData::IsXMMRegister() const {
	return IsRegister() && std::holds_alternative<XMMRegisters>(_register);
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

FunctionDefinition::FunctionDefinition(const char *name, const type_info &type, const char *sig, const HookSystem::ArgData *argdata, int nArgs, unsigned int flags, void **outfunc) :
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

void FunctionHook_private::Push(HookSystem::GPRegisters reg, unsigned char* text) {
	*text = 0x50 + reg;
	++text;
}

void FunctionHook_private::Pop(HookSystem::GPRegisters reg, unsigned char* text) {
	*text = 0x58 + reg;
	++text;
}

void FunctionHook_private::Mov(HookSystem::GPRegisters dst, HookSystem::GPRegisters src, unsigned char* text) {
	*text = 0x89;
	++text;
	*text = 0xC0 | src << 3 | dst;
	++text;
}

void FunctionHook_private::Ret(unsigned short size, unsigned char* ptr) {
	if (size == 0) {
		*ptr = 0xC3;
		ptr++;
	}
	else {
		*ptr = 0xC2;
		ptr++;
		*(unsigned short*)ptr = size;
		ptr += 2;
	}
}

void FunctionHook_private::EmitPrologue(FunctionDefinition const* def, unsigned char* ptr) {
	using namespace HookSystem;
	Push(EBP, ptr);
	Mov(EBP, ESP, ptr);

	/// Preserve registers 

	// If the function doesn't return a value, eax must be preserved
	if (def->IsVoid()) {
		Push(EAX, ptr);
	}

	// If the function needs less than 32 bits to return a value, or if the function doesn't return anything preserve edx
	if (!def->IsLongLong() || def->IsVoid()) {
		Push(EDX, ptr);
	}

	// Preserve ebx, ecx, edi, esi in all cases
	Push(EBX, ptr);
	Push(ECX, ptr);
	Push(EDI, ptr);
	Push(ESI, ptr);
}

void FunctionHook_private::EmitEpilogue(FunctionDefinition const* def, unsigned char stackPos, unsigned char* ptr) {
	using namespace HookSystem;

	Pop(ESI, ptr);
	Pop(EDI, ptr);
	Pop(ECX, ptr);
	Pop(EBX, ptr);

	if (!def->IsLongLong() || def->IsVoid()) {
		Pop(EDX, ptr);
	}

	if (def->IsVoid()) {
		Pop(EAX, ptr);
	}

	Mov(ESP, EBP, ptr);
	Pop(EBP, ptr);

	// If the function is **callee** cleanup, clean the stack
	if (stackPos > 0 && !def->NeedsCallerCleanup()) {
		Ret(stackPos, ptr);
	}
	else {
		Ret(0, ptr);
	}
}

int FunctionHook_private::Install()
{
	using namespace HookSystem;

	FunctionDefinition* def = dynamic_cast<FunctionDefinition*>(Definition::Find(_name));
	if (!def)
	{
		sprintf_s(g_hookLastError, "Failed to install hook for %s: function not found", _name);
		return 0;
	}

	const HookSystem::ArgData* args = def->GetArgData();
	int argc = def->GetArgCount();

	/// Create the hook caller.
	/// _internalHook will contain the code that will call the hook and place the
	/// arguments on the stack, minus the this pointer, if any (in ecx).
	/// Detours will be used to redirect the original function to the hook caller.

	unsigned char* ptr = _internalHook;
	EmitPrologue(def, ptr);

	/// Call the hook

	// Compute the size of the stack. Stack arguments must be pushed in reverse order
	// so we need to find the "bottom" of the call stack for the active frame.
	uint8_t stackPos = 8;  // Start at 8 because we need to ignore the ebp that was saved in the prologue
							// and we need to ignore the return address below.
	for (int i = 0; i < argc; ++i) {
		if (!args[i].IsRegister()) {
			uint8_t orig = stackPos;
			stackPos += 4 * args[i]._size;

			if (stackPos <= orig) {
				std::ostringstream str;
				str << "[FATAL] Function " << _name << " takes more than 255 bytes on the stack" << std::endl;
				throw std::runtime_error(str.str());
			}
		}
	}

	// Position of the next stack argument
	uint8_t k = stackPos;
	
	// Number of bytes pushed on the stack to call the hook
	// This may be different than the amount pushed on the stack to call 
	// the original function as function parameters are also pushed on 
	// the stack when calling the hook.
	uint32_t pushed = 0;

	// Arguments in registers are moved on the stack, arguments on the stack are pushed on the stack
	// Iterate in reverse order as arguments need to be pushed in reverse order.
	// Farthest argument of the call to the original function must be pushed first.
	for (int i = argc - 1; i >= 0; --i) {
		if (i == 0 && def->IsThiscall()) {
			continue; // Just assume "this" is already in the appropriate register
		}

		HookSystem::ArgData const& data = args[i];
		// Arg passed in register, push it onto the stack
		if (data.IsRegister()) {
			if (data.IsGPRegister()) {
				HookSystem::GPRegisters reg = std::get<HookSystem::GPRegisters>(data._register);
				Push(reg, ptr);
			}
			else {
				HookSystem::XMMRegisters reg = std::get<HookSystem::XMMRegisters>(data._register);
				// With GP registers push decrements the stack pointer and writes the value at
				// the new position of the stack pointer. With XMM registers, decrement the stack
				// pointer and movd from the XMM register into the position of the stack pointer

				DecrESP(4, ptr);
				// movd [esp], xmmX
				P(0x66); P(0x0F); P(0x7E); P(reg << 3 | GPRegisters::ESP); P(0x24); 
			}

			pushed += 4;
		}
		else {
			for (int j = 0; j < data._size; ++j) {
				// push dword ptr ss:[ebp+8+k]
				k -= 4;
				P(0xFF); P(0x75); P(k);
			}

			pushed += 4 * data._size;
		}
	}

	// Call the hook
	// Offset = target - offset of next instruction
	// offset of next instruction = (ptr + 5)
	// (5 is the length of a relative jump instruction)
	int offset = (unsigned int)_hook - (unsigned int)ptr - 5;
	P(0xE8); PL(offset);

	// If the function needs caller cleanup, we clean the stack
	if (def->NeedsCallerCleanup() && pushed > 0) {
		IncrESP(pushed, ptr); // add esp, pushed
	}

	/// Clean after the hook has been called
	EmitEpilogue(def, stackPos - 8, ptr);
	
	DWORD oldProtect = 0;
	_hSize = ptr - _internalHook;
	VirtualProtect(_internalHook, _hSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	/// Detour the original function

	try {
		_detour = new MologieDetours::Detour<void*>(def->GetAddress(), _internalHook);
	}
	catch (MologieDetours::DetourException& e) {
		sprintf_s(g_hookLastError, "Failed to install hook for %s: %s", _name, e.what());
		return 0;
	}

	/// Call the original function

	void* originalFunction = _detour->GetOriginalFunction();

	ptr = _internalSuper;

	EmitPrologue(def, ptr);

	// Start at 8 to ignore the prologue and return address
	stackPos = 8;

	// Compute the amount of stack space used for super.
	// All parameters are assumed to be on the stack.
	for (int i = 0; i < argc; ++i) {
		if (args[i].IsRegister()) {
			// Skip "this", already in ecx, not on the stack
			if (i == 0 && def->IsThiscall()) {
				continue;
			}

			stackPos += 4;
		}
		else {
			stackPos += 4 * args[i]._size;
		}
	}

	/// Place function parameters in their appropriate location

	k = stackPos;
	pushed = 0;
	// Iterate in reverse order as arguments need to be pushed on the stack in reverse order
	for (int i = argc - 1; i >= 0; --i) {
		HookSystem::ArgData const& data = args[i];

		// Thiscall function, "this" parameter -> do nothing, this is already in ecx
		if (i == 0 && def->IsThiscall()) {
			continue;
		}

		// All arguments of super are on the stack, retrieve them and put them in the appropriate place
		if (data.IsRegister()) {
			k -= 4;
			if (data.IsGPRegister()) {
				// mov reg, [ebp + k]
				P(0x8B); /* Mod R/M = b(01 reg 101) */ P(0x45 | std::get<GPRegisters>(data._register) << 3); P(k);
			}
			else {
				// movd reg, [ebp + k]
				P(0x66); P(0x0F); P(0x6E); /* Mod R/M = b(01 reg 101) */ P(0x45 | std::get<XMMRegisters>(data._register) << 3); P(k);
			}
		}
		else {
			for (int j = 0; j < data._size; ++j) {
				// push [ebp + k]
				k -= 4;
				P(0xFF); P(0x75); P(k); 
				pushed += 4;
			}
		}
	}

	/// Call original function

	int diff = (unsigned int)originalFunction - (unsigned int)ptr - 5;
	// call originalFunction
	P(0xE8); PL(diff);

	if (def->NeedsCallerCleanup() && stackPos > 8) {
		IncrESP(stackPos - 8, ptr);
	}

	EmitEpilogue(def, pushed, ptr);

	_sSize = ptr - _internalSuper;
	VirtualProtect(_internalSuper, _sSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	*_outInternalSuper = _internalSuper;

	Log("Successfully hooked function %s\n", _name);
}

/* int FunctionHook_private::Install()
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
	if (stackPos > 8 && def->NeedsCallerCleanup()) {
		P(0x81);
		P(0xC4);
		PL(stackPos - 8);
	}

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
	if(stackPos > 8 && !def->NeedsCallerCleanup())
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
} */

//================================================================================
// FunctionHook

void FunctionHook::Add(const char *name, const type_info &type, void *hook, void **outInternalSuper, int priority)
{
	new FunctionHook_private(name, type, hook, outInternalSuper, priority);
}

void FunctionHookCustom::Add(const char* name, const type_info& type, void* hook, void** outInternalSuper, int priority)
{
	new FunctionHookCustom_private(name, type, hook, outInternalSuper, priority);
}

//================================================================================
// FunctionHook

int FunctionHookCustom_private::Install() {
	FunctionDefinition* def = dynamic_cast<FunctionDefinition*>(Definition::Find(_name)); // function definition
	if (!def)
	{
		sprintf_s(g_hookLastError, "Failed to install hook for %s: Function not found", _name);
		return 0;
	}

	struct ArgData
	{
		char r;
		char s;
	};

	const ArgData* argd = (const ArgData*)def->GetArgData();
	int argc = def->GetArgCount();
	unsigned char* ptr;
	int stackPos;
	int k;
	DWORD oldProtect;

	//==================================================
	// Internal hook
	ptr = _internalHook;

	// Prologue
	P(0x55);					// push ebp
	P(0x89); P(0xe5);			// mov ebp, esp

	// In original this only computes how many bytes are pushed on the stack.
	// As the hook needs to be called with a cdecl style convention, all arguments
	// are pushed on the stack regardless in this case.
	stackPos = 8;
	for (int i = 0; i < argc; ++i) {
		stackPos += 4 * argd[i].s;
	}

	// Push general purpose registers
	if (def->IsVoid() || !def->IsLongLong())
		P(0x52);	// push edx
	if (def->IsVoid())
		P(0x50);	// push eax
	P(0x51);	// push ecx
	P(0x53);	// push ebx
	P(0x56);	// push esi
	P(0x57);	// push edi

	// Copy arguments to their appropriate location
	k = stackPos;
	for (int i = argc - 1; i >= 0; --i) {
		// Removed the case where the function is thiscall.
		// The calling convention bug cannot occur on thiscall functions.
		if (argd[i].r >= 0)
		{
			P(0x50 + argd[i].r);							// push XXX
		}
		else
		{
			for (int j = 0; j < argd[i].s; ++j)
			{
				k -= 4;
				P(0xff); P(0x75); P(k);						// push [ebp+8+4*X]
			}
		}
	}

	// Call the hook
	P(0xE8); PL((unsigned int)_hook - (unsigned int)ptr - 4);	// call _hook
	if (stackPos > 8 && def->NeedsCallerCleanup()) {
		P(0x81);
		P(0xC4);
		PL(stackPos - 8);
	}

	// Restore all saved registers
	P(0x5f);	// pop edi
	P(0x5e);	// pop esi
	P(0x5b);	// pop ebx
	P(0x59);	// pop ecx
	if (def->IsVoid())
		P(0x58);	// pop eax
	if (def->IsVoid() || !def->IsLongLong())
		P(0x5a);	// pop edx

	// Epilogue
	P(0x89); P(0xec);				// mov esp, ebp
	P(0x5d);						// pop ebp
	if (stackPos > 8 && !def->NeedsCallerCleanup())
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
	catch (MologieDetours::DetourException& e)
	{
		sprintf_s(g_hookLastError, "Failed to install hook for %s: %s", _name, e.what());
		return 0;
	}
	void* original = ((MologieDetours::Detour<void*>*)_detour)->GetOriginalFunction();

	//==================================================
	// Internal super
	ptr = _internalSuper;

	// Prologue
	P(0x55);					// push ebp
	P(0x89); P(0xe5);			// mov ebp, esp

	// Push general purpose registers
	if (def->IsVoid() || !def->IsLongLong())
		P(0x52);	// push edx
	if (def->IsVoid())
		P(0x50);	// push eax
	P(0x51);	// push ecx
	P(0x53);	// push ebx
	P(0x56);	// push esi
	P(0x57);	// push edi

	// In the original version, this would perform a check for thiscall
	// In this version it is not necessary because thiscall functions don't 
	// suffer from the same problem.
	stackPos = 8;
	for (int i = 0; i < argc; ++i)
		stackPos += 4 * argd[i].s;

	// Stack arguments first
	int sizePushed = 0;
	k = stackPos;
	for (int i = argc - 1; i >= 0; --i)
	{
		if (argd[i].r < 0)
		{
			for (int j = 0; j < argd[i].s; ++j)
			{
				k -= 4;
				P(0xff); P(0x75); P(k);				// push [ebp+8+4*X]
				sizePushed += 4;
			}
		}
		else
			k -= 4 * argd[i].s;
	}

	// Now register based arguments
	k = 8;
	for (int i = 0; i < argc; ++i)
	{
		if (argd[i].r >= 0)
		{
			P(0x8b); P(0x45 | ((argd[i].r & 7) << 3));
			P(k);							// mov XXX, [ebp+8+4*X]
		}
		k += 4 * argd[i].s;
	}

	// Call the original function
	P(0xE8); PL((unsigned int)original - (unsigned int)ptr - 4);	// call original

	// If the function requires caller cleanup, increment the stack pointer here
	if (def->NeedsCallerCleanup())
	{
		if (sizePushed < 128) { P(0x83); P(0xc4); P(sizePushed); }	// add esp, N8
		else { P(0x81); P(0xc4); PL(sizePushed); }	// add esp, N32
	}

	// Pop general purpose registers
	P(0x5f);	// pop edi
	P(0x5e);	// pop esi
	P(0x5b);	// pop ebx
	P(0x59);	// pop ecx
	if (def->IsVoid())
		P(0x58);	// pop eax
	if (def->IsVoid() || !def->IsLongLong())
		P(0x5a);	// pop edx

	// Epilogue
	P(0x89); P(0xec);				// mov esp, ebp
	P(0x5d);						// pop ebp
	if (stackPos > 8 && !def->NeedsCallerCleanup())
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
	if (!g_hookLog) g_hookLog = fopen("hooks.log", "w");

	Log("%s\ninternalHook:\n", _name);
	for (unsigned int i = 0; i < _hSize; ++i)
		Log("%02x ", _internalHook[i]);

	Log("\ninternalSuper:\n", _name);

	for (unsigned int i = 0; i < _sSize; ++i)
		Log("%02x ", _internalSuper[i]);

	Log("\n\n");
#endif

	return 1;
}