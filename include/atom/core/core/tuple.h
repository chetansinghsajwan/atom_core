#pragma once
#include <tuple>

namespace atom
{
    template <typename... value_ts>
    using tuple = std::tuple<value_ts...>;
}
