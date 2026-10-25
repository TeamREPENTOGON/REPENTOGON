#pragma once

#include <array>
#include <cassert>
#include <stdexcept>
#include <type_traits>

namespace REPENTOGON
{

template<typename T>
class Array
{
private:
    T* m_ptr;
    const size_t m_size;

private:
    // Trait used to automatically detect any type that exposes data() and size() with a compatible pointer type
    // Used to automatically create a constructor for these types.
    template<typename C, typename = void>
    struct is_contiguous_container : std::false_type {};

    template<typename C>
    struct is_contiguous_container<C, std::void_t<
        decltype(std::data(std::declval<C const&>())),
        decltype(std::size(std::declval<C const&>()))>>
        : std::is_convertible<
              std::remove_pointer_t<decltype(std::data(std::declval<C const&>()))> (*)[],
              T const (*)[]>
    {};

public:
    constexpr Array(T* ptr, size_t size)
        : m_ptr(ptr), m_size(size) {}

    constexpr Array(T* first, T* last)
        : m_ptr(first), m_size(static_cast<size_t>(last - first)) {}

    template<size_t N>
    constexpr Array(T(&arr)[N])
        : m_ptr(arr), m_size(N) {}
    
    template<size_t N>
    constexpr Array(std::array<T, N>& arr)
        : m_ptr(arr.data()), m_size(N) {}

    template<typename C,
             typename = std::enable_if_t<
                 !std::is_same_v<std::decay_t<C>, Array> &&
                 is_contiguous_container<C>::value>>
    constexpr Array(C& c)
        : m_ptr(std::data(c)), m_size(std::size(c)) {}

    constexpr Array(const Array&) = default;
    constexpr Array& operator=(Array&) = default;

    constexpr size_t size() const { return m_size; }
    constexpr size_t length() const { return m_size; }
    constexpr size_t size_bytes() const { return m_size * sizeof(T); }
    constexpr bool empty() const { return m_size == 0; }

    constexpr T& operator[](size_t i) const noexcept
    {
        assert(i < m_size);
        return m_ptr[i];
    }

    T& at(size_t i) const
    {
        if (i >= m_size) throw std::out_of_range("Array::at");
        return m_ptr[i];
    }
};

} // namespace REPENTOGON