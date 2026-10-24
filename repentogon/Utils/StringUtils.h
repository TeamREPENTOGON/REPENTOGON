#pragma once

#include <cassert>
#include <cstdarg>
#include <charconv>
#include <optional>
#include <string>
#include <string_view>

namespace REPENTOGON
{
    inline std::string StringFormat(const char* format, va_list args)
    {
        va_list copy; va_copy(copy, args);
        int size = vsnprintf(nullptr, 0, format, copy);
        assert(size >= 0);

        std::string result(size, '\0');
        vsnprintf(&result[0], size + 1, format, args);

        return result;
    }

    inline std::string StringFormat(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        std::string result = StringFormat(format, args);
        va_end(args);
        return result;
    }

    /// @brief Concatenates all passed strings.
    /// all arguments must be convertible to std::string_view
    template <typename... Args, std::enable_if_t<(std::is_convertible_v<Args, std::string_view> && ...),int> = 0>
    inline std::string StringConcat(const Args&... args)
    {
        const std::size_t size = (std::string_view(args).size() + ...);

        std::string result;
        result.reserve(size);

        (result.append(std::string_view(args)), ...);

        return result;
    }

    template <typename T>
    inline std::optional<T> StringToNumber(const std::string_view& string)
    {
        T value;
        const char* start = string.data();
        const char* end = string.data() + string.size();

        auto [ptr, ec] = std::from_chars(start, end, value);

        if (ec == std::errc{} && ptr == end) {
            return std::make_optional(value);
        }

        return std::nullopt;
    }
}