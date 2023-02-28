#pragma once

#include "HookSystem.h"

class LIBZHL_API Definition
{
public:
	static int Init();
	static const char *GetLastError();
	static Definition *Find(const char *name);

protected:
	static void Add(const char *name, Definition *def);

public:
	virtual int Load() = 0;
};

//=================================================================================================

class LIBZHL_API FunctionDefinition : public Definition
{
private:
	char _shortName[128];
	char _name[256];

	const char *_sig;
	const short *_argdata;
	int _nArgs;
	void **_outFunc;
	void *_address;

	unsigned int _flags;

private:
	void SetName(const char *name, const char *type);

public:
	FunctionDefinition(const char *name, const type_info &type, const char *sig, const short *argdata, int nArgs, unsigned int flags, void **outfunc);

	virtual int Load();

	bool IsThiscall() const {return (_flags & 1) != 0;}
	bool NeedsCallerCleanup() const {return (_flags & 2) != 0;}
	bool IsVoid() const {return (_flags & 4) != 0;}
	bool IsLongLong() const {return (_flags & 8) != 0;}

	const short *GetArgData() const {return _argdata;}
	int GetArgCount() const {return _nArgs;}
	void *GetAddress() const {return _address;}
};

//=================================================================================================

class LIBZHL_API VariableDefinition : public Definition
{
private:
	void *_outVar;
	const char *_name;
	const char *_sig;

public:
	VariableDefinition(const char *name, const char *sig, void *outvar);

	virtual int Load();
};

//=================================================================================================

#define FUNCTIONHOOK_MAXSIZE 128

class FunctionHook_private
{
private:
	char _shortName[128];
	char _name[256];

	void *_hook;
	void **_outInternalSuper;

	unsigned char _internalHook[FUNCTIONHOOK_MAXSIZE];
	unsigned char _internalSuper[FUNCTIONHOOK_MAXSIZE];
	unsigned int _hSize;
	unsigned int _sSize;
	int _priority;

	void *_detour;
	
public:
	static int Init();
	static const char *GetLastError();

private:
	static void Add(FunctionHook_private *hook);
	void SetName(const char *name, const char *type);
	int Install();

public:
	FunctionHook_private(const char *name, const type_info &type, void *hook, void **outInternalSuper, int priority) :
		_hook(hook),
		_outInternalSuper(outInternalSuper),
		_hSize(0),
		_sSize(0),
		_detour(NULL),
		_priority(priority)
	{
		SetName(name, type.raw_name());
		Add(this);
	}
	~FunctionHook_private();
};
