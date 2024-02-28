export module atom.core:static_string;
import :string_functions;
import :containers.static_array;
import :core;
import :std;

namespace atom
{
    export template <usize size>
    class static_string: public string_functions<static_string<size>, static_array<char, size>>
    {
        using base_type = string_functions<static_string<size>, static_array<char, size>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
