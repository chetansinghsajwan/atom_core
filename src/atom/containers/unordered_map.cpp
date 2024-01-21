export module atom.core:containers.unordered_map;
import :std;

namespace atom
{
    export template <typename key_type, typename elem_type>
    using unordered_map = std::unordered_map<key_type, elem_type>;
}
