#pragma once
#include <iostream>
#include <type_traits>

namespace Atom
{
    template <typename T>
    constexpr auto mov(T&& t) -> typename std::remove_reference<T>::type&&
    {
        return static_cast<typename std::remove_reference<T>::type&&>(t);
    }

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

    template <typename T>
    using TPure = std::remove_cvref_t<T>;
}
