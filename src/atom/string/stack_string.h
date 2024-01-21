#pragma once
#include "_string_impl.h"

namespace atom
{
    template <usize size>
    class stack_string_container_provider
    {
    public:
        template <typename value_type>
        using type = static_array<value_type, size>;
    };

    template <usize size>
    using stack_string = _string_impl<stack_string_container_provider<size>::template type>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// primitives implementations
    //// these implementations could not be defined before due to circular depenedencies.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    extern "C++" template <std::size_t size>
    class int_string: public stack_string<size>
    {
        using base_type = stack_string<size>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    extern "C++" template <std::size_t size>
    class float_string: public stack_string<size>
    {
        using base_type = stack_string<size>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    template <typename this_final_type, typename value_type, typename limit_type>
    constexpr auto _int_impl<this_final_type, value_type, limit_type>::to_string(value_type val)
        -> string_type
    {
        string_type str;
        char* begin = _to_string(val, str.mut_data() + str.count() - 1);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }

    template <typename this_final_type, typename value_type>
    constexpr auto _float_impl<this_final_type, value_type>::to_string(value_type val)
        -> string_type
    {
        string_type str;
        char* begin = _to_string(val, str.mut_data() + str.count() - 1);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }
}
