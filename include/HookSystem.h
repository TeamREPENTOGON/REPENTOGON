#pragma once

#pragma warning (disable: 4530)

#include "libzhl.h"
#include <cstdlib>
#include <typeinfo>

#include <string>

//=================================================================================================

namespace ZHL
{
	LIBZHL_API void Init();
	LIBZHL_API void AddFilesystemPath(const char *path);
}

class LIBZHL_API FunctionHook
{
private:
	static void Add(const char *name, const type_info &type, void *hook, void **outInternalSuper, int priority);

public:
	template <class T> FunctionHook(const char *name, const type_info &type, T hook, void **outInternalSuper, int priority)
	{
		Add(name, type, *(void**)&hook, outInternalSuper, priority);
	}
};

class LIBZHL_API FunctionHookCustom {
private:
	static void Add(const char* name, const type_info& type, void* hook, void** outInternalSuper, int priority);

public:
	template <class T> FunctionHookCustom(const char* name, const type_info& type, T hook, void** outInternalSuper, int priority)
	{
		Add(name, type, *(void**)&hook, outInternalSuper, priority);
	}
};

//=================================================================================================
// static_assert(std::is_class_v<_classname>);
// static_assert(std::is_member_function_pointer_v<decltype(_classname::_name)>); 
#define _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, _type) \
	namespace { namespace Hook_##_id { \
		static_assert(std::is_class_v<_classname>, "Cannot HOOK_METHOD on something that is not a class"); \
		static_assert(std::is_member_function_pointer_v<decltype(&_classname::_name)>, "Cannot HOOK_METHOD on something that is not a method (use HOOK_STATIC for static methods)"); \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			auto hook _type ; \
			auto super _type ; \
		}; \
		static FunctionHook hookObj(#_classname "::" #_name, typeid(auto (_classname::*) _type), &wrapper::hook, &internalSuper, _priority); \
	} } \
	auto __declspec(naked) Hook_##_id :: wrapper::super _type {__asm jmp internalSuper} \
	auto Hook_##_id ::wrapper::hook _type

#define _DEFINE_OVERLOADED_METHOD_HOOK(_id, _classname, _name, _ret, _args, _priority, _type) \
	namespace { namespace Hook_##_id { \
		static_assert(std::is_class_v<_classname>, "Cannot HOOK_METHOD on something that is not a class"); \
		static_assert(std::is_member_function_pointer_v<decltype(static_cast<_ret (_classname::*)_args>(&_classname::_name))>, "Cannot HOOK_METHOD on something that is not a method (use HOOK_STATIC for static methods)"); \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			auto hook _type ; \
			auto super _type ; \
		}; \
		static FunctionHook hookObj(#_classname "::" #_name, typeid(auto (_classname::*) _type), &wrapper::hook, &internalSuper, _priority); \
	} } \
	auto __declspec(naked) Hook_##_id :: wrapper::super _type {__asm jmp internalSuper} \
	auto Hook_##_id ::wrapper::hook _type

#define _DEFINE_METHOD_HOOK0(_id, _classname, _name, _priority, _type) _DEFINE_METHOD_HOOK1(_id, _classname, _name, _priority, _type)

#define HOOK_METHOD(_classname, _name, _type) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, 0, _type)
#define HOOK_OVERLOADED_METHOD(_classname, _name, _ret, _args, _type) _DEFINE_OVERLOADED_METHOD_HOOK(__LINE__, _classname, _name, _ret, _args, 0, _type)
#define HOOK_METHOD_PRIORITY(_classname, _name, _priority, _type) _DEFINE_METHOD_HOOK0(__LINE__, _classname, _name, _priority, _type)
#define HOOK_OVERLOADED_METHOD_PRIORITY(_classname, _name, _priority, _ret, _args, _type) _DEFINE_OVERLOADED_METHOD_HOOK(__LINE__, _classname, _name, _ret, _ars, _priority, _type)

//=================================================================================================

