export module atom.core:static_string;
import :string_functions;
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
    class static_string
        : public string_functions<static_string_container_provider<size>::template type>
    {
        using base_type = string_functions<static_string_container_provider<size>::template type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
