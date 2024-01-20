export module atom.core:containers.static_array;
import :core;
import :mem_ptr;
import :array_iter;
import :range;

namespace atom
{
    template <typename in_elem_type, usize in_count>
    class basic_static_array
    {
    public:
        using elem_type = in_elem_type;
        using iter_type = array_iter<elem_type>;
        using iter_end_type = iter_type;
        using mut_iter_type = mut_array_iter<elem_type>;
        using mut_iter_end_type = mut_iter_type;

    public:
        constexpr basic_static_array() = default;

        template <usize other_count>
        constexpr basic_static_array(const elem_type (&arr)[other_count])
            requires(other_count <= in_count)
            : _arr{ arr }
        {}

        template <typename... arg_types>
        constexpr basic_static_array(arg_types&&... args)
            requires(rconvertible_to<arg_types, elem_type> and ...)
                    and (sizeof...(arg_types) <= in_count.unwrap())
            : _arr{ 0 }
        {}

    public:
        constexpr auto data() const -> mem_ptr<elem_type>
        {
            return _arr;
        }

        constexpr auto mut_data() -> mut_mem_ptr<elem_type>
        {
            return _arr;
        }

        constexpr auto count() const -> usize
        {
            return in_count;
        }

        constexpr auto iter() const -> iter_type
        {
            return iter_type{ _arr };
        }

        constexpr auto iter_end() const -> iter_end_type
        {
            return iter_end_type{ _arr + in_count.unwrap() };
        }

        constexpr auto mut_iter() -> mut_iter_type
        {
            return mut_iter_type{ _arr };
        }

        constexpr auto mut_iter_end() -> mut_iter_end_type
        {
            return mut_iter_end_type{ _arr + in_count };
        }

    private:
        elem_type _arr[in_count.unwrap()];
    };

    export template <typename elem_type, usize count>
    class static_array: public mut_array_range_extensions<basic_static_array<elem_type, count>>
    {
        using base_type = mut_array_range_extensions<basic_static_array<elem_type, count>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
