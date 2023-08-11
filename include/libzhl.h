#pragma once

#if defined(libzhl_EXPORTS)
#define LIBZHL_API __declspec(dllexport)
#else
#define LIBZHL_API __declspec(dllimport)
#endif

#define MOD_EXPORT extern "C" __declspec(dllexport)

#include <type_traits>
#include <variant>

template<typename T>
struct IsCallerCleanup {
	static constexpr bool value = false;
};

template<typename R, typename... Args>
struct IsCallerCleanup<R(Args...)> {
	static constexpr bool value = true;
};

template<typename R, typename... Args>
struct IsCallerCleanup<R(*)(Args...)> {
	static constexpr bool value = true;
};

/* template<typename R, typename... Args>
struct IsCallerCleanup<R __cdecl(Args...)> {
	static constexpr bool value = true;
}; */

template<typename C, typename R, typename... Args>
struct IsCallerCleanup<R(__cdecl C::*)(Args...)> {
	static constexpr bool value = true;
};

template<typename C, typename R, typename... Args>
struct IsCallerCleanup<R(__cdecl C::*)(Args...) const> {
	static constexpr bool value = true;
};

template<typename C, typename R, typename... Args>
struct IsCallerCleanup<R(__cdecl C::*)(Args...) volatile> {
	static constexpr bool value = true;
};

template<typename C, typename R, typename... Args>
struct IsCallerCleanup<R(__cdecl C::*)(Args...) volatile const> {
	static constexpr bool value = true;
};

namespace CallerCleanupCheck {
	void f(); // __cdecl by default
	void __cdecl g(); // explicitly cdecl
	void __stdcall h();
	void __fastcall i();

	struct StructCheck {
		void f(); // implicitly __thiscall
		void __stdcall g();
		void __fastcall h();
		void __cdecl i();

		void fc() const;
		void __stdcall gc() const;
		void __fastcall hc() const;
		void __cdecl ic() const;

		void fv() volatile;
		void __stdcall gv() volatile;
		void __fastcall hv() volatile;
		void __cdecl iv() volatile;

		void fcv() const volatile;
		void __stdcall gcv() const volatile;
		void __fastcall hcv() const volatile;
		void __cdecl icv() const volatile;

		static void sf();
		static void __stdcall sg();
		static void __fastcall sh();
		static void __cdecl si();
	};

	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::f)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::g)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::h)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::i)>);

	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::fc)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::gc)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::hc)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::ic)>);

	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::fv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::gv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::hv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::iv)>);

	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::fcv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::gcv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::hcv)>);
	static_assert(std::is_member_function_pointer_v<decltype(&StructCheck::icv)>);

	static_assert(!std::is_member_function_pointer_v<decltype(&StructCheck::sf)>);
	static_assert(!std::is_member_function_pointer_v<decltype(&StructCheck::sg)>);
	static_assert(!std::is_member_function_pointer_v<decltype(&StructCheck::sh)>);
	static_assert(!std::is_member_function_pointer_v<decltype(&StructCheck::si)>);

	static_assert(IsCallerCleanup<decltype(f)>::value);
	static_assert(IsCallerCleanup<decltype(g)>::value);
	static_assert(!IsCallerCleanup<decltype(h)>::value);
	static_assert(!IsCallerCleanup<decltype(i)>::value);

	static_assert(!IsCallerCleanup<decltype(&StructCheck::f)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::g)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::h)>::value);
	static_assert(IsCallerCleanup<decltype(&StructCheck::i)>::value);

	static_assert(!IsCallerCleanup<decltype(&StructCheck::fc)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::gc)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::hc)>::value);
	static_assert(IsCallerCleanup<decltype(&StructCheck::ic)>::value);

	static_assert(!IsCallerCleanup<decltype(&StructCheck::fv)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::gv)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::hv)>::value);
	static_assert(IsCallerCleanup<decltype(&StructCheck::iv)>::value);

	static_assert(!IsCallerCleanup<decltype(&StructCheck::fcv)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::gcv)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::hcv)>::value);
	static_assert(IsCallerCleanup<decltype(&StructCheck::icv)>::value);

	static_assert(IsCallerCleanup<decltype(&StructCheck::sf)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::sg)>::value);
	static_assert(!IsCallerCleanup<decltype(&StructCheck::sh)>::value);
	static_assert(IsCallerCleanup<decltype(&StructCheck::si)>::value);
}

template<typename T, typename... Args>
struct IsInPack;

template<typename T, typename U>
struct IsInPack<T, U> {
	constexpr static bool value = std::is_same_v<T, U>;
};

template<typename T, typename H, typename... Tail>
struct IsInPack<T, H, Tail...> {
	constexpr static bool value = std::is_same_v<T, H> || IsInPack<T, Tail...>::value;
};

template<typename T, typename... Types>
struct IsInPack<T, std::variant<Types...>> {
	constexpr static bool value = IsInPack<T, Types...>::value;
};

template<typename T, typename... Types>
constexpr bool IsInPackV = IsInPack<T, Types...>::value;