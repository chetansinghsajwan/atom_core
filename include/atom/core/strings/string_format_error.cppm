export module atom.core:strings.string_format_error;

import :core;
import :strings.string_view;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// error type used to represent to string formatting error.
    /// --------------------------------------------------------------------------------------------
    export class string_format_error: public error
    {
    public:
        constexpr string_format_error(string_view msg)
            : error(msg)
        {}
    };
}