#define _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type, _conv, _hookKind) \
	static_assert(std::is_class_v<_classname>, "Cannot HOOK_STATIC on something that is not a class"); \
	static_assert(!std::is_member_function_pointer_v<decltype(&_classname::_name)>, "Cannot HOOK_STATIC on something that is not a static method (use HOOK_METHOD for a non static method)"); \
	static_assert(IsCallerCleanup<decltype(&_classname::_name)>::value == IsCallerCleanup<void (_conv*)(void)>::value, "Inconsistent calling convention"); \
	namespace { namespace Hook_##_id { \
		static void *internalSuper = NULL; \
		struct wrapper : public _classname { \
			static auto _conv hook _type ; \
			static auto _conv super _type ; \
		}; \
		static _hookKind hookObj(#_classname "::" #_name, typeid(auto (*) _type), &wrapper::hook, &internalSuper, _priority); \
	} } \
	auto __declspec(naked) Hook_##_id :: wrapper::super _type {__asm jmp internalSuper} \
	auto Hook_##_id ::wrapper::hook _type

#define _DEFINE_STATIC_HOOK0(_id, _classname, _name, _priority, _type, _conv) _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type, _conv, FunctionHook)

#define HOOK_STATIC(_classname, _name, _type, _conv) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, 0, _type, _conv)
#define HOOK_STATIC_PRIORITY(_classname, _name, _priority, _type, _conv) _DEFINE_STATIC_HOOK0(__LINE__, _classname, _name, _priority, _type, _conv)

#define _DEFINE_STATIC_CUSTOM_HOOK0(_id, _classname, _name, _priority, _type, _conv) _DEFINE_STATIC_HOOK1(_id, _classname, _name, _priority, _type, _conv, FunctionHookCustom)

#define HOOK_STATIC_CUSTOM(_classname, _name, _type, _conv) _DEFINE_STATIC_CUSTOM_HOOK0(__LINE__, _classname, _name, 0, _type, _conv)
#define HOOK_STATIC_PRIORITY_CUSTOM(_classname, _name, _priority, _type, _conv) _DEFINE_STATIC_CUSTOM_HOOK0(__LINE__, _classname, _name, _priority, _type, _conv)

//=================================================================================================

#define _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type, _conv, _hookKind) \
	namespace { namespace Hook_##_id { \
		static_assert(IsCallerCleanup<decltype(&_name)>::value == IsCallerCleanup<void (_conv*)(void)>::value, "Inconsistent calling convention"); \
		static void *internalSuper = NULL; \
		static auto _conv hook _type ; \
		static auto _conv super _type ; \
		\
		static _hookKind hookObj(#_name, typeid(auto (*) _type), &hook, &internalSuper, _priority); \
	} } \
	auto __declspec(naked) _conv Hook_##_id ::super _type {__asm jmp internalSuper} \
	auto _conv Hook_##_id ::hook _type

#define _DEFINE_GLOBAL_HOOK0(_id, _name, _priority, _type, _conv) _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type, _conv, FunctionHook)

#define HOOK_GLOBAL(_name, _type, _conv) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, 0, _type, _conv)
#define HOOK_GLOBAL_PRIORITY(_name, _priority, _type, _conv) _DEFINE_GLOBAL_HOOK0(__LINE__, _name, _priority, _type, _conv)


#define _DEFINE_GLOBAL_CUSTOM_HOOK0(_id, _name, _priority, _type, _conv) _DEFINE_GLOBAL_HOOK1(_id, _name, _priority, _type, _conv, FunctionHookCustom)

#define HOOK_GLOBAL_CUSTOM(_name, _type, _conv) _DEFINE_GLOBAL_CUSTOM_HOOK0(__LINE__, _name, 0, _type, _conv)
#define HOOK_GLOBAL_PRIORITY_CUSTOM(_name, _priority, _type, _conv) _DEFINE_GLOBAL_CUSTOM_HOOK0(__LINE__, _name, _priority, _type, _conv)