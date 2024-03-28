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
        constexpr error()
            : msg()
        {}

        template <typename string_view_t>
        constexpr error(string_view_t msg)
            : msg(msg.get_data(), msg.get_count())
        {}

    public:
        std::string_view msg;
    };

    class value_exists_error: public error
    {};
}
