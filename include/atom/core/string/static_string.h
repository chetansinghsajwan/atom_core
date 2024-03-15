#pragma once
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/static_array.h"

namespace atom
{
    template <usize size>
    class static_string: public string_functions<static_string<size>, static_array<char, size>>
    {
        using base_t = string_functions<static_string<size>, static_array<char, size>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
