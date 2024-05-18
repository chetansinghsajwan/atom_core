export module atom.core:containers.unordered_map;

import std;

namespace atom
{
    export template <typename key_t, typename value_t>
    using unordered_map = std::unordered_map<key_t, value_t>;
}
