#pragma once

#include "Log.h"
#include "MiscFunctions.h"

#include <cstdint>
#include <cassert>
#include <array>
#include <string>
#include <vector>

#pragma push_macro("ERROR")
#undef ERROR

namespace LogUtility
{
    typedef std::optional<std::string> Error;

    enum class eLogType
    {
        INFO = 0,
        WARN = 1,
        ERROR = 2
    };

    class LogContext
    {
        std::vector<std::string> m_Context;
		std::string m_ContextCache;
		std::array<uint32_t, 3> m_LoggedMessagesCount = { 0 };
    private:
        const char* get_log_type_string(eLogType logType)
        {
            switch (logType)
            {
            case eLogType::INFO:
                return "INFO";
            case eLogType::WARN:
                return "WARN";
            case eLogType::ERROR:
                return "ERROR";
            default:
                assert(false);
            }

            return nullptr;
        }

        std::string build_context_string()
        {
            std::string result = "";

            for (const auto& contextString : m_Context)
            {
                result += contextString;
            }

            return result;
        }

    public:
        inline void push_back(const std::string& message)
        {
            m_Context.push_back(message);
			m_ContextCache += m_Context.back();
        }

        inline void emplace_back(std::string&& message)
        {
            m_Context.emplace_back(std::move(message));
			m_ContextCache += m_Context.back();
        }

        inline void pop_back()
        {
			const std::string& last = m_Context.back();
			m_ContextCache.erase(m_ContextCache.size() - last.size(), last.size());
            m_Context.pop_back();
        }

        inline void LogMessage(eLogType logType, const char* message)
        {
            ZHL::Log("[%s] %s%s\n", get_log_type_string(logType), m_ContextCache.c_str(), message);
            m_LoggedMessagesCount[(size_t)logType]++;
        }

        inline uint32_t GetLoggedMessagesCount(eLogType logType)
        {
            return m_LoggedMessagesCount[(size_t)logType];
        }
    };

	static uint32_t GetLogTypeConsoleColor(eLogType logType)
	{
		switch (logType)
		{
		case eLogType::WARN:
			return 0xFFFCCA03;
		case eLogType::ERROR:
			return 0xFFF08080;
		default:
			return 0xFFD3D3D3;
		}
	}

	static void PrintConsole(eLogType logType, const std::string& message)
	{
		g_Game->GetConsole()->Print(message, GetLogTypeConsoleColor(logType), 0x96u);
	}

    namespace Lua
    {
		static std::string GetStackLevelInfo(lua_State* L, int level) noexcept
		{
			lua_Debug ar;
			if (!lua_getstack(L, level, &ar))
			{
				return "";
			}

			lua_getinfo(L, "Sln", &ar);
			if (ar.currentline <= 0)
			{
				return REPENTOGON::StringFormat("%s: in %s", ar.short_src, REPENTOGON::Lua::GetFunctionName(L, &ar).c_str());
			}
			else
			{
				return REPENTOGON::StringFormat("%s:%d: in %s", ar.short_src, ar.currentline, REPENTOGON::Lua::GetFunctionName(L, &ar).c_str());
			}
		}

		static inline void LogInvalidArg(LogContext& logContext, const char* error, const char* fieldName = nullptr) noexcept
		{
			assert(error);
			
			if (fieldName)
			{
				logContext.LogMessage(eLogType::ERROR, REPENTOGON::StringFormat("invalid argument for \"%s\" (%s)", fieldName, error).c_str());
			}
			else
			{
				logContext.LogMessage(eLogType::ERROR, REPENTOGON::StringFormat("invalid argument (%s)", error).c_str());
			}
		}

		static inline bool ValidateTable(lua_State* L, int index, LogContext& logContext, const char* fieldName = nullptr, bool optional = true) noexcept
		{
			if (!lua_istable(L, index))
			{
				if (!optional || !lua_isnil(L, index))
				{
					LogInvalidArg(logContext, REPENTOGON::Lua::GenerateInvalidTypeMessage(L, index, "table").c_str(), fieldName);
				}
				return false;
			}

			return true;
		}

		static inline std::optional<bool> ReadBool(lua_State* L, int index, LogContext& logContext, const char* fieldName = nullptr, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Lua::ReadBool(L, index, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), nullptr);
			}

