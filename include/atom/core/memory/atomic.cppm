export module atom.core:atomic;

import std;

namespace atom
{
    export template <typename value_t>
    using atomic = std::atomic<value_t>;
}
