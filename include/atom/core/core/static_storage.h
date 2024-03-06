#pragma once
#include "atom/core/core.h"
#include "atom/core/core/type_list.h"
#include "atom/core/contracts_decl.h"

namespace atom
{
    template <typename... types>
    class static_storage_for
    {
    private:
        using _types = type_list<types...>;

    public:
        alignas(_types::max_align.to_unwrapped()) byte storage[_types::max_size.to_unwrapped()];
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

            return ptr(_storage + i.to_unwrapped());
        }

        constexpr auto mut_mem(usize i = 0) -> void*
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return mut_ptr(_storage + i.to_unwrapped());
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return _storage[i.to_unwrapped()];
        }

        constexpr auto mut_ref(usize i) -> byte&
        {
            ATOM_DEBUG_EXPECTS(i < size());

            return _storage[i.to_unwrapped()];
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
            for (usize i = usize(0); i < size(); i++)
            {
                if (_storage[i.to_unwrapped()] != that._storage[i.to_unwrapped()])
                    return false;
            }

            return true;
        }

    public:
        byte _storage[size().to_unwrapped()];
    };
}
