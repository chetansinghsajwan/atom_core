#pragma once
#include "atom/core/core.h"
// #include "atom/core/types.h"
#include "atom/core/contracts.h"

namespace atom
{
    template <typename... values_t>
    class static_storage_for
    {
    private:
        using value_types_list = typelist<values_t...>;

    public:
        alignas(value_types_list::max_align) byte storage[value_types_list::max_size];
    };

    template <usize in_size>
    class static_storage
    {
    public:
        static consteval auto size() -> usize
        {
            return in_size;
        }

    public:
        constexpr auto mem(usize i = 0) const -> const void*
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return ptr(_storage + i);
        }

        constexpr auto mut_mem(usize i = 0) -> void*
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return mut_ptr(_storage + i);
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return _storage[i];
        }

        constexpr auto mut_ref(usize i) -> byte&
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return _storage[i];
        }

        constexpr auto operator[](usize i) const -> const byte&
        {
            return ref(i);
        }

        constexpr auto operator[](usize i) -> byte&
        {
            return mut_ref(i);
        }

        constexpr auto is_eq(const static_storage& that) const -> bool
        {
            for (usize i = 0; i < size(); i++)
            {
                if (_storage[i] != that._storage[i])
                    return false;
            }

            return true;
        }

    public:
        byte _storage[size()];
    };
}
