export module atom.core:core.static_storage;

import :contracts;
import :types;
import :core.core;
import :core.byte;
import :core.int_wrapper;

namespace atom
{
    export template <typename... value_types>
    union union_storage;

    template <typename this_value_type, typename... other_value_types>
    union union_storage<this_value_type, other_value_types...>
    {
        using this_type = union_storage;
        using next_type = union_storage<other_value_types...>;
        using this_field_value_type =
            type_utils::conditional_type<not type_info<this_value_type>::is_void(),
                this_value_type>;

    public:
        constexpr union_storage()
            : _next{}
        {}

        constexpr union_storage(const this_type& that) = default;
        constexpr union_storage& operator=(const this_type& that) = default;
        constexpr union_storage(this_type&& that) = default;
        constexpr union_storage& operator=(this_type&& that) = default;

        template <typename value_type, typename... arg_types>
        constexpr union_storage(create_by_emplace_tag<value_type> tag, arg_types&&... args)
            : _next{ tag, forward<arg_types>(args)... }
        {}

        template <typename... arg_types>
        constexpr union_storage(create_by_emplace_tag<this_value_type> tag, arg_types&&... args)
            : _value{ forward<arg_types>(args)... }
        {}

        constexpr ~union_storage() {}

    public:
        template <typename value_type>
        constexpr auto get_data_as() const -> const value_type*
        {
            if constexpr (type_info<value_type>::template is_same_as<this_value_type>())
                return &_value;
            else
                return _next.template get_data_as<value_type>();
        }

        template <typename value_type>
        constexpr auto get_data_as() -> value_type*
        {
            if constexpr (type_info<value_type>::template is_same_as<this_value_type>())
                return &_value;
            else
                return _next.template get_data_as<value_type>();
        }

    public:
        this_field_value_type _value;
        next_type _next;
    };

    template <>
    union union_storage<>
    {
        using this_type = union_storage;

        struct _dummy_type
        {};

    public:
        constexpr union_storage()
            : _value{}
        {}

        constexpr union_storage(const this_type& that) = default;
        constexpr union_storage& operator=(const this_type& that) = default;
        constexpr union_storage(this_type&& that) = default;
        constexpr union_storage& operator=(this_type&& that) = default;

        constexpr ~union_storage() {}

    public:
        _dummy_type _value;
    };

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
