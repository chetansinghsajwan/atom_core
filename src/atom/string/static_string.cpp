export module atom.core:static_string;
import :_string_impl;
import :containers.static_array;
import :core;
import :std;

namespace atom
{
    template <usize size>
    class static_string_container_provider
    {
    public:
        template <typename value_type>
        using type = static_array<value_type, size>;
    };

    export template <usize size>
    using static_string = _string_impl<static_string_container_provider<size>::template type>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// primitives implementations
    //// these implementations could not be defined before due to circular depenedencies.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    export extern "C++" template <std::size_t size>
    class int_string: public static_string<size>
    {
        using base_type = static_string<size>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    export extern "C++" template <std::size_t size>
    class float_string: public static_string<size>
    {
        using base_type = static_string<size>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
