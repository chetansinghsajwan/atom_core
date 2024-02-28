export module atom.core:dynamic_string;
import :string_functions;
import :containers.dynamic_array;

namespace atom
{
    export template <typename allocator_type>
    class dynamic_string
        : public string_functions<dynamic_string<allocator_type>,
              dynamic_array<char, allocator_type>>
    {
        using base_type =
            string_functions<dynamic_string<allocator_type>, dynamic_array<char, allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
