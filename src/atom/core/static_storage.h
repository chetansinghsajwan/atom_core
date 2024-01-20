#pragma once
#include "atom/core/primitives.h"
// #include "atom/core/type_list.h"

namespace atom
{
    template <typename... ts>
    class static_storage_for
    {
    private:
        using _types = type_list<ts...>;

    public:
        alignas(_types::max_align.unwrap()) byte storage[_types::max_size];
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
        constexpr auto mem(usize i = 0) const -> mem_ptr<void>
        {
            contracts::debug_expects(i < size());

            return mem_ptr(_storage + i.unwrap());
        }

        constexpr auto mut_mem(usize i = 0) -> mut_mem_ptr<void>
        {
            contracts::debug_expects(i < size());

            return mut_mem_ptr(_storage + i.unwrap());
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            contracts::debug_expects(i < size());

            return _storage[i.unwrap()];
        }

        constexpr auto mut_ref(usize i) -> byte&
        {
            contracts::debug_expects(i < size());

            return _storage[i.unwrap()];
        }

        constexpr auto operator[](usize i) const -> const byte&
        {
            return ref(i);
        }

        constexpr auto operator[](usize i) -> byte&
        {
            return mut_ref(i);
        }

        constexpr auto eq(const static_storage& that) const -> bool
        {
            for (usize i = usize(0); i < size(); i++)
            {
                if (_storage[i.unwrap()] != that._storage[i.unwrap()])
                    return false;
            }

            return true;
        }

        constexpr auto ne(const static_storage& that) const -> bool
        {
            return not(*this == that);
        }

    public:
        byte _storage[in_size.unwrap()];
    };
}
