#pragma once
#include "Version.h"
#include <Log.h>
#include <IsaacRepentance.h>
#include <Windows.h>

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <optional>
#include "document.h" // rapidjson

extern std::string GetUserPath();

namespace REPENTOGON {
	static auto gameStartTime = std::chrono::high_resolution_clock::now();
	static std::string optionsPath(GetUserPath() + "/Documents/My Games/Binding of Isaac Repentance+/Repentogon/");
	extern char stocktitle[256];
	extern char moddedtitle[256];

	static void ChangeWindowTitle(const char* text) {
		sprintf(stocktitle, "The Binding of Isaac: Repentance+ %s (REPENTOGON %s)%s", &g_GameVersionString, CMAKE_REPENTOGON_VERSION, text);
		SetWindowTextA(GetActiveWindow(), stocktitle);
	}

	static void SetStockWindowTitle() {
		SetWindowTextA(GetActiveWindow(), stocktitle);
	};

	template <typename T>
	static void AssertStructSizeFromVector(std::vector<T>& vector)
	{
#ifndef NDEBUG
		uintptr_t* data = reinterpret_cast<uintptr_t*>(&vector);
		uintptr_t begin = data[0];
		uintptr_t end = data[1];
		uintptr_t capacity = data[2];

		assert((end - begin) % sizeof(T) == 0);
		assert((capacity - begin) % sizeof(T) == 0);
#endif // !NDEBUG
	}

