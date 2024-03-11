#pragma once
#include <tuple>

namespace atom
{
    template <typename... value_types>
    using tuple = std::tuple<value_types...>;
}
