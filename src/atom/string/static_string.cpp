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
    class static_string: public _string_impl<static_string_container_provider<size>::template type>
    {
        using base_type = _string_impl<static_string_container_provider<size>::template type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
