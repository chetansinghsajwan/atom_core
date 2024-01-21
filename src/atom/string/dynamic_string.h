#pragma once
#include "_string_impl.h"

namespace atom
{
    template <typename allocator_type>
    class dynamic_string_container_provider
    {
    public:
        template <typename value_type>
        using type = dynamic_array<value_type, allocator_type>;
    };

    template <typename allocator_type>
    using dynamic_string =
        _string_impl<typename dynamic_string_container_provider<allocator_type>::type>;
}
