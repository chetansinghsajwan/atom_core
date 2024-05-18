export module atom.core:core.tuple;

import std;

namespace atom
{
    export template <typename... value_ts>
    using tuple = std::tuple<value_ts...>;

    export template <typename... value_ts>
    using pair = std::pair<value_ts...>;
}
