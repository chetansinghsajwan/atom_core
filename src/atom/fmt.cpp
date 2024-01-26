module;

#include "fmt/format.h"
#include "fmt/ranges.h"

export module atom.core:fmt;

export namespace fmt
{
    using fmt::format_context;
    using fmt::basic_format_context;
    using fmt::format_parse_context;
    using fmt::basic_format_parse_context;
    using fmt::format_string;
    using fmt::basic_format_string;
    using fmt::string_view;
    using fmt::basic_string_view;
    using fmt::buffer_context;
    using fmt::format;
    using fmt::format_to;
    using fmt::format_error;
    using fmt::formatter;
    using fmt::make_format_args;
    using fmt::runtime_format_string;
    using fmt::type_identity_t;

    namespace detail
    {
        using fmt::detail::buffer_appender;
        using fmt::detail::iterator_buffer;
        using fmt::detail::locale_ref;
        using fmt::detail::vformat_to;
    }
}
