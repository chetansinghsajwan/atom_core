#pragma once
#include "atom/core/core/error.h"
#include "atom/core/string/string_view.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// error type used to represent to string formatting error.
    /// --------------------------------------------------------------------------------------------
    class string_format_error: public error
    {
    public:
        constexpr string_format_error(string_view msg)
            : error(msg)
        {}
    };
}