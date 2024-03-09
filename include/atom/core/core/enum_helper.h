#pragma once
#include "atom/core/string/string_view.h"
#include "magic_enum.hpp"
#include "magic_enum_all.hpp"

namespace atom
{
    class enum_helper
    {
    public:
        template <typename enum_type>
        static constexpr auto to_string_view(enum_type value) -> string_view
        {
            return string_view::from_std(magic_enum::enum_name(value));
        }
    };
}
