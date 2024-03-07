#pragma once
#include "atom/core/_std.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class error
    {
    public:
        template <typename string_view_type>
        constexpr error(string_view_type msg)
            : msg(msg.get_data(), msg.get_count())
        {}

    public:
        std::string_view msg;
    };
}
