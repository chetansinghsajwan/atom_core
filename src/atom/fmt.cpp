module;

#include "fmt/format.h"

export module atom.core:fmt;

export namespace fmt
{
    using fmt::basic_format_context;
    using fmt::basic_format_parse_context;
    using fmt::basic_format_string;
    using fmt::basic_string_view;
    using fmt::format;
    using fmt::format_error;
    using fmt::formatter;
    using fmt::runtime_format_string;
    using fmt::type_identity_t;
    using fmt::make_format_args;
    using fmt::buffer_context;

    namespace detail
    {
        using fmt::detail::iterator_buffer;
        using fmt::detail::vformat_to;
        using fmt::detail::locale_ref;
        using fmt::detail::buffer_appender;
    }
}
