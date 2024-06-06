export module atom.core:containers.unordered_map;

import std;

namespace atom
{
    export template <typename key_type, typename value_type>
    using unordered_map = std::unordered_map<key_type, value_type>;
}