			return result;
		}

		template<typename T>
		static std::optional<T> ReadInteger(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");

			Error error;
			auto result = REPENTOGON::Lua::ReadInteger<T>(L, index, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), nullptr);
			}

			return result;
		}

		template<typename T>
		static std::optional<T> ReadNumber(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			Error error;
			auto result = REPENTOGON::Lua::ReadNumber<T>(L, index, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), nullptr);
			}

			return result;
		}

		static inline std::optional<std::string> ReadString(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Lua::ReadString(L, index, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), nullptr);
			}

			return result;
		}

		static inline std::optional<bool> ReadBoolField(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Lua::ReadBoolField(L, index, fieldName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), fieldName);
			}

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadIntegerField(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");

			Error error;
			auto result = REPENTOGON::Lua::ReadIntegerField<T>(L, index, fieldName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), fieldName);
			}

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadNumberField(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			Error error;
			auto result = REPENTOGON::Lua::ReadNumberField<T>(L, index, fieldName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), fieldName);
			}

			return result;
		}

		static inline std::optional<std::string> ReadStringField(lua_State* L, int index, const char* fieldName, LogContext& logContext, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Lua::ReadStringField(L, index, fieldName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), fieldName);
			}

			return result;
		}
    }

	namespace Json
	{
		static void LogInvalidArg(LogContext& logContext, const char* error, const char* memberName = nullptr) noexcept
		{
			assert(error);

			if (memberName)
			{
				logContext.LogMessage(eLogType::ERROR, REPENTOGON::StringFormat("invalid argument for <%s> (%s)", memberName, error).c_str());
			}
			else
			{
				logContext.LogMessage(eLogType::ERROR, REPENTOGON::StringFormat("invalid argument (%s)", error).c_str());
			}
		}

		static inline bool ValidateArray(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			if (!jsonValue.IsArray())
			{
				LogInvalidArg(logContext, REPENTOGON::Json::GenerateInvalidTypeMessage(jsonValue, "Array").c_str(), memberName);
				return false;
			}

			return true;
		}

		static inline bool ValidateObject(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			if (!jsonValue.IsObject())
			{
				LogInvalidArg(logContext, REPENTOGON::Json::GenerateInvalidTypeMessage(jsonValue, "Object").c_str(), memberName);
				return false;
			}

			return true;
		}

		static inline std::optional<bool> ReadBool(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			Error error;
			auto result = REPENTOGON::Json::ReadBool(jsonValue, error);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		template<typename T>
		static std::optional<T> ReadInteger(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");

			Error error;
			auto result = REPENTOGON::Json::ReadInteger<T>(jsonValue, error);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		template<typename T>
		static std::optional<T> ReadNumber(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			Error error;
			auto result = REPENTOGON::Json::ReadNumber<T>(jsonValue, error);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		static inline std::optional<std::string> ReadString(const rapidjson::Value& jsonValue, LogContext& logContext, const char* memberName = nullptr) noexcept
		{
			Error error;
			auto result = REPENTOGON::Json::ReadString(jsonValue, error);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		static inline std::optional<bool> ReadBoolMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Json::ReadBoolMember(jsonValue, memberName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadIntegerMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_integral<T>::value, "T must be an integer type");
			Error error;
			auto result = REPENTOGON::Json::ReadIntegerMember<T>(jsonValue, memberName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		template<typename T>
		static inline std::optional<T> ReadNumberMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
			Error error;
			auto result = REPENTOGON::Json::ReadNumberMember<T>(jsonValue, memberName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		static inline std::optional<std::string> ReadStringMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			Error error;
			auto result = REPENTOGON::Json::ReadStringMember(jsonValue, memberName, error, optional);
			if (error)
			{
				LogInvalidArg(logContext, error.value().c_str(), memberName);
			}

			return result;
		}

		static inline std::optional<const rapidjson::Value*> GetArrayMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			assert(jsonValue.IsObject());
			assert(memberName);

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					LogInvalidArg(logContext, "member does not exist", memberName);
				}
				return std::nullopt;
			}

			if (!ValidateArray(it->value, logContext, memberName))
			{
				return std::nullopt;
			}

			return &it->value;
		}

		static inline std::optional<const rapidjson::Value*> GetObjectMember(const rapidjson::Value& jsonValue, const char* memberName, LogContext& logContext, bool optional = true) noexcept
		{
			assert(jsonValue.IsObject());
			assert(memberName);

			auto it = jsonValue.FindMember(memberName);
			if (it == jsonValue.MemberEnd())
			{
				if (!optional)
				{
					LogInvalidArg(logContext, "member does not exist", memberName);
				}
				return std::nullopt;
			}

			if (!ValidateObject(it->value, logContext, memberName))
			{
				return std::nullopt;
			}

			return &it->value;
		}
	}
}

#pragma pop_macro("ERROR")