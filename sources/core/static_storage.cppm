export module atom_core:core.static_storage;

import :contracts;
import :types;
import :core.core;
import :core.byte;
import :core.int_wrapper;

namespace atom
{
    export template <typename... value_types>
    class static_storage_for
    {
    private:
        using value_types_list = type_list<value_types...>;

    public:
        constexpr auto get_data() -> void*
        {
            return _storage;
        }

        constexpr auto get_data() const -> const void*
        {
            return _storage;
        }

        constexpr auto get_size() const -> usize
        {
            return value_types_list::get_max_size();
        }

    public:
        alignas(value_types_list::get_max_align()) byte _storage[value_types_list::get_max_size()];
    };

    export template <usize in_size>
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
            contract_debug_expects(i < size());

            return ptr(_storage + i);
        }

        constexpr auto mut_mem(usize i = 0) -> void*
        {
            contract_debug_expects(i < size());

            return mut_ptr(_storage + i);
        }

        constexpr auto ref(usize i) const -> const byte&
        {
            contract_debug_expects(i < size());

            return _storage[i];
        }

        constexpr auto mut_ref(usize i) -> byte&
        {
            contract_debug_expects(i < size());

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

        constexpr auto operator==(const static_storage& that) const -> bool
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
