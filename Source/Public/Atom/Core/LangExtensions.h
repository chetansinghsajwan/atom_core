#pragma once

namespace Atom
{
    template <typename T>
    class _remove_ref
    {
    public:
        using value = T;
    };

    template <typename T>
    class _remove_ref<T&>
    {
    public:
        using value = T;
    };

    template <typename T>
    class _remove_ref<T&&>
    {
    public:
        using value = T;
    };

    template <typename T>
    using _remove_ref_t = _remove_ref<T>::value;
}

#define ATOM_IS_CONFIG_DEBUG true
#define ATOM_IF_DEBUG if constexpr (ATOM_IS_CONFIG_DEBUG)

#define mov(...) static_cast<Atom::_remove_ref_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define abstract = 0

#include <iostream>

namespace Atom
{
    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type& t)
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename std::remove_reference<T>::type&& t)
    {
        static_assert(
            !std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");

        return static_cast<T&&>(t);
    }
}