	inline std::string FloatToStringPrecision(float number, int precision) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(precision) << number;
		return ss.str();
	}

	static std::string StringFormat(const char* format, va_list args) noexcept
	{
		int size = vsnprintf(nullptr, 0, format, args);

		std::string result(size, '\0');
		vsnprintf(&result[0], size + 1, format, args);

		return result;
	}

	static inline std::string StringFormat(const char* format, ...) noexcept
	{
		va_list args;
		va_start(args, format);
		std::string result = StringFormat(format, args);
		va_end(args);
		return result;
	}

	template <typename NewType, typename OldType>
	static inline NewType DynamicNumberCast(OldType value, std::optional<std::string>& error) noexcept
	{
		static_assert(std::is_arithmetic<NewType>::value, "NewType must be a number type");
		static_assert(std::is_arithmetic<OldType>::value, "OldType must be a number type");

		constexpr auto min = std::numeric_limits<NewType>::lowest();
		constexpr auto max = std::numeric_limits<NewType>::max();

		constexpr bool differentArithmeticType = std::is_integral<NewType>::value != std::is_integral<OldType>::value;

		error.reset();

		if constexpr (!std::is_signed<NewType>::value && std::is_signed<OldType>::value)
		{
			if (value < 0)
			{
				error = StringFormat("value \"%s\" is not unsigned", std::to_string(value).c_str());
				return 0;
			}
		}

		if constexpr (differentArithmeticType || (sizeof(NewType) < sizeof(OldType)))
		{
			if (value < static_cast<OldType>(min) || value > static_cast<OldType>(max))
			{
				error = StringFormat("value \"%s\" is not within numeric limits (%s, %s)", std::to_string(value).c_str(), std::to_string(min).c_str(), std::to_string(max).c_str());
				return 0;
			}
		}

		return static_cast<NewType>(value);
	}

	static void FinishProgressDisplay() {
		auto finish = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish - gameStartTime).count() / 1000.0f;
		ZHL::Log("STARTUP PROGRESS FINISHED: %s\n", std::to_string(diff).c_str());

		ChangeWindowTitle("");
	}

	static void UpdateProgressDisplay(const char* text) {
		auto finish = std::chrono::high_resolution_clock::now();
		auto diffTotal = std::chrono::duration_cast<std::chrono::milliseconds>(finish - gameStartTime).count() / 1000.0f;
		std::string formattedDiff(FloatToStringPrecision(diffTotal, 3));
		ZHL::Log("STARTUP PROGRESS: %s\t%s\n", text, formattedDiff.c_str());

		char newText[128];
		sprintf(newText, " - %s (cur startup time: %s s)", text, formattedDiff.c_str());
		ChangeWindowTitle(newText);
	}

	static std::string GetRGONGfxAbsolutePath(const char* relpath) {
		char buffer[MAX_PATH];
		DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);
		std::string base = std::string(buffer);
		return base + "/resources-repentogon/" + std::string(relpath);
	}

	static const char* GetRepentogonDataPath() {
		if (CreateDirectory(optionsPath.c_str(), NULL)) {
			ZHL::Log("Newly created REPENTOGON savedata folder @ %s\n", optionsPath.c_str());
			std::string optionsPathMinus = optionsPath;
			optionsPathMinus.erase(optionsPathMinus.find_last_of('+'), 1);  // REP- savedata path is the same, just without the '+'
			ZHL::Log("Checking for legacy REPENTOGON savedata @ %s\n", optionsPathMinus.c_str());
			if (std::filesystem::is_directory(optionsPathMinus)) {
				ZHL::Log("Legacy REPENTOGON savedata directory found. Copying contents to new savedata folder...\n");
				std::filesystem::copy(optionsPathMinus, optionsPath, std::filesystem::copy_options::recursive);
				ZHL::Log("...Done!\n");
			} else {
				ZHL::Log("No legacy REPENTOGON savedata directory found.\n");
			}
		} else if (GetLastError() != ERROR_ALREADY_EXISTS) {
			ZHL::Log("Error %s creating Repentogon Save directory: %s\n", GetLastError(), optionsPath.c_str());
			return "";
		}
		return optionsPath.c_str();
	}

	namespace Lua
	{
		static std::string GetFunctionName(lua_State* L, lua_Debug* ar) noexcept
		{
			if (*ar->namewhat != '\0')
			{
				return StringFormat("function '%s'", ar->name);
			}
			if (*ar->what == 'm')
			{
				return "main chunk";
			}

			return StringFormat("function at line %d", ar->linedefined);
		}

		static std::string CleanTraceback(lua_State* L, int level) noexcept
		{
			lua_Debug ar;
			std::string stackTraceback = "Stack Traceback:\n";

			while (lua_getstack(L, level++, &ar))
			{
				lua_getinfo(L, "Sln", &ar);
				if (*ar.what == 'C')
				{
					if (std::strcmp(ar.name, "xpcall") == 0)
					{
						break;
					}

					stackTraceback += StringFormat("\n in method '%s'", ar.name);
				}
				else
				{
					if (ar.currentline <= 0)
					{
						stackTraceback += StringFormat("\n  %s: in ", ar.short_src);
					}
					else
					{
						stackTraceback += StringFormat("\n  %s:%d: in ", ar.short_src, ar.currentline);
					}

					stackTraceback += GetFunctionName(L, &ar);
				}
			}

			return stackTraceback;
		}

		static std::string GenerateInvalidTypeMessage(lua_State* L, int idx, const char* expectedType) noexcept
		{
			return StringFormat("%s expected, got %s", expectedType, lua_typename(L, lua_type(L, idx)));
		}

		static inline std::optional<bool> ReadBool(lua_State* L, int index, std::optional<std::string>& error, bool optional = true) noexcept
		{
			error.reset();

			if (!lua_isboolean(L, index))
			{
				if (!optional || !lua_isnil(L, index))
				{
					error = GenerateInvalidTypeMessage(L, index, "boolean");
				}
				return std::nullopt;
			}

			return lua_toboolean(L, index);
		}

		template<typename T>
		static std::optional<T> ReadInteger(lua_State* L, int index, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");

			error.reset();

			if (!lua_isinteger(L, index))
			{
				if (!optional || !lua_isnil(L, index))
				{
					error = GenerateInvalidTypeMessage(L, index, "integer");
				}
				return std::nullopt;
			}

			if constexpr (!std::is_signed<T>::value)
			{
				if (lua_tonumber(L, index) < 0)
				{
					error = GenerateInvalidTypeMessage(L, index, "unsigned integer");
					return false;
				}

				lua_Unsigned luaInteger = lua_tointeger(L, index);
				T value = DynamicNumberCast<T>(luaInteger, error);
				if (error)
				{
					std::nullopt;
				}

				return value;
			}
			else
			{
				lua_Integer luaInteger = lua_tointeger(L, index);
				T value = DynamicNumberCast<T>(luaInteger, error);
				if (error)
				{
					std::nullopt;
				}

				return value;
			}
		}

		template<typename T>
		static std::optional<T> ReadNumber(lua_State* L, int index, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			error.reset();

			if (!lua_isnumber(L, index))
			{
				if (!optional || !lua_isnil(L, index))
				{
					error = GenerateInvalidTypeMessage(L, index, "number");
				}
				return std::nullopt;
			}

			lua_Number luaNumber = lua_tonumber(L, index);
			T value = DynamicNumberCast<T>(luaNumber, error);
			if (error)
			{
				std::nullopt;
			}

			return value;
		}

		static inline std::optional<std::string> ReadString(lua_State* L, int index, std::optional<std::string>& error, bool optional = true) noexcept
		{
			error.reset();

			if (!lua_isstring(L, index))
			{
				if (!optional || !lua_isnil(L, index))
				{
					error = GenerateInvalidTypeMessage(L, index, "string");
				}
				return std::nullopt;
			}

			return lua_tostring(L, index);
		}

		static inline std::optional<bool> ReadBoolField(lua_State* L, int index, const char* fieldName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			assert(lua_istable(L, index));
			assert(fieldName);

			lua_getfield(L, index, fieldName);
			auto result = ReadBool(L, -1, error, optional);
			lua_pop(L, 1);

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadIntegerField(lua_State* L, int index, const char* fieldName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");
			assert(lua_istable(L, index));
			assert(fieldName);

			lua_getfield(L, index, fieldName);
			auto result = ReadInteger<T>(L, -1, error, optional);
			lua_pop(L, 1);

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadNumberField(lua_State* L, int index, const char* fieldName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
			assert(lua_istable(L, index));
			assert(fieldName);

			lua_getfield(L, index, fieldName);
			auto result = ReadNumber<T>(L, -1, error, optional);
			lua_pop(L, 1);

			return result;
		}

		static inline std::optional<std::string> ReadStringField(lua_State* L, int index, const char* fieldName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			assert(lua_istable(L, index));
			assert(fieldName);

			lua_getfield(L, index, fieldName);
			auto result = ReadString(L, -1, error, optional);
			lua_pop(L, 1);

			return result;
		}
	}

	namespace Json
	{
		static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

		static std::string GenerateInvalidTypeMessage(const rapidjson::Value& jsonValue, const char* expectedType) noexcept
		{
			return StringFormat("%s expected, got %s", expectedType, kTypeNames[jsonValue.GetType()]);
		}

		static inline std::optional<bool> ReadBool(const rapidjson::Value& jsonValue, std::optional<std::string>& error) noexcept
		{
			error.reset();

			if (!jsonValue.IsBool())
			{
				error = GenerateInvalidTypeMessage(jsonValue, "Bool");
				return std::nullopt;
			}

			return jsonValue.GetBool();
		}

		template<typename T>
		static std::optional<T> ReadInteger(const rapidjson::Value& jsonValue, std::optional<std::string>& error) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");

			constexpr const char* type = std::is_unsigned<T>::value ? "Unsigned Integer" : "Integer";
			constexpr auto Validator = std::is_unsigned<T>::value ? &rapidjson::Value::IsUint64 : &rapidjson::Value::IsInt64;

			error.reset();

			if (!(jsonValue.*Validator)())
			{
				error = GenerateInvalidTypeMessage(jsonValue, type);
				return std::nullopt;
			}

			if constexpr (!std::is_signed<T>::value)
			{
				T value = DynamicNumberCast<T>(jsonValue.GetUint64(), error);
				if (error)
				{
					std::nullopt;
				}

				return value;
			}
			else
			{
				T value = DynamicNumberCast<T>(jsonValue.GetInt64(), error);
				if (error)
				{
					std::nullopt;
				}

				return value;
			}
		}

		template<typename T>
		static std::optional<T> ReadNumber(const rapidjson::Value& jsonValue, std::optional<std::string>& error) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			error.reset();

			if (!jsonValue.IsDouble())
			{
				error = GenerateInvalidTypeMessage(jsonValue, "Number");
				return std::nullopt;
			}

			T value = DynamicNumberCast<T>(jsonValue.GetDouble(), error);
			if (error)
			{
				std::nullopt;
			}

			return value;
		}

		static inline std::optional<std::string> ReadString(const rapidjson::Value& jsonValue, std::optional<std::string>& error) noexcept
		{
			error.reset();

			if (!jsonValue.IsString())
			{
				error = GenerateInvalidTypeMessage(jsonValue, "String");
				return std::nullopt;
			}

			return jsonValue.GetString();
		}

		static inline std::optional<bool> ReadBoolMember(const rapidjson::Value& jsonValue, const char* memberName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			assert(jsonValue.IsObject());
			assert(memberName);

			error.reset();

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					error = "member does not exist";
				}
				return std::nullopt;
			}

			return ReadBool(it->value, error);
		}

		template<typename T>
		static inline std::optional<T> ReadIntegerMember(const rapidjson::Value& jsonValue, const char* memberName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");
			assert(jsonValue.IsObject());
			assert(memberName);

			error.reset();

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					error = "member does not exist";
				}
				return std::nullopt;
			}

			return ReadInteger<T>(it->value, error);
		}

		template<typename T>
		static inline std::optional<T> ReadNumberMember(const rapidjson::Value& jsonValue, const char* memberName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
			assert(jsonValue.IsObject());
			assert(memberName);

			error.reset();

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					error = "member does not exist";
				}
				return std::nullopt;
			}

			return ReadNumber<T>(it->value, error);
		}

		static inline std::optional<std::string> ReadStringMember(const rapidjson::Value& jsonValue, const char* memberName, std::optional<std::string>& error, bool optional = true) noexcept
		{
			assert(jsonValue.IsObject());
			assert(memberName);

			error.reset();

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					error = "member does not exist";
				}
				return std::nullopt;
			}

			return ReadString(it->value, error);
		}
	}
}

