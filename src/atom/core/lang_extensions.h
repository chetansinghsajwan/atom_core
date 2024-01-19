#pragma once

// #include <iostream>
// #include <type_traits>

namespace atom
{
    template <typename type>
    constexpr auto mov(type&& t) -> typename std::remove_reference_t<type>&&
    {
        return static_cast<typename std::remove_reference_t<type>&&>(t);
    }

    template <typename type>
    constexpr type&& forward(typename std::remove_reference_t<type>& t)
    {
        return static_cast<type&&>(t);
    }

    template <typename type>
    constexpr type&& forward(typename std::remove_reference_t<type>&& t)
    {
        static_assert(
            !std::is_lvalue_reference_v<type>, "can not forward an rvalue as an lvalue.");

        return static_cast<type&&>(t);
    }

    template <typename type>
    using tpure = std::remove_cvref_t<type>;
}
