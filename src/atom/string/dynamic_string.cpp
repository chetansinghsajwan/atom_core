export module atom.core:dynamic_string;
import :_string_impl;
import :containers.dynamic_array;

namespace atom
{
    template <typename allocator_type>
    class dynamic_string_container_provider
    {
    public:
        template <typename value_type>
        using type = dynamic_array<value_type, allocator_type>;
    };

    export template <typename allocator_type>
    using dynamic_string =
        _string_impl<dynamic_string_container_provider<allocator_type>::template type>;
}
