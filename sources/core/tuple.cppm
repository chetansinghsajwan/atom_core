export module atom.core:core.tuple;

import std;

namespace atom
{
    export template <typename... value_types>
    using tuple = std::tuple<value_types...>;

    export template <typename... value_types>
    using pair = std::pair<value_types...>;
}
